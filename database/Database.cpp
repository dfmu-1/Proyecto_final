
#include "Database.h"

#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Database::Database()
{
}

bool Database::open()
{
    const QString connectionName = "StockManagerConnection";

    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }

    if (db.isOpen()) {
        return true;
    }

    db.setDatabaseName(
        QCoreApplication::applicationDirPath()
        + "/stockmanager.db"
    );

    if (!db.open()) {
        qDebug() << "Error al conectar:"
                 << db.lastError().text();

        return false;
    }

    return true;
}
QSqlDatabase Database::connection() const
{
    return db;
}

bool Database::initialize()
{
    QSqlQuery query(db);

    query.exec("PRAGMA foreign_keys = ON");

    const QStringList tables = {

        "CREATE TABLE IF NOT EXISTS CATEGORIAS ("
        "id_categoria INTEGER PRIMARY KEY AUTOINCREMENT,"
        "nombre TEXT NOT NULL UNIQUE,"
        "descripcion TEXT"
        ")",

        "CREATE TABLE IF NOT EXISTS PRODUCTOS ("
        "id_producto INTEGER PRIMARY KEY AUTOINCREMENT,"
        "codigo TEXT NOT NULL UNIQUE,"
        "nombre TEXT NOT NULL,"
        "cantidad INTEGER NOT NULL DEFAULT 0 CHECK(cantidad >= 0),"
        "precio REAL NOT NULL DEFAULT 0 CHECK(precio >= 0),"
        "ubicacion TEXT,"
        "fecha_adquisicion TEXT,"
        "stock_minimo INTEGER NOT NULL DEFAULT 5 CHECK(stock_minimo >= 0),"
        "id_categoria INTEGER,"
        "FOREIGN KEY(id_categoria) REFERENCES CATEGORIAS(id_categoria)"
        ")",

        "CREATE TABLE IF NOT EXISTS MOVIMIENTOS ("
        "id_movimiento INTEGER PRIMARY KEY AUTOINCREMENT,"
        "id_producto INTEGER NOT NULL,"
        "tipo TEXT NOT NULL CHECK(tipo IN ('ENTRADA', 'SALIDA')),"
        "cantidad INTEGER NOT NULL CHECK(cantidad > 0),"
        "fecha TEXT NOT NULL DEFAULT CURRENT_DATE,"
        "observacion TEXT,"
        "FOREIGN KEY(id_producto) REFERENCES PRODUCTOS(id_producto)"
        ")"
    };

    for (const QString &table : tables) {

        if (!query.exec(table)) {

            qDebug() << "Error creando tabla:"
                     << query.lastError().text();

            return false;
        }
    }

    return true;
}
