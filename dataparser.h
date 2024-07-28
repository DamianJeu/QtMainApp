#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>

struct ParsedData {
    double pressure;
    double temperature;
};

class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);
    double extractField(const QString &data, const QString &fieldName);

public slots:
    void dataReceived(const QByteArray &data);


signals:
    void giveParsedData(ParsedData data);
};

#endif // DATAPARSER_H
