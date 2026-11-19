
#include "AlertaController.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AlertaController::AlertaController(QSqlDatabase database)
    : db(database)
{
}

QList<Producto> AlertaController::obtenerProductosConStockBajo()
{
    QList<Producto> productos;

    if (!db.isValid() || !db.isOpen()) {
        qWarning() << "La conexión a la base de datos no está disponible.";
        return productos;
    }

    QSqlQuery query(db);

    query.prepare(
        "SELECT * FROM PRODUCTOS "
        "WHERE cantidad <= stock_minimo "
        "ORDER BY cantidad ASC"
    );

    if (!query.exec()) {
        qWarning() << "Error consultando alertas:"
                   << query.lastError().text();

        return productos;
    }

    while (query.next()) {

        Producto producto;

        producto.id = query.value("id_producto").toInt();
        producto.codigo = query.value("codigo").toString();
        producto.nombre = query.value("nombre").toString();
        producto.cantidad = query.value("cantidad").toInt();
        producto.precio = query.value("precio").toDouble();
        producto.ubicacion = query.value("ubicacion").toString();
        producto.stockMinimo = query.value("stock_minimo").toInt();
        producto.idCategoria = query.value("id_categoria").toInt();

        productos.append(producto);
    }

    return productos;
}
