#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <QObject>

/**
 * @brief The LowPassFilter class
 *
 * This class is responsible for filtering data using a low pass filter.
 */
class LowPassFilter : public QObject
{
    Q_OBJECT
public:
    explicit LowPassFilter(QObject *parent = nullptr);

signals:


public slots:

    void setAlpha(double newAlpha);
    double calculateLowPassFilter(double newValue , double previousFilteredValue);

protected:

     double alpha;

};

#endif // LOWPASSFILTER_H
