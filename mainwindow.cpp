#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QtSerialPort/QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    driver = new COMDriver(this);

    auto ports = driver->getPorts();
    for(auto port: ports){
        ui->comboBox->addItem( port.portName() );
    }
}

MainWindow::~MainWindow()
{
    driver->closePort();
    delete ui;
}

void MainWindow::on_ButtonConnect_clicked()
{
    if( driver->isPortOpen ){
        driver->closePort();
        qDebug()<<"Disconnecting!";
        ui->ButtonConnect->setText("Open");
    }else{
        QString portName = ui->comboBox->currentText();
        qDebug()<<"Attempting to connect "<<portName;
        if( driver->openPort( portName ) ){
            qDebug()<<"Success!";
            ui->ButtonConnect->setText("Close");
        }
    }
}

void MainWindow::on_ButtonReset_clicked()
{
    if( driver->isPortOpen ){
        qDebug()<<"Reset";
        driver->resetDevice();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if( driver->isPortOpen ){
        qDebug()<<"DL send "<<ui->lineEdit_msg->text();
        driver->beginSendingDLFrame(ui->comboBox_frequency->currentIndex(), ui->comboBox_modulation->currentIndex(), ui->lineEdit_msg->text());
    }
}
