
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class InventarioController;
class CategoriaController;
class MovimientoController;
class AlertaController;
class ReporteController;

class MainWindow : public QMainWindow
{
public:
    MainWindow(InventarioController *inventarioController,
           CategoriaController *categoriaController,
           MovimientoController *movimientoController,
           AlertaController *alertaController,
           ReporteController *reporteController,
           QWidget *parent = nullptr);
};

#endif
