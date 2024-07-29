#include "floor_algorithm.h"
#include <cmath>
#include <QDebug>

/**
 * @brief Constructor FloorAlgorithm class.
 */
FloorAlgorithm::FloorAlgorithm(QObject *parent)
    : LowPassFilter{parent}, sensitivity{35.0}
{



}

/**
 * @brief Deconstructor FloorAlgorithm class.
 */
FloorAlgorithm::~FloorAlgorithm()
{

    qDebug()<<"Algorithm destroyed";

}

/**
 * @brief Detects floor change based on pressure changes.
 *
 * @param double previousPressure Previous pressure value
 * @param double currentPressure Current pressure value
 * @return bool true if floor change detected, false otherwise
 *
 * This method is responsible for detecting floor changes based on pressure changes.
 */
bool FloorAlgorithm::detectFloorChange(double previousPressure, double currentPressure)
{
    double difference = std::abs(currentPressure - previousPressure);
    if (difference > sensitivity) {
        return true;
    }
    return false;
}

/**
 * @brief Returns the sensitivity value.
 *
 * @return double Sensitivity value
 *
 * This method is responsible for returning the sensitivity value.
 */
double FloorAlgorithm::getSensitivity() const
{
    return sensitivity;
}

/**
 * @brief Sets the sensitivity value.
 *
 * @param double newSensitivity New sensitivity value
 *
 * This method is responsible for setting the sensitivity value.
 */
void FloorAlgorithm::setSensitivity(double newSensitivity)
{
    sensitivity = newSensitivity;
}




