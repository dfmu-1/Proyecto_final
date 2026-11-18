#ifndef PRODUCTOVIEW_H
#define PRODUCTOVIEW_H

#include <QWidget>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QTableWidget;

class InventarioController;
class CategoriaController;
class QComboBox;

class ProductoView : public QWidget

{
    Q_OBJECT

public:
    explicit ProductoView(
    InventarioController *controller,
    CategoriaController *categoriaController,
    QWidget *parent = nullptr
);

private:
    InventarioController *controller;
    CategoriaController *categoriaController;
    QComboBox *categoriaInput;

    QLineEdit *codigoInput;
    QLineEdit *nombreInput;
    QLineEdit *ubicacionInput;

    QSpinBox *cantidadInput;
    QDoubleSpinBox *precioInput;
    QSpinBox *stockMinimoInput;

    QPushButton *registrarButton;
    QTableWidget *tablaProductos;

    void configurarInterfaz();
    void cargarCategorias();
    void registrarProducto();
    void cargarProductos();
};

#endif