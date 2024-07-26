#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    client = new Client(this);
    timer = new QTimer(this);
    parser = new DataParser(this);


    QHBoxLayout *layout = new QHBoxLayout(ui->frameChart);

    chart = new Chart(this, layout );


    connect(timer, &QTimer::timeout, this, &MainWindow::sendData, Qt::AutoConnection);
    connect(client, &Client::dataReceived, parser, &DataParser::dataReceived, Qt::AutoConnection);
    connect(parser, &DataParser::giveParsedData, this, &MainWindow::receiveParsedData, Qt::AutoConnection);
    //connect
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    client->setHost(QHostAddress(ui->lineEditIp->text()));
    client->setPort(ui->spinBoxPort->text().toUShort());
    client->connectToServer();
}


void MainWindow::on_pushButtonLocalHost_clicked()
{
    client->setHost(QHostAddress::LocalHost);
    client->setPort(1234);

    ui->lineEditIp->setText(client->host().toString());
    ui->spinBoxPort->setValue(client->port());
}


void MainWindow::on_pushButtonStartData_clicked()
{
    quint16 frequency = ui->spinBoxFrequency->value();
    quint16 period = 1000 / frequency;

    if(frequency == 0)
    {
        qDebug() << "Frequency is 0";
        return;
    }

    if(timer->isActive())
    {
        qDebug() << "Timer is already active, changing frequency";
        timer->stop();
    }
    timer->start(period);

    qDebug() << "Frequency:"<< frequency << ", Period:" << period;

}


void MainWindow::on_pushButton_clicked()
{
    client->disconnectFromServer();
}

void MainWindow::sendData()
{
    client->sendDataToServer("Hello, world!");
}


void MainWindow::on_pushButtonStopData_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        qDebug() << "Timer stopped";
    }
    else
    {
        qDebug() << "Timer is not active";
    }
}

double extractField(const QString &data, const QString &fieldName) {

    QString pattern = fieldName + R"(\s*:\s*([\d\.]+))";
    QRegularExpression regex(pattern);


    QRegularExpressionMatch match = regex.match(data);


    if (match.hasMatch()) {
        QString fieldValue = match.captured(1);
        return fieldValue.toDouble();
    }

    return -1;
}

//SRC:   218 2021-10-17 08:16:51.618 UWB CNT: 10, DSTR: false,
//MODE: MOV, EXC: 0 WID:   222 GID:   228 BAT:  91 PRS: 101379
//TMP: 6.7 TS: 1021106004569 RSSI: -98 FPPL: -109 CSQ: 0

void MainWindow::receiveParsedData(const QString &data)
{
    ui->textBrowser->append(data);

    if(dataType == DataType::Pressure)
    {
        chart->addNewSample(extractField(data, "PRS"));
    }
    else if(dataType == DataType::Temperature)
    {
        chart->addNewSample(extractField(data, "TMP"));
    }
    else
    {
        qDebug() << "Unknown data type";
    }
}


void MainWindow::on_pushButtonCleanLogs_clicked()
{
    ui->textBrowser->clear();
}


void MainWindow::on_pushButtonPressure_clicked()
{
    chart->changeSeries();
    dataType = DataType::Pressure;

}


void MainWindow::on_pushButtonTemperature_clicked()
{
    chart->changeSeries();
    dataType = DataType::Temperature;
}

