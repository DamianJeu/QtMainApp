#include "dataparser.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

DataParser::DataParser(QObject *parent)
    : QObject{parent}
{


}

DataParser::~DataParser()
{
    qDebug() << "DataParser destroyed";
}

double DataParser::extractField(const QString &data, const QString &fieldName)
{
    QString pattern = fieldName + R"(\s*:\s*([\d\.]+))";
    QRegularExpression regex(pattern);

    QRegularExpressionMatch match = regex.match(data);

    if (match.hasMatch()) {
        QString fieldValue = match.captured(1);
        return fieldValue.toDouble();
    }

    return -1;
}

QString DataParser::extractDateTime(const QString &input)
{

    QRegularExpression pattern(R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}))");
    QRegularExpressionMatch match = pattern.match(input);

    if (match.hasMatch()) {
        return match.captured(1);
    }

    return QString();

}

void DataParser::dataReceived(const QByteArray &data)
{
    QString dataToParse = QString::fromUtf8(data);

    //advanced parsing algorithm here :)
    //TODO: implement i2c driver and data serialization


    //if end of file detected emit signal or send parsed data to the main
    if(dataToParse == "EOF")
    {
        emit endOfData();
        return;
    }
    else
    {
        ParsedData parsedData;

        parsedData.pressure = extractField(dataToParse, "PRS");
        parsedData.temperature = extractField(dataToParse, "TMP");
        parsedData.dataTime = extractDateTime(dataToParse);

        emit giveParsedData(parsedData);
    }

}


