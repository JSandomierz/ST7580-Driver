#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "comdriver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ButtonConnect_clicked();
    void on_ButtonReset_clicked();

private:
    Ui::MainWindow *ui;

    COMDriver *driver;
};

#endif // MAINWINDOW_H
