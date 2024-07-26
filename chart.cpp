#include "chart.h"

Chart::Chart(QWidget *parent, QHBoxLayout *layout)
    : QWidget{parent}, chart{new QChart()}, series{new QLineSeries()}, axisX{new QValueAxis()}, axisY{new QValueAxis()},
    chartView{new QChartView(chart)}, layout{layout}, m_xValue{0}, m_yValue{0}, m_yMaxDetectecValue{0}, m_yMinDetectecValue{0}
{

    chart->legend()->hide();
    chart->setTitle("Data analyser");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->addSeries(series);

    axisX->setMax(100);
    axisX->setTitleText("Odczyt");


    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);


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



    if(m_xValue==0 || point <= m_yMinDetectecValue)
    {
        m_yMinDetectecValue = point;
       // if(!m_xValue)axisY->setMin(point);

    }

    if(m_xValue==0 || point >= m_yMaxDetectecValue)
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


    if(point >= axisY->max())
    {
        qDebug()<<"difference "<<point <<" minValue:"<<m_yMinDetectecValue<<" MaxValue:"<<m_yMaxDetectecValue<< " point:"<<point;
        axisY->setMax(point+100);
        axisY->setMin(m_yMinDetectecValue);
    }




    m_xValue++;

}


