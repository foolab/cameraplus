/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
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

#include "gstcopy.h"

GST_DEBUG_CATEGORY_STATIC (gst_copy_debug);
#define GST_CAT_DEFAULT gst_copy_debug

#ifndef PACKAGE
#define PACKAGE "copy"
#endif

#ifndef VERSION
#define VERSION "0.1"
#endif

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
								   GST_PAD_SRC,
								   GST_PAD_ALWAYS,
								   GST_STATIC_CAPS ("ANY")
								   );

static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
								    GST_PAD_SINK,
								    GST_PAD_ALWAYS,
								    GST_STATIC_CAPS ("ANY")
								    );

GST_BOILERPLATE (GstCopy, gst_copy, GstBaseTransform,
		 GST_TYPE_BASE_TRANSFORM);

static GstFlowReturn gst_copy_transform_ip(GstBaseTransform *trans, GstBuffer *buf);
static GstFlowReturn gst_copy_prepare_output_buffer(GstBaseTransform *trans, GstBuffer *input,
						    gint size, GstCaps *caps, GstBuffer **buf);

static void
gst_copy_base_init(gpointer gclass) {
  GstElementClass *element_class = GST_ELEMENT_CLASS (gclass);

  gst_element_class_set_details_simple(element_class,
				       "Copy",
				       "Generic",
				       "Copy buffers",
				       "Mohammed Sameer <msameer@foolab.org>>");

  gst_element_class_add_pad_template(element_class,
				     gst_static_pad_template_get(&src_factory));
  gst_element_class_add_pad_template(element_class,
				     gst_static_pad_template_get(&sink_factory));
}

static void
gst_copy_class_init(GstCopyClass *klass) {
  GstBaseTransformClass *gstbasetrans_class = (GstBaseTransformClass *)klass;

  gstbasetrans_class->transform_ip = GST_DEBUG_FUNCPTR(gst_copy_transform_ip);
  gstbasetrans_class->prepare_output_buffer = GST_DEBUG_FUNCPTR(gst_copy_prepare_output_buffer);
}

static void
gst_copy_init(GstCopy *copy, GstCopyClass *) {
  GstBaseTransform *trans = GST_BASE_TRANSFORM(copy);

  gst_base_transform_set_passthrough(trans, FALSE);
  gst_base_transform_set_in_place(trans, TRUE);
}

static GstFlowReturn
gst_copy_transform_ip(GstBaseTransform *trans, GstBuffer *) {
  GST_DEBUG_OBJECT(trans, "transform ip");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_copy_prepare_output_buffer(GstBaseTransform *trans, GstBuffer *input,
			       gint, GstCaps *, GstBuffer **buf) {

  GST_DEBUG_OBJECT(trans, "prepare output buffer");

  *buf = gst_buffer_copy(input);

  return GST_FLOW_OK;
}

static gboolean
copy_init(GstPlugin *copy) {
  GST_DEBUG_CATEGORY_INIT(gst_copy_debug, "copy",
			  0, "Copy element");

  return gst_element_register(copy, "copy", GST_RANK_NONE,
			      GST_TYPE_COPY);
}


GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, GST_VERSION_MINOR, "copy", "Copy element",
		  copy_init, VERSION, "GPL", "", "")

gboolean qt_cam_copy_register() {
  return gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR,
				  "copy", "Copy element",
				  copy_init, VERSION,
				  "GPL", "", "", "");
}
