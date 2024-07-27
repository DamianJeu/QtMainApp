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




