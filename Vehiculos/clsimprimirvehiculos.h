#ifndef CLSIMPRIMIRVEHICULOS_H
#define CLSIMPRIMIRVEHICULOS_H

#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include "Centros/centro.h"
#include "Vehiculos/vehiculo.h"

class clsImprimirVehiculos
{
public:
    clsImprimirVehiculos();
    void ImprimirLista(QString strFiltro);
    void ImprimirFicha(const clsVehiculos *Vehi);
private:
    void DividirEnPaginasLV(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLV(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLV(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLV(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLV(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLV(QPainter *painter);

    void DividirEnPaginasFV(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFV(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFV(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFV(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFVM(QPainter *painter, const QString &cadena);
    void ImprimirLineaFV(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFV(QPainter *painter);
    void ImprimirCabeceraFVM(QPainter *painter);
    void ImprimirImagen(QPainter *painter);

    QPrinter *printer;
    int colorLinea;
    int maxAlt;
    int maxAnch;
    int Espacio, EspacioCorto;
    int maxAltDispo;
    int cod_diagnostico;
    int AnchoFMat;
    int AnchoMat;
    int AnchoCol;
    int AnchoCol1;
    int AnchoCol2;
    int yLinea;
    int numPaginas;
    QPen penInicial;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QRect rectTitulo,rectCabecera,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial,rectImagen;
    QString titulo;
    QImage *imagen;
};

#endif // CLSIMPRIMIRVEHICULOS_H
