#include "mainwindow.h"
#include "./ui_mainwindow.h"


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
    QHBoxLayout *layout2 = new QHBoxLayout(ui->frameChartFilter);

    chart = new Chart(this, layout );
    chart2 = new Chart(this, layout2 );

    connect(timer, &QTimer::timeout, this, &MainWindow::sendData, Qt::AutoConnection);
    connect(client, &Client::dataReceived, parser, &DataParser::dataReceived, Qt::AutoConnection);
    connect(parser, &DataParser::giveParsedData, this, &MainWindow::receiveParsedData, Qt::AutoConnection);
    connect(client, &Client::disconnectedFromServer, this, &MainWindow::on_pushButtonStopData_clicked, Qt::AutoConnection);
    connect(chart, &Chart::newMaxYDetected, this, &MainWindow::newMaxYDetected, Qt::AutoConnection);
    connect(chart, &Chart::newMinYDetected, this, &MainWindow::newMinYDetected, Qt::AutoConnection);
    connect(client, &Client::connectedToServer, this, &MainWindow::clientConnected, Qt::AutoConnection);
    connect(client, &Client::disconnectedFromServer, this, &MainWindow::clientDisconnected, Qt::AutoConnection);
    connect(parser, &DataParser::endOfData, this, &MainWindow::on_pushButtonStopData_clicked, Qt::DirectConnection);
    connect(client, &Client::errorSignal, this, &MainWindow::addErrorToLogs, Qt::AutoConnection);


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
            stopTimer();
        }
        startTimer(period);

        qDebug() << "Frequency:"<< frequency << ", Period:" << period;
    }
}


void MainWindow::on_pushButton_clicked()
{
    client->disconnectFromServer();
}

void MainWindow::sendData()
{
    client->sendDataToServer("Give me data!");
}


void MainWindow::on_pushButtonStopData_clicked()
{
    firstSample = 0;

    if(timer->isActive())
    {
        stopTimer();
        qDebug() << "Timer stopped";
    }
    else
    {
        qDebug() << "Timer is not active";
    }
}


//SRC:   218 2021-10-17 08:16:51.618 UWB CNT: 10, DSTR: false,
//MODE: MOV, EXC: 0 WID:   222 GID:   228 BAT:  91 PRS: 101379  <<<<1013,79
//TMP: 6.7 TS: 1021106004569 RSSI: -98 FPPL: -109 CSQ: 0

void MainWindow::receiveParsedData(ParsedData data)
{
    ui->textBrowser->append(QString::number(data.pressure) + "[Pa], " + QString::number(data.temperature) + "[°C] ");

    if(data.pressure<0)
    {
        qDebug() << "Negative pressure detected!";
        return;
    }
    if(dataType == DataType::Pressure)
    {
        if(lowpassFilterEnabled)
        {
            if(!firstSample)
            {

                lastFilteredData = data.pressure;
                qDebug() << "First sample:" << lastFilteredData;
            }

            lastFilteredData = floorAlgorithm->calculateLowPassFilter(data.pressure,lastFilteredData);
            bool floorChanged = floorAlgorithm->detectFloorChange(lastAlgorithmData,lastFilteredData);

            if(floorChanged && firstSample)
            {
                addFloorToLogs(data.pressure, lastFilteredData, data.dataTime);

                lastAlgorithmData = lastFilteredData;
                chart->addScatterSample(lastFilteredData);
                chart2->addScatterSample(lastFilteredData);

            }else
            {
                chart->addNewSample2(lastFilteredData);
                chart2->addNewSample(lastFilteredData);
                chart->addNewSample(data.pressure);
            }

        }
        else
        {
            chart->addNewSample(data.pressure);
        }

        if(!firstSample)firstSample=1;

    }
    else if(dataType == DataType::Temperature)
    {
        if(lowpassFilterEnabled)
        {
            if(!firstSample)
            {
                firstSample = 1;
                lastFilteredData = data.temperature;
                qDebug() << "First sample:" << lastFilteredData;
            }

            lastFilteredData = floorAlgorithm->calculateLowPassFilter(data.temperature,lastFilteredData);

            chart->addNewSample2(lastFilteredData);
            chart2->addNewSample(lastFilteredData);
            chart->addNewSample(data.temperature);
        }
        else
        {
            chart->addNewSample(data.temperature);
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
    lastFilteredData=0;
    lastAlgorithmData = 0;

    chart->changeSeries();
    chart2->changeSeries();
    dataType = DataType::Pressure;
}


void MainWindow::on_pushButtonTemperature_clicked()
{
    firstSample = 0;
    lastFilteredData=0;
    lastAlgorithmData = 0;
    chart->changeSeries();
    chart2->changeSeries();
    dataType = DataType::Temperature;
}


void MainWindow::on_pushButtonAlgorithmUpdate_clicked()
{
    double alpha = ui->doubleSpinBoxAlpha->value();
    double sensitivity = ui->doubleSpinBoxSensitivity->value();

    floorAlgorithm->setAlpha(alpha);
    floorAlgorithm->setSensitivity(sensitivity);

    qDebug() << "Alpha:" << alpha;
}


void MainWindow::on_pushButtonAlgorithmReset_clicked()
{
    firstSample = 0;
    lastFilteredData=0;
    lastAlgorithmData = 0;
}


void MainWindow::on_checkBoxLowpassFilter_stateChanged(int arg1)
{

    firstSample = 0;
    lastFilteredData=0;
    lastAlgorithmData = 0;

    if(ui->checkBoxLowpassFilter->isChecked())
    {
        lowpassFilterEnabled = true;
        qDebug() << "Lowpass filter enabled";
    }
    else
    {
        lowpassFilterEnabled = false;
        qDebug() << "Lowpass filter disabled";
    }
}

void MainWindow::newMaxYDetected(double maxY)
{
    ui->labelMaxYDisplay->setText(QString::number(maxY));
}

void MainWindow::newMinYDetected(double minY)
{
    ui->labelMinYDisplay->setText(QString::number(minY));
}


void MainWindow::on_doubleSpinBoxYMax_valueChanged(double arg1)
{
    chart->setNewMaxY(arg1);
}


void MainWindow::on_doubleSpinBoxYMin_valueChanged(double arg1)
{
    chart->setNewMinY(arg1);
}

void MainWindow::clientConnected()
{
    ui->labelConnectionStatus->setText("Connected");
    ui->labelConnectionStatus->setStyleSheet("QLabel { background-color : green; }");
}

void MainWindow::clientDisconnected()
{
    ui->labelConnectionStatus->setText("Disconnected");
    ui->labelConnectionStatus->setStyleSheet("QLabel { background-color : default; }");
}

void MainWindow::addErrorToLogs(const QString &error)
{
    ui->textBrowser->setTextColor(QColorConstants::Red);
    ui->textBrowser->append(error);
}



void MainWindow::stopTimer()
{
    ui->labelTimerStatus->setText("Stop");
    ui->labelTimerStatus->setStyleSheet("QLabel { background-color : default; }");
    timer->stop();
}

void MainWindow::startTimer(quint16 interval)
{
    ui->labelTimerStatus->setText("Working..");
    ui->labelTimerStatus->setStyleSheet("QLabel { background-color : green; }");
    timer->start(interval);
}

void MainWindow::addFloorToLogs(double pressure, double filteredPressure, QString & data)
{

    if(pressure<0)
    {
        qDebug() << "Negative pressure detected!";
        return;
    }

    ui->textBrowserFloors->setTextColor(QColorConstants::Green);

    QString floorChangeMsg= "Zmiana piętra!, Ciśnienie:"
                             + QString::number(pressure) + "[Pa],"
                             + " Odfiltrowane:" + QString::number(filteredPressure) + "[Pa],"
                             + " Data:" + data;

    ui->textBrowserFloors->append(floorChangeMsg);
}


void MainWindow::on_pushButtonCleanLogsFloor_clicked()
{
    ui->textBrowserFloors->clear();
}


void MainWindow::on_pushButtonSimPtrReset_clicked()
{
    client->sendDataToServer("RST");
}

