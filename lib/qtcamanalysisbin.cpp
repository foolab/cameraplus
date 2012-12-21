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
#include <QDebug>

#define FACTORY_NAME(x) gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(gst_element_get_factory(x)))

GstElement *qt_cam_analysis_bin_create(QList<GstElement *>& children, const char *name) {
  GstElement *bin = 0;

  QList<GstElement *> added;

  if (children.isEmpty()) {
    return 0;
  }

  if (children.size() == 1) {
    return qt_cam_analysis_bin_create(children.takeFirst(), name);
  }

  bin = gst_bin_new("analysis_bin_bin");

  while (!children.isEmpty()) {
    GstElement *elem = children.takeFirst();

    if (!gst_bin_add(GST_BIN(bin), elem)) {
      qWarning() << "Failed to add element" << FACTORY_NAME(elem) << "to bin";
      gst_object_unref(elem);
    }
    else {
      added << elem;
    }
  }

  for (int x = 1; x < added.count(); x++) {
    GstElement *elem = added[x];
    GstElement *prev = added[x - 1];

    if (!gst_element_link(prev, elem)) {
      qWarning() << "Failed to link" << FACTORY_NAME(prev) << "and" << FACTORY_NAME(elem);
    }
  }

  GstPad *pad = gst_element_get_static_pad(added.first(), "sink");
  gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
  gst_object_unref(GST_OBJECT(pad));

  pad = gst_element_get_static_pad(added.last(), "src");
  gst_element_add_pad(bin, gst_ghost_pad_new("src", pad));
  gst_object_unref(GST_OBJECT(pad));

  return qt_cam_analysis_bin_create(bin, name);
}

/*
 *       -- identity -- ghost pad
 * tee -
 *       -- queue -- copy -- filters -- fakesink
 */
GstElement *qt_cam_analysis_bin_create(GstElement *child, const char *name) {
  GstPad *pad = 0;

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

  return bin;

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
