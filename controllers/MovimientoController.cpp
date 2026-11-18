
#include "MovimientoController.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

MovimientoController::MovimientoController(QSqlDatabase database)
    : db(database)
{
}

bool MovimientoController::registrarMovimiento(
    const Movimiento &movimiento)
{
    // 1. Verificar la conexión
    if (!db.isValid() || !db.isOpen()) {
        qWarning() << "La base de datos no está disponible.";
        return false;
    }

    // 2. Validar los datos
    if (movimiento.idProducto <= 0 ||
        movimiento.cantidad <= 0 ||
        (movimiento.tipo != "ENTRADA" &&
         movimiento.tipo != "SALIDA")) {

        qWarning() << "Datos del movimiento inválidos.";
        return false;
    }

    // 3. Iniciar una transacción
    if (!db.transaction()) {
        qWarning() << "No se pudo iniciar la transacción:"
                   << db.lastError().text();
        return false;
    }

    // 4. Consultar si el producto existe y obtener su cantidad
    QSqlQuery consulta(db);

    consulta.prepare(
        "SELECT cantidad FROM PRODUCTOS "
        "WHERE id_producto = ?"
    );

    consulta.addBindValue(movimiento.idProducto);

    if (!consulta.exec() || !consulta.next()) {
        qWarning() << "No se encontró el producto:"
                   << consulta.lastError().text();

        db.rollback();
        return false;
    }

    // 5. Actualizar las existencias
    QSqlQuery actualizacion(db);

    if (movimiento.tipo == "ENTRADA") {

        actualizacion.prepare(
            "UPDATE PRODUCTOS "
            "SET cantidad = cantidad + ? "
            "WHERE id_producto = ?"
        );

    } else {

        actualizacion.prepare(
            "UPDATE PRODUCTOS "
            "SET cantidad = cantidad - ? "
            "WHERE id_producto = ? AND cantidad >= ?"
        );

        actualizacion.addBindValue(movimiento.cantidad);
        actualizacion.addBindValue(movimiento.idProducto);
        actualizacion.addBindValue(movimiento.cantidad);
    }

    if (movimiento.tipo == "ENTRADA") {
        actualizacion.addBindValue(movimiento.cantidad);
        actualizacion.addBindValue(movimiento.idProducto);
    }

    if (!actualizacion.exec() ||
        actualizacion.numRowsAffected() != 1) {

        qWarning() << "No se pudo actualizar el inventario:"
                   << actualizacion.lastError().text();

        db.rollback();
        return false;
    }

    // 6. Registrar el movimiento en el historial
    QSqlQuery insercion(db);

    insercion.prepare(
        "INSERT INTO MOVIMIENTOS "
        "(id_producto, tipo, cantidad, fecha, observacion) "
        "VALUES (?, ?, ?, ?, ?)"
    );

    insercion.addBindValue(movimiento.idProducto);
    insercion.addBindValue(movimiento.tipo);
    insercion.addBindValue(movimiento.cantidad);

    insercion.addBindValue(
        movimiento.fecha.isValid()
            ? movimiento.fecha.toString("yyyy-MM-dd")
            : QDate::currentDate().toString("yyyy-MM-dd")
    );

    insercion.addBindValue(movimiento.observacion);

    if (!insercion.exec()) {
        qWarning() << "Error registrando movimiento:"
                   << insercion.lastError().text();

        db.rollback();
        return false;
    }

    // 7. Confirmar los cambios
    if (!db.commit()) {
        qWarning() << "No se pudieron confirmar los cambios:"
                   << db.lastError().text();

        db.rollback();
        return false;
    }

    return true;
}

QList<Movimiento> MovimientoController::obtenerMovimientos()
{
    QList<Movimiento> movimientos;

    QSqlQuery query(db);

    if (!query.exec(
        "SELECT id_movimiento, id_producto, tipo, "
        "cantidad, fecha, observacion "
        "FROM MOVIMIENTOS "
        "ORDER BY id_movimiento DESC"
    )) {
        qWarning() << "Error consultando movimientos:"
                   << query.lastError().text();

        return movimientos;
    }

    while (query.next()) {

        Movimiento movimiento;

        movimiento.id =
            query.value("id_movimiento").toInt();

        movimiento.idProducto =
            query.value("id_producto").toInt();

        movimiento.tipo =
            query.value("tipo").toString();

        movimiento.cantidad =
            query.value("cantidad").toInt();

        movimiento.fecha = QDate::fromString(
            query.value("fecha").toString(),
            "yyyy-MM-dd"
        );

        movimiento.observacion =
            query.value("observacion").toString();

        movimientos.append(movimiento);
    }

    return movimientos;
}
