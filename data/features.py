#!/usr/bin/python

import ConfigParser
import re
import string

cfg = ConfigParser.ConfigParser()
cfg.read("features.txt")

PROP = """  Q_PROPERTY(%TYPE% %NAME% READ %NAME% CONSTANT);"""
DECL = """  %TYPE% %NAME%() const;"""
DEF_STR = """
%TYPE% DeviceFeatures::%NAME() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("%KEY%")).toList().contains("%VALUE%");
}"""

DEF_NUM = """
%TYPE% DeviceFeatures::%NAME() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("%KEY%")).toList().size();
}"""

DEF_BOOL = """
%TYPE% DeviceFeatures::%NAME() const {
  return m_settings->value(QString("%1/%2").arg(m_id).arg("%KEY%"), false).toBool();
}"""

def name(val):
    return "".join(string.capwords(val.replace("-", " ")).split(" "))

def single(val):
    return name(val)[:-1]

props = []
decls = []
defs = []

for section in cfg.sections():
    if cfg.has_option(section, "values"):
        for value in cfg.get(section, "values").split(","):
            prop_name = "is%s%sSupported" % (name(value), single(section))
            props.append(PROP.replace("%TYPE%", "bool").replace("%NAME%", prop_name))
            decls.append(DECL.replace("%TYPE%", "bool").replace("%NAME%", prop_name))
            defs.append(DEF_STR.replace("%TYPE%", "bool").replace("%NAME", prop_name).replace("%KEY%", section).replace("%VALUE%", value))
        num_name = "numberOf%s" % name(section)
        props.append(PROP.replace("%TYPE%", "int").replace("%NAME%", num_name))
        decls.append(DECL.replace("%TYPE%", "int").replace("%NAME%", num_name))
        defs.append(DEF_NUM.replace("%TYPE%", "int").replace("%NAME", num_name).replace("%KEY%", section))
    else:
        prop_name = "is%sSupported" % (name(section))
        props.append(PROP.replace("%TYPE%", "bool").replace("%NAME%", prop_name))
        decls.append(DECL.replace("%TYPE%", "bool").replace("%NAME%", prop_name))
        defs.append(DEF_BOOL.replace("%TYPE%", "bool").replace("%NAME", prop_name).replace("%KEY%", section).replace("%VALUE%", value))

print "\n".join(props)
print "\n".join(decls)
print "\n".join(defs)
