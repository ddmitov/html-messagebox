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
 Dimitar D. Mitov, 2013 - 2016
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
#include <QWebPage>
#include <QWebView>
#include <QWebFrame>
#include <QtNetwork/QNetworkRequest>
#include "htmlmsg.h"

int main (int argc, char **argv)
{
    QApplication application (argc, argv);

    // Display command line help if the program is started without arguments:
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.count() < 2) {
        Settings settings;
        settings.displayHelp();
        return 0;
    }

    // Display command line help if '--help' argument is used:
    foreach (QString argument, arguments){
        if (argument.contains ("--help")) {
            Settings settings;
            settings.displayHelp();
            return 0;
        }
    }

#if QT_VERSION >= 0x050000
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF8"));
#else
    QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF8"));
#endif

    TopLevel toplevel;
    toplevel.show();
    application.exec();
}

Settings::Settings()
    : QObject (0)
{
    windowWidth = 200;
    windowHeigth = 200;
    timeoutSeconds = 0;

    // Read command line arguments:
    QStringList arguments = QCoreApplication::arguments();
    foreach (QString argument, arguments){
        if (argument.contains ("--width") or argument.contains ("-w")) {
            if (argument.section ("=", 1, 1).toInt() > 200) {
                windowWidth = argument.section ("=", 1, 1).toInt();
            }
        }
        if (argument.contains ("--heigth") or argument.contains ("-h")) {
            if (argument.section ("=", 1, 1).toInt() > 200) {
                windowHeigth = argument.section ("=", 1, 1).toInt();
            }
        }
        if (argument.contains ("--timeout") or argument.contains ("-t")) {
            if (argument.section ("=", 1, 1).toInt() > 3) {
                timeoutSeconds = argument.section ("=", 1, 1).toInt();
            }
        }
    }
}

Page::Page()
    : QWebPage (0)
{
    QWebSettings::globalSettings()->setDefaultTextEncoding (QString ("utf-8"));
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::PluginsEnabled, false);
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::SpatialNavigationEnabled, false);
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::LinksIncludedInFocusChain, false);
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::PrivateBrowsingEnabled, true);
    QWebSettings::globalSettings()->
            setAttribute (QWebSettings::AutoLoadImages, true);
    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::setObjectCacheCapacities (0, 0, 0);
    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::clearMemoryCaches();
}

TopLevel::TopLevel()
    : QWebView (0)
{
    Settings settings;

    mainPage = new Page();
    setPage (mainPage);
    mainPage->setLinkDelegationPolicy (QWebPage::DelegateAllLinks);
    mainPage->mainFrame()->
            setScrollBarPolicy (Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainPage->mainFrame()->
            setScrollBarPolicy (Qt::Vertical, Qt::ScrollBarAlwaysOff);

    setContextMenuPolicy (Qt::NoContextMenu);
    setWindowFlags (Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    setFixedSize (settings.windowWidth, settings.windowHeigth);
    QRect screenRect = QDesktopWidget().screen()->rect();
    move (QPoint(screenRect.width()/2 - width()/2,
                          screenRect.height()/2 - height()/2));

    setWindowTitle ("Message");

    QPixmap pix (16, 16);
    pix.fill (Qt::transparent);
    setWindowIcon (QIcon (pix));

    if (settings.timeoutSeconds > 0) {
        int timeoutMilliseconds = settings.timeoutSeconds * 1000;
        QTimer *timer = new QTimer();
        timer->singleShot (timeoutMilliseconds, this, SLOT (closeAppSlot()));
    }

    QTextStream qtin(stdin);
    QString input = qtin.readAll();
    setHtml (input);
}

bool Page::acceptNavigationRequest (QWebFrame *frame,
                                   const QNetworkRequest &request,
                                   QWebPage::NavigationType navigationType)
{
    Q_UNUSED(frame);

    if (navigationType == QWebPage::NavigationTypeLinkClicked) {
        if (request.url().scheme() == "close") {
            qApp->exit();
        } else {
            QDesktopServices::openUrl (request.url());
        }
    }

    return false;
}
