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

#include "qtcampropertysetter.h"
#include "qtcamdevice_p.h"
#include <QSettings>
#include <QDebug>

class QtCamPropertySetterPrivate {
public:
  void binAdded(GstElement *bin) {
    g_signal_connect(bin, "element-added",
		     G_CALLBACK(QtCamPropertySetterPrivate::element_added), this);

    setProperties(bin);

    // Let's traverse its children:
    GstIterator *iter = gst_bin_iterate_elements(GST_BIN(bin));
    if (!iter) {
      return;
    }

    bool done = false;

    GstElement *elem = 0;

#if GST_CHECK_VERSION(1,0,0)
    GValue val = G_VALUE_INIT;
#endif

    while (!done) {
#if GST_CHECK_VERSION(1,0,0)
      switch (gst_iterator_next(iter, &val)) {
#else
      switch (gst_iterator_next(iter, (gpointer *)&elem)) {
#endif
      case GST_ITERATOR_OK:
#if GST_CHECK_VERSION(1,0,0)
	elem = (GstElement *)g_value_get_object (&val);
#endif
	if (GST_IS_BIN(elem)) {
	  binAdded(elem);
	}
	else {
	  setProperties(elem);
	}
#if GST_CHECK_VERSION(1,0,0)
	g_value_reset (&val);
#else
	gst_object_unref(elem);
#endif
	break;

      case GST_ITERATOR_RESYNC:
	gst_iterator_resync(iter);
	break;

      case GST_ITERATOR_ERROR:
      case GST_ITERATOR_DONE:
	done = true;
	break;
      }
    }

    gst_iterator_free(iter);
  }

  void setProperties(GstElement *element) {
    QLatin1String name = elementName(element);
    if (!name.latin1()) {
      return;
    }

    QVariantMap map = properties(name);
    if (map.isEmpty()) {
      return;
    }

    QStringList keys = map.keys();
    foreach (const QString& key, keys) {
      QVariant value = map[key];
      GParamSpec *pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(element),
						       key.toUtf8().constData());
      if (!pspec) {
	qWarning() << "Property" << key << "not available for" << name;
	continue;
      }

      GType type = getType(pspec->value_type);

      switch (type) {
      case G_TYPE_INT:
      case G_TYPE_ENUM:
	setInt(element, key, value);
	break;
      case G_TYPE_UINT:
	setUnsignedInt(element, key, value);
	break;
      case G_TYPE_STRING:
	setString(element, key, value);
	break;
      case G_TYPE_BOOLEAN:
	setBoolean(element, key, value);
	break;
      default:
	if (type == gstFraction) {
	  setFraction(element, key, value);
	  break;
	}

	qWarning() << "Unsupported property type"
		   << g_type_name(pspec->value_type) << "of parent" << g_type_name(type)
		   << "for" << key << "of element" << name;
	break;
      }
    }
  }

  QVariantMap properties(const QLatin1String& name) {
    QVariantMap map;

    conf->beginGroup(name);

    QStringList keys = conf->allKeys();
    foreach (const QString& key, keys) {
      map[key] = conf->value(key);
    }

    conf->endGroup();
    return map;
  }

  QLatin1String elementName(GstElement *elem) {
    GstElementFactory *factory = gst_element_get_factory(elem);
    if (!factory) {
      return QLatin1String(0);
    }

    const char *n = gst_plugin_feature_get_name(GST_PLUGIN_FEATURE(factory));
    if (!n) {
      return QLatin1String(0);
    }

    QLatin1String name(n);

    return name;
  }

  void setInt(GstElement *elem, const QString& key, const QVariant& value) {
    if (!value.canConvert(QVariant::Int)) {
      qWarning() << "Cannot convert" << value << "to int";
      return;
    }

    g_object_set(elem, key.toUtf8().constData(), value.toInt(), NULL);
  }

  void setUnsignedInt(GstElement *elem, const QString& key, const QVariant& value) {
    if (!value.canConvert(QVariant::UInt)) {
      qWarning() << "Cannot convert" << value << "to unsigned int";
      return;
    }

    g_object_set(elem, key.toUtf8().constData(), value.toUInt(), NULL);
  }

  void setString(GstElement *elem, const QString& key, const QVariant& value) {
    if (!value.canConvert(QVariant::String)) {
      qWarning() << "Cannot convert" << value << "to string";
      return;
    }

    g_object_set(elem, key.toUtf8().constData(), value.toString().toUtf8().constData(), NULL);
  }

  void setBoolean(GstElement *elem, const QString& key, const QVariant& value) {
    if (!value.canConvert(QVariant::Bool)) {
      qWarning() << "Cannot convert" << value << "to bool";
      return;
    }

    g_object_set(elem, key.toUtf8().constData(), value.toBool() ? TRUE : FALSE, NULL);
  }

  void setFraction(GstElement *elem, const QString& key, const QVariant& value) {
    if (!value.canConvert(QVariant::List)) {
      qWarning() << "Cannot convert" << value << "to list";
      return;
    }

    QList<QVariant> list = value.toList();
    if (list.size() != 2) {
      qWarning() << "fraction list must contain 2 items";
      return;
    }

    if (!list[0].canConvert(QVariant::Int) || !list[1].canConvert(QVariant::Int)) {
      qWarning() << "list items cannot be converted to int";
      return;
    }

    int val1 = list[0].toInt();
    int val2 = list[1].toInt();

    g_object_set(elem, key.toUtf8().constData(), val1, val2, NULL);
  }

  GType getType(GType parent) {
    if (G_TYPE_IS_FUNDAMENTAL(parent)) {
      return parent;
    }

    return getType(g_type_parent(parent));
  }

  static void element_added(GstBin *bin, GstElement *child, gpointer user_data) {
    Q_UNUSED(bin);

    QtCamPropertySetterPrivate *d = static_cast<QtCamPropertySetterPrivate *>(user_data);

    if (GST_IS_BIN(child)) {
      d->binAdded(child);
    }
    else {
      d->setProperties(child);
    }
  }

  QSettings *conf;
  GType gstFraction;
};

QtCamPropertySetter::QtCamPropertySetter(QtCamDevicePrivate *pvt) :
  d_ptr(new QtCamPropertySetterPrivate) {
  d_ptr->gstFraction = GST_TYPE_FRACTION;

  d_ptr->conf = new QSettings(pvt->conf->lookUp("properties.ini"), QSettings::IniFormat);

  d_ptr->binAdded(pvt->cameraBin);
}

QtCamPropertySetter::~QtCamPropertySetter() {
  delete d_ptr->conf; d_ptr->conf = 0;
  delete d_ptr; d_ptr = 0;
}
