#include "lowpassfilter.h"
#include <QDebug>

/**
 * @brief Constructor LowPassFilter class.
 */
LowPassFilter::LowPassFilter(QObject *parent)
    : QObject{parent}, alpha{0.05}
{

}

/**
 * @brief Deconstructor LowPassFilter class.
 */
LowPassFilter::~LowPassFilter()
{
    qDebug()<<"LowPassFilter destroyed";
}

/**
 * @brief Sets the alpha value.
 *
 * @param double newAlpha New alpha value
 *
 * This method is responsible for setting the alpha value.
 */
void LowPassFilter::setAlpha(double newAlpha)
{
    alpha = newAlpha;
}

/**
 * @brief Calculates the low pass filter.
 *
 * @param double newValue New value
 * @param double previousFilteredValue Previous filtered value
 * @return double Filtered value
 *
 * This method is responsible for calculating the lowpass filter.
 */
double LowPassFilter::calculateLowPassFilter(double newValue , double previousFilteredValue)
{
    double filterOut = previousFilteredValue + alpha * (newValue - previousFilteredValue);
    return filterOut;
}




