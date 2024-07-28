#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QTimer>
#include "dataparser.h"
#include "chart.h"
#include "Algorithm/floor_algorithm.h"

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

    void stopTimer();
    void startTimer(quint16 interval);
    void addFloorToLogs(double pressure, double filteredPressure, QString & data);

private slots:
    void on_pushButtonConnect_clicked();

    void on_pushButtonLocalHost_clicked();

    void on_pushButtonStartData_clicked();

    void on_pushButton_clicked();

    void sendData();

    void on_pushButtonStopData_clicked();

    void receiveParsedData(ParsedData data);

    void on_pushButtonCleanLogs_clicked();

    void on_pushButtonPressure_clicked();

    void on_pushButtonTemperature_clicked();

    void on_pushButtonAlgorithmUpdate_clicked();

    void on_pushButtonAlgorithmReset_clicked();

    void on_checkBoxLowpassFilter_stateChanged(int arg1);

    void newMaxYDetected(double maxY);
    void newMinYDetected(double minY);


    void on_doubleSpinBoxYMax_valueChanged(double arg1);

    void on_doubleSpinBoxYMin_valueChanged(double arg1);


    void clientConnected();
    void clientDisconnected();
    void addErrorToLogs(const QString &error);

    void on_pushButtonCleanLogsFloor_clicked();


    void on_pushButtonSimPtrReset_clicked();

private:
    Ui::MainWindow *ui;

    quint8 firstSample;

    Client *client;
    QTimer *timer;
    DataParser *parser;
    Chart *chart;
    Chart *chart2;
    FloorAlgorithm *floorAlgorithm;


    DataType dataType;
    bool lowpassFilterEnabled;

    double lastFilteredData;
    double lastAlgorithmData;



};
#endif // MAINWINDOW_H
