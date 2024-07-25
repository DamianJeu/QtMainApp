#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    client = new Client(this);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::sendData);
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
    timer->start(100);
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

