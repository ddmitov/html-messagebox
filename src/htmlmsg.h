#ifndef HTMLMSG_H
#define HTMLMSG_H

#include <QApplication>
#include <QtGui>
#include <QWebPage>
#include <QWebView>

class Page : public QWebPage
{
    Q_OBJECT

public:
    Page();

protected:
    bool acceptNavigationRequest( QWebFrame *frame,
                                  const QNetworkRequest &request,
                                  QWebPage::NavigationType type );
};

class TopLevel : public QWebView
{
    Q_OBJECT

public slots:

    void closeAppSlot()
    {
        QFile::remove ( QDir::tempPath() + "/output.htm" );
        qApp->exit();
    };

public:
    TopLevel();
    QTimer* timer;
private:
    Page *main_page;
};

#endif // HTMLMSG_H
