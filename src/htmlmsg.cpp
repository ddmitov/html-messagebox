
// HTML Message Box, v.0.1

// HTML Message Box is a small Qt4/5 program, that has only one purpose in life -
// to show nice HTML messageboxes from command line scripts.

// This software is licensed under the terms of GNU GPL v.3 and
// is provided without warranties of any kind!
// Dimitar D. Mitov, 2013 - 2014, ddmitov (at) yahoo (dot) com

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
    QApplication app (argc, argv);

    // Display command line help if the program is started without arguments:
    QStringList arguments = QCoreApplication::arguments();
    if (arguments.count() < 2) {
        Settings settings;
        settings.displayHelp();
        return 1;
        QApplication::exit();
    }

    // Display command line help if '--help' argument is used:
    foreach (QString argument, arguments){
        if (argument.contains ("--help")) {
            Settings settings;
            settings.displayHelp();
            return 1;
            QApplication::exit();
        }
    }

#if QT_VERSION >= 0x050000
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF8"));
#else
    QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF8"));
#endif
    QWebSettings::globalSettings()->setDefaultTextEncoding (QString ("utf-8"));
    QWebSettings::globalSettings()->setAttribute (QWebSettings::PluginsEnabled, false);
    QWebSettings::globalSettings()->setAttribute (QWebSettings::JavascriptEnabled, true);
    QWebSettings::globalSettings()->setAttribute (QWebSettings::SpatialNavigationEnabled, false);
    QWebSettings::globalSettings()->setAttribute (QWebSettings::LinksIncludedInFocusChain, false);
    QWebSettings::globalSettings()->setAttribute (QWebSettings::PrivateBrowsingEnabled, true);
    QWebSettings::globalSettings()->setAttribute (QWebSettings::AutoLoadImages, true);
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
}

Settings::Settings()
    : QObject (0)
{

    // Read command line arguments:
    QStringList arguments = QCoreApplication::arguments();
    foreach (QString argument, arguments){
        if (argument.contains ("--input") or argument.contains ("-i")) {
            input = argument.section ("=", 1, 1);
        }
        if (argument.contains ("--width") or argument.contains ("-w")) {
            windowWidth = argument.section ("=", 1, 1).toInt();
        }
        if (argument.contains ("--heigth") or argument.contains ("-h")) {
            windowHeigth = argument.section ("=", 1, 1).toInt();
        }
        if (argument.contains ("--timeout") or argument.contains ("-t")) {
            timeoutSeconds = argument.section ("=", 1, 1).toInt();
        }
    }

    // Default values if command line arguments have values bellow minimal:
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

}

Page::Page()
    : QWebPage (0)
{

}

TopLevel::TopLevel()
    : QWebView (0)
{

    Settings settings;

    mainPage = new Page();
    setPage (mainPage);
    mainPage->setLinkDelegationPolicy (QWebPage::DelegateAllLinks);
    mainPage->mainFrame()->setScrollBarPolicy (Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    mainPage->mainFrame()->setScrollBarPolicy (Qt::Vertical, Qt::ScrollBarAlwaysOff);

    if (settings.input != "stdin") {
        QObject::connect (mainPage, SIGNAL (loadFinished(bool)),
                          this, SLOT (pageLoaded(bool)));
    }

    QShortcut *escapeShortcut = new QShortcut (Qt::Key_Escape, this);
    QObject::connect (escapeShortcut, SIGNAL (activated()), this, SLOT (closeAppSlot()));

    QShortcut *enterShortcut = new QShortcut (Qt::Key_Return, this);
    QObject::connect (enterShortcut, SIGNAL (activated()), this, SLOT (closeAppSlot()));

    setFixedSize (settings.windowWidth, settings.windowHeigth);
    setContextMenuPolicy (Qt::NoContextMenu);

    if (settings.input == "stdin") {
        setWindowTitle ("Message");
    }

    QPixmap pix (16, 16);
    pix.fill (Qt::transparent);
    setWindowIcon (QIcon (pix));

    if (settings.input != "stdin") {
        QUrl startUrl = "file://" +
                QApplication::applicationDirPath()+
                QDir::separator()+settings.input;
        setUrl (startUrl);
        setFocus();
    }

    if (settings.input == "stdin") {
        QTextStream qtin (stdin);
        QString input = qtin.readAll();
        setHtml (input);
    }

    if (settings.timeoutSeconds > 0) {
        int timeoutMilliseconds = settings.timeoutSeconds * 1000;
        QTimer::singleShot (timeoutMilliseconds, this, SLOT (closeAppSlot()));
    }

}

bool Page::acceptNavigationRequest (QWebFrame *frame,
                                   const QNetworkRequest &request,
                                   QWebPage::NavigationType navigationType)
{

    if (navigationType == QWebPage::NavigationTypeLinkClicked) {
        if (request.url().scheme().contains ("close")) {
            qApp->exit();
        } else {
            QDesktopServices::openUrl (request.url());
            return false;
        }
    }

    return QWebPage::acceptNavigationRequest (frame, request, navigationType);

}
