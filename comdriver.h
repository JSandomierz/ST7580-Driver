#ifndef COMDRIVER_H
#define COMDRIVER_H

#include <QObject>
#include <QtSerialPort>

class COMDriver: public QObject
{
    Q_OBJECT
public:
    bool isPortOpen;
    COMDriver(QObject *_parent);
    QList<QSerialPortInfo> getPorts();
    bool openPort(QString &portName);
    void closePort();
    void resetDevice();
    void beginSendingDLFrame(QString message);
    //void sendMessage(QString message);

private:
    QSerialPort* qSerialPort;

    QTimer* timer;
    QVector<unsigned char> receivedData;

    unsigned char ACK = 0x06;
    unsigned char NAK = 0x15;

    int transmissionBeginDelay = 1000;//ms
    int Tic = 10;//ms
    int Tsr = 200;//ms
    int Tack = 40;//ms

    QString message;

public slots:
    void receiveBytes();
    void receiveFrame();

    void sendReset();
    void sendDLFrame();
};

#endif // COMDRIVER_H
