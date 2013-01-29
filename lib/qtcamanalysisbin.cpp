/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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

#include "qtcamanalysisbin.h"
#include <QStringList>
#include <QDebug>
#include <QHash>

#define FACTORY_NAME(x) gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(gst_element_get_factory(x)))

static QtCamAnalysisBinPrivate *qt_cam_analysis_bin_create(const QStringList& factories,
							   const char *name);
static QtCamAnalysisBinPrivate *qt_cam_analysis_bin_create(GstElement *child,
							   const char *name);

class QtCamAnalysisBinPrivate {
public:
  QtCamAnalysisBinPrivate() :
    bin(0),
    probe(0),
    queuePad(0) {

  }

  static gboolean block_buffers(GstPad *pad, GstMiniObject *o) {
    Q_UNUSED(pad);
    Q_UNUSED(o);

    // Drop data
    return FALSE;
  }

  GstElement *bin;
  gulong probe;
  GstPad *queuePad;
  QMultiHash<QString, GstElement *> elements;
};

QtCamAnalysisBin::QtCamAnalysisBin(QtCamAnalysisBinPrivate *d) :
  d_ptr(d) {
  gst_object_ref(d_ptr->bin);
}

QtCamAnalysisBin::~QtCamAnalysisBin() {
  d_ptr->elements.clear();

  setBlocked(false);
  gst_object_unref(GST_OBJECT(d_ptr->queuePad));
  gst_object_unref(d_ptr->bin);
  delete d_ptr; d_ptr = 0;
}

void QtCamAnalysisBin::setBlocked(bool blocked) {
  if (blocked == isBlocked()) {
    return;
  }

  if (blocked) {
    d_ptr->probe = gst_pad_add_buffer_probe(d_ptr->queuePad,
					    G_CALLBACK(QtCamAnalysisBinPrivate::block_buffers),
					    d_ptr);
  }
  else {
    gst_pad_remove_data_probe(d_ptr->queuePad, d_ptr->probe);
    d_ptr->probe = 0;
  }
}

bool QtCamAnalysisBin::isBlocked() const {
  return d_ptr->probe != 0;
}

GstElement *QtCamAnalysisBin::bin() {
  return d_ptr->bin;
}

QtCamAnalysisBin *QtCamAnalysisBin::create(const QStringList& factories, const char *name) {
  QList<GstElement *> elements;
  if (factories.isEmpty()) {
    return 0;
  }

  QtCamAnalysisBinPrivate *d = qt_cam_analysis_bin_create(factories, name);
  if (!d) {
    return 0;
  }

  return new QtCamAnalysisBin(d);
}

QtCamAnalysisBinPrivate *qt_cam_analysis_bin_create(const QStringList& factories,
						    const char *name) {

  if (factories.isEmpty()) {
    return 0;
  }

  GstElement *bin = 0;
  QHash<QString, GstElement *> elements;
  QList<GstElement *> added;

  bin = gst_bin_new("analysis-bin-bin");

  foreach (const QString& factory, factories) {
    GstElement *element = gst_element_factory_make(factory.toUtf8().constData(), NULL);
    if (!element) {
      qWarning() << "Failed to create element" << factory;
      continue;
    }

    if (!gst_bin_add(GST_BIN(bin), element)) {
      qWarning() << "Failed to add element" << factory << "to bin";
      gst_object_unref(element);
    }

    elements.insert(factory, element);
    added << element;
  }

  if (added.size() > 1) {
    for (int x = 1; x < added.count(); x++) {
      GstElement *elem = added[x];
      GstElement *prev = added[x - 1];

      if (!gst_element_link(prev, elem)) {
	qWarning() << "Failed to link" << FACTORY_NAME(prev) << "and" << FACTORY_NAME(elem);
      }
    }
  }

  GstPad *pad = gst_element_get_static_pad(added.first(), "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
  gst_object_unref(GST_OBJECT(pad));

  pad = gst_element_get_static_pad(added.last(), "src");
  gst_element_add_pad(bin, gst_ghost_pad_new("src", pad));
  gst_object_unref(GST_OBJECT(pad));

  QtCamAnalysisBinPrivate *d = qt_cam_analysis_bin_create(bin, name);
  if (!d) {
    return 0;
  }

  d->elements = elements;

  return d;
}

/*
 *       -- identity -- ghost pad
 * tee -
 *       -- queue -- copy -- filters -- fakesink
 */
QtCamAnalysisBinPrivate *qt_cam_analysis_bin_create(GstElement *child, const char *name) {
  GstPad *pad = 0;
  GstPad *queuePad = 0;
  QtCamAnalysisBinPrivate *d = 0;

  GstElement *bin = gst_bin_new(name);

  GstElement *tee = gst_element_factory_make("tee", "analysis-bin-tee");
  GstElement *queue = gst_element_factory_make("queue", "analysis-bin-queue");
  GstElement *fakesink = gst_element_factory_make("fakesink", "analysis-bin-fakesink");
  GstElement *copy = gst_element_factory_make("copy", "analysis-bin-copy");
  GstElement *identity = gst_element_factory_make("identity", "analysis-bin-identity");

  if (!bin || !tee || !queue || !fakesink || !copy || !identity) {
    qWarning() << "Failed to create some elements";
    goto free_and_out;
  }

  gst_bin_add_many(GST_BIN(bin), tee, queue, copy, fakesink, child, identity, NULL);

  g_object_set(tee, "silent", TRUE, NULL);
  g_object_set(queue, "silent", TRUE, "leaky", 2, "max-size-buffers", 1, NULL);
  g_object_set(fakesink, "silent", TRUE, "sync", FALSE, "async", FALSE, NULL);
  g_object_set(identity, "silent", TRUE, "signal-handoffs", FALSE, NULL);

  gst_element_link(tee, identity);
  gst_element_link(tee, queue);
  gst_element_link(queue, copy);
  gst_element_link(copy, child);
  gst_element_link(child, fakesink);

  pad = gst_element_get_static_pad(tee, "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
  gst_object_unref(GST_OBJECT(pad));

  pad = gst_element_get_static_pad(identity, "src");
  gst_element_add_pad(bin, gst_ghost_pad_new("src", pad));
  gst_object_unref(GST_OBJECT(pad));

  pad = gst_element_get_static_pad(tee, "src0");
  g_object_set(tee, "alloc-pad", pad, NULL);
  gst_object_unref(GST_OBJECT(pad));

  queuePad = gst_element_get_static_pad(queue, "src");

  d = new QtCamAnalysisBinPrivate;
  d->queuePad = queuePad;
  d->bin = bin;
  return d;

 free_and_out:
  if (bin) {
    gst_object_unref(bin);
  }
  else {
    gst_object_unref(child);
  }

  if (tee) {
    gst_object_unref(tee);
  }

  if (queue) {
    gst_object_unref(queue);
  }

  if (copy) {
    gst_object_unref(copy);
  }

  if (identity) {
    gst_object_unref(identity);
  }

  if (fakesink) {
    gst_object_unref(fakesink);
  }

  return 0;
}

QList<GstElement *> QtCamAnalysisBin::lookup(const QString& factory) {
  if (d_ptr->elements.contains(factory)) {
    return d_ptr->elements.values(factory);
  }

  return QList<GstElement *>();
}
