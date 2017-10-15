#include "comdriver.h"
#include "localframe.h"

COMDriver::COMDriver(QObject *_parent): QObject(_parent){
    qSerialPort = new QSerialPort();
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(receiveBytes()));

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
    if(qSerialPort->open(QSerialPort::ReadWrite)){
        isPortOpen = true;
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

void COMDriver::resetDevice(){//we assume that port is open
    qSerialPort->setRequestToSend(false);
    qSerialPort->waitForReadyRead(transmissionBeginDelay);
    //wait for one more character
    timer.start( 1 * Tic );
    receiveBytes();
    qSerialPort->setRequestToSend(true);

    unsigned char statusCode, statusData;
    if( receivedData.size() >= 2 ){
        statusCode = receivedData.at(0);
        statusData = receivedData.at(1);

        LocalFrame resetFrame(0x02, 0x00, 0x3C, nullptr);
        qSerialPort->write((const char*)resetFrame.packFrame(), resetFrame.getFrameLength());

        receiveBytes();
        if( receivedData.size() >= 1 ){
            unsigned char response = receivedData.at(0);
            if(response == ACK){
                qDebug()<<"RESET OK\n";
            }else{
                qDebug()<<"RESET NAK\n";
            }
        }else{
            qDebug()<<"No data in response";
        }
    }else{
        qDebug()<<"No data in status";
    }
}

void COMDriver::receiveBytes(){
    qSerialPort->waitForReadyRead(Tic);
    receivedData = qSerialPort->read(1024);
}
