#include "comdriver.h"
#include "localframe.h"

COMDriver::COMDriver(QObject *_parent): QObject(_parent){
    qSerialPort = new QSerialPort();
    timer = new QTimer();
    //connect(timer, &QTimer::timeout, this, &COMDriver::timeoutHandler);
    connect(timer, &QTimer::timeout, this, &COMDriver::receiveFrame);
    connect(qSerialPort, &QSerialPort::readyRead, this, &COMDriver::receiveBytes);
    timer->start(20);
    isPortOpen = false;
}

QList<QSerialPortInfo> COMDriver::getPorts(){
    return QSerialPortInfo::availablePorts();
}

bool COMDriver::openPort(QString &portName){//407
    qSerialPort->setPortName(portName);
    qSerialPort->setBaudRate(QSerialPort::Baud57600);
    qSerialPort->setDataBits(QSerialPort::Data8);
    qSerialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);
    qSerialPort->setStopBits(QSerialPort::StopBits::OneStop);
    qSerialPort->setParity(QSerialPort::NoParity);
    if(qSerialPort->open(QSerialPort::ReadWrite)){
        isPortOpen = true;        
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

void COMDriver::resetDevice(){//we assume that port is open
    qSerialPort->setRequestToSend(true);
    QTimer::singleShot(Tsr-20,this,SLOT(sendReset()));
}

void COMDriver::beginSendingDLFrame(int frequencyChoice, int modulationChoice, QString message){
    this->message = message;
    this->frequencyChoice = frequencyChoice;
    this->modulationChoide = modulationChoice;
    if(modulationChoice==6) this->modulationChoide = 7; //manual p 26
    qSerialPort->setRequestToSend(true);
    QTimer::singleShot(Tsr-100,this,SLOT(sendDLFrame()));
}

void COMDriver::sendReset(){
    LocalFrame resetFrame(0x02, 0x3C, nullptr, 0);
    qSerialPort->write((const char*)resetFrame.packFrame(), resetFrame.getFrameLength());
    qSerialPort->setRequestToSend(false);
}

void COMDriver::sendDLFrame(){
    unsigned char* data = new unsigned char[message.length()+1];
    auto x = message.toLatin1();
    //data[0] = 0x44;
    data[0] = 0x00 | (frequencyChoice<<5) | (modulationChoide<<1);
    for(int i=0;i<message.length(); i++){
        data[i+1] = x[i];
    }
    LocalFrame resetFrame(0x02, 0x50, data, message.length()+1);
    qSerialPort->write((const char*)resetFrame.packFrame(), resetFrame.getFrameLength());
    qSerialPort->setRequestToSend(false);
}

void COMDriver::receiveBytes(){
    auto x = qSerialPort->readAll();
    for(unsigned char i: x){
        receivedData.push_back(i);
    }
}

void COMDriver::receiveFrame(){
    if(receivedData.size()>1){
        unsigned char x = receivedData.at(0);
        if(0x3F == x){
            x = receivedData.at(1);
            qDebug()<<"Status: " << QString("%1").arg(x, 0, 16);
            qDebug()<<"Active Layer: " << QString("%1").arg( (x>>3 & 0x03 ) , 0, 16);
            receivedData.removeFirst();
            receivedData.removeFirst();
        }else if(0x02 == x && receivedData.size()>=5){
            //qDebug()<<"Transmisja";
            unsigned char frameLen = receivedData.at(1);
            if(receivedData.size() >= frameLen+5){
                unsigned char cmd = receivedData.at(2);
                unsigned char* data = new unsigned char[frameLen];
                for(int i=0; i<frameLen; ++i){
                    data[i] = ( receivedData.at(i+3) );
                }
                unsigned int checksum;
                checksum = receivedData.at(frameLen+3);
                checksum = checksum<<8;
                checksum += receivedData.at(frameLen+4);
                LocalFrame f(0x02, cmd, data, frameLen);
                unsigned char response[1];
                if(f.computeChecksum() == checksum){
                    response[0] = ACK;
                    qSerialPort->write( (const char*)response, 1 );
                    qDebug()<<"Ramka ok";
                    if(0x3D == cmd){//reset confirm
                        qDebug()<<"Reset confirm";
                    }
                    if(0x3E == cmd){
                        qDebug()<<"Reset indication";
                    }
                    if(0x51 == cmd){
                        qDebug()<<"DL Data comfirm";
                        /*for(int i=0;i<frameLen;++i){
                            qDebug()<< QString::number(data[i],16);
                        }*/
                    }
                    if(0x52 == cmd){
                        qDebug()<<"DL Data indication";
                        QString message;
                        switch( ((char)data[0]>>5) & 0x07 ){
                            case 0:
                            qDebug()<<"Modulation B-PSK";
                            break;
                            case 1:
                            qDebug()<<"Modulation Q-PSK";
                            break;
                            case 2:
                            qDebug()<<"Modulation 8-PSK";
                            break;
                            case 3:
                            qDebug()<<"Modulation B-FSK";
                            break;
                            case 4:
                            qDebug()<<"Modulation B-PSK coded";
                            break;
                            case 5:
                            qDebug()<<"Modulation Q-PSK coded";
                            break;
                            case 7:
                            qDebug()<<"Modulation B-PSK w/ PNA";
                            break;
                        }
                        if( (((char)data[0]>>4) & 0x01) == 0x01 ){
                            qDebug()<<"RX channel high";
                        }else{
                            qDebug()<<"RX channel low";
                        }
                        qDebug()<<"PGA Value: "<<(((char)data[0]) & 0x0F);
                        qDebug()<<"SNR Value: "<<(((char)data[1]) & 0xFF);
                        short zcDelay = ((char)data[2]);
                        zcDelay <<= 8;
                        zcDelay += ((char)data[3]);
                        qDebug()<<"ZC  Value: "<<zcDelay;
                        for(int i=4;i<frameLen;++i){
                            message += (char)data[i];
                        }//0265
                        qDebug()<<"Message: "<<message;
                    }
                }else{
                    response[0] = NAK;
                    qSerialPort->write( (const char*)response, 1 );
                    qDebug()<<"NAK";
                    for(int i=0;i<receivedData.size();++i){
                        qDebug()<< QString::number(receivedData[i],16) << " " << (char)receivedData[i];
                    }
                }
                for(int i=0; i<frameLen+5; i++){
                    if(receivedData.size()<0){
                        break;
                    }else{
                        receivedData.removeFirst();
                    }
                }
            }
        }else if(0x03 == x){
            qDebug()<<"Retransmisja?";
            receivedData.removeFirst();
        }else{
            receivedData.removeFirst();
        }
    }
}
