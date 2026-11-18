
#ifndef MOVIMIENTOVIEW_H
#define MOVIMIENTOVIEW_H

#include <QWidget>

class MovimientoController;
class InventarioController;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QTableWidget;

class MovimientoView : public QWidget
{
    Q_OBJECT

public:
    explicit MovimientoView(
        MovimientoController *movimientoController,
        InventarioController *inventarioController,
        QWidget *parent = nullptr
    );

private:
    MovimientoController *movimientoController;
    InventarioController *inventarioController;

    QComboBox *productoInput;
    QComboBox *tipoInput;
    QSpinBox *cantidadInput;
    QLineEdit *observacionInput;

    QPushButton *registrarButton;
    QTableWidget *tablaMovimientos;

    void configurarInterfaz();
    void cargarProductos();
    void cargarMovimientos();

private slots:
    void registrarMovimiento();
};

#endif
