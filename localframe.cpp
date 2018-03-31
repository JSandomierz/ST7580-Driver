#include "localframe.h"

#include <qstring.h>
#include <qdebug.h>

LocalFrame::LocalFrame(unsigned char STX, unsigned char CMD, unsigned char *Data, unsigned char dataLength){
    this->STX = STX;
    this->Length = dataLength;
    this->CommandCode = CMD;
    this->Data = Data;
    checksum=0;
    checksum+=Length;
    checksum+=CMD;
    if(Data!=nullptr){
        for(int i=0;i<Length;++i){
            checksum+=Data[i];
        }
    }
    //test
    QString valueInHex= QString("%1").arg(checksum , 0, 16);
    //qDebug()<<"Checksum = "<<valueInHex;
}

LocalFrame::~LocalFrame(){
    if(Data!=nullptr){
        delete[] Data;
    }
}
unsigned int LocalFrame::getFrameLength(){
    return Length + 5;
}
unsigned char* LocalFrame::packFrame(){
    unsigned char *data = new unsigned char[5+Length];//STX+Len+CMD+Checksum + data
    data[0] = STX;
    data[1] = Length;
    data[2] = CommandCode;
    for(unsigned char i=0;i<Length;++i){
        data[i+3] = Data[i];
    }
    data[Length + 3] = (unsigned char)( checksum );
    data[Length + 4] = (unsigned char)( checksum >>8);
    return data;
}

unsigned int LocalFrame::computeChecksum(){
    unsigned char* f = this->packFrame();
    unsigned int checksum = 0;
    checksum += f[Length + 3] << 8;
    checksum += ( f[Length + 4]);
    return checksum;
}

unsigned char* LocalFrame::constructPHYFrame(QString data){
    unsigned char* dataArr = new unsigned char[data.size()+1];
    unsigned char settings = 0;
    dataArr[0] = settings;
    for(int i=1;i<data.size() && i<255;++i){
        dataArr[i] = data.at(i).toLatin1();
    }
    LocalFrame f(0x02, 0x24, dataArr, data.size()+1);
    return f.packFrame();
}
