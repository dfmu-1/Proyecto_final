
#ifndef MOVIMIENTOCONTROLLER_H
#define MOVIMIENTOCONTROLLER_H

#include "../models/Movimiento.h"
#include <QSqlDatabase>
#include <QList>

class MovimientoController
{
public:
    explicit MovimientoController(QSqlDatabase database);

    bool registrarMovimiento(const Movimiento &movimiento);

    QList<Movimiento> obtenerMovimientos();

private:
    QSqlDatabase db;
};

#endif
