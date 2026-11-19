
#include "ReporteController.h"
#include <QPrinter>
#include <QTextDocument>
#include <QPageSize>
#include <QFileInfo>
#include <QDate>
#include <QLocale>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>

ReporteController::ReporteController(QSqlDatabase database)
    : db(database)
{
}

QList<Reporte> ReporteController::obtenerReporte()
{
    QList<Reporte> reportes;

    if (!db.isOpen()) {
        qDebug() << "Error: la base de datos no esta abierta.";
        return reportes;
    }

    QSqlQuery query(db);

    QString sql = R"(
        SELECT
            P.codigo,
            P.nombre,
            COALESCE(C.nombre, 'Sin categoria'),
            P.cantidad,
            P.precio
        FROM PRODUCTOS P
        LEFT JOIN CATEGORIAS C
            ON P.id_categoria = C.id_categoria
        ORDER BY P.nombre ASC
    )";

    if (!query.exec(sql)) {
        qDebug() << "Error al generar reporte:"
                 << query.lastError().text();
        return reportes;
    }

    while (query.next()) {
        Reporte reporte;

        reporte.codigo = query.value(0).toString();
        reporte.nombre = query.value(1).toString();
        reporte.categoria = query.value(2).toString();
        reporte.cantidad = query.value(3).toInt();
        reporte.precio = query.value(4).toDouble();

        reporte.valorTotal = reporte.cantidad * reporte.precio;

        reportes.append(reporte);
    }

    return reportes;
}
bool ReporteController::exportarCSV(const QString &rutaArchivo)
{
    QFile archivo(rutaArchivo);

    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error al crear el archivo CSV:"
                 << archivo.errorString();
        return false;
    }

    QTextStream salida(&archivo);
    salida.setEncoding(QStringConverter::Utf8);

    // Encabezados del reporte
    salida << "Codigo;Producto;Categoria;Cantidad;"
              "Precio unitario;Valor total\n";

    QList<Reporte> reportes = obtenerReporte();

    for (const Reporte &reporte : reportes) {
        salida << reporte.codigo << ";"
               << reporte.nombre << ";"
               << reporte.categoria << ";"
               << reporte.cantidad << ";"
               << reporte.precio << ";"
               << reporte.valorTotal << "\n";
    }

    archivo.close();

    return true;
}
bool ReporteController::exportarPDF(const QString &rutaArchivo)
{
    QList<Reporte> reportes = obtenerReporte();

    QString html;

    html += R"(
        <html>
        <head>
            <meta charset="UTF-8">
            <style>
                body {
                    font-family: Arial;
                    font-size: 10pt;
                }

                h1 {
                    text-align: center;
                    color: #1f2937;
                }

                p {
                    text-align: center;
                }

                table {
                    width: 100%;
                    border-collapse: collapse;
                }

                th {
                    background-color: #dbeafe;
                }

                th, td {
                    border: 1px solid #777;
                    padding: 6px;
                    text-align: center;
                }

                .total {
                    text-align: right;
                    font-weight: bold;
                    margin-top: 20px;
                }
            </style>
        </head>
        <body>
    )";

    html += "<h1>Reporte de Inventario</h1>";

    html += "<p>Fecha de generación: ";
    html += QDate::currentDate().toString("dd/MM/yyyy");
    html += "</p>";

    html += R"(
        <table>
            <tr>
                <th>Código</th>
                <th>Producto</th>
                <th>Categoría</th>
                <th>Cantidad</th>
                <th>Precio unitario</th>
                <th>Valor total</th>
            </tr>
    )";

    double totalGeneral = 0.0;

    QLocale locale(QLocale::Spanish, QLocale::Colombia);

    for (const Reporte &reporte : reportes) {

        html += "<tr>";

        html += "<td>" + reporte.codigo.toHtmlEscaped() + "</td>";
        html += "<td>" + reporte.nombre.toHtmlEscaped() + "</td>";
        html += "<td>" + reporte.categoria.toHtmlEscaped() + "</td>";

        html += "<td>" + QString::number(reporte.cantidad) + "</td>";

        html += "<td>" +
                locale.toCurrencyString(reporte.precio, "$") +
                "</td>";

        html += "<td>" +
                locale.toCurrencyString(reporte.valorTotal, "$") +
                "</td>";

        html += "</tr>";

        totalGeneral += reporte.valorTotal;
    }

    html += "</table>";

    html += "<p class='total'>Valor total del inventario: ";
    html += locale.toCurrencyString(totalGeneral, "$");
    html += "</p>";

    html += "</body></html>";

    QTextDocument documento;
    documento.setHtml(html);

    QPrinter impresora(QPrinter::HighResolution);

    impresora.setOutputFormat(QPrinter::PdfFormat);
    impresora.setOutputFileName(rutaArchivo);
    impresora.setPageSize(QPageSize(QPageSize::A4));

    documento.print(&impresora);

    QFileInfo archivo(rutaArchivo);

    return archivo.exists() && archivo.size() > 0;
}