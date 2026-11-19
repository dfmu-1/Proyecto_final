
#include "MainWindow.h"
#include "ProductoView.h"
#include "CategoriaView.h"
#include "MovimientoView.h"
#include "AlertaView.h"
#include "ReporteView.h"

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(InventarioController *inventarioController,
                       CategoriaController *categoriaController,
                       MovimientoController *movimientoController,
                       AlertaController *alertaController,
                       ReporteController *reporteController,
                       QWidget *parent)

    : QMainWindow(parent)
{
    setWindowTitle("StockManager - Gestor de Inventario");
    resize(1100, 700);

    // Widget principal
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Menu lateral
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet(
        "background-color: #263449;"
    );

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);

    QLabel *title = new QLabel("StockManager");
    title->setStyleSheet(
        "color: white; font-size: 22px; font-weight: bold;"
    );

    sidebarLayout->addWidget(title);
    sidebarLayout->addSpacing(25);

    // Panel de contenido
    QStackedWidget *pages = new QStackedWidget();

    QStringList sections = {
        "Inicio",
        "Productos",
        "Categorías",
        "Movimientos",
        "Alertas",
        "Reportes"
    };

    for (int i = 0; i < sections.size(); ++i) {

        QPushButton *button = new QPushButton(sections[i]);

        button->setStyleSheet(
            "QPushButton {"
            "color: white;"
            "background-color: #34465e;"
            "padding: 12px;"
            "text-align: left;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: #456080;"
            "}"
        );

        sidebarLayout->addWidget(button);

        // Crear pagina
        QWidget *page = new QWidget();
        QVBoxLayout *pageLayout = new QVBoxLayout(page);

        if (i == 1) {

    pageLayout->addWidget(
        new ProductoView(inventarioController, categoriaController, page)
    );

        } else if (i == 2) {

        pageLayout->addWidget(
            new CategoriaView(categoriaController, page)
            );

        } else if (i == 3) {

        pageLayout->addWidget(
        new MovimientoView(movimientoController, inventarioController, page)
        );

        } else if (i == 4) {

        pageLayout->addWidget(
        new AlertaView(alertaController, page)
        );

        } else if (i == 5) {
        page = new ReporteView(reporteController, page);
        }
        pages->addWidget(page);

        // Navegacion entre paginas
        connect(button, &QPushButton::clicked,
                pages, [pages, i]() {
                    pages->setCurrentIndex(i);
                });
    }

    sidebarLayout->addStretch();

    // Agregar elementos al layout principal
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pages);

}
