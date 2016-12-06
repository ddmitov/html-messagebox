
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
