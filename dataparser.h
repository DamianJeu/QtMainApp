#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>
#include <QString>

struct ParsedData {
    double pressure;
    double temperature;
    QString dataTime;
};

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
