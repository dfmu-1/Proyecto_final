
#ifndef ALERTACONTROLLER_H
#define ALERTACONTROLLER_H

#include "../models/Producto.h"
#include <QSqlDatabase>
#include <QList>

class AlertaController
{
public:
    explicit AlertaController(QSqlDatabase database);

    QList<Producto> obtenerProductosConStockBajo();

private:
    QSqlDatabase db;
};

#endif
