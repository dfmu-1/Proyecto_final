
#include "MovimientoView.h"

#include "../models/Movimiento.h"
#include "../models/Producto.h"
#include "../controllers/MovimientoController.h"
#include "../controllers/InventarioController.h"

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QDate>

MovimientoView::MovimientoView(
    MovimientoController *movimientoController,
    InventarioController *inventarioController,
    QWidget *parent
)
    : QWidget(parent),
      movimientoController(movimientoController),
      inventarioController(inventarioController)
{
    configurarInterfaz();
    cargarProductos();
    cargarMovimientos();
}

void MovimientoView::configurarInterfaz()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

    QFormLayout *formulario = new QFormLayout();

    // Selector de productos
    productoInput = new QComboBox();

    // Tipo de movimiento
    tipoInput = new QComboBox();
    tipoInput->addItem("ENTRADA");
    tipoInput->addItem("SALIDA");

    // Cantidad
    cantidadInput = new QSpinBox();
    cantidadInput->setRange(1, 1000000);

    // Observación
    observacionInput = new QLineEdit();
    observacionInput->setPlaceholderText(
        "Ejemplo: Compra de mercancía"
    );

    formulario->addRow("Producto:", productoInput);
    formulario->addRow("Tipo:", tipoInput);
    formulario->addRow("Cantidad:", cantidadInput);
    formulario->addRow("Observación:", observacionInput);

    registrarButton = new QPushButton("Registrar movimiento");

    // Tabla del historial
    tablaMovimientos = new QTableWidget();

    tablaMovimientos->setColumnCount(6);

    tablaMovimientos->setHorizontalHeaderLabels({
        "ID", "Producto", "Tipo",
        "Cantidad", "Fecha", "Observación"
    });

    tablaMovimientos->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tablaMovimientos->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    layoutPrincipal->addLayout(formulario);
    layoutPrincipal->addWidget(registrarButton);
    layoutPrincipal->addWidget(tablaMovimientos);

    connect(
        registrarButton,
        &QPushButton::clicked,
        this,
        &MovimientoView::registrarMovimiento
    );
}

void MovimientoView::cargarProductos()
{
    productoInput->clear();

    QList<Producto> productos =
        inventarioController->obtenerProductos();

    for (const Producto &producto : productos) {

        QString nombre = producto.nombre
                         + " (" + producto.codigo + ")";

        productoInput->addItem(nombre, producto.id);
    }
}

void MovimientoView::registrarMovimiento()
{
    if (productoInput->currentIndex() < 0) {
        QMessageBox::warning(
            this,
            "Producto no seleccionado",
            "Primero debes registrar y seleccionar un producto."
        );

        return;
    }

    Movimiento movimiento;

    movimiento.idProducto =
        productoInput->currentData().toInt();

    movimiento.tipo = tipoInput->currentText();

    movimiento.cantidad = cantidadInput->value();

    movimiento.fecha = QDate::currentDate();

    movimiento.observacion =
        observacionInput->text().trimmed();

    if (movimientoController->registrarMovimiento(movimiento)) {

        QMessageBox::information(
            this,
            "Movimiento registrado",
            "El movimiento se registró correctamente."
        );

        cantidadInput->setValue(1);
        observacionInput->clear();

        cargarProductos();
        cargarMovimientos();

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo registrar el movimiento. "
            "Verifica las existencias disponibles."
        );
    }
}

void MovimientoView::cargarMovimientos()
{
    QList<Movimiento> movimientos =
        movimientoController->obtenerMovimientos();

    QList<Producto> productos =
        inventarioController->obtenerProductos();

    tablaMovimientos->setRowCount(0);

    for (const Movimiento &movimiento : movimientos) {

        int fila = tablaMovimientos->rowCount();

        tablaMovimientos->insertRow(fila);

        QString nombreProducto = "Producto no encontrado";

        for (const Producto &producto : productos) {
            if (producto.id == movimiento.idProducto) {
                nombreProducto = producto.nombre;
                break;
            }
        }

        tablaMovimientos->setItem(
            fila, 0,
            new QTableWidgetItem(
                QString::number(movimiento.id)
            )
        );

        tablaMovimientos->setItem(
            fila, 1,
            new QTableWidgetItem(nombreProducto)
        );

        tablaMovimientos->setItem(
            fila, 2,
            new QTableWidgetItem(movimiento.tipo)
        );

        tablaMovimientos->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::number(movimiento.cantidad)
            )
        );

        tablaMovimientos->setItem(
            fila, 4,
            new QTableWidgetItem(
                movimiento.fecha.toString("yyyy-MM-dd")
            )
        );

        tablaMovimientos->setItem(
            fila, 5,
            new QTableWidgetItem(movimiento.observacion)
        );
    }
}
