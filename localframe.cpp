#include "localframe.h"

#include <qstring.h>
#include <qdebug.h>

LocalFrame::LocalFrame(unsigned char STX, unsigned char Length, unsigned char CMD, unsigned char *Data){
    this->STX = STX;
    this->Length = Length;
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
    qDebug()<<"Checksum = "<<valueInHex;
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
