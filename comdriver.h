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
private:
    QSerialPort* qSerialPort;

    QTimer timer;
    QByteArray receivedData;

    unsigned char ACK = 0x06;
    unsigned char NAK = 0x15;

    int transmissionBeginDelay = 1000;//ms
    int Tic = 10;//ms
public slots:
    void receiveBytes();
};

#endif // COMDRIVER_H
