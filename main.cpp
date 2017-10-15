
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

#include "comdriver.h"
#include "localframe.h"

QT_USE_NAMESPACE
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LocalFrame frame(0x02, 0xFF, 0x01, nullptr);
    qDebug()<< frame.getFrameLength();

    /*auto layout = new QVBoxLayout;

    COMDriver comDrv(nullptr);

    const auto infos = comDrv.getPorts();
    for (const QSerialPortInfo &info : infos) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                    + QObject::tr("Location: ") + info.systemLocation() + "\n"
                    + QObject::tr("Description: ") + info.description() + "\n"
                    + QObject::tr("Manufacturer: ") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        auto label = new QLabel(s);
        layout->addWidget(label);
    }

    auto workPage = new QWidget;
    workPage->setLayout(layout);

    QScrollArea area;
    area.setWindowTitle(QObject::tr("Info about all available serial ports."));
    area.setWidget(workPage);
    area.show();*/
/*
    return a.exec();
}
*/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
