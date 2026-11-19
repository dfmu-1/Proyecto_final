
#ifndef INVENTARIOCONTROLLER_H
#define INVENTARIOCONTROLLER_H

#include "../models/Producto.h"
#include <QSqlDatabase>
#include <QList>
#include <QString>

class InventarioController
{
public:
    explicit InventarioController(QSqlDatabase database);

    bool registrarProducto(const Producto &producto);
    QList<Producto> filtrarProductos(
    const QString &texto,
    int idCategoria,
    int estadoStock
    );
    QList<Producto> obtenerProductos();
    QList<Producto> buscarProductos(const QString &texto);
    bool actualizarProducto(const Producto &producto);
    bool eliminarProducto(int idProducto);

private:
    QSqlDatabase db;
};

#endif
