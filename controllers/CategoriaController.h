
#ifndef CATEGORIACONTROLLER_H
#define CATEGORIACONTROLLER_H

#include "../models/Categoria.h"

#include <QSqlDatabase>
#include <QList>

class CategoriaController
{
public:
    explicit CategoriaController(QSqlDatabase database);

    bool registrarCategoria(const Categoria &categoria);
    QList<Categoria> obtenerCategorias();
    bool actualizarCategoria(const Categoria &categoria);
    bool eliminarCategoria(int id);

private:
    QSqlDatabase db;
};

#endif
