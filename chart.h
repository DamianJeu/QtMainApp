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

    quint16 xValue() const;
    void setXValue(quint16 newXValue);

signals:

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


    quint16 m_xValue;
    quint16 m_yValue;
};

#endif // CHART_H
