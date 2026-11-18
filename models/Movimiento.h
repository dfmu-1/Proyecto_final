
#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include <QString>
#include <QDate>

class Movimiento
{
public:
    Movimiento();

    int id;
    int idProducto;
    QString tipo;
    int cantidad;
    QDate fecha;
    QString observacion;
};

#endif
