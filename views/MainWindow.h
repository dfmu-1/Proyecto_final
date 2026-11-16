
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class InventarioController;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(InventarioController *controller,
                        QWidget *parent = nullptr);
};

#endif
