// -*- c++ -*-

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

#ifndef GST_COPY_H
#define GST_COPY_H

#include <gst/gst.h>
#include <gst/base/gstbasetransform.h>

G_BEGIN_DECLS

#define GST_TYPE_COPY \
  (gst_copy_get_type())
#define GST_COPY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_COPY,GstCopy))
#define GST_COPY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_COPY,GstCopyClass))
#define GST_IS_COPY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_COPY))
#define GST_IS_COPY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_COPY))

typedef struct _GstCopy      GstCopy;
typedef struct _GstCopyClass GstCopyClass;

struct _GstCopy
{
  GstBaseTransform element;
};

struct _GstCopyClass
{
  GstBaseTransformClass parent_class;
};

GType gst_copy_get_type (void);

gboolean gst_zip_src_register();

G_END_DECLS

gboolean qt_cam_copy_register();

#endif /* GST_COPY_H */
