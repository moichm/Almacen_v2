#ifndef CLSIMPRIMIROPERARIOS_H
#define CLSIMPRIMIROPERARIOS_H

#include <QPainter>
#include <QStringList>
#include "Operarios/operario.h"

class clsImprimirOperarios
{
public:
    clsImprimirOperarios();
    void ImprimirListaOperario(QString strFiltro);
    void ImprimirFichaOp(const clsOperarios *Op);
private:
    void DividirEnPaginasLOp(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLOp(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLOp(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLOp(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLOp(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLOp(QPainter *painter);

    void DividirEnPaginasFOp(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFOp(QPainter *painter, const QString &entrada);
    int AltoEntradaFOpSal(QPainter *painter, const QString &entrada);
    int AltoEntradaFOpPartes(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFOp(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFOp(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFOp(QPainter *painter, const QString &cadena);
    void ImprimirCabecera(QPainter *painter);
    void ImprimirCabeceraFOpS(QPainter *painter);
    void ImprimirCabeceraFOpP(QPainter *painter);
    void ImprimirCabeceraFOpPRes(QPainter *painter);
    void ImprimirCabeceraFOpPLin(QPainter *painter);
    void ObtenerSalidas(const QString Op, QStringList &entradasSal);
    void ObtenerPartesRes(const QString Op, QStringList &entradasParteRes);
    void ObtenerPartesLin(const QString Op, QStringList &entradasParteLin);
    void ImprimirImagen(QPainter *painter);

    QPrinter *printer;
    int colorLinea;
    int maxAlt;
    int maxAnch;
    int Espacio, EspacioCorto;
    int maxAltDispo;
    int yLinea;
    int AnchoNIF;
    int AnchoNomAp;
    int AnchoCentro;
    int AnchoFecha;
    int AnchoCol1;
    int AnchoCol2;
    int numPaginas;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QPen penInicial;
    QRect rectTitulo,rectCabecera,rectImagen,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial,rectTabla;
    QString titulo;
    QImage *imagen;
};

#endif // CLSIMPRIMIROPERARIOS_H
