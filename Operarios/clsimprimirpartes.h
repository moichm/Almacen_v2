#ifndef CLSIMPRIMIRPARTES_H
#define CLSIMPRIMIRPARTES_H

#include <QPainter>
#include "Operarios/operario.h"

class clsImprimirPartes
{
public:
    clsImprimirPartes();
    void ImprimirListaPartes(QString strFiltro);
    void ImprimirFichaPartes(const clsPartes_Trabajo *Parte);
    ~clsImprimirPartes();
private:
    void DividirEnPaginasLP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLP(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLP(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLP(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLP(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLP(QPainter *painter);

    int CalcularPaginasFP(QPainter *painter, const QStringList &entradas);
    void DividirEnPaginasFP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFP(QPainter *painter, const QString &entrada);
    int AltoEntradaFPL(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFP(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFP(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFP(QPainter *painter, const QString &cadena);
    void ImprimirLineaFPL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFP(QPainter *painter);
    void ImprimirCabeceraFPL(QPainter *painter);
    void ImprimirFirmaFP(QPainter *painter);

    QPrinter *printer;
    int TotalPaginas;
    int maxAlt;
    int maxAnch;
    int maxAltDispo;
    int colorLinea;
    int yLinea;
    int Espacio, EspacioCorto;
    int AnchoDesc;
    int AnchoFecha;
    int AnchoNum;
    int AnchoNIF;
    int AnchoRes;
    int AnchoNumH;
    int AnchoTHora;
    int AnchoCol1;
    int AnchoCol2;
    int AnchoCol;
    int numPaginas;
    QString entrada;
    QString entradaCab;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QPen penInicial;
    QRect rectTitulo,rectCabecera,rectImagen,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial,rectFirmaRes,rectFirmaRRHH;
    QString titulo;
};

#endif // CLSIMPRIMIRPARTES_H
