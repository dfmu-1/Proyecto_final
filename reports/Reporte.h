
#ifndef REPORTE_H
#define REPORTE_H

#include <QString>

class Reporte {
public:
    QString codigo;
    QString nombre;
    QString categoria;

    int cantidad;
    double precio;
    double valorTotal;

    Reporte();
};

#endif
