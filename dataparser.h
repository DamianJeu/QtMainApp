#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QObject>

class DataParser : public QObject
{
    Q_OBJECT
public:
    explicit DataParser(QObject *parent = nullptr);


public slots:
    void dataReceived(const QByteArray &data);



signals:
    void giveParsedData(const QString &data);
};

#endif // DATAPARSER_H
