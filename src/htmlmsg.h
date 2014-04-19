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

    QString input;
    int windowWidth;
    int windowHeigth;
    int timeoutSeconds;

public slots:

    void displayHelp()
    {
        // Get application start date and time:
        QString applicationStart = QDateTime::currentDateTime().toString ("dd.MM.yyyy hh:mm:ss");
        // Display help:
        std::cout << " " << std::endl;
        std::cout << "HTML Message Box v.0.1 started on: "
                  << applicationStart.toLatin1().constData() << std::endl;
        std::cout << "Application file path: "
                  << (QDir::toNativeSeparators (
                          QApplication::applicationFilePath()).toLatin1().constData())
                  << std::endl;
        std::cout << "Qt WebKit version: " << QTWEBKIT_VERSION_STR << std::endl;
        std::cout << "Qt version: " << QT_VERSION_STR << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "  htmlmsg --option=value -o=value" << std::endl;
        std::cout << " " << std::endl;
        std::cout << "Command line options:" << std::endl;
        std::cout << "  --input   -i    input: 'stdin' or file path"
                  << std::endl;
        std::cout << "  --width   -w    message width in points. Minimum: 50 points"
                  << std::endl;
        std::cout << "  --heigth  -h    message heigth in points. Minimum: 50 points"
                  << std::endl;
        std::cout << "  --timeout -t    timeout in seconds. Minimum: 3 seconds. 0 means no timeout."
                  << std::endl;
        std::cout << "  --help          this help"
                  << std::endl;
        std::cout << " " << std::endl;
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

    void pageLoaded (bool ok)
    {
        if (ok) {
            setWindowTitle (TopLevel::title());
        }
    }

public:

    TopLevel();

    QTimer *timer;

private:

    Page *mainPage;

};

#endif // HTMLMSG_H
