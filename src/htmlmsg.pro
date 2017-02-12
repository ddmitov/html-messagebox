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
# Dimitar D. Mitov, 2013 - 2017
# https://github.com/ddmitov/html-messagebox

win32 {
    error ("HTML Message Box does not support the Windows platform.")
}

message ("Going to configure HTML Message Box for Qt $$[QT_VERSION]")

lessThan (QT_MAJOR_VERSION, 5) {
    error ("HTML Message Box requires Qt versions 5.0 to 5.5.")
}

equals (QT_MAJOR_VERSION, 5) {
    greaterThan (QT_MINOR_VERSION, 5) {
        error ("HTML Message Box requires Qt versions 5.0 to 5.5.")
    }

    message ("Qt Header files: $$[QT_INSTALL_HEADERS]")
    message ("Qt Libraries: $$[QT_INSTALL_LIBS]")

    TEMPLATE = app
    TARGET = htmlmsg
    DEPENDPATH += .

    QT += widgets webkitwidgets
    DEFINES += HAVE_QT5

    HEADERS += htmlmsg.h
    SOURCES += htmlmsg.cpp

    # Resources:
    RESOURCES += resources/htmlmsg.qrc

    # Destination directory for the compiled binary:
    DESTDIR = $$PWD/../

    # Temporary folder:
    MOC_DIR = tmp
    OBJECTS_DIR = tmp
    RCC_DIR = tmp
}
