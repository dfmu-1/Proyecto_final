
#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <QString>
#include <QDate>

class Producto
{
public:
    Producto();

    int id;
    QString codigo;
    QString nombre;
    int cantidad;
    double precio;
    QString ubicacion;
    QDate fechaAdquisicion;
    int stockMinimo;
    int idCategoria;
};

#endif
