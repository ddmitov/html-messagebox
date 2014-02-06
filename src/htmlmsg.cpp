
// HTML Message Box, v.0.1

// HTML Message Box is a small Qt4/5 program, that has only one purpose in life -
// to show nice HTML messageboxes from command line scripts.

// This software is licensed under the terms of GNU GPL v.3 and
// is provided without warranties of any kind!
// Dimitar D. Mitov, 2013, ddmitov (at) yahoo (dot) com

// REFERENCES:
// https://gitorious.org/qt-examples/qt-examples/source/sitespecificbrowser
// http://qt-project.org/doc/qt-5.0/qtwebkit/qwebsettings.html
// http://qt-project.org/faq/answer/is_there_a_way_i_can_get_rid_of_the_icon_in_the_mdi_child_windows_titlebar
// http://qt-project.org/forums/viewthread/23835
// http://qt-project.org/forums/viewthread/5318
// http://qt-project.org/forums/viewthread/8876
// http://www.qtcentre.org/threads/23094-cancle-right-click-context-menu-in-qtwebkit
// http://www.qtcentre.org/threads/46016-Place-QMessageBox-on-middle-of-screen
// http://www.qtcentre.org/threads/50214-stdin-reading
// http://developer.nokia.com/Community/Discussion/showthread.php/212357-How-to-disable-the-scrollbar-of-QWebView
// http://stackoverflow.com/questions/9685615/qt-and-linux-pipes
// http://stackoverflow.com/questions/14987007/what-is-the-expected-encoding-for-qwebviewsethtml
// http://stackoverflow.com/questions/10666998/qwebkit-display-local-webpage
// http://stackoverflow.com/questions/8026101/correct-way-to-quit-a-qt-program
// http://stackoverflow.com/questions/6955281/how-to-stop-qhttp-qtwebkit-from-caching-pages
// http://www.codeprogress.com/cpp/libraries/qt/qMessageBoxAutoClose.php

#include <QtGui>
#include <QtNetwork/QNetworkRequest>
#include <QWebFrame>
#include <QDesktopWidget>
#include <QShortcut>

#include "htmlmsg.h"

// Initialization of global variables for settings:
QString input;
int windowWidth;
int windowHeigth;
int timeoutSeconds;

int main (int argc, char **argv)
{
    QApplication app (argc, argv);

    input = QString (argv[1]);
    windowWidth = QString (argv[2]).toInt();
    windowHeigth = QString (argv[3]).toInt();
    timeoutSeconds = QString (argv[4]).toInt();

    if (input.length() < 1) {
        input = "htmlmsg.htm";
    }
    if (windowWidth < 50) {
        windowWidth = 400;
    }
    if (windowHeigth < 50) {
        windowHeigth = 200;
    }
    if (timeoutSeconds < 3){
        timeoutSeconds = 0;
    }

#if QT_VERSION >= 0x050000
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF8"));
#else
    QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF8"));
#endif
    QWebSettings::globalSettings() -> setDefaultTextEncoding (QString ("utf-8"));
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::PluginsEnabled, false);
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::SpatialNavigationEnabled, false);
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::LinksIncludedInFocusChain, false);
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::PrivateBrowsingEnabled, true);
    QWebSettings::globalSettings() -> setAttribute (QWebSettings::AutoLoadImages, true);
    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::setObjectCacheCapacities (0, 0, 0);
    QWebSettings::setMaximumPagesInCache (0);
    QWebSettings::clearMemoryCaches();

    TopLevel toplevel;

    QRect screenRect = QDesktopWidget().screen()->rect();
    toplevel.move (QPoint(screenRect.width()/2 - toplevel.width()/2,
                          screenRect.height()/2 - toplevel.height()/2));
    toplevel.setWindowFlags (Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    toplevel.show();
    app.exec();
};

Page::Page()
    : QWebPage(0)
{
}

TopLevel::TopLevel()
    : QWebView(0)
{
    main_page = new Page();
    setPage ( main_page );
    main_page -> setLinkDelegationPolicy (QWebPage::DelegateAllLinks);
    main_page -> mainFrame() -> setScrollBarPolicy (Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    main_page -> mainFrame() -> setScrollBarPolicy (Qt::Vertical, Qt::ScrollBarAlwaysOff);

    if (input != "stdin") {
        QObject::connect (main_page, SIGNAL (loadFinished(bool)),
                          this, SLOT (pageLoaded(bool)));
    }

    QShortcut *escapeShortcut = new QShortcut (Qt::Key_Escape, this);
    QObject::connect ( escapeShortcut, SIGNAL (activated()), this, SLOT (closeAppSlot()));

    QShortcut *enterShortcut = new QShortcut (Qt::Key_Return, this);
    QObject::connect ( enterShortcut, SIGNAL (activated()), this, SLOT (closeAppSlot()));

    setFixedSize (windowWidth, windowHeigth);
    setWindowFlags (Qt::FramelessWindowHint);
    setContextMenuPolicy (Qt::NoContextMenu);

    if (input == "stdin") {
        setWindowTitle ("Message");
    }

    QPixmap pix (16, 16);
    pix.fill (Qt::transparent);
    setWindowIcon (QIcon (pix));

    if (input != "stdin") {
        QUrl startUrl = "file://" +
                QApplication::applicationDirPath() +
                QDir::separator() + input;
        setUrl (startUrl);
        setFocus();
    }

    if (input == "stdin") {
        QTextStream qtin (stdin);
        QString input = qtin.readAll();
        setHtml (input);
    }

    if (timeoutSeconds > 0) {
        int timeoutMilliseconds = timeoutSeconds * 1000;
        QTimer::singleShot (timeoutMilliseconds, this, SLOT (closeAppSlot()));
    }

}

bool Page::acceptNavigationRequest(QWebFrame *frame,
                                   const QNetworkRequest &request,
                                   QWebPage::NavigationType type)
{
    QUrl CloseBase (QUrl ("local://close/" ));
    if (type == QWebPage::NavigationTypeLinkClicked) {
        if ( CloseBase.isParentOf (request.url())) {
            qApp->exit();
        }
    }
    if (type == QWebPage::NavigationTypeLinkClicked) {
        if ( !CloseBase.isParentOf (request.url())) {
            QDesktopServices::openUrl (request.url());
            return false;
        }
    }
    return QWebPage::acceptNavigationRequest (frame, request, type);
}
