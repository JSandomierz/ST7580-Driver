#ifndef LOCALFRAME_H
#define LOCALFRAME_H

#include <QString>

class LocalFrame
{
public:
    LocalFrame(unsigned char STX, unsigned char CMD, unsigned char *Data, unsigned char dataLength);
    ~LocalFrame();
    unsigned char *packFrame();
    unsigned int getFrameLength();
    unsigned char* constructPHYFrame(QString data);
    unsigned int computeChecksum();

    unsigned char STX;
    unsigned char Length;
    unsigned char CommandCode;
    unsigned int checksum;//two bytes used
    unsigned char *Data;
};

#endif // LOCALFRAME_H
