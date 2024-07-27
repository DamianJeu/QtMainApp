#include "chart.h"

Chart::Chart(QWidget *parent, QHBoxLayout *layout)
    : QWidget{parent}, chart{new QChart()}, series{new QLineSeries(chart)}, axisX{new QValueAxis(chart)}, axisY{new QValueAxis(chart)},
    chartView{new QChartView(chart, this)}, layout{layout}, m_xValue{0}, m_yValue{0}, m_yMaxDetectecValue{0}, m_yMinDetectecValue{0}
{

    chart->legend()->hide();
    chart->setTitle("Data analyser");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->addSeries(series);

    axisX->setMax(100);
    axisY->setTickCount(10);

    axisX->setTickCount(10);
    axisX->setTitleText("Odczyt");


    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->setColor(Qt::red);
    series->attachAxis(axisX);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);


}

Chart::~Chart()
{
    delete chart;
}

void Chart::clearChart()
{

    series->clear();
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

    }

    if(point >= m_yMaxDetectecValue)
    {
        m_yMaxDetectecValue = point;
        // if(!m_xValue)axisY->setMax(point);
    }

    difference = m_yMaxDetectecValue - m_yMinDetectecValue;



    if(m_xValue >= axisX->max())
    {
        axisX->setMax(m_xValue+100);
        //axisX->setMin(m_xValue - 100);
    }


    if(point > axisY->max())
    {

        axisY->setMax(point+(difference/10));


    }

    if(point<axisY->min())
    {
        axisY->setMin(point-(difference/10));
    }


    axisY->setTickInterval(difference);

    m_xValue++;

}


