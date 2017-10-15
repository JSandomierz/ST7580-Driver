#ifndef LOCALFRAME_H
#define LOCALFRAME_H


class LocalFrame
{
public:
    LocalFrame(unsigned char STX, unsigned char Length, unsigned char CMD, unsigned char *Data);
    ~LocalFrame();
    unsigned char *packFrame();
    unsigned int getFrameLength();

    unsigned char STX;
    unsigned char Length;
    unsigned char CommandCode;
    unsigned int checksum;//two bytes used
    unsigned char *Data;
};

#endif // LOCALFRAME_H
