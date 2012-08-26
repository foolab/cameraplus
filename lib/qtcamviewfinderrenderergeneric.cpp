#include "qtcamviewfinderrenderergeneric.h"
#include <QDebug>
#include <gst/video/video.h>
#include <QPainter>
#include "qtcamconfig.h"

QT_CAM_VIEWFINDER_RENDERER(RENDERER_TYPE_GENERIC, QtCamViewfinderRendererGeneric);

#define CAPS "video/x-raw-rgb, bpp = (int) 32, depth = (int) 24, red_mask = (int) 65280, green_mask = (int) 16711680, blue_mask = (int) -16777216"

// TODO: this needs to be debugged or rewritten. There are race conditions.
QtCamViewfinderRendererGeneric::QtCamViewfinderRendererGeneric(QtCamConfig *config,
							       QObject *parent) :
  QtCamViewfinderRenderer(config, parent), m_elem(0), m_sink(0), m_id(0) {

}

QtCamViewfinderRendererGeneric::~QtCamViewfinderRendererGeneric() {
  m_mutex.lock();

  if (m_elem) {
    g_object_remove_toggle_ref(G_OBJECT(m_elem), (GToggleNotify)sink_notify, this);
    m_elem = 0;

    g_signal_handler_disconnect(m_sink, m_id);
  }

  m_mutex.unlock();
}

void QtCamViewfinderRendererGeneric::paint(QPainter *painter) {
  // TODO: scale and keep aspect ratio.
  m_mutex.lock();

  if (!m_image.isNull()) {
    painter->drawImage(QRectF(QPointF(0, 0), m_size), m_image);
  }

  m_mutex.unlock();
}

void QtCamViewfinderRendererGeneric::resize(const QSizeF& size) {
  m_size = size;
}

void QtCamViewfinderRendererGeneric::reset() {
  m_image = QImage();
}

GstElement *QtCamViewfinderRendererGeneric::sinkElement() {
  if (!m_elem) {
    m_elem = gst_bin_new("QtCamViewfinderRendererGenericBin");
    if (!m_elem) {
      qCritical() << "Failed to create sink bin";
      return 0;
    }

    GstElement *sink = gst_element_factory_make("fakesink", "QtCamViewfinderRendererGenericSink");
    if (!sink) {
      qCritical() << "Failed to create fakesink";
      gst_object_unref(m_elem);
      m_elem = 0;
      return 0;
    }

    g_object_set(G_OBJECT(sink), "signal-handoffs", TRUE, "sync", TRUE, NULL);
    m_id = g_signal_connect(sink, "handoff", G_CALLBACK(on_gst_buffer), this);

    GstElement *csp = gst_element_factory_make("ffmpegcolorspace",
					       "QtCamViewfinderRendererGenericCsp");
    if (!csp) {
      qCritical() << "Failed to create ffmpegcolorspace";
      gst_object_unref(sink);
      gst_object_unref(m_elem);
      m_elem = 0;
      return 0;
    }

    GstElement *filter = gst_element_factory_make("capsfilter",
						  "QtCamViewfinderRendererGenericCapsFilter");
    if (!filter) {
      qCritical() << "Failed to create capsfilter";
      gst_object_unref(sink);
      gst_object_unref(csp);
      gst_object_unref(m_elem);
      m_elem = 0;
      return 0;
    }

    GstCaps *caps = gst_caps_from_string(CAPS);
    g_object_set(filter, "caps", caps, NULL);
    gst_caps_unref(caps);

    gst_bin_add_many(GST_BIN(m_elem), csp, filter, sink, NULL);
    gst_element_link_many(csp, filter, sink, NULL);

    GstPad *pad = gst_element_get_static_pad(csp, "sink");
    gst_element_add_pad(m_elem, gst_ghost_pad_new("sink", pad));
    gst_object_unref(pad);

    g_object_add_toggle_ref(G_OBJECT(m_elem), (GToggleNotify)sink_notify, this);

    m_sink = sink;
  }

  return m_elem;
}

void QtCamViewfinderRendererGeneric::on_gst_buffer(GstElement *element,
						   GstBuffer *buf, GstPad *pad,
						   QtCamViewfinderRendererGeneric *q) {

  q->m_mutex.lock();

  if (!q->m_elem) {
    q->m_mutex.unlock();
    return;
  }

  int width, height;

  if (!gst_video_get_size(pad, &width, &height)) {
    return;
  }

  QImage image(buf->data, width, height, QImage::Format_RGB32);

  q->m_image = image.copy();

  QMetaObject::invokeMethod(q, "updateRequested", Qt::QueuedConnection);

  q->m_mutex.unlock();
}

void QtCamViewfinderRendererGeneric::sink_notify(QtCamViewfinderRendererGeneric *q,
						 GObject *object,
						 gboolean is_last_ref) {

  if (is_last_ref) {
    g_object_remove_toggle_ref(G_OBJECT(q->m_elem), (GToggleNotify)sink_notify, q);
    q->m_elem = 0;
    q->m_sink = 0;
  }
}
