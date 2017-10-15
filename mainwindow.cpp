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
        driver->resetDevice();
    }
}