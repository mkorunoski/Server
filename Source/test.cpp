#include "test.h"

#include <algorithm>

#include <QDebug>
#include <QLineSeries>
#include <QValueAxis>
#include <QPainter>

Graph::Graph(const QString &title, const QList<double> &data)
    : mChartViewPtr(nullptr)
{
    QLineSeries *seriesPtr = new QLineSeries;
    int i = 1;
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
        seriesPtr->append(i++, *it);
    }

    QChart *chartPtr = new QChart;
    chartPtr->legend()->hide();
    chartPtr->addSeries(seriesPtr);
    chartPtr->setTitle(title);

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Run");
    axisX->setLabelFormat("%d");
    axisX->setTickCount(8);
    chartPtr->addAxis(axisX, Qt::AlignBottom);
    seriesPtr->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Execution time");
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(8);
    chartPtr->addAxis(axisY, Qt::AlignLeft);
    seriesPtr->attachAxis(axisY);

    mChartViewPtr = new QChartView(chartPtr);
    mChartViewPtr->resize(800, 600);
    mChartViewPtr->setRenderHint(QPainter::Antialiasing);
}

Graph::~Graph()
{
    delete mChartViewPtr;
}

void Graph::saveAsImage(const QString &fileName) const
{
    QPixmap pixmap = mChartViewPtr->grab();

    QFile file(fileName);
    file.open(QFile::WriteOnly);
    pixmap.save(&file, "png");
}

Test* Test::mInstancePtr = nullptr;

Test *Test::instance()
{
    if (!mInstancePtr)
        mInstancePtr = new Test;

    return mInstancePtr;
}

Test::~Test()
{
    delete mInstancePtr;
}

void Test::newExecTime(double execTime)
{
    mExecTimes.append(execTime);
}

void Test::saveResults(const QString &location) const
{
    Graph graph(QString("Server execution time."), mExecTimes);
    graph.saveAsImage(location + "/graph.png");
}
