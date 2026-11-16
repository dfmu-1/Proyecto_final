
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class InventarioController;
class CategoriaController;

class MainWindow : public QMainWindow
{
public:
    MainWindow(
        InventarioController *inventarioController,
        CategoriaController *categoriaController,
        QWidget *parent = nullptr
    );
};

#endif
