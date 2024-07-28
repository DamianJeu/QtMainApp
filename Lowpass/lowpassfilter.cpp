#include "lowpassfilter.h"

LowPassFilter::LowPassFilter(QObject *parent)
    : QObject{parent}, alpha{0.25}
{




}

void LowPassFilter::setAlpha(double newAlpha)
{
    alpha = newAlpha;
}
\


double LowPassFilter::calculateLowPassFilter(double newValue , double previousFilteredValue)
{



    double filterOut = previousFilteredValue + alpha * (newValue - previousFilteredValue);

   // double filterOut = alpha * newValue + (1.0f-alpha) * previousFilteredValue;

    return filterOut;

    //filter.out = filter.alpha * new_data + (1.0f-filter.alpha) * filter.out;


}




