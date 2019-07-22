/***********************************************************************
***********Copyright (c) 2009-2012 The Bitcoin developers***************
******************Copyright (c) 2010-2019 Nur1Labs**********************
>Distributed under the MIT/X11 software license, see the accompanying
>file COPYING or http://www.opensource.org/licenses/mit-license.php.
************************************************************************/

#ifndef MUBDI_QT_TRAFFICGRAPHWIDGET_H
#define MUBDI_QT_TRAFFICGRAPHWIDGET_H

#include <QQueue>
#include <QWidget>

class ClientModel;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QTimer;
QT_END_NAMESPACE

class TrafficGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrafficGraphWidget(QWidget* parent = 0);
    void setClientModel(ClientModel* model);
    int getGraphRangeMins() const;

protected:
    void paintEvent(QPaintEvent*);

public slots:
    void updateRates();
    void setGraphRangeMins(int mins);
    void clear();

private:
    void paintPath(QPainterPath& path, QQueue<float>& samples);

    QTimer* timer;
    float fMax;
    int nMins;
    QQueue<float> vSamplesIn;
    QQueue<float> vSamplesOut;
    quint64 nLastBytesIn;
    quint64 nLastBytesOut;
    ClientModel* clientModel;
};

#endif /* MUBDI_QT_TRAFFICGRAPHWIDGET_H */