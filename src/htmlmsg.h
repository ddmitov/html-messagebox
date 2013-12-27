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
        qApp->exit();
    };

    void pageLoaded ( bool ok )
    {
        if ( ok )
        {
            setWindowTitle ( TopLevel::title() );
        }
    }

public:
    TopLevel();
    QTimer* timer;

private:
    Page *main_page;

};

#endif // HTMLMSG_H
