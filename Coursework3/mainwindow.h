#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QLocalSocket>

#define APP_NAME "ICS0025 Coursework 3"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_exitBtn_clicked();

    void on_breakBtn_clicked();

    void on_computeBtn_clicked();

    void on_connectBtn_clicked();

    void connectToServer();

    void connectionError(QLocalSocket::LocalSocketError error);

    //void socketDisconnected();

private:
    Ui::MainWindow *ui;
    QLocalSocket* socket;
};
#endif // MAINWINDOW_H
