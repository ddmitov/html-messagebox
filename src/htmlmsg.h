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

#include <iostream> // for std::cout

#include <qglobal.h>
#include <QApplication>
#include <QtWidgets>
#include <QtNetwork/QNetworkRequest>
#include <QWebPage>
#include <QWebFrame>
#include <QWebView>
#include <QWebElement>

// ==============================
// FILE READER CLASS DEFINITION:
// Usefull for both files inside binary resources and files on disk
// ==============================
class QFileReader : public QObject
{
    Q_OBJECT

public:
    explicit QFileReader(QString filePath);
    QString fileContents;
};

// ==============================
// PAGE CLASS CONSTRUCTOR:
// ==============================
class QPage : public QWebPage
{
    Q_OBJECT

public:
    QPage();

protected:
    bool acceptNavigationRequest(QWebFrame *frame,
                                 const QNetworkRequest &request,
                                 QWebPage::NavigationType navigationType)
    {
        Q_UNUSED(frame);

        // '<a href="close://">Close</a>' will close the message box:
        if (navigationType == QWebPage::NavigationTypeLinkClicked) {
            if (request.url().scheme() == "close") {
                QApplication::exit();
            } else {
                // All other links will be handled by
                // the default applications of the operating system:
                QDesktopServices::openUrl (request.url());
            }
        }

        return false;
    }
};

// ==============================
// VIEW CLASS DEFINITION:
// ==============================
class QView : public QWebView
{
    Q_OBJECT

public slots:
    void qChangeTitleSlot()
    {
        setWindowTitle(QView::title());
    }

    void qReadStdin()
    {
        QTextStream qtin(stdin);
        QString input;

        if (qtin.status() == QTextStream::Ok) {
            input = qtin.readLine();
            qtin.flush();
        }

        if (input.length() > 0) {
            QWebElement targetDomElement =
                    mainPage->
                    mainFrame()->documentElement().findFirst("#stdin");

            if (!targetDomElement.isNull()) {
                targetDomElement.setInnerXml(input);
            } else {
                mainPage->mainFrame()->setHtml(input);
            }
        }
    }

    void qCloseApplicationSlot()
    {
        QApplication::exit();
    }

public:
    QView();
    QWebPage *mainPage;

};

#endif // HTMLMSG_H
