#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QIntValidator>
#include <QTcpSocket>
#include <QMessageBox>
#include <iostream>

#include <cstring>

//Main window (there is only one)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setValidator(new QIntValidator(0,65536,this));
    ui->payload->setPlainText("Hello World!!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Button slot
void MainWindow::on_listenButton_clicked()
{
    ui->payload->appendPlainText(tr("======>> on_listenButton_clicked() ======="));

    bytesReceived = 0;

    if (serverIsListening == false) {
        ui->listenButton->setText("&STOP");

        //Set port
        bool parsedAsInt = false;
        quint16 port = ui->port->text().toUInt(&parsedAsInt);
        if (parsedAsInt == true) {
            ui->payload->appendPlainText(tr("Settin port to %1").arg(port));
        } else {
            ui->payload->appendPlainText(tr("Cannot read port value: using server assigned."));
            port = 0;
        }

        //Hook up signals and slots for the server events
        connect(&tcpServer, &QTcpServer::newConnection, this, &MainWindow::acceptConnection);
        connect(&tcpServer, &QTcpServer::hasPendingConnections, this, &MainWindow::pendingConnection); // check ithis works
        connect(&tcpServer, &QTcpServer::acceptError, this, &MainWindow::acceptError);

        //Non-blocking call to put server into listening state
        while (!tcpServer.listen(QHostAddress::Any, port)) {
            QMessageBox::StandardButton ret = QMessageBox::critical(this,tr("Loopback"), tr("Unable to start the test: %1.").arg(tcpServer.errorString()), QMessageBox::Retry | QMessageBox::Cancel);
            if (ret == QMessageBox::Cancel)
                return;
        }

        ui->payload->appendPlainText(tr("Listening on port %1").arg(tcpServer.serverPort()));
        serverIsListening = true;
    } else {
        //User clicked the disconnect button
        tearDown();
    }
    ui->payload->appendPlainText(tr("======<< on_listenButton_clicked() ======="));
}

// slot for pendingConnectionAvailable signal
void MainWindow::pendingConnection() {
    ui->payload->appendPlainText(tr("====== PENDING CONNECTION SIGNAL AVAILABLE ======="));
}

//slot for newConnection signal
void MainWindow::acceptConnection()
{
    ui->payload->appendPlainText(tr("======>> acceptConnection() ======="));

    //Get the TCP/IP socket (type QTcpSocket)
    tcpServerConnection = tcpServer.nextPendingConnection();
    if (!tcpServerConnection) {
        ui->payload->appendPlainText(tr("======<< FAILED: acceptConnection() %s =======").arg(tcpServer.errorString()));
        tearDown();
        return;
    }

    //Hook up signals and slots for TCP/IP socket
    connect(tcpServerConnection, &QIODevice::readyRead, this, &MainWindow::updateServerProgress);
    connect(tcpServerConnection, &QAbstractSocket::errorOccurred,this, &MainWindow::displayError);
    connect(tcpServerConnection, &QTcpSocket::disconnected, tcpServerConnection, &QTcpSocket::deleteLater);

    ui->payload->appendPlainText(tr("======<< acceptConnection() ======="));
}

// slot for case where accept did not succeed.
void MainWindow::acceptError(QAbstractSocket::SocketError err)
{
    ui->payload->appendPlainText(tr("====== ACCEPT ERROR ======="));
    ui->payload->appendPlainText(tr("%1 %2").arg(err).arg(tcpServer.errorString()));
}

// slot for handling incoming data
void MainWindow::updateServerProgress()
{
    ui->payload->appendPlainText(tr("======>> updateServerProgress() ======="));
    unsigned bytesThisTransaction = int(tcpServerConnection->bytesAvailable());
    bytesReceived += bytesThisTransaction;
    QByteArray bytes = tcpServerConnection->readLine();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString data = codec->toUnicode(bytes);

    ui->payload->appendPlainText(tr("Received %1 Bytes, total %2").arg(bytesReceived).arg(bytesThisTransaction));
    QStringList longdata = data.split("-");

    for (unsigned n=1; n<longdata.size(); n++) {
        if(n==1){
            ui->payload->appendPlainText(tr("Temperature in Celceus:"));
        } else if(n==2){
            ui->payload->appendPlainText(tr("Temperature in Fahreheit:"));
        } else if (n==3){
            ui->payload->appendPlainText(tr("Humidity:"));
        } else if (n==4){
            ui->payload->appendPlainText(tr("CO2 in PPM:"));
        } else if (n==5){
            ui->payload->appendPlainText(tr("Light:"));
        }

        ui->payload->appendPlainText(tr("%1").arg(longdata[n]));
    }

    //ui->payload->appendPlainText(tr("Temperature in Celceus: %*").arg(longdata[-5]));
    //ui->payload->appendPlainText(tr("Temperature in Fahreheit: %*").arg(longdata[-4]));
    //ui->payload->appendPlainText(tr("Humidity: %* g/Kg").arg(longdata[-3]));
    //ui->payload->appendPlainText(tr("CO2: %* ppm").arg(longdata[-2]));
    //ui->payload->appendPlainText(tr("Light: %*").arg(longdata[-1]));

    ui->payload->appendPlainText(tr("======<< updateServerProgress() ======="));

}

// slot for error handling, including the remote host closing
void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    ui->payload->appendPlainText(tr("======>> displayError() ======="));
    ui->payload->appendPlainText(tr("displayError: %1").arg(tcpServer.errorString()));

    if (socketError == QTcpSocket::RemoteHostClosedError) {
        ui->payload->appendPlainText(tr("Remote Host Closed"));
        tearDownTCPSocket();
        return;
    }

    //For all other errors, tear down everything
    tearDown();

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    ui->payload->appendPlainText(tr("======<< displayError() ======="));

}

void MainWindow::tearDownTCPSocket()
{
    //Close current TCP connection (there is only one in this application)
    if (tcpServerConnection) {
        if (tcpServerConnection->isOpen()) {
            disconnect(tcpServerConnection, nullptr, nullptr, nullptr);
            tcpServerConnection->close();
            tcpServerConnection = nullptr; //IMPORTANT!
            ui->payload->appendPlainText(tr("Server connection closed"));
        }
    }
}
void MainWindow::tearDown()
{
    tearDownTCPSocket();

    //Stop the server from accepting incoming connections
    if (tcpServer.isListening()) {
        tcpServer.close();
        ui->payload->appendPlainText(tr("Server closed"));
    }
    //Disconnect signals from slots
    disconnect(&tcpServer, nullptr, nullptr, nullptr);

    //Update UI
    ui->listenButton->setEnabled(true);
    ui->listenButton->setText("&LISTEN");
    serverIsListening = false;
}
