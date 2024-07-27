#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <QObject>


//double newValue

// double lowPassFilter(double previousFilteredValue, double newValue, double timeConstant, double deltaTime) {
//     double alpha = deltaTime / (timeConstant + deltaTime);
//     return previousFilteredValue + alpha * (newValue - previousFilteredValue);
// }


class LowPassFilter : public QObject
{
    Q_OBJECT
public:
    explicit LowPassFilter(QObject *parent = nullptr);



signals:


public slots:


    void changeAlpha(double newAlpha);


    double calculateLowPassFilter(double newValue , double previousFilteredValue);




protected:

     double alpha;


};

#endif // LOWPASSFILTER_H
