#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QLocalSocket(this);

    connect(socket, &QLocalSocket::connected, this, &MainWindow::connectToServer); //connection to server
    connect(socket, QOverload<QLocalSocket::LocalSocketError>::of(&QLocalSocket::error), this, &MainWindow::connectionError); //error msgs
    ui->breakBtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    socket->close();
    delete ui;
}


void MainWindow::on_exitBtn_clicked()
{
    socket->abort();
    QApplication::exit();
}


void MainWindow::on_breakBtn_clicked()
{

        ui->serverLogs->setText("Disconnected");

        std::string stopMsg = "Stop";
        const std::wstring stopMsgWstr (stopMsg.begin(),stopMsg.end());

        ulong packetLength = sizeof(ulong) + (stopMsgWstr.size()*sizeof(wchar_t)) + 2*sizeof(char);
        QByteArray msgToServer;
        QByteArray msgFormatter;
        msgFormatter.append(std::_Bit_cast<const char*>(stopMsgWstr.c_str()), stopMsgWstr.length()*2);

        msgToServer.append(std::_Bit_cast<const char *>(&packetLength), sizeof(ulong));
        msgToServer.append(msgFormatter);
        msgToServer.append('\0');
        msgToServer.append('\0');

        socket->write(msgToServer);
        socket->flush();
        socket->abort();

        // Disable self & compute button, enable connect button
        ui->breakBtn->setEnabled(false);
        ui->computeBtn->setEnabled(false);
        ui->connectBtn->setEnabled(true);
}


void MainWindow::on_computeBtn_clicked()
{
    ui->serverLogs->setText("Computing...");

    //collecting parameters
    const std::wstring functionType = ui->comboBox->currentText().toStdWString();
    double xo = ui->x0Text->text().toDouble();
    double xn = ui->xnText->text().toDouble();
    ulong nPoints = ui->nPointsText->text().toInt();
    ulong order = ui->orderText->text().toInt();

    ulong packetLength = sizeof(ulong) + (functionType.size()*sizeof(wchar_t)) + 2*sizeof(char) + 2*sizeof(double) + 2*sizeof(ulong);

    // Build packet and send
    QByteArray msgToServer;
    QByteArray functionByteArray;
    functionByteArray.append(std::_Bit_cast<const char*>(functionType.c_str()), functionType.length()*2);

    msgToServer.append(std::_Bit_cast<const char *>(&packetLength), sizeof(ulong));
    msgToServer.append(functionByteArray);
    msgToServer.append('\0');
    msgToServer.append('\0');
    msgToServer.append(std::_Bit_cast<const char *>(&xo), sizeof(double));
    msgToServer.append(std::_Bit_cast<const char *>(&xn), sizeof(double));
    msgToServer.append(std::_Bit_cast<const char *>(&nPoints), sizeof(ulong));
    msgToServer.append(std::_Bit_cast<const char *>(&order), sizeof(ulong));

    socket->write(msgToServer);
    socket->flush();

    // Interpret incoming packet and plot points
    socket->waitForReadyRead();
    QByteArray result = socket->readAll();


    // Verify if result contains "Curve", print error to serverLogs
    QString controlMsg = QTextCodec::codecForMib(1015)->toUnicode( result.mid(4, 10) );

    if(controlMsg.compare("Curve")!=0)
    {
      // Decode and show error in UI
      result.remove(0, 4);
      ui->serverLogs->setText(QTextCodec::codecForMib(1015)->toUnicode( result ));
      return;
    }

    // Clean up data and prepare for plotting
    result.remove(0, 16);
    const double* data = std::_Bit_cast<const double*>(result.data());

    QVector<double> x(nPoints), y(nPoints);
    int xidx=0, yidx=0;
    for (ulong i=0; i<nPoints*2; i++)
     {
      if(i%2==0)
      {
        x[xidx++] = data[i];
      }
      else
      {
        y[yidx++] = data[i];
      }
     }
      ui->diagram->addGraph();
      ui->diagram->graph(0)->setData(x, y);
      ui->diagram->xAxis->setLabel("X");
      ui->diagram->yAxis->setLabel("Y");
      ui->diagram->xAxis->setRange(xo, xn);
      ui->diagram->yAxis->setRange(*std::min_element(y.constBegin(), y.constEnd()), *std::max_element(y.constBegin(), y.constEnd()));
      ui->diagram->replot();
      ui->serverLogs->setText("Plotting done!");

}


void MainWindow::on_connectBtn_clicked()
{
    socket->connectToServer(R"(\\.\pipe\ICS0025)");
}


void MainWindow::connectToServer()
{
    if(socket->isValid()){
        // Disable self, enable compute & break buttons
        ui->connectBtn->setEnabled(false);
        ui->computeBtn->setEnabled(true);
        ui->breakBtn->setEnabled(true);
        ui->exitBtn->setEnabled(true);
        ui->serverLogs->setText("Connected!");
    }
    /*else
     ui->serverLogs->setText("Unexpected error!");*/
}


void MainWindow::connectionError(QLocalSocket::LocalSocketError error)
{
    qDebug() << error;
    ui->serverLogs->setText("Connection error!");
    QMessageBox warning(QMessageBox::Warning, APP_NAME, "Connection error! Check if the server is running!",
    QMessageBox::Close);
    warning.exec();
}

