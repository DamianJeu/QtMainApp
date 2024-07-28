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


public slots:




private:




};

#endif // FLOOR_ALGORITHM_H
