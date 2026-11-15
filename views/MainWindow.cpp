
#include "MainWindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("StockManager - Gestor de Inventario");
    resize(1000, 650);

    QLabel *mensaje = new QLabel("Bienvenido a StockManager", this);

    mensaje->setAlignment(Qt::AlignCenter);

    setCentralWidget(mensaje);
}
