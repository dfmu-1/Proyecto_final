
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
#include <QAbstractItemView>
#include <QVariant>

// Constructor
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

// Cargar categorías desde la base de datos
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

// Configurar la interfaz gráfica
void ProductoView::configurarInterfaz()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

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

    // Botones
    registrarButton = new QPushButton("Registrar producto");
    actualizarButton = new QPushButton("Actualizar producto");
    eliminarButton = new QPushButton("Eliminar producto");

    actualizarButton->setEnabled(false);
    eliminarButton->setEnabled(false);

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
    tablaProductos->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaProductos->setSelectionMode(QAbstractItemView::SingleSelection);

    // Agregar elementos al diseño
    layoutPrincipal->addLayout(formulario);
    layoutPrincipal->addWidget(registrarButton);
    layoutPrincipal->addWidget(actualizarButton);
    layoutPrincipal->addWidget(eliminarButton);
    layoutPrincipal->addWidget(tablaProductos);

    // Conectar botones y selección de filas
    connect(registrarButton, &QPushButton::clicked,
            this, &ProductoView::registrarProducto);

    connect(actualizarButton, &QPushButton::clicked,
            this, &ProductoView::actualizarProducto);

    connect(eliminarButton, &QPushButton::clicked,
            this, &ProductoView::eliminarProducto);

    connect(tablaProductos, &QTableWidget::cellClicked,
            this, &ProductoView::seleccionarProducto);
}

// Registrar un producto
void ProductoView::registrarProducto()
{
    if (codigoInput->text().trimmed().isEmpty() ||
        nombreInput->text().trimmed().isEmpty()) {

        QMessageBox::warning(
            this,
            "Campos incompletos",
            "Debes ingresar el código y el nombre del producto."
        );

        return;
    }

    Producto producto;

    producto.codigo = codigoInput->text().trimmed();
    producto.nombre = nombreInput->text().trimmed();
    producto.ubicacion = ubicacionInput->text().trimmed();
    producto.cantidad = cantidadInput->value();
    producto.precio = precioInput->value();
    producto.stockMinimo = stockMinimoInput->value();
    producto.fechaAdquisicion = QDate::currentDate();
    producto.idCategoria = categoriaInput->currentData().toInt();

    if (controller->registrarProducto(producto)) {

        QMessageBox::information(
            this,
            "Registro exitoso",
            "El producto se registró correctamente."
        );

        limpiarFormulario();
        cargarProductos();

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo registrar el producto. Verifica que el código no esté repetido."
        );
    }
}

// Cargar los productos en la tabla
void ProductoView::cargarProductos()
{
    QList<Producto> productos = controller->obtenerProductos();

    tablaProductos->setRowCount(0);

    for (const Producto &producto : productos) {

        int fila = tablaProductos->rowCount();

        tablaProductos->insertRow(fila);

        QTableWidgetItem *codigoItem =
            new QTableWidgetItem(producto.codigo);

        // Guardar el ID real en la primera columna.
        codigoItem->setData(Qt::UserRole, producto.id);

        tablaProductos->setItem(fila, 0, codigoItem);

        tablaProductos->setItem(
            fila, 1, new QTableWidgetItem(producto.nombre)
        );

        tablaProductos->setItem(
            fila, 2,
            new QTableWidgetItem(QString::number(producto.cantidad))
        );

        tablaProductos->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::number(producto.precio, 'f', 2)
            )
        );

        tablaProductos->setItem(
            fila, 4, new QTableWidgetItem(producto.ubicacion)
        );

        tablaProductos->setItem(
            fila, 5,
            new QTableWidgetItem(QString::number(producto.stockMinimo))
        );

        tablaProductos->setItem(
            fila, 6, new QTableWidgetItem(producto.nombreCategoria)
        );
    }
}

// Seleccionar un producto de la tabla
void ProductoView::seleccionarProducto(int fila, int columna)
{
    Q_UNUSED(columna);

    QTableWidgetItem *item = tablaProductos->item(fila, 0);

    if (!item) {
        return;
    }

    productoSeleccionadoId = item->data(Qt::UserRole).toInt();

    QList<Producto> productos = controller->obtenerProductos();

    for (const Producto &producto : productos) {

        if (producto.id == productoSeleccionadoId) {

            codigoInput->setText(producto.codigo);
            nombreInput->setText(producto.nombre);
            ubicacionInput->setText(producto.ubicacion);

            cantidadInput->setValue(producto.cantidad);
            precioInput->setValue(producto.precio);
            stockMinimoInput->setValue(producto.stockMinimo);

            int indice = categoriaInput->findData(producto.idCategoria);

            if (indice >= 0) {
                categoriaInput->setCurrentIndex(indice);
            } else {
                categoriaInput->setCurrentIndex(0);
            }

            actualizarButton->setEnabled(true);
            eliminarButton->setEnabled(true);

            return;
        }
    }
}

// Actualizar el producto seleccionado
void ProductoView::actualizarProducto()
{
    if (productoSeleccionadoId <= 0) {
        return;
    }

    if (codigoInput->text().trimmed().isEmpty() ||
        nombreInput->text().trimmed().isEmpty()) {

        QMessageBox::warning(
            this,
            "Campos incompletos",
            "El código y el nombre son obligatorios."
        );

        return;
    }

    Producto producto;

    producto.id = productoSeleccionadoId;
    producto.codigo = codigoInput->text().trimmed();
    producto.nombre = nombreInput->text().trimmed();
    producto.ubicacion = ubicacionInput->text().trimmed();
    producto.cantidad = cantidadInput->value();
    producto.precio = precioInput->value();
    producto.stockMinimo = stockMinimoInput->value();
    producto.idCategoria = categoriaInput->currentData().toInt();

    // Conservar la fecha original de adquisición.
    QList<Producto> productos = controller->obtenerProductos();

    for (const Producto &existente : productos) {
        if (existente.id == productoSeleccionadoId) {
            producto.fechaAdquisicion = existente.fechaAdquisicion;
            break;
        }
    }

    if (controller->actualizarProducto(producto)) {

        QMessageBox::information(
            this,
            "Actualización exitosa",
            "El producto se actualizó correctamente."
        );

        limpiarFormulario();
        cargarProductos();

    } else {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo actualizar el producto. Verifica que el código no esté repetido."
        );
    }
}

// Eliminar el producto seleccionado
void ProductoView::eliminarProducto()
{
    if (productoSeleccionadoId <= 0) {
        return;
    }

    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this,
        "Confirmar eliminación",
        "¿Estás seguro de que deseas eliminar este producto?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    if (controller->eliminarProducto(productoSeleccionadoId)) {

        QMessageBox::information(
            this,
            "Eliminación exitosa",
            "El producto se eliminó correctamente."
        );

        limpiarFormulario();
        cargarProductos();

    } else {

        QMessageBox::warning(
            this,
            "No se pudo eliminar",
            "El producto no pudo eliminarse. Es posible que tenga movimientos registrados."
        );
    }
}

// Limpiar el formulario y restablecer los botones
void ProductoView::limpiarFormulario()
{
    productoSeleccionadoId = -1;

    codigoInput->clear();
    nombreInput->clear();
    ubicacionInput->clear();

    cantidadInput->setValue(0);
    precioInput->setValue(0);
    stockMinimoInput->setValue(0);

    categoriaInput->setCurrentIndex(0);

    actualizarButton->setEnabled(false);
    eliminarButton->setEnabled(false);

    tablaProductos->clearSelection();
}
