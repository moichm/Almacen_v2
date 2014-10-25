#ifndef CLSIMPRIMIRPRODUCTOS_H
#define CLSIMPRIMIRPRODUCTOS_H

#include <QPainter>
#include "Productos/producto.h"

class clsImprimirProductos
{
public:
    clsImprimirProductos();
    void ImprimirListaProd(QString strFiltro);
    void ImprimirListaCategoria(QString strFiltro);
    void ImprimirListaFamilia(QString strFiltro);
    void ImprimirListaHistorico(QString strFiltro);
    void ImprimirFichaProd(const clsProductos *Prod);
private:
    void DividirEnPaginasLP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLP(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLP(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLP(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLP(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLP(QPainter *painter);

    void DividirEnPaginasFP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFPProd(QPainter *painter, const QString &entrada);
    int AltoEntradaFPSal(QPainter *painter, const QString &entrada);
    int AltoEntradaFPEnv(QPainter *painter, const QString &entrada);
    int AltoEntradaFPStock(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFP(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFP(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFP(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFP(QPainter *painter);
    void ImprimirCabeceraFPS(QPainter *painter);
    void ImprimirCabeceraFPEnv(QPainter *painter);
    void ImprimirCabeceraFPStock(QPainter *painter);
    void ObtenerSalidas(const QString Prod, QStringList &entradasSal);
    void ObtenerEnvios(const QString Prod, QStringList &entradasEnv);
    void ObtenerStock(const QString Prod, QStringList &entradasSto);
    void ImprimirImagen(QPainter *painter);

    void DividirEnPaginasLCat(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLCat(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLCat(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLCat(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLCat(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLCat(QPainter *painter);

    void DividirEnPaginasLFam(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLFam(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLFam(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLFam(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLFam(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLFam(QPainter *painter);

    void DividirEnPaginasLHist(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLHist(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLHist(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLHist(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLHist(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLHist(QPainter *painter);

    QPrinter *printer;
    int colorLinea;
    int maxAlt;
    int maxAnch;
    int Espacio, EspacioCorto;
    int maxAltDispo;
    int cod_diagnostico;
    int AnchoCod;
    int AnchoDesc;
    int AnchoCertif;
    int AnchoDispo;
    int AnchoCerti;
    int AnchoFam;
    int AnchoCat;
    int AnchoLim;
    int yLinea;
    int AnchoCol1,AnchoCol2;
    int AnchoStock;
    int AnchoFrec;
    int AnchoFechCert;
    int AnchoEmisor;
    int numPaginas;
    QPen penInicial;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QRect rectTitulo,rectCabecera,rectImagen,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial,rectTabla;
    QString titulo;
    QImage *imagen;
};

#endif // CLSIMPRIMIRPRODUCTOS_H
