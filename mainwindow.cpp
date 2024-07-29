#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //creating objects
    client = new Client(this);
    timer = new QTimer(this);
    parser = new DataParser(this);
    floorAlgorithm = new FloorAlgorithm(this);

    QHBoxLayout *layout = new QHBoxLayout(ui->frameChart);
    QHBoxLayout *layout2 = new QHBoxLayout(ui->frameChartFilter);

    chart = new Chart(this, layout );
    chart2 = new Chart(this, layout2 );

    //connect signals and slots
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

/**
 * @brief Connect to server.
 */
void MainWindow::on_pushButtonConnect_clicked()
{
    client->setHost(QHostAddress(ui->lineEditIp->text()));
    client->setPort(ui->spinBoxPort->text().toUShort());
    client->connectToServer();
}

/**
 * @brief Connect to local host.
 */
void MainWindow::on_pushButtonLocalHost_clicked()
{
    client->setHost(QHostAddress::LocalHost);
    client->setPort(1234);

    ui->lineEditIp->setText(client->host().toString());
    ui->spinBoxPort->setValue(client->port());
}

/**
 * @brief Start sending data.
 */
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
/**
 * @brief Disconnect from server.
 */
void MainWindow::on_pushButton_clicked()
{
    client->disconnectFromServer();
}

/**
 * @brief Send data.
 */
void MainWindow::sendData()
{
    //i2c driver would be added, need to change
    client->sendDataToServer("Give me data!");
}

/**
 * @brief Stop sending data.
 */
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

/**
 * @brief Receive parsed data.
 *
 * @param ParsedData data Parsed data
 */
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
                //init lowpass filter data
                lastFilteredData = data.pressure;
                qDebug() << "First sample:" << lastFilteredData;
                firstSample=1;
            }

            lastFilteredData = floorAlgorithm->calculateLowPassFilter(data.pressure,lastFilteredData); //lowpass filter
            bool floorChanged = floorAlgorithm->detectFloorChange(lastAlgorithmData,lastFilteredData); //floor change detection

            if(floorChanged)
            {
                //if algorithm detected floor change
                addFloorToLogs(data.pressure, lastFilteredData, data.dataTime);
                lastAlgorithmData = lastFilteredData;
                chart->addScatterSample(lastFilteredData);
                chart2->addScatterSample(lastFilteredData);

            }else
            {
                //if algorithm didn't detect floor change
                chart->addNewSample2(lastFilteredData);
                chart2->addNewSample(lastFilteredData);
                chart->addNewSample(data.pressure);
            }

        }
        else
        {
            chart->addNewSample(data.pressure);
        }

    }
    else if(dataType == DataType::Temperature) //temperature block
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

/**
 * @brief Clean logs.
 */
void MainWindow::on_pushButtonCleanLogs_clicked()
{
    ui->textBrowser->clear();
}

/**
 * @brief Change data type to pressure.
 */
void MainWindow::on_pushButtonPressure_clicked()
{
    on_pushButtonAlgorithmReset_clicked();

    chart->changeSeries();
    chart2->changeSeries();
    dataType = DataType::Pressure;
}

/**
 * @brief Change data type to temperature.
 */
void MainWindow::on_pushButtonTemperature_clicked()
{
    on_pushButtonAlgorithmReset_clicked();

    chart->changeSeries();
    chart2->changeSeries();
    dataType = DataType::Temperature;
}

/**
 * @brief Update algorithm.
 */
void MainWindow::on_pushButtonAlgorithmUpdate_clicked()
{
    double alpha = ui->doubleSpinBoxAlpha->value();
    double sensitivity = ui->doubleSpinBoxSensitivity->value();

    floorAlgorithm->setAlpha(alpha);
    floorAlgorithm->setSensitivity(sensitivity);

    qDebug() << "Alpha:" << alpha;
}

/**
 * @brief Reset algorithm.
 */
void MainWindow::on_pushButtonAlgorithmReset_clicked()
{
    firstSample = 0;
    lastFilteredData=0;
    lastAlgorithmData = 0;
}

/**
 * @brief Lowpass filter state changed.
 *
 * @param int arg1 state of the checkbox
 */
void MainWindow::on_checkBoxLowpassFilter_stateChanged(int arg1)
{

    on_pushButtonAlgorithmReset_clicked();

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

/**
 * @brief Set new maximum Y value.
 *
 * @param double arg1 New maximum Y value
 */
void MainWindow::newMaxYDetected(double maxY)
{
    ui->labelMaxYDisplay->setText(QString::number(maxY));
}

/**
 * @brief Set new minimum Y value.
 *
 * @param double arg1 New minimum Y value
 */
void MainWindow::newMinYDetected(double minY)
{
    ui->labelMinYDisplay->setText(QString::number(minY));
}

/**
 * @brief Client connected.
 */
void MainWindow::clientConnected()
{
    ui->labelConnectionStatus->setText("Connected");
    ui->labelConnectionStatus->setStyleSheet("QLabel { background-color : green; }");
}

/**
 * @brief Client disconnected.
 */
void MainWindow::clientDisconnected()
{
    ui->labelConnectionStatus->setText("Disconnected");
    ui->labelConnectionStatus->setStyleSheet("QLabel { background-color : default; }");
}

/**
 * @brief Add error to logs.
 *
 * @param QString error Error
 */
void MainWindow::addErrorToLogs(const QString &error)
{
    ui->textBrowser->setTextColor(QColorConstants::Red);
    ui->textBrowser->append(error);
    ui->textBrowser->setTextColor(QColorConstants::White);
}

/**
 * @brief Stop timer.
 */
void MainWindow::stopTimer()
{
    ui->labelTimerStatus->setText("Stop");
    ui->labelTimerStatus->setStyleSheet("QLabel { background-color : default; }");
    timer->stop();
}

/**
 * @brief Start timer.
 *
 * @param quint16 interval Interval
 */
void MainWindow::startTimer(quint16 interval)
{
    ui->labelTimerStatus->setText("Working..");
    ui->labelTimerStatus->setStyleSheet("QLabel { background-color : green; }");
    timer->start(interval);
}

/**
 * @brief Add floor to logs.
 *
 * @param double pressure Pressure
 * @param double filteredPressure Filtered pressure
 * @param QString data Data
 */
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

/**
 * @brief Clean logs.
 */
void MainWindow::on_pushButtonCleanLogsFloor_clicked()
{
    ui->textBrowserFloors->clear();
}

/**
 * @brief Reset simulation txt file pointer.
 */
void MainWindow::on_pushButtonSimPtrReset_clicked()
{
    client->sendDataToServer("RST");
}

