/***********************************************************************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_QT_NETWORKSTYLE_H
#define MUBDI_QT_NETWORKSTYLE_H

#include <QIcon>
#include <QPixmap>
#include <QString>

/* Coin network-specific GUI style information */
class NetworkStyle
{
public:
    /** Get style associated with provided BIP70 network id, or 0 if not known */
    static const NetworkStyle* instantiate(const QString& networkId);

    const QString& getAppName() const { return appName; }
    const QIcon& getAppIcon() const { return appIcon; }
    const QString& getTitleAddText() const { return titleAddText; }
    const QPixmap& getSplashImage() const { return splashImage; }

private:
    NetworkStyle(const QString& appName, const QString& appIcon, const char* titleAddText, const QString& splashImage);

    QString appName;
    QIcon appIcon;
    QString titleAddText;
    QPixmap splashImage;
};

#endif /* MUBDI_QT_NETWORKSTYLE_H */