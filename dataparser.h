#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QString>

/**
 * @brief The ParsedData struct
 *
 * This struct is responsible for storing the parsed data.
 */
struct ParsedData {
    double pressure;
    double temperature;
    QString dataTime;
};

/**
 * @brief The DataParser class
 *
 * This class is responsible for parsing the data received from the server.
 */
class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);
    ~DataParser();


    double extractField(const QString &data, const QString &fieldName);
    QString extractDateTime(const QString& input);

public slots:
    void dataReceived(const QByteArray &data);


signals:
    void giveParsedData(ParsedData data);
    void endOfData();
};

#endif // DATAPARSER_H
