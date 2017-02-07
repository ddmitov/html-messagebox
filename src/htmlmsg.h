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
#include <QWebFrame>
#include <QWebView>
#include <QWebElement>
#include <iostream> // for std::cout

class QPage : public QWebPage
{
    Q_OBJECT

public:
    QPage();

protected:
    bool acceptNavigationRequest(QWebFrame *frame,
                                 const QNetworkRequest &request,
                                 QWebPage::NavigationType navigationType);
};

class QWebViewWindow : public QWebView
{
    Q_OBJECT

public slots:
    void qReadStdin()
    {
        QTextStream qtin(stdin);
        QString input;

        if (qtin.status() == QTextStream::Ok) {
            input = qtin.readLine();
            qtin.flush();
        }

        QWebElement targetDomElement =
                mainPage->mainFrame()->documentElement().findFirst("#stdin");

        if (!targetDomElement.isNull()) {
            targetDomElement.setInnerXml(input);
        } else {
            mainPage->mainFrame()->setHtml(input);
        }
    }

    void qCloseApplicationSlot()
    {
        QApplication::exit();
    }

public:
    QWebViewWindow();
    QWebPage *mainPage;

};

#endif // HTMLMSG_H
