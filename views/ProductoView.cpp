
#include "ProductoView.h"
#include "../models/Producto.h"
#include "../models/Categoria.h"
#include "../controllers/InventarioController.h"
#include "../controllers/CategoriaController.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>

ProductoView::ProductoView(
    InventarioController *controller,
    CategoriaController *categoriaController,
    QWidget *parent
)
    : QWidget(parent),
      controller(controller),
      categoriaController(categoriaController)
{
    configurarInterfaz();
    cargarCategorias();
    cargarProductos();
}

void ProductoView::cargarCategorias()
{
    categoriaInput->clear();
    categoriaInput->addItem("Sin categoría", 0);

    if (!categoriaController) {
        return;
    }

    for (const Categoria &categoria : categoriaController->obtenerCategorias()) {
        categoriaInput->addItem(categoria.nombre, categoria.id);
    }
}

void ProductoView::configurarInterfaz()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

    // Formulario de registro
    QFormLayout *formulario = new QFormLayout();

    codigoInput = new QLineEdit();
    nombreInput = new QLineEdit();
    ubicacionInput = new QLineEdit();
    categoriaInput = new QComboBox();

    cantidadInput = new QSpinBox();
    cantidadInput->setRange(0, 1000000);

    precioInput = new QDoubleSpinBox();
    precioInput->setRange(0, 100000000);
    precioInput->setDecimals(2);

    stockMinimoInput = new QSpinBox();
    stockMinimoInput->setRange(0, 1000000);

    formulario->addRow("Código:", codigoInput);
    formulario->addRow("Nombre:", nombreInput);
    formulario->addRow("Ubicación:", ubicacionInput);
    formulario->addRow("Categoría:", categoriaInput);
    formulario->addRow("Cantidad:", cantidadInput);
    formulario->addRow("Precio:", precioInput);
    formulario->addRow("Stock mínimo:", stockMinimoInput);

    registrarButton = new QPushButton("Registrar producto");

    // Tabla de productos
    tablaProductos = new QTableWidget();

    tablaProductos->setColumnCount(7);
    tablaProductos->setHorizontalHeaderLabels({
    "Código", "Nombre", "Cantidad",
    "Precio", "Ubicación", "Stock mínimo", "Categoría"
    });

    tablaProductos->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tablaProductos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layoutPrincipal->addLayout(formulario);
    layoutPrincipal->addWidget(registrarButton);
    layoutPrincipal->addWidget(tablaProductos);

    connect(registrarButton, &QPushButton::clicked,
            this, &ProductoView::registrarProducto);
}

void ProductoView::registrarProducto()
{
    // Validar campos obligatorios
    if (codigoInput->text().trimmed().isEmpty() ||
        nombreInput->text().trimmed().isEmpty()) {

        QMessageBox::warning(
            this,
            "Campos incompletos",
            "Debes ingresar el código y el nombre del producto."
        );

        return;
    }

    // Crear objeto Producto
    Producto producto;

    producto.codigo = codigoInput->text().trimmed();
    producto.nombre = nombreInput->text().trimmed();
    producto.ubicacion = ubicacionInput->text().trimmed();
    producto.cantidad = cantidadInput->value();
    producto.precio = precioInput->value();
    producto.stockMinimo = stockMinimoInput->value();
    producto.fechaAdquisicion = QDate::currentDate();
    producto.idCategoria = categoriaInput->currentData().toInt();

    // Guardar en la base de datos
    if (controller->registrarProducto(producto)) {

        QMessageBox::information(
            this,
            "Registro exitoso",
            "El producto se registró correctamente."
        );

        codigoInput->clear();
        nombreInput->clear();
        ubicacionInput->clear();
        cantidadInput->setValue(0);
        precioInput->setValue(0);
        stockMinimoInput->setValue(0);

        cargarProductos();

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo registrar el producto. Verifica que el código no esté repetido."
        );
    }
}

void ProductoView::cargarProductos()
{
    QList<Producto> productos = controller->obtenerProductos();

    tablaProductos->setRowCount(0);

    for (const Producto &producto : productos) {

        int fila = tablaProductos->rowCount();

        tablaProductos->insertRow(fila);

        tablaProductos->setItem(
            fila, 0, new QTableWidgetItem(producto.codigo)
        );

        tablaProductos->setItem(
            fila, 1, new QTableWidgetItem(producto.nombre)
        );

        tablaProductos->setItem(
            fila, 2, new QTableWidgetItem(
                QString::number(producto.cantidad)
            )
        );

        tablaProductos->setItem(
            fila, 3, new QTableWidgetItem(
                QString::number(producto.precio, 'f', 2)
            )
        );

        tablaProductos->setItem(
            fila, 4, new QTableWidgetItem(producto.ubicacion)
        );

        tablaProductos->setItem(
            fila, 5, new QTableWidgetItem(
                QString::number(producto.stockMinimo)
            )
        );
        tablaProductos->setItem(
        fila, 6,
        new QTableWidgetItem(producto.nombreCategoria)
        );
    }
}
