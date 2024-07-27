#ifndef CHART_H
#define CHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QHBoxLayout>



class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent, QHBoxLayout *layout);
    ~Chart();

    quint16 xValue() const;
    void setXValue(quint16 newXValue);

signals:
    void  newMaxYDetected(double maxY);
    void newMinYDetected(double minY);

public slots:

    void clearChart();
    void changeSeries();
    void addNewSample(double point);


private:


    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QChartView *chartView;
    QHBoxLayout *layout;


    quint64 m_xValue;
    quint64 m_yValue;

    quint64 m_yMaxDetectecValue;
    quint64 m_yMinDetectecValue;
};

#endif // CHART_H
