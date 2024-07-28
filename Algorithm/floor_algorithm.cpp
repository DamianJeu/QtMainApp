#include "floor_algorithm.h"
#include <cmath>
#include <QDebug>

FloorAlgorithm::FloorAlgorithm(QObject *parent)
    : LowPassFilter{parent}
{



}

FloorAlgorithm::~FloorAlgorithm()
{

    qDebug()<<"Algorithm destroyed";

}

bool FloorAlgorithm::detectFloorChange(double previousPressure, double currentPressure)
{
    double difference = std::abs(currentPressure - previousPressure);
    if (difference > sensitivity) {
        return true;
    }
    return false;
}

double FloorAlgorithm::getSensitivity() const
{
    return sensitivity;
}

void FloorAlgorithm::setSensitivity(double newSensitivity)
{
    sensitivity = newSensitivity;
}




