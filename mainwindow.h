#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QTimer>
#include "dataparser.h"
#include "chart.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


enum class DataType
{
    Pressure,
    Temperature
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonConnect_clicked();

    void on_pushButtonLocalHost_clicked();

    void on_pushButtonStartData_clicked();

    void on_pushButton_clicked();

    void sendData();

    void on_pushButtonStopData_clicked();

    void receiveParsedData(const QString &data);

    void on_pushButtonCleanLogs_clicked();

    void on_pushButtonPressure_clicked();

    void on_pushButtonTemperature_clicked();

private:
    Ui::MainWindow *ui;


    Client *client;
    QTimer *timer;
    DataParser *parser;
    Chart *chart;

    DataType dataType;


};
#endif // MAINWINDOW_H
