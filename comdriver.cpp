#include "comdriver.h"
#include "localframe.h"

COMDriver::COMDriver(QObject *_parent): QObject(_parent){
    qSerialPort = new QSerialPort();
    timer.setSingleShot(true);
    //connect(&timer, &QTimer::timeout, this, &COMDriver::timeoutHandler);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeoutHandler()));
    connect(qSerialPort, &QSerialPort::readyRead, this, &COMDriver::receiveBytes);

    isPortOpen = false;
}

QList<QSerialPortInfo> COMDriver::getPorts(){
    return QSerialPortInfo::availablePorts();
}

bool COMDriver::openPort(QString &portName){
    qSerialPort->setPortName(portName);
    qSerialPort->setBaudRate(QSerialPort::Baud57600);
    qSerialPort->setDataBits(QSerialPort::Data8);
    qSerialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    qSerialPort->setStopBits(QSerialPort::StopBits::OneStop);
    qSerialPort->setParity(QSerialPort::NoParity);
    if(qSerialPort->open(QSerialPort::ReadWrite)){
        isPortOpen = true;        
        if( qSerialPort->isRequestToSend() ){
             qDebug()<<"RTS ON";
        }else{
             qDebug()<<"RTS OFF";
        }
        qSerialPort->setRequestToSend(false);
        return true;
    }
    return false;
}

void COMDriver::closePort(){
    isPortOpen = false;
    if(qSerialPort->isOpen()){
        qSerialPort->close();
    }
}

void COMDriver::receiveBytes(){
    auto received = qSerialPort->readAll();
    for(char i: received){
        qDebug() << "Reading byte: " << QString::number( i , 16);
        receivedData.push_back( i );
    }
    qDebug()<<"Current: "<<receivedData.size();
}

void COMDriver::timeoutHandler(){
    qDebug()<<"timeout";
    timeoutFlag = true;
}

void COMDriver::resetDevice(){//we assume that port is open
    qDebug()<<"RTS";
    receivedData.clear();
    qSerialPort->setRequestToSend(true);
    //qSerialPort->setRequestToSend(false);
    QTimer::singleShot(Tsr-20,this,SLOT(sendReset()));
}

void COMDriver::sendReset(){
    //qDebug()<<"Response";

    LocalFrame resetFrame(0x02, 0x3C, nullptr, 0);
    qSerialPort->write((const char*)resetFrame.packFrame(), resetFrame.getFrameLength());
    qSerialPort->setRequestToSend(false);
}

void COMDriver::receiveAck(){
    /*for(int i=0;i<receivedData.size();++i){
        qDebug() << QString::number( receivedData.takeFirst() , 16);
    }*/
}
