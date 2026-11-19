
#include "InventarioController.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QStringList>

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

    QString consulta = R"(
        SELECT
            P.id_producto,
            P.codigo,
            P.nombre,
            P.cantidad,
            P.precio,
            P.ubicacion,
            P.fecha_adquisicion,
            P.stock_minimo,
            P.id_categoria,
            C.nombre AS nombre_categoria
        FROM PRODUCTOS P
        LEFT JOIN CATEGORIAS C
            ON P.id_categoria = C.id_categoria
        ORDER BY P.id_producto
    )";

    if (!query.exec(consulta)) {
        qDebug() << "Error consultando productos:"
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

        producto.fechaAdquisicion = QDate::fromString(
            query.value("fecha_adquisicion").toString(),
            "yyyy-MM-dd"
        );

        producto.stockMinimo = query.value("stock_minimo").toInt();
        producto.idCategoria = query.value("id_categoria").toInt();

        producto.nombreCategoria =
            query.value("nombre_categoria").toString();

        if (producto.nombreCategoria.isEmpty()) {
            producto.nombreCategoria = "Sin categoría";
        }

        productos.append(producto);
    }

    return productos;
}

bool InventarioController::actualizarProducto(const Producto &producto)
{
    if (!db.isOpen()) {
        return false;
    }

    if (producto.id <= 0 ||
        producto.codigo.trimmed().isEmpty() ||
        producto.nombre.trimmed().isEmpty() ||
        producto.cantidad < 0 ||
        producto.precio < 0 ||
        producto.stockMinimo < 0 ||
        producto.idCategoria < 0) {
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE PRODUCTOS SET "
        "codigo = ?, "
        "nombre = ?, "
        "cantidad = ?, "
        "precio = ?, "
        "ubicacion = ?, "
        "fecha_adquisicion = ?, "
        "stock_minimo = ?, "
        "id_categoria = ? "
        "WHERE id_producto = ?"
    );

    query.addBindValue(producto.codigo);
    query.addBindValue(producto.nombre);
    query.addBindValue(producto.cantidad);
    query.addBindValue(producto.precio);
    query.addBindValue(producto.ubicacion);
    query.addBindValue(producto.fechaAdquisicion);
    query.addBindValue(producto.stockMinimo);

    if (producto.idCategoria == 0) {
        query.addBindValue(QVariant());
    } else {
        query.addBindValue(producto.idCategoria);
    }

    query.addBindValue(producto.id);

    if (!query.exec()) {
        return false;
    }

    return query.numRowsAffected() == 1;
}


bool InventarioController::eliminarProducto(int idProducto)
{
    if (!db.isOpen() || idProducto <= 0) {
        return false;
    }

    // Comprobar si el producto tiene movimientos registrados.
    QSqlQuery consulta(db);

    consulta.prepare(
        "SELECT COUNT(*) FROM MOVIMIENTOS "
        "WHERE id_producto = ?"
    );

    consulta.addBindValue(idProducto);

    if (!consulta.exec() || !consulta.next()) {
        return false;
    }

    int cantidadMovimientos = consulta.value(0).toInt();

    // No permitir eliminar productos con historial.
    if (cantidadMovimientos > 0) {
        return false;
    }

    // Eliminar el producto.
    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM PRODUCTOS WHERE id_producto = ?"
    );

    query.addBindValue(idProducto);

    if (!query.exec()) {
        return false;
    }

    return query.numRowsAffected() == 1;
}

QList<Producto> InventarioController::filtrarProductos(
    const QString &texto,
    int idCategoria,
    int estadoStock)
{
    QList<Producto> productos;

    if (!db.isOpen()) {
        return productos;
    }

    QSqlQuery query(db);

    QString consulta = R"(
        SELECT
            P.id_producto,
            P.codigo,
            P.nombre,
            P.cantidad,
            P.precio,
            P.ubicacion,
            P.fecha_adquisicion,
            P.stock_minimo,
            P.id_categoria,
            C.nombre AS nombre_categoria
        FROM PRODUCTOS P
        LEFT JOIN CATEGORIAS C
            ON P.id_categoria = C.id_categoria
    )";

    QStringList condiciones;

    // 1. Buscar por código, nombre o ubicación.
    if (!texto.trimmed().isEmpty()) {
        condiciones.append(
            "(P.codigo LIKE ? OR P.nombre LIKE ? "
            "OR P.ubicacion LIKE ?)"
        );
    }

    // 2. Filtrar por categoría.
    if (idCategoria == 0) {
        condiciones.append("P.id_categoria IS NULL");
    }
    else if (idCategoria > 0) {
        condiciones.append("P.id_categoria = ?");
    }

    // 3. Filtrar por existencias.
    if (estadoStock == 1) {
        condiciones.append("P.cantidad <= P.stock_minimo");
    }
    else if (estadoStock == 2) {
        condiciones.append("P.cantidad > P.stock_minimo");
    }

    // Agregar las condiciones a la consulta.
    if (!condiciones.isEmpty()) {
        consulta += " WHERE " + condiciones.join(" AND ");
    }

    consulta += " ORDER BY P.id_producto";

    query.prepare(consulta);

    // Enlazar el texto de búsqueda.
    if (!texto.trimmed().isEmpty()) {
        QString busqueda = "%" + texto.trimmed() + "%";

        query.addBindValue(busqueda);
        query.addBindValue(busqueda);
        query.addBindValue(busqueda);
    }

    // Enlazar la categoría cuando corresponda.
    if (idCategoria > 0) {
        query.addBindValue(idCategoria);
    }

    if (!query.exec()) {
        qWarning() << "Error filtrando productos:"
                   << query.lastError().text();

        return productos;
    }

    // Convertir los resultados SQL en objetos Producto.
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
        producto.idCategoria = query.value("id_categoria").toInt();

        producto.nombreCategoria =
            query.value("nombre_categoria").toString();

        if (producto.nombreCategoria.isEmpty()) {
            producto.nombreCategoria = "Sin categoría";
        }

        productos.append(producto);
    }

    return productos;
}