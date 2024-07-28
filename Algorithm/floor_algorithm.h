#ifndef FLOOR_ALGORITHM_H
#define FLOOR_ALGORITHM_H

#include <QObject>
#include "../Lowpass/lowpassfilter.h"

class FloorAlgorithm :public LowPassFilter
{
    Q_OBJECT
public:
    explicit FloorAlgorithm(QObject *parent = nullptr);
    ~FloorAlgorithm();

    bool detectFloorChange(double previousPressure, double currentPressure);

    double getSensitivity() const;
    void setSensitivity(double newSensitivity);

public slots:




private:


double sensitivity;

};

#endif // FLOOR_ALGORITHM_H
