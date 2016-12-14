# HTML Message Box Project File

# This program is free software;
# you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License,
# as published by the Free Software Foundation;
# either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.
# Dimitar D. Mitov, 2013 - 2016
# https://github.com/ddmitov/html-messagebox

TEMPLATE = app
TARGET = htmlmsg
DEPENDPATH += .

lessThan (QT_MAJOR_VERSION, 5) {
  QT += webkit
}

greaterThan (QT_MAJOR_VERSION, 4) {
  QT += widgets webkitwidgets
  DEFINES += HAVE_QT5
}

HEADERS += htmlmsg.h
SOURCES += htmlmsg.cpp
