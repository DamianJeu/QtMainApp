#include "dataparser.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

DataParser::DataParser(QObject *parent)
    : QObject{parent}
{


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

void DataParser::dataReceived(const QByteArray &data)
{


    QString dataToParse = QString::fromUtf8(data);

    //advanced parsing algorithm here :)

    ParsedData parsedData;

    parsedData.pressure = extractField(dataToParse, "PRS");
    parsedData.temperature = extractField(dataToParse, "TMP");


    emit giveParsedData(parsedData);


}
