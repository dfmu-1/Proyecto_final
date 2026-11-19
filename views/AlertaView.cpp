
#include "AlertaView.h"

#include "../models/Producto.h"
#include "../controllers/AlertaController.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QAbstractItemView>

AlertaView::AlertaView(
    AlertaController *controller,
    QWidget *parent
)
    : QWidget(parent),
      controller(controller)
{
    configurarInterfaz();
    cargarAlertas();
}

void AlertaView::configurarInterfaz()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

    // Título
    QLabel *titulo = new QLabel("Alertas de stock bajo");
    titulo->setStyleSheet(
        "font-size: 22px; font-weight: bold;"
    );

    // Contador de alertas
    contadorAlertas = new QLabel("Productos que necesitan reposición: 0");

    // Botón para actualizar
    actualizarButton = new QPushButton("Actualizar alertas");

    // Tabla
    tablaAlertas = new QTableWidget();

    tablaAlertas->setColumnCount(5);

    tablaAlertas->setHorizontalHeaderLabels({
        "Código",
        "Producto",
        "Existencias",
        "Stock mínimo",
        "Ubicación"
    });

    tablaAlertas->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tablaAlertas->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Organizar elementos
    layoutPrincipal->addWidget(titulo);
    layoutPrincipal->addWidget(contadorAlertas);
    layoutPrincipal->addWidget(actualizarButton);
    layoutPrincipal->addWidget(tablaAlertas);

    connect(
        actualizarButton,
        &QPushButton::clicked,
        this,
        &AlertaView::actualizarAlertas
    );
}

void AlertaView::cargarAlertas()
{
    QList<Producto> productos =
        controller->obtenerProductosConStockBajo();

    tablaAlertas->setRowCount(0);

    for (const Producto &producto : productos) {

        int fila = tablaAlertas->rowCount();

        tablaAlertas->insertRow(fila);

        tablaAlertas->setItem(
            fila, 0,
            new QTableWidgetItem(producto.codigo)
        );

        tablaAlertas->setItem(
            fila, 1,
            new QTableWidgetItem(producto.nombre)
        );

        tablaAlertas->setItem(
            fila, 2,
            new QTableWidgetItem(
                QString::number(producto.cantidad)
            )
        );

        tablaAlertas->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::number(producto.stockMinimo)
            )
        );

        tablaAlertas->setItem(
            fila, 4,
            new QTableWidgetItem(producto.ubicacion)
        );
    }

    contadorAlertas->setText(
        "Productos que necesitan reposición: "
        + QString::number(productos.size())
    );
}

void AlertaView::actualizarAlertas()
{
    cargarAlertas();
}
