
#include "CategoriaView.h"
#include "../models/Categoria.h"
#include "../controllers/CategoriaController.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>

CategoriaView::CategoriaView(
    CategoriaController *controller,
    QWidget *parent
)
    : QWidget(parent),
      controller(controller),
      categoriaSeleccionada(0)
{
    configurarInterfaz();
    cargarCategorias();
}

void CategoriaView::configurarInterfaz()
{
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);

    // Formulario
    QFormLayout *formulario = new QFormLayout();

    nombreInput = new QLineEdit();
    descripcionInput = new QLineEdit();

    formulario->addRow("Nombre:", nombreInput);
    formulario->addRow("Descripción:", descripcionInput);

    // Botones
    QHBoxLayout *layoutBotones = new QHBoxLayout();

    registrarButton = new QPushButton("Registrar");
    actualizarButton = new QPushButton("Actualizar");
    eliminarButton = new QPushButton("Eliminar");

    layoutBotones->addWidget(registrarButton);
    layoutBotones->addWidget(actualizarButton);
    layoutBotones->addWidget(eliminarButton);

    // Tabla
    tablaCategorias = new QTableWidget();

    tablaCategorias->setColumnCount(3);
    tablaCategorias->setHorizontalHeaderLabels({
        "ID", "Nombre", "Descripción"
    });

    tablaCategorias->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tablaCategorias->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Agregar elementos al diseño
    layoutPrincipal->addLayout(formulario);
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(tablaCategorias);

    // Conectar botones
    connect(registrarButton, &QPushButton::clicked,
            this, &CategoriaView::registrarCategoria);

    connect(actualizarButton, &QPushButton::clicked,
            this, &CategoriaView::actualizarCategoria);

    connect(eliminarButton, &QPushButton::clicked,
            this, &CategoriaView::eliminarCategoria);

    connect(tablaCategorias, &QTableWidget::cellClicked,
            this, &CategoriaView::seleccionarCategoria);
}

// Registrar categoría
void CategoriaView::registrarCategoria()
{
    if (nombreInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo obligatorio",
            "Debes ingresar el nombre de la categoría."
        );
        return;
    }

    Categoria categoria;

    categoria.nombre = nombreInput->text().trimmed();
    categoria.descripcion = descripcionInput->text().trimmed();

    if (controller->registrarCategoria(categoria)) {
        QMessageBox::information(
            this,
            "Registro exitoso",
            "La categoría se registró correctamente."
        );

        nombreInput->clear();
        descripcionInput->clear();

        cargarCategorias();
    } else {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo registrar la categoría. Verifica los datos."
        );
    }
}

// Actualizar categoría
void CategoriaView::actualizarCategoria()
{
    if (categoriaSeleccionada <= 0) {
        QMessageBox::warning(
            this,
            "Selección requerida",
            "Selecciona una categoría de la tabla."
        );
        return;
    }

    if (nombreInput->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo obligatorio",
            "El nombre no puede estar vacío."
        );
        return;
    }

    Categoria categoria;

    categoria.id = categoriaSeleccionada;
    categoria.nombre = nombreInput->text().trimmed();
    categoria.descripcion = descripcionInput->text().trimmed();

    if (controller->actualizarCategoria(categoria)) {
        QMessageBox::information(
            this,
            "Actualización exitosa",
            "La categoría se actualizó correctamente."
        );

        nombreInput->clear();
        descripcionInput->clear();
        categoriaSeleccionada = 0;

        cargarCategorias();
    } else {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo actualizar la categoría."
        );
    }
}

// Eliminar categoría
void CategoriaView::eliminarCategoria()
{
    if (categoriaSeleccionada <= 0) {
        QMessageBox::warning(
            this,
            "Selección requerida",
            "Selecciona una categoría de la tabla."
        );
        return;
    }

    QMessageBox::StandardButton respuesta =
        QMessageBox::question(
            this,
            "Confirmar eliminación",
            "¿Estás seguro de eliminar esta categoría?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    if (controller->eliminarCategoria(categoriaSeleccionada)) {
        QMessageBox::information(
            this,
            "Eliminación exitosa",
            "La categoría fue eliminada."
        );

        nombreInput->clear();
        descripcionInput->clear();
        categoriaSeleccionada = 0;

        cargarCategorias();
    } else {
        QMessageBox::warning(
            this,
            "No se pudo eliminar",
            "La categoría puede tener productos asociados."
        );
    }
}

// Seleccionar categoría de la tabla
void CategoriaView::seleccionarCategoria(int fila, int columna)
{
    Q_UNUSED(columna);

    QTableWidgetItem *itemId = tablaCategorias->item(fila, 0);

    if (!itemId) {
        return;
    }

    categoriaSeleccionada = itemId->text().toInt();

    nombreInput->setText(
        tablaCategorias->item(fila, 1)->text()
    );

    descripcionInput->setText(
        tablaCategorias->item(fila, 2)->text()
    );
}

// Cargar categorías desde SQLite
void CategoriaView::cargarCategorias()
{
    QList<Categoria> categorias = controller->obtenerCategorias();

    tablaCategorias->setRowCount(0);

    for (const Categoria &categoria : categorias) {
        int fila = tablaCategorias->rowCount();

        tablaCategorias->insertRow(fila);

        tablaCategorias->setItem(
            fila, 0,
            new QTableWidgetItem(QString::number(categoria.id))
        );

        tablaCategorias->setItem(
            fila, 1,
            new QTableWidgetItem(categoria.nombre)
        );

        tablaCategorias->setItem(
            fila, 2,
            new QTableWidgetItem(categoria.descripcion)
        );
    }
}
