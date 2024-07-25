#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

private:
    Ui::MainWindow *ui;


    Client *client;
    QTimer *timer;


};
#endif // MAINWINDOW_H
