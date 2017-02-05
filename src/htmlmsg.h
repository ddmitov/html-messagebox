/*
 HTML Message Box

 This program is free software;
 you can redistribute it and/or modify it under the terms of the
 GNU Lesser General Public License,
 as published by the Free Software Foundation;
 either version 3 of the License, or (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.
 Dimitar D. Mitov, 2013 - 2017
 https://github.com/ddmitov/html-messagebox
*/

#ifndef HTMLMSG_H
#define HTMLMSG_H

#include <qglobal.h>

#if QT_VERSION >= 0x050000
// Qt5 code:
#include <QtWidgets>
#else
// Qt4 code:
#include <QtGui>
#endif

#include <QApplication>
#include <QWebPage>
#include <QWebView>
#include <iostream> // for std::cout

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings();

    int windowWidth;
    int windowHeigth;
    int timeoutSeconds;

public slots:
    void displayHelp()
    {
        // Display help:
        std::cout << " " << std::endl;
        std::cout << "HTML Message Box v.0.1" << std::endl;
        std::cout << "Executable: "
                  << (QDir::toNativeSeparators (
                          QApplication::applicationFilePath())
                      .toLatin1().constData())
                  << std::endl;
        std::cout << "Qt version: " << QT_VERSION_STR << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "  htmlmsg --option=value -o=value" << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Command line options:" << std::endl;
        std::cout << "  --width   -w    "
                  << "message width in points. Minimum: 200 points"
                  << std::endl;
        std::cout << "  --heigth  -h    "
                  << "message heigth in points. Minimum: 200 points"
                  << std::endl;
        std::cout << "  --timeout -t    "
                  << "timeout in seconds. "
                  << "Less than 3 seconds means no timeout."
                  << std::endl;
        std::cout << "  --help          this help"
                  << std::endl;
        std::cout << " " << std::endl;
        QApplication::exit();
    }
};

class Page : public QWebPage
{
    Q_OBJECT

public:
    Page();

protected:

    bool acceptNavigationRequest (QWebFrame *frame,
                                  const QNetworkRequest &request,
                                  QWebPage::NavigationType navigationType);
};

class TopLevel : public QWebView
{
    Q_OBJECT

public slots:
    void closeAppSlot()
    {
        qApp->exit();
    }

public:
    TopLevel();

private:
    Page *mainPage;
};

#endif // HTMLMSG_H
