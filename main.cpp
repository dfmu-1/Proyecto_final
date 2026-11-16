
#include <QApplication>
#include <QMessageBox>

#include "views/MainWindow.h"
#include "database/Database.h"
#include "controllers/InventarioController.h"
#include "controllers/CategoriaController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Inicializar base de datos
    Database database;

    if (!database.open() || !database.initialize()) {
        QMessageBox::critical(
            nullptr,
            "Error de base de datos",
            "No fue posible iniciar la base de datos."
        );

        return -1;
    }

    // Crear controladores
    InventarioController inventarioController(database.connection());

    CategoriaController categoriaController(database.connection());

    // Crear ventana principal
    MainWindow ventana(
        &inventarioController,
        &categoriaController
    );

    ventana.show();

    return app.exec();
}
