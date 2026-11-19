
#ifndef ALERTAVIEW_H
#define ALERTAVIEW_H

#include <QWidget>

class AlertaController;
class QTableWidget;
class QPushButton;
class QLabel;

class AlertaView : public QWidget
{
    Q_OBJECT

public:
    explicit AlertaView(
        AlertaController *controller,
        QWidget *parent = nullptr
    );

private:
    AlertaController *controller;

    QTableWidget *tablaAlertas;
    QLabel *contadorAlertas;
    QPushButton *actualizarButton;

    void configurarInterfaz();
    void cargarAlertas();

private slots:
    void actualizarAlertas();
};

#endif
