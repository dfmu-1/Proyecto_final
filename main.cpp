
#include <QApplication>
#include <QMessageBox>
#include "controllers/MovimientoController.h"
#include "controllers/AlertaController.h"
#include "controllers/ReporteController.h"

#include "views/MainWindow.h"
#include "database/Database.h"

#include "controllers/InventarioController.h"
#include "controllers/CategoriaController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Inicializar la base de datos
    Database database;

    if (!database.open() || !database.initialize()) {
        QMessageBox::critical(
            nullptr,
            "Error de base de datos",
            "No fue posible iniciar la base de datos."
        );

        return -1;
    }
    MovimientoController movimientoController(
        database.connection()
    );

    // Crear los controladores
    InventarioController inventarioController(
        database.connection()
    );

    CategoriaController categoriaController(
        database.connection()
    );
    AlertaController alertaController(
        database.connection()
    );
    ReporteController reporteController(
        database.connection()
    );

    // Crear la ventana principal
    MainWindow window(
    &inventarioController,
    &categoriaController,
    &movimientoController,
    &alertaController,
    &reporteController
);

    // Mostrar ventana
    window.show();

    return app.exec();
}
