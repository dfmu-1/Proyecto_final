
#include "CategoriaController.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

// Constructor
CategoriaController::CategoriaController(QSqlDatabase database)
    : db(database)
{
}

// Registrar una categoría
bool CategoriaController::registrarCategoria(const Categoria &categoria)
{
    if (categoria.nombre.trimmed().isEmpty()) {
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "INSERT INTO CATEGORIAS (nombre, descripcion) "
        "VALUES (:nombre, :descripcion)"
    );

    query.bindValue(":nombre", categoria.nombre.trimmed());
    query.bindValue(":descripcion", categoria.descripcion.trimmed());

    if (!query.exec()) {
        qDebug() << "Error al registrar categoría:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

// Consultar todas las categorías
QList<Categoria> CategoriaController::obtenerCategorias()
{
    QList<Categoria> categorias;

    QSqlQuery query(db);

    query.prepare(
        "SELECT id_categoria, nombre, descripcion "
        "FROM CATEGORIAS ORDER BY id_categoria"
    );

    if (!query.exec()) {
        qDebug() << "Error al consultar categorías:"
                 << query.lastError().text();
        return categorias;
    }

    while (query.next()) {
        Categoria categoria;

        categoria.id = query.value("id_categoria").toInt();
        categoria.nombre = query.value("nombre").toString();
        categoria.descripcion = query.value("descripcion").toString();

        categorias.append(categoria);
    }

    return categorias;
}

// Actualizar una categoría
bool CategoriaController::actualizarCategoria(const Categoria &categoria)
{
    if (categoria.id <= 0 ||
        categoria.nombre.trimmed().isEmpty()) {
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "UPDATE CATEGORIAS "
        "SET nombre = :nombre, descripcion = :descripcion "
        "WHERE id_categoria = :id"
    );

    query.bindValue(":nombre", categoria.nombre.trimmed());
    query.bindValue(":descripcion", categoria.descripcion.trimmed());
    query.bindValue(":id", categoria.id);

    if (!query.exec()) {
        qDebug() << "Error al actualizar categoría:"
                 << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// Eliminar una categoría
bool CategoriaController::eliminarCategoria(int id)
{
    if (id <= 0) {
        return false;
    }

    QSqlQuery query(db);

    query.prepare(
        "DELETE FROM CATEGORIAS "
        "WHERE id_categoria = :id"
    );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error al eliminar categoría:"
                 << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
