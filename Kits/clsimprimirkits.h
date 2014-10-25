#ifndef CLSIMPRIMIRKITS_H
#define CLSIMPRIMIRKITS_H

#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include "kit.h"

class clsImprimirKits
{
public:
    clsImprimirKits();
    void ImprimirLista(QString strFiltro);
    void ImprimirFicha(const clsKits *Kit);

private:
    void DividirEnPaginasLK(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLK(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLK(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLK(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLK(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLK(QPainter *painter);

    void DividirEnPaginasFK(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas1);
    int AltoEntradaFKL(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFK(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFK(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFK(QPainter *painter, const QString &cadena);
    void ImprimirLineaFKL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFK(QPainter *painter);
    void ImprimirCabeceraFKL(QPainter *painter);

    QPrinter *printer;
    int colorLinea;
    int maxAlt;
    int maxAnch;
    int Espacio, EspacioCorto;
    int maxAltDispo;
    int AnchoCod;
    int AnchoDesc;
    int AnchoCant;
    int yLinea;
    int numPaginas;
    QPen penInicial;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QRect rectTitulo,rectCabecera,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial;
    QString titulo;
    QString strDescKit;
    QString strCodKit;
};

#endif // CLSIMPRIMIRKITS_H
