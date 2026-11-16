
#include "InventarioController.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

InventarioController::InventarioController(QSqlDatabase database)
    : db(database)
{
}

bool InventarioController::registrarProducto(
    const Producto &producto)
{
    if (!db.isValid() || !db.isOpen()) {
        qWarning() << "No se puede registrar el producto: "
                      "la conexión a la base de datos no está abierta.";
        return false;
    }

    if (producto.codigo.trimmed().isEmpty()
        || producto.nombre.trimmed().isEmpty()) {
        qWarning() << "No se puede registrar el producto: "
                      "el código y el nombre son obligatorios.";
        return false;
    }

    if (producto.cantidad < 0
        || producto.precio < 0
        || producto.stockMinimo < 0
        || producto.idCategoria < 0) {
        qWarning() << "No se puede registrar el producto: "
                      "cantidad, precio, stock mínimo y categoría "
                      "no pueden ser negativos.";
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO PRODUCTOS "
        "(codigo, nombre, cantidad, precio, ubicacion, "
        "fecha_adquisicion, stock_minimo, id_categoria) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
    );

    query.addBindValue(producto.codigo.trimmed());
    query.addBindValue(producto.nombre.trimmed());
    query.addBindValue(producto.cantidad);
    query.addBindValue(producto.precio);
    query.addBindValue(producto.ubicacion);
    query.addBindValue(producto.fechaAdquisicion.isValid()
                           ? QVariant(producto.fechaAdquisicion.toString(
                                 "yyyy-MM-dd"))
                           : QVariant());
    query.addBindValue(producto.stockMinimo);
    query.addBindValue(producto.idCategoria == 0
                       ? QVariant()
                       : QVariant(producto.idCategoria));

    if (!query.exec()) {
        qWarning() << "Error registrando producto:"
                   << query.lastError().text();

        return false;
    }

    return true;
}

QList<Producto> InventarioController::obtenerProductos()
{
    QList<Producto> productos;

    QSqlQuery query(db);

    if (!query.exec("SELECT * FROM PRODUCTOS")) {
        qDebug() << query.lastError().text();
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

        producto.fechaAdquisicion = QDate::fromString(
            query.value("fecha_adquisicion").toString(),
            "yyyy-MM-dd"
        );

        producto.stockMinimo = query.value("stock_minimo").toInt();

        producto.idCategoria =
            query.value("id_categoria").toInt();

        productos.append(producto);
    }

    return productos;
}
