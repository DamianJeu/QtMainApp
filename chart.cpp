#include "chart.h"

Chart::Chart(QWidget *parent, QHBoxLayout *layout)
    : QWidget{parent}, chart{new QChart()}, series{new QLineSeries(chart)}, series2{new QLineSeries(chart)}, scatterSeries{new QScatterSeries(chart)}, axisX{new QValueAxis(chart)}, axisY{new QValueAxis(chart)},
    chartView{new QChartView(chart, this)}, layout{layout}, m_xValue{0}, m_yValue{0}, m_yMaxDetectecValue{0}, m_yMinDetectecValue{0}
{

    chart->legend()->hide();
    chart->setTitle("Data");
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    chart->addSeries(series);
    chart->addSeries(series2);
    chart->addSeries(scatterSeries);

    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeStar);
    scatterSeries->setColor(QColorConstants::Yellow);
    scatterSeries->setMarkerSize(15.0);

    axisY->setTickCount(10);
    axisY->applyNiceNumbers();
    axisY->setTickType(QValueAxis::TicksFixed);

    axisX->setMax(100);
    axisX->setLabelFormat("%d");
    axisX->setTickCount(10);
    axisX->applyNiceNumbers();
    axisX->setTickType(QValueAxis::TicksFixed);
    axisX->setTitleText("Odczyt");

    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisY);
    series2->attachAxis(axisY);
    scatterSeries->attachAxis(axisY);

    chart->addAxis(axisX, Qt::AlignBottom);

    series->setColor(Qt::red);
    series->attachAxis(axisX);
    series2->setColor(Qt::green);
    series2->attachAxis(axisX);
    chartView->setRenderHint(QPainter::Antialiasing);

    scatterSeries->attachAxis(axisX);

    layout->addWidget(chartView);

}

Chart::~Chart()
{
    delete chart;
    qDebug()<<"Chart destroyed";
}

void Chart::clearChart()
{

    series->clear();
    series2->clear();
    scatterSeries->clear();

    chartView->chart()->scroll(0, 0);

    axisX->setMax(100);
    axisX->setMin(0);

    axisY->setMax(10);
    axisY->setMin(0);

    m_xValue = 0;
    m_yValue = 0;

    m_yMaxDetectecValue=0;
    m_yMinDetectecValue=0;
}

void Chart::changeSeries()
{
    clearChart();

}

void Chart::addNewSample(double point)
{


    series->append(m_xValue, point);

    quint64 difference;

    if(!m_xValue || point <= m_yMinDetectecValue)
    {
        m_yMinDetectecValue = point;
        if(!m_xValue)axisY->setMin(point);
        emit newMinYDetected(m_yMinDetectecValue);

    }

    if(point >= m_yMaxDetectecValue)
    {
        m_yMaxDetectecValue = point;
        emit newMaxYDetected(m_yMaxDetectecValue);
    }

    difference = m_yMaxDetectecValue - m_yMinDetectecValue;


    if(m_xValue >= axisX->max())
    {
        axisX->setMax(m_xValue+100);
    }


    if(point > axisY->max())
    {
        axisY->setMax(point+(difference/10));
    }

    if(point<axisY->min())
    {
        axisY->setMin(point-(difference/10));
    }

    m_xValue++;

}

void Chart::setNewMaxY(double maxY)
{
    axisY->setMax(maxY);
}

void Chart::setNewMinY(double minY)
{
    axisY->setMin(minY);
}

void Chart::addNewSample2(double point)
{
    series2->append(m_xValue, point);

    quint64 difference;

    if(!m_xValue || point <= m_yMinDetectecValue)
    {
        m_yMinDetectecValue = point;
        if(!m_xValue)axisY->setMin(point);
        emit newMinYDetected(m_yMinDetectecValue);

    }

    if(point >= m_yMaxDetectecValue)
    {
        m_yMaxDetectecValue = point;
        emit newMaxYDetected(m_yMaxDetectecValue);
    }

    difference = m_yMaxDetectecValue - m_yMinDetectecValue;

    if(m_xValue >= axisX->max())
    {
        axisX->setMax(m_xValue+100);
    }


    if(point > axisY->max())
    {
        axisY->setMax(point+(difference/10));
    }

    if(point<axisY->min())
    {
        axisY->setMin(point-(difference/10));
    }

}

void Chart::addScatterSample(double y)
{
    scatterSeries->append(m_xValue, y);
    m_xValue++;
}




