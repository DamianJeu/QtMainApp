#include "chart.h"

/**
 * @brief Constructor Chart class.
 */
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

/**
 * @brief Deconstructor Chart class.
 */
Chart::~Chart()
{
    delete chart;
    qDebug()<<"Chart destroyed";
}

/**
 * @brief Clears the chart.
 *
 * This method is responsible for clearing the chart.
 */
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

/**
 * @brief Changes the series.
 *
 * This method is responsible for changing the series.
 */
void Chart::changeSeries()
{
    clearChart();
}

/**
 * @brief Adds a new sample.
 *
 * @param double point New sample
 *
 * This method is responsible for adding a new sample.
 */
void Chart::addNewSample(double point)
{
    series->append(m_xValue, point);

    quint64 difference;

    // If the new sample is lower than the minimum detected value
    if(!m_xValue || point <= m_yMinDetectecValue)
    {
        m_yMinDetectecValue = point;
        if(!m_xValue)axisY->setMin(point);
        emit newMinYDetected(m_yMinDetectecValue);

    }
    // If the new sample is higher than the maximum detected value
    if(point >= m_yMaxDetectecValue)
    {
        m_yMaxDetectecValue = point;
        emit newMaxYDetected(m_yMaxDetectecValue);
    }

    // Calculate the difference between the maximum and minimum detected values
    difference = m_yMaxDetectecValue - m_yMinDetectecValue;

    // If the x value is higher than the maximum x value
    if(m_xValue >= axisX->max())
    {
        axisX->setMax(m_xValue+100);
    }

    // If the new sample is higher than the maximum y value
    if(point > axisY->max())
    {
        axisY->setMax(point+(difference/10)); //auto scale chart max Y axis
    }

    if(point<axisY->min())
    {
        axisY->setMin(point-(difference/10)); //auto scale chart min Y axis
    }

    m_xValue++;

}

/**
 * @brief Sets the new maximum y value.
 *
 * @param double maxY New maximum y value
 *
 * This method is responsible for setting the new maximum y value.
 */
void Chart::setNewMaxY(double maxY)
{
    axisY->setMax(maxY);
}

/**
 * @brief Sets the new minimum y value.
 *
 * @param double minY New minimum y value
 *
 * This method is responsible for setting the new minimum y value.
 */
void Chart::setNewMinY(double minY)
{
    axisY->setMin(minY);
}

/**
 * @brief Adds a new sample in second series.
 *
 * @param double point New sample
 *
 * This method is responsible for adding a new sample.
 */
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

/**
 * @brief Adds a new scatter sample.
 *
 * @param double y New scatter sample
 *
 * This method is responsible for adding a new scatter sample. (start event point representing floor detection)
 *
 */
void Chart::addScatterSample(double y)
{
    scatterSeries->append(m_xValue, y);
    m_xValue++;
}




