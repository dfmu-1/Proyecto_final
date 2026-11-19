
#ifndef REPORTECONTROLLER_H
#define REPORTECONTROLLER_H

#include "../reports/Reporte.h"
#include <QSqlDatabase>
#include <QList>
#include <QString>

class ReporteController {
public:
    explicit ReporteController(QSqlDatabase database);
    bool exportarPDF(const QString &rutaArchivo);

    QList<Reporte> obtenerReporte();

    bool exportarCSV(const QString &rutaArchivo);

private:
    QSqlDatabase db;
};

#endif
