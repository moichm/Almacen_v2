#ifndef CLSIMPRIMIRMOVIMIENTOS_H
#define CLSIMPRIMIRMOVIMIENTOS_H

#include <QPainter>
#include "Movimientos/movimientos.h"

class clsImprimirMovimientos
{
public:
    clsImprimirMovimientos();
    void ImprimirListaSalidasEntradas(const QString tipo,QString strFiltro="",bool bPorDevolver=false);
    void ImprimirListaEnviosRecepciones(const QString tipo,QString strFiltro="");
    void ImprimirListaStock(QString strFiltro="");
    void ImprimirListaPedidos(QString strFiltro="");

    void ImprimirFichaSalida(const clsMovimiento_Salida *Sal);
    void ImprimirFichaEntrada(const clsMovimiento_Salida *Sal);
    void ImprimirFichaEnvio(const clsMovimiento_Envio *Env);
    void ImprimirFichaRecepcion(const clsMovimiento_Envio *Envio);
    void ImprimirFichaPedido(const clsMovimiento_Pedido *Ped);
    void ImprimirFichaStock(const clsMovimiento_Stock *Stock);

    ~clsImprimirMovimientos();
private:
    void DividirEnPaginasLSalEent(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLSalEent(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLSalEent(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLSalEent(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLSalEent(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLSalEent(QPainter *painter);

    void DividirEnPaginasLEnvRec(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLEnvRec(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLEnvRec(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLEnvRec(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLEnvRec(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLEnvRec(QPainter *painter);

    void DividirEnPaginasLStock(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLStock(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLStock(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLStock(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLStock(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLStock(QPainter *painter);

    void DividirEnPaginasLPed(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaLPed(QPainter *painter, const QString &entrada);
    void ImprimirPaginasLPed(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaLPed(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaLPed(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraLPed(QPainter *painter);

    void ImprimirCabeceraF(QPainter *painter);
    void ImprimirFirmaF(QPainter *painter);

    void ImprimirLineaFSalEnt(QPainter *painter, const QString &cadena);
    int AltoEntradaFSalEnt(QPainter *painter, const QString &entrada);

    int CalcularPaginasFSal(QPainter *painter, const QStringList &entradas);
    void DividirEnPaginasFSal(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFSalL(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFSal(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFSal(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFSalL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFSalL(QPainter *painter);

    int CalcularPaginasFEnt(QPainter *painter, const QStringList &entradas);
    void DividirEnPaginasFEnt(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    int AltoEntradaFEntL(QPainter *painter, const QString &entrada);
    void ImprimirPaginasFEnt(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFEnt(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFEntL(QPainter *painter, const QString &cadena);
    void ImprimirLineaFEntSalL(QPainter *painter, const QString &cadena);
    void ImprimirLineaFEnt(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFEntL(QPainter *painter);

    void ImprimirLineaFEnvRec(QPainter *painter, const QString &cadena);
    int AltoEntradaFEnvRec(QPainter *painter, const QString &entrada);
    int AltoEntradaFEnvRecL(QPainter *painter, const QString &entrada);
    int CalcularPaginasFEnvRec(QPainter *painter, const QStringList &entradas);
    void DividirEnPaginasFEnvRec(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);

    void ImprimirPaginasFEnv(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFEnv(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFEnvL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFEnvL(QPainter *painter);

    void ImprimirPaginasFRec(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFRec(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFRecL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFRecL(QPainter *painter);

    void ImprimirLineaFPed(QPainter *painter, const QString &cadena);
    int AltoEntradaFPed(QPainter *painter, const QString &entrada);
    int AltoEntradaFPedL(QPainter *painter, const QString &entrada);
    int CalcularPaginasFPed(QPainter *painter, const QStringList &entradas);
    void DividirEnPaginasFPed(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas);
    void ImprimirPaginasFPed(QPainter *painter,const QList<QStringList> &paginas);
    void ImprimirPaginaFPed(QPainter *painter,const QStringList &entradas, int NumeroPagina);
    void ImprimirLineaFPedL(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFPedL(QPainter *painter);

    void ImprimirPaginasFStock(QPainter *painter,const QStringList &entradas);
    void ImprimirPaginaFStock(QPainter *painter,const QStringList &entradas);
    void ImprimirLineaFStock(QPainter *painter, const QString &cadena);
    void ImprimirCabeceraFStock(QPainter *painter);

    QPrinter *printer;
    int TotalPaginas;
    int maxAlt;
    int maxAnch;
    int maxAltDispo;
    int colorLinea;
    int yLinea;
    int Espacio, EspacioCorto;
    int AnchoCol1;
    int AnchoCol2;
    int AnchoCol3;
    int AnchoCol4;
    int AnchoCol5;
    int AnchoCol6;
    int AnchoCol7;
    int AnchoCol8;
    int AnchoCol9;
    int AnchoCol;
    int numPaginas;
    int altoLinea;
    QPen penInicial;
    QString strTipo;
    QString entradaCab;
    QString entrada;
    QBrush brushInicial;
    QFont fuenteInicial;
    QFont fuenteTitulo;
    QFont fuenteNegrita;
    QFont fuenteMedTitulo;
    QRect rectTitulo,rectFicha,rectCabecera,rectImagen,rectFecha,rectPie,rectfuenteNegrita,rectfuenteInicial,rectFirmaRes,rectFirmaRRHH;
    QString titulo;
};

#endif // CLSIMPRIMIRMOVIMIENTOS_H
