
#ifndef REPORTEVIEW_H
#define REPORTEVIEW_H

#include <QWidget>

class ReporteController;
class QTableWidget;
class QLabel;
class QPushButton;


class ReporteView : public QWidget {
    Q_OBJECT

public:
    explicit ReporteView(ReporteController *controller,
                         QWidget *parent = nullptr);

private:
    ReporteController *controller;
    QPushButton *exportarCSVButton;
    QPushButton *exportarPDFButton;

    QTableWidget *tablaReportes;
    QLabel *totalInventario;
    QPushButton *actualizarButton;

    void configurarInterfaz();
    void cargarReportes();

private slots:
    void actualizarReportes();
    void exportarCSV();
    void exportarPDF();
};

#endif
