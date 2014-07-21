/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "qtcamviewfinderrenderermeego.h"
#include <QDebug>
#include <gst/video/video.h>
#include <QPainter>
#include "qtcamconfig.h"
#include <QX11Info>
#include <QGLContext>
#include <QGLShaderProgram>
#include <gst/interfaces/meegovideotexture.h>

QT_CAM_VIEWFINDER_RENDERER(RENDERER_TYPE_MEEGO, QtCamViewfinderRendererMeeGo);

#define GL_TEXTURE_EXTERNAL_OES                  0x8060

typedef void *EGLSyncKHR;
#define EGL_SYNC_FENCE_KHR                       0x30F9

typedef EGLSyncKHR(EGLAPIENTRYP _PFNEGLCREATESYNCKHRPROC)(EGLDisplay dpy, EGLenum type,
							  const EGLint *attrib_list);

_PFNEGLCREATESYNCKHRPROC eglCreateSyncKHR = 0;

static const QString FRAGMENT_SHADER = ""
    "#extension GL_OES_EGL_image_external: enable\n"
    "uniform samplerExternalOES texture0;"
    "varying lowp vec2 fragTexCoord;"
    "void main() {"
    "  gl_FragColor = texture2D(texture0, fragTexCoord);"
    "}"
  "";

static const QString VERTEX_SHADER = ""
    "attribute highp vec4 inputVertex;"
    "attribute lowp vec2 textureCoord;"
    "uniform highp mat4 matrix;"
    "uniform highp mat4 matrixWorld;"
    "varying lowp vec2 fragTexCoord;"
    ""
    "void main() {"
    "  gl_Position = matrix * matrixWorld * inputVertex;"
    "  fragTexCoord = textureCoord;"
    "}"
  "";

QtCamViewfinderRendererMeeGo::QtCamViewfinderRendererMeeGo(QtCamConfig *config,
							   QObject *parent) :
  QtCamViewfinderRenderer(config, parent),
  m_conf(config),
  m_sink(0),
  m_frame(-1),
  m_id(0),
  m_notify(0),
  m_needsInit(true),
  m_program(0) {

  bzero(&m_vertexCoords, 8);
}

QtCamViewfinderRendererMeeGo::~QtCamViewfinderRendererMeeGo() {
  if (m_sink) {
    g_signal_handler_disconnect(m_sink, m_id);
    g_signal_handler_disconnect(m_sink, m_notify);
    g_object_remove_toggle_ref(G_OBJECT(m_sink), (GToggleNotify)sink_notify, this);
    m_sink = 0;
  }
}

bool QtCamViewfinderRendererMeeGo::needsNativePainting() {
  return true;
}

void QtCamViewfinderRendererMeeGo::paint(const QMatrix4x4& matrix, const QRectF& viewport) {
  QMutexLocker locker(&m_frameMutex);
  if (m_frame == -1) {
    return;
  }

  if (m_needsInit) {
    calculateProjectionMatrix(viewport);

    if (!eglCreateSyncKHR && m_conf->viewfinderUseFence()) {
      eglCreateSyncKHR = (_PFNEGLCREATESYNCKHRPROC)eglGetProcAddress("eglCreateSyncKHR");

      if (!eglCreateSyncKHR) {
	qWarning() << "eglCreateSyncKHR not found. Fences disabled";
      }
    }

    m_needsInit = false;
  }

  if (!m_program) {
    // Program will be created if needed and will never be deleted even
    // if attaching the shaders fail.
    createProgram();
  }

  paintFrame(matrix, m_frame);
}

void QtCamViewfinderRendererMeeGo::resize(const QSizeF& size) {
  if (size == m_size) {
    return;
  }

  m_size = size;

  m_renderArea = QRectF();

  calculateCoords();

  // This will destroy everything
  // but we need a way to reset the viewport and the transformation matrix only.
  m_needsInit = true;

  emit renderAreaChanged();
}

void QtCamViewfinderRendererMeeGo::reset() {
  QMutexLocker locker(&m_frameMutex);
  m_frame = -1;
}

void QtCamViewfinderRendererMeeGo::start() {

}

GstElement *QtCamViewfinderRendererMeeGo::sinkElement() {
  if (!QGLContext::currentContext()) {
    qCritical() << "Cannot create the GStreamer element without an OpenGL context.";
    return 0;
  }

  if (!m_sink) {
    m_sink = gst_element_factory_make(m_conf->viewfinderSink().toLatin1().data(),
				      "QtCamViewfinderRendererMeeGoSink");
    if (!m_sink) {
      qCritical() << "Failed to create" << m_conf->viewfinderSink();
      return 0;
    }

    g_object_add_toggle_ref(G_OBJECT(m_sink), (GToggleNotify)sink_notify, this);
  }

  Display *d = QX11Info::display();
  g_object_set(G_OBJECT(m_sink), "x-display", d, "use-framebuffer-memory", TRUE, NULL);

  m_dpy = eglGetDisplay((EGLNativeDisplayType)d);
  if (m_dpy == EGL_NO_DISPLAY) {
    qCritical() << "Failed to obtain EGL Display";
  }

  EGLContext context = eglGetCurrentContext();
  if (context == EGL_NO_CONTEXT) {
    qCritical() << "Failed to obtain EGL context";
  }

  g_object_set(G_OBJECT(m_sink), "egl-display", m_dpy, "egl-context", context, NULL);

  m_id = g_signal_connect(G_OBJECT(m_sink), "frame-ready", G_CALLBACK(frame_ready), this);

  GstPad *pad = gst_element_get_static_pad(m_sink, "sink");
  m_notify = g_signal_connect(G_OBJECT(pad), "notify::caps",
			      G_CALLBACK(sink_caps_changed), this);
  gst_object_unref(pad);

  return m_sink;
}

void QtCamViewfinderRendererMeeGo::frame_ready(GstElement *sink, int frame,
					       QtCamViewfinderRendererMeeGo *r) {
  Q_UNUSED(sink);
  Q_UNUSED(frame);

  r->m_frameMutex.lock();
  r->m_frame = frame;
  r->m_frameMutex.unlock();

  QMetaObject::invokeMethod(r, "updateRequested", Qt::QueuedConnection);
}

void QtCamViewfinderRendererMeeGo::sink_notify(QtCamViewfinderRendererMeeGo *q,
					       GObject *object, gboolean is_last_ref) {

  Q_UNUSED(object);

  if (is_last_ref) {
    g_signal_handler_disconnect(q->m_sink, q->m_id);
    g_object_remove_toggle_ref(G_OBJECT(q->m_sink), (GToggleNotify)sink_notify, q);
    q->m_sink = 0;
  }
}

void QtCamViewfinderRendererMeeGo::sink_caps_changed(GObject *obj, GParamSpec *pspec,
						     QtCamViewfinderRendererMeeGo *q) {
  Q_UNUSED(obj);
  Q_UNUSED(pspec);

  int width, height;
  if (obj && gst_video_get_size(GST_PAD(obj), &width, &height)) {
    QMetaObject::invokeMethod(q, "setVideoSize", Qt::QueuedConnection,
			      Q_ARG(QSizeF, QSizeF(width, height)));
  }
}

void QtCamViewfinderRendererMeeGo::calculateProjectionMatrix(const QRectF& rect) {
  m_projectionMatrix = QMatrix4x4();
  m_projectionMatrix.ortho(rect);
}

void QtCamViewfinderRendererMeeGo::createProgram() {
  if (m_program) {
    delete m_program;
  }

  m_program = new QGLShaderProgram(this);

  if (!m_program->addShaderFromSourceCode(QGLShader::Vertex, VERTEX_SHADER)) {
    qCritical() << "Failed to add vertex shader";
    return;
  }

  if (!m_program->addShaderFromSourceCode(QGLShader::Fragment, FRAGMENT_SHADER)) {
    qCritical() << "Failed to add fragment shader";
    return;
  }

  m_program->bindAttributeLocation("inputVertex", 0);
  m_program->bindAttributeLocation("textureCoord", 1);

  if (!m_program->link()) {
    qCritical() << "Failed to link program!";
    return;
  }

  if (!m_program->bind()) {
    qCritical() << "Failed to bind program";
    return;
  }

  m_program->setUniformValue("texture0", 0); // texture UNIT 0
  m_program->release();
}

void QtCamViewfinderRendererMeeGo::paintFrame(const QMatrix4x4& matrix, int frame) {

  // TODO: we don't flip the front camera
  static GLfloat back_coordinates[4][8] = {
    {0, 1, 1, 1, 1, 0, 0, 0}, // 0
    {0, 1, 1, 1, 1, 0, 0, 0}, // 90 // TODO:
    {1, 0, 0, 0, 0, 1, 1, 1}, // 180
    {0, 1, 1, 1, 1, 0, 0, 0}, // 270 // TODO:
  };

  int index = m_angle == 0 ? 0 : m_angle == -1 ? 0 : 360 / m_angle;

  EGLSyncKHR sync = 0;

  if (frame == -1) {
    return;
  }

  MeegoGstVideoTexture *sink = MEEGO_GST_VIDEO_TEXTURE(m_sink);
  if (!meego_gst_video_texture_acquire_frame(sink, frame)) {
    qDebug() << "Failed to acquire frame";
    return;
  }

  m_program->bind();

  m_program->setUniformValue("matrix", m_projectionMatrix);
  m_program->setUniformValue("matrixWorld", matrix);

  if (!meego_gst_video_texture_bind_frame(sink, GL_TEXTURE_EXTERNAL_OES, frame)) {
    qDebug() << "Failed to bind frame";
    m_program->release();
    return;
  }

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, &m_vertexCoords);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, back_coordinates[index]);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  if (!meego_gst_video_texture_bind_frame(sink, GL_TEXTURE_EXTERNAL_OES, -1)) {
    qDebug() << "Failed to unbind frame";
  }

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  m_program->release();

  if (eglCreateSyncKHR) {
    sync = eglCreateSyncKHR(m_dpy, EGL_SYNC_FENCE_KHR, NULL);
  }

  meego_gst_video_texture_release_frame(sink, frame, sync);
}

void QtCamViewfinderRendererMeeGo::calculateCoords() {
  if (!m_size.isValid() || !m_videoSize.isValid()) {
    return;
  }

  QRectF area = renderArea();

  qreal leftMargin = area.x();
  qreal topMargin = area.y();
  QSizeF renderSize = area.size();

  m_vertexCoords[0] = leftMargin;
  m_vertexCoords[1] = topMargin + renderSize.height();

  m_vertexCoords[2] = renderSize.width() + leftMargin;
  m_vertexCoords[3] = topMargin + renderSize.height();

  m_vertexCoords[4] = renderSize.width() + leftMargin;
  m_vertexCoords[5] = topMargin;

  m_vertexCoords[6] = leftMargin;
  m_vertexCoords[7] = topMargin;
}

QRectF QtCamViewfinderRendererMeeGo::renderArea() {
  if (!m_renderArea.isNull()) {
    return m_renderArea;
  }

  QSizeF renderSize = m_videoSize;
  renderSize.scale(m_size, Qt::KeepAspectRatio);

  qreal leftMargin = (m_size.width() - renderSize.width())/2.0;
  qreal topMargin = (m_size.height() - renderSize.height())/2.0;

  m_renderArea = QRectF(QPointF(leftMargin, topMargin), renderSize);

  return m_renderArea;
}

QSizeF QtCamViewfinderRendererMeeGo::videoResolution() {
  return m_videoSize;
}

void QtCamViewfinderRendererMeeGo::setVideoSize(const QSizeF& size) {
  if (size == m_videoSize) {
    return;
  }

  m_videoSize = size;

  m_renderArea = QRectF();

  calculateCoords();

  m_needsInit = true;

  emit renderAreaChanged();
  emit videoResolutionChanged();
}
