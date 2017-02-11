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

#include <qglobal.h>

#if QT_VERSION >= 0x050000
// Qt5 code:
#include <QtWidgets>
#else
// Qt4 code:
#include <QtGui>
#endif

#include <QApplication>
#include <QtNetwork/QNetworkRequest>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>
#include <iostream> // std::cout
#include <unistd.h> // isatty()
#include "htmlmsg.h"

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    application.setApplicationName("HTML Message Box");
    application.setApplicationVersion("0.2.0");

#if QT_VERSION >= 0x050000
    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("UTF8"));
#else
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName ("UTF8"));
#endif

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

    // Initiate the message box:
    QWebViewWindow window;

    // External HTML message template:
    QString externalHtmlFilePath =
            application.applicationDirPath() +
            QDir::separator() + "htmlmsg.html";

    QFile externalHtmlFile(externalHtmlFilePath);
    if (externalHtmlFile.exists()) {
        // Load the external HTML message template:
        QFileReader *resourceReader =
                new QFileReader(externalHtmlFilePath);
        QString htmlContent = resourceReader->fileContents;

        window.mainPage->mainFrame()->setHtml(htmlContent);
    } else {
        // Load the embedded HTML message template:
        QFileReader *resourceReader =
                new QFileReader(QString(":/htmlmsg.html"));
        QString htmlContent = resourceReader->fileContents;

        window.mainPage->mainFrame()->setHtml(htmlContent);
    }

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
    stdinNotifier->setEnabled(true);

    // Display the message box:
    window.show();
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
// WEB PAGE CLASS CONSTRUCTOR:
// ==============================
QPage::QPage()
    : QWebPage (0)
{
    QWebSettings::globalSettings()->setDefaultTextEncoding(QString("utf-8"));

    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::PluginsEnabled, false);

    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::AutoLoadImages, true);
    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->
            setAttribute(QWebSettings::PrivateBrowsingEnabled, true);

    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::setObjectCacheCapacities (0, 0, 0);
    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::clearMemoryCaches();
}

// ==============================
// WEB VIEW CLASS CONSTRUCTOR:
// ==============================
QWebViewWindow::QWebViewWindow()
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

    setWindowFlags(Qt::WindowStaysOnTopHint);

    // Set an empty transparent icon:
    QPixmap pix(16, 16);
    pix.fill(Qt::transparent);
    setWindowIcon(QIcon(pix));

    // Signal and slot for changing the window title:
    QObject::connect(mainPage, SIGNAL(loadFinished(bool)),
                     this, SLOT(qChangeTitleSlot()));
}

bool QPage::acceptNavigationRequest(QWebFrame *frame,
                                    const QNetworkRequest &request,
                                    QWebPage::NavigationType navigationType)
{
    Q_UNUSED(frame);

    if (navigationType == QWebPage::NavigationTypeLinkClicked) {
        if (request.url().scheme() == "close") {
            QApplication::exit();
        } else {
            QDesktopServices::openUrl (request.url());
        }
    }

    return false;
}
