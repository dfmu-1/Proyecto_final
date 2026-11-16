
#ifndef INVENTARIOCONTROLLER_H
#define INVENTARIOCONTROLLER_H

#include "../models/Producto.h"
#include <QSqlDatabase>
#include <QList>

class InventarioController
{
public:
    explicit InventarioController(QSqlDatabase database);

    bool registrarProducto(const Producto &producto);
    QList<Producto> obtenerProductos();

private:
    QSqlDatabase db;
};

#endif
