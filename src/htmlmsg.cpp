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

#include <iostream> // std::cout
#include <unistd.h> // isatty()

#include <qglobal.h>
#include <QApplication>
#include <QtWidgets>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>

#include "htmlmsg.h"

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    application.setApplicationName("HTML Message Box");
    application.setApplicationVersion("0.2.0");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("UTF8"));

    int windowWidth = 400;
    int windowHeigth = 200;
    int timeoutSeconds = 0;

    // Read command line arguments:
    QStringList arguments = QCoreApplication::arguments();

    foreach (QString argument, arguments) {
        if (argument.contains("--help")) {
            std::cout << " " << std::endl;
            std::cout << qApp->applicationName().toLatin1().constData()
                      << " version "
                      << qApp->applicationVersion().toLatin1().constData()
                      << std::endl;
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
            std::cout << "Options:" << std::endl;
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
            return 0;
        }

        if (argument.contains("--width") or argument.contains ("-w")) {
            if (argument.section ("=", 1, 1).toInt() > 200) {
                windowWidth = argument.section ("=", 1, 1).toInt();
                application.setProperty("windowWidth", windowWidth);
            }
        }

        if (argument.contains("--heigth") or argument.contains ("-h")) {
            if (argument.section ("=", 1, 1).toInt() > 200) {
                windowHeigth = argument.section ("=", 1, 1).toInt();
                application.setProperty("windowHeigth", windowHeigth);
            }
        }

        if (argument.contains("--timeout") or argument.contains ("-t")) {
            if (argument.section ("=", 1, 1).toInt() > 3) {
                timeoutSeconds = argument.section ("=", 1, 1).toInt();
                application.setProperty("timeoutSeconds", timeoutSeconds);
            }
        }
    }

    // Message box icon:
    QString externalIconFilePath =
            application.applicationDirPath() +
            QDir::separator() + "htmlmsg.png";
    QFile iconFile(externalIconFilePath);
    QPixmap icon(32, 32);

    if (iconFile.exists()) {
        // Set the external icon:
        icon.load(externalIconFilePath);
        QApplication::setWindowIcon(icon);
    } else {
        // Set the embedded icon:
        icon.load(":/htmlmsg.png");
        QApplication::setWindowIcon(icon);
    }

    // HTML message template:
    QString htmlFolder =
            application.applicationDirPath() + QDir::separator() + "msgbox";
    QString htmlFilePath =
            htmlFolder + QDir::separator() + "htmlmsg.html";
    QFile htmlFile(htmlFilePath);
    QString htmlContent;

    if (htmlFile.exists()) {
        // Load the external HTML message template:
        QFileReader *resourceReader =
                new QFileReader(htmlFilePath);
        htmlContent = resourceReader->fileContents;

        htmlContent.replace("local://",
                            QString(
                                "file://" + htmlFolder + QDir::separator()));
    } else {
        // Load the embedded HTML message template:
        QFileReader *resourceReader =
                new QFileReader(QString(":/htmlmsg.html"));
        htmlContent = resourceReader->fileContents;
    }

    // Initiate the message box and load the HTML message:
    QView window;
    window.mainPage->mainFrame()->setHtml(htmlContent);

    // Read any initial STDIN data:
    if (isatty(fileno(stdin))) {
        window.qReadStdin();
    }

    // Set the message box dimensions and center it on screen:
    window.setFixedSize (windowWidth, windowHeigth);
    QRect screenRect = QDesktopWidget().screen()->rect();
    window.move (QPoint(
                       screenRect.width()/2 - windowWidth/2,
                       screenRect.height()/2 - windowHeigth/2));

    // Display the message box:
    window.show();

    // Set the message box timeout:
    if (timeoutSeconds > 0) {
        int timeoutMilliseconds = timeoutSeconds * 1000;
        QTimer *timer = new QTimer();
        timer->singleShot (timeoutMilliseconds,
                           &window, SLOT (qCloseApplicationSlot()));
    }

    // Set the STDIN watcher:
    QSocketNotifier *stdinNotifier =
            new QSocketNotifier(fileno(stdin), QSocketNotifier::Read);
    QObject::connect(stdinNotifier, SIGNAL(activated(int)),
                     &window, SLOT(qReadStdin()));

    if (isatty(fileno(stdin))) {
        stdinNotifier->setEnabled(true);
    }

    application.exec();
}

// ==============================
// FILE READER CONSTRUCTOR:
// Usefull for both files inside binary resources and files on disk
// ==============================
QFileReader::QFileReader(QString filePath)
    : QObject(0)
{
    QString fileName(filePath);
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream fileStream(&file);
    fileContents = fileStream.readAll();
    file.close();
}

// ==============================
// PAGE CLASS CONSTRUCTOR:
// ==============================
QPage::QPage()
    : QWebPage (0)
{
    QWebSettings::globalSettings()->setDefaultTextEncoding(QString("utf-8"));

    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::JavascriptEnabled, false);
    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);
    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::PluginsEnabled, false);

    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::AutoLoadImages, true);

    // All links are handled by the application itself:
    setLinkDelegationPolicy(QWebPage::DontDelegateLinks);

    // Disable cache:
    QWebSettings::setMaximumPagesInCache(0);
    QWebSettings::setObjectCacheCapacities(0, 0, 0);
}

// ==============================
// WINDOW CLASS CONSTRUCTOR:
// ==============================
QView::QView()
    : QWebView (0)
{
    mainPage = new QPage();
    mainPage->setLinkDelegationPolicy (QWebPage::DelegateAllLinks);

    // Disable scrolbars:
    mainPage->mainFrame()->setScrollBarPolicy(Qt::Horizontal,
                                              Qt::ScrollBarAlwaysOff);
    mainPage->mainFrame()->setScrollBarPolicy(Qt::Vertical,
                                              Qt::ScrollBarAlwaysOff);

    setPage(mainPage);

    // Disable context menu:
    setContextMenuPolicy(Qt::NoContextMenu);

    // The message box is always on top of all other windows:
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // Signal and slot for setting the HTML title as the window title:
    QObject::connect(mainPage, SIGNAL(loadFinished(bool)),
                     this, SLOT(qChangeTitleSlot()));
}
