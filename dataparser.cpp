#include "dataparser.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

/**
 * @brief Constructor DataParser class.
 */
DataParser::DataParser(QObject *parent)
    : QObject{parent}
{


}
/**
 * @brief Deconstructor DataParser class.
 */
DataParser::~DataParser()
{
    qDebug() << "DataParser destroyed";
}

/**
 * @brief Extracts field from the data.
 *
 * @param const QString &data Data to extract field from
 * @param const QString &fieldName Field name
 * @return double Extracted field
 *
 * This method is responsible for extracting field from the data.
 */
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

/**
 * @brief Extracts date and time from the input.
 *
 * @param const QString &input Input to extract date and time from
 * @return QString Extracted date and time
 *
 * This method is responsible for extracting date and time from the input.
 */
QString DataParser::extractDateTime(const QString &input)
{

    QRegularExpression pattern(R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3}))");
    QRegularExpressionMatch match = pattern.match(input);

    if (match.hasMatch()) {
        return match.captured(1);
    }

    return QString();

}

/**
 * @brief Data received slot.
 *
 * @param const QByteArray &data Data received
 *
 * This slot is responsible for receiving the data.
 */
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


