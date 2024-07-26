#include "dataparser.h"

DataParser::DataParser(QObject *parent)
    : QObject{parent}
{


}

void DataParser::dataReceived(const QByteArray &data)
{


    QString parsedData = QString::fromUtf8(data);

    //advanced parsing algorithm here :)

    emit giveParsedData(parsedData);


}
