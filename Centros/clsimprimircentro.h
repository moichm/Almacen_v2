#ifndef CLSIMPRIMIRCENTRO_H
#define CLSIMPRIMIRCENTRO_H

#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include "centro.h"

class clsImprimirCentro
{
public:
    clsImprimirCentro();
    void ImprimirLista(QString strFiltro);

private:
    void DividirEnPaginasLC(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLC(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLC(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLC(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLC(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLC(QPainter *painter);

    QPrinter *printer;
    int colorLinea;
    int maxAlt;
    int maxAnch;
    int Espacio, EspacioCorto;
    int maxAltDispo;
    int AnchoCod;
    int AnchoNombre;
    int AnchoCiudad;
    int AnchoDirec;
    int yLinea;
    int numPaginas;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QPen penInicial;
    QRect rectTitulo,rectCabecera,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial;
    QString titulo;
};

#endif // CLSIMPRIMIRCENTRO_H
