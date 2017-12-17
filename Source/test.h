#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QChartView>

QT_CHARTS_USE_NAMESPACE

class Graph
{
public:
    Graph(const QString &title, const QList<double> &data);
    ~Graph();
    void saveAsImage(const QString &fileName) const;

private:
    QChartView *mChartViewPtr;

};

class Test
{
    friend class Experiment;

public:
    static Test *instance();
    ~Test();

    void newExecTime(double execTime);
    void saveResults(const QString &location) const;

private:
    Test() = default;
    static Test *mInstancePtr;

    QList<double> mExecTimes;

};

#endif // EXPERIMENT_H
