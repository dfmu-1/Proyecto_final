
#ifndef CATEGORIAVIEW_H
#define CATEGORIAVIEW_H

#include <QWidget>

class CategoriaController;
class QLineEdit;
class QPushButton;
class QTableWidget;

class CategoriaView : public QWidget
{
    Q_OBJECT

public:
    explicit CategoriaView(
        CategoriaController *controller,
        QWidget *parent = nullptr
    );

private:
    CategoriaController *controller;

    QLineEdit *nombreInput;
    QLineEdit *descripcionInput;

    QPushButton *registrarButton;
    QPushButton *actualizarButton;
    QPushButton *eliminarButton;

    QTableWidget *tablaCategorias;

    int categoriaSeleccionada;

    void configurarInterfaz();
    void cargarCategorias();

private slots:
    void registrarCategoria();
    void actualizarCategoria();
    void eliminarCategoria();
    void seleccionarCategoria(int fila, int columna);
};

#endif
