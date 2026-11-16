
#include "MainWindow.h"
#include "ProductoView.h"

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(InventarioController *controller, QWidget *parent)
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
            pageLayout->addWidget(new ProductoView(controller, page));
        } else {
            QLabel *pageTitle = new QLabel(sections[i]);
            pageTitle->setStyleSheet(
                "font-size: 26px; font-weight: bold;"
            );

            pageLayout->addWidget(pageTitle);
            pageLayout->addStretch();
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
