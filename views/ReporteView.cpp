
#include "ReporteView.h"
#include "../controllers/ReporteController.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QLocale>
#include <QFileDialog>
#include <QMessageBox>

ReporteView::ReporteView(ReporteController *controller,
                         QWidget *parent)
    : QWidget(parent),
      controller(controller)
{
    configurarInterfaz();
    cargarReportes();
}

void ReporteView::configurarInterfaz()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titulo = new QLabel("Reportes del inventario");
    titulo->setStyleSheet(
        "font-size: 22px; font-weight: bold;"
    );

    totalInventario = new QLabel("Valor total del inventario: $0");

    actualizarButton = new QPushButton("Actualizar reporte");

    // Crear el boton de exportacion
    exportarCSVButton = new QPushButton("Exportar CSV");
    exportarPDFButton = new QPushButton("Exportar PDF");

    tablaReportes = new QTableWidget(this);

    tablaReportes->setColumnCount(6);

    tablaReportes->setHorizontalHeaderLabels({
        "Código",
        "Producto",
        "Categoría",
        "Cantidad",
        "Precio unitario",
        "Valor total"
    });

    tablaReportes->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tablaReportes->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Agregar los elementos al layout
    layout->addWidget(titulo);
    layout->addWidget(totalInventario);
    layout->addWidget(actualizarButton);
    layout->addWidget(exportarCSVButton);
    layout->addWidget(exportarPDFButton);
    layout->addWidget(tablaReportes);

    // Conectar los botones
    connect(actualizarButton, &QPushButton::clicked,
            this, &ReporteView::actualizarReportes);

    connect(exportarCSVButton, &QPushButton::clicked,
            this, &ReporteView::exportarCSV);

    connect(exportarPDFButton, &QPushButton::clicked,
            this, &ReporteView::exportarPDF);
}

void ReporteView::cargarReportes()
{
    QList<Reporte> reportes = controller->obtenerReporte();

    tablaReportes->setRowCount(0);

    double totalGeneral = 0.0;

    QLocale locale(QLocale::Spanish, QLocale::Colombia);

    for (const Reporte &reporte : reportes) {

        int fila = tablaReportes->rowCount();

        tablaReportes->insertRow(fila);

        tablaReportes->setItem(fila, 0,
            new QTableWidgetItem(reporte.codigo));

        tablaReportes->setItem(fila, 1,
            new QTableWidgetItem(reporte.nombre));

        tablaReportes->setItem(fila, 2,
            new QTableWidgetItem(reporte.categoria));

        tablaReportes->setItem(fila, 3,
            new QTableWidgetItem(
                QString::number(reporte.cantidad)
            ));

        tablaReportes->setItem(fila, 4,
            new QTableWidgetItem(
                locale.toCurrencyString(reporte.precio, "$")
            ));

        tablaReportes->setItem(fila, 5,
            new QTableWidgetItem(
                locale.toCurrencyString(reporte.valorTotal, "$")
            ));

        totalGeneral += reporte.valorTotal;
    }

    totalInventario->setText(
        "Valor total del inventario: " +
        locale.toCurrencyString(totalGeneral, "$")
    );
}

void ReporteView::actualizarReportes()
{
    cargarReportes();
}

void ReporteView::exportarCSV()
{
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar reporte CSV",
        "Reporte_Inventario.csv",
        "Archivos CSV (*.csv)"
    );

    if (ruta.isEmpty()) {
        return;
    }

    if (!ruta.endsWith(".csv", Qt::CaseInsensitive)) {
        ruta += ".csv";
    }

    if (controller->exportarCSV(ruta)) {
        QMessageBox::information(
            this,
            "Exportación exitosa",
            "El reporte CSV se guardó correctamente."
        );
    } else {
        QMessageBox::critical(
            this,
            "Error",
            "No fue posible guardar el reporte CSV."
        );
    }
}

void ReporteView::exportarPDF()
{
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar reporte PDF",
        "Reporte_Inventario.pdf",
        "Archivos PDF (*.pdf)"
    );

    if (ruta.isEmpty()) {
        return;
    }

    if (!ruta.endsWith(".pdf", Qt::CaseInsensitive)) {
        ruta += ".pdf";
    }

    if (controller->exportarPDF(ruta)) {
        QMessageBox::information(
            this,
            "Exportación exitosa",
            "El reporte PDF se guardó correctamente."
        );
    } else {
        QMessageBox::critical(
            this,
            "Error",
            "No fue posible guardar el reporte PDF."
        );
    }
}

