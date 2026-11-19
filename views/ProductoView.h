
#ifndef PRODUCTOVIEW_H
#define PRODUCTOVIEW_H

#include <QWidget>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;
class QTableWidget;
class QComboBox;

class InventarioController;
class CategoriaController;

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
    QLineEdit *busquedaInput;

    QSpinBox *cantidadInput;
    QDoubleSpinBox *precioInput;
    QSpinBox *stockMinimoInput;

    QPushButton *registrarButton;
    QPushButton *actualizarButton;
    QPushButton *eliminarButton;

    QTableWidget *tablaProductos;

    int productoSeleccionadoId = -1;

    void configurarInterfaz();
    void cargarCategorias();
    void registrarProducto();
    void cargarProductos();

    void seleccionarProducto(int fila, int columna);
    void actualizarProducto();
    void eliminarProducto();
    void limpiarFormulario();
    void buscarProductos();
};

#endif
