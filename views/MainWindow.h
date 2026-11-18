
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class InventarioController;
class CategoriaController;
class MovimientoController;

class MainWindow : public QMainWindow
{
public:
    MainWindow(
        InventarioController *inventarioController,
        CategoriaController *categoriaController,
        MovimientoController *movimientoController,
        QWidget *parent = nullptr
    );
};

#endif
