#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>



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



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    client = new Client(this);
    timer = new QTimer(this);
    parser = new DataParser(this);
    floorAlgorithm = new FloorAlgorithm(this);

    QHBoxLayout *layout = new QHBoxLayout(ui->frameChart);

    chart = new Chart(this, layout );


    connect(timer, &QTimer::timeout, this, &MainWindow::sendData, Qt::AutoConnection);
    connect(client, &Client::dataReceived, parser, &DataParser::dataReceived, Qt::AutoConnection);
    connect(parser, &DataParser::giveParsedData, this, &MainWindow::receiveParsedData, Qt::AutoConnection);
    connect(client, &Client::disconnectedFromServer, this, &MainWindow::on_pushButtonStopData_clicked, Qt::AutoConnection);
    connect(chart, &Chart::newMaxYDetected, this, &MainWindow::newMaxYDetected, Qt::AutoConnection);
    connect(chart, &Chart::newMinYDetected, this, &MainWindow::newMinYDetected, Qt::AutoConnection);

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
    firstSample = 0;

    if(client->isConnected())
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
    firstSample = 0;

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


//SRC:   218 2021-10-17 08:16:51.618 UWB CNT: 10, DSTR: false,
//MODE: MOV, EXC: 0 WID:   222 GID:   228 BAT:  91 PRS: 101379
//TMP: 6.7 TS: 1021106004569 RSSI: -98 FPPL: -109 CSQ: 0

void MainWindow::receiveParsedData(const QString &data)
{
    ui->textBrowser->append(data);

    double lastFilteredData;


    if(dataType == DataType::Pressure)
    {
        if(lowpassFilterEnabled)
        {
            if(!firstSample)
            {
                firstSample = 1;
                lastFilteredData = extractField(data, "PRS");
                qDebug() << "First sample:" << lastFilteredData;
            }

            lastFilteredData = floorAlgorithm->calculateLowPassFilter(extractField(data, "PRS"),lastFilteredData);

            chart->addNewSample(lastFilteredData);
        }
        else
        {
            chart->addNewSample(extractField(data, "PRS"));
        }

    }
    else if(dataType == DataType::Temperature)
    {
        if(lowpassFilterEnabled)
        {
            if(!firstSample)
            {
                firstSample = 1;
                lastFilteredData = extractField(data, "TMP");
                qDebug() << "First sample:" << lastFilteredData;
            }

            lastFilteredData = floorAlgorithm->calculateLowPassFilter(extractField(data, "TMP"),lastFilteredData);

            chart->addNewSample(lastFilteredData);
        }
        else
        {
            chart->addNewSample(extractField(data, "TMP"));
        }

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
    firstSample = 0;
    chart->changeSeries();
    dataType = DataType::Pressure;

}


void MainWindow::on_pushButtonTemperature_clicked()
{
    firstSample = 0;
    chart->changeSeries();
    dataType = DataType::Temperature;
}


void MainWindow::on_pushButtonAlgorithmUpdate_clicked()
{
    double alpha = ui->doubleSpinBoxAlpha->value();
    double sensitivity = ui->doubleSpinBoxSensitivity->value();

    floorAlgorithm->changeAlpha(alpha);

    qDebug() << "Alpha:" << alpha;


}


void MainWindow::on_pushButtonAlgorithmReset_clicked()
{
    firstSample = 0;
}


void MainWindow::on_checkBoxLowpassFilter_stateChanged(int arg1)
{

    if(ui->checkBoxLowpassFilter->isChecked())
    {
        firstSample = 0;
        lowpassFilterEnabled = true;
        qDebug() << "Lowpass filter enabled";
    }
    else
    {
        firstSample = 0;
        lowpassFilterEnabled = false;
        qDebug() << "Lowpass filter disabled";
    }
}

void MainWindow::newMaxYDetected(double maxY)
{
    ui->labelMaxY->setText(QString::number(maxY));
}

void MainWindow::newMinYDetected(double minY)
{
    ui->labelMinY->setText(QString::number(minY));
}

void MainWindow::setNewMaxY(double maxY)
{

}

void MainWindow::setNewMinY(double minY)
{

}

