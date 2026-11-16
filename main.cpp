
#include <QApplication>
#include <QMessageBox>

#include "views/MainWindow.h"
#include "database/Database.h"
#include "controllers/InventarioController.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 1. Crear y abrir la base de datos
    Database database;

    if (!database.open() || !database.initialize()) {
        QMessageBox::critical(
            nullptr,
            "Error de base de datos",
            "No fue posible iniciar la base de datos."
        );

        return -1;
    }

    // 2. Crear el controlador del inventario
    InventarioController controller(database.connection());

    // 3. Crear la ventana principal y pasarle el controlador
    MainWindow ventana(&controller);

    // 4. Mostrar la aplicación
    ventana.show();

    return app.exec();
}
