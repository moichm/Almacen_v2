#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include "clsimprimirmovimientos.h"
#include "Operarios/operario.h"
#include "Centros/centro.h"
#include "Productos/producto.h"

clsImprimirMovimientos::clsImprimirMovimientos(){}

clsImprimirMovimientos::~clsImprimirMovimientos(){}

void clsImprimirMovimientos::ImprimirListaSalidasEntradas(QString tipo,QString strFiltro,bool bPorDevolver)
{
    strTipo = tipo;
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);

    if (printDialog.exec())
    {
        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        clsMovimiento_Salida Salida;
        QVector<QStringList> listaSalidas = Salida.ListaSalida(strFiltro);

        for (int i=0;i<listaSalidas.size();i++)
        {
            QStringList strSalidas = listaSalidas.at(i);

            if(bPorDevolver)
            {
                bool bMuestra = false;
                int intCantidad = 0;

                Salida.Cargar(strSalidas.at(0).toInt());
                int j = 0;
                while((j < Salida.DetallesSalida().size()) && !bMuestra)
                {
                    clsMovimiento_DetSal DetSal(Salida.DetallesSalida().at(j));
                    DetSal.Cargar(Salida.DetallesSalida().at(j).Salida(),Salida.DetallesSalida().at(j).CodProd(),Salida.DetallesSalida().at(j).Fecha_Salida());

                    intCantidad = DetSal.Cantidad();

                    int intDevueltas = 0,intPerdidas = 0;
                    for(int k = 0;k < DetSal.DetallesEntrada().size();k++)
                    {
                        intDevueltas += DetSal.DetallesEntrada().at(k).Devueltas();
                        intPerdidas += DetSal.DetallesEntrada().at(k).Perdidas();
                    }
                    bMuestra = (intCantidad > intDevueltas + intPerdidas) ? true:false;

                    j++;
                }

                if (bMuestra)
                {
                    entrada = strSalidas.at(0) + "-|-" + strSalidas.at(1) + "-|-" + strSalidas.at(2) + "-|-" + strSalidas.at(3) + "-|-" + strSalidas.at(4);
                    listaEntradas << entrada;
                }
            }
            else
            {
                entrada = strSalidas.at(0) + "-|-" + strSalidas.at(1) + "-|-" + strSalidas.at(2) + "-|-" + strSalidas.at(3) + "-|-" + strSalidas.at(4);
                listaEntradas << entrada;
            }
        }

        titulo = QObject::tr("Lista de ") + strTipo + QObject::tr("s de Productos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCol1 = QFontMetrics(painter.font()).width(QObject::tr("Nº ")) + QFontMetrics(painter.font()).width(strTipo);
        AnchoCol2 = (maxAnch - AnchoCol1 - 4*Espacio)/4;
        AnchoCol3 = (maxAnch - AnchoCol1 - 4*Espacio)/4;
        AnchoCol4 = (maxAnch - AnchoCol1 - 4*Espacio)/4;
        AnchoCol5 = (maxAnch - AnchoCol1 - 4*Espacio)/4;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLSalEent(&painter,&pages,listaEntradas);
        ImprimirPaginasLSalEent(&painter,pages);
    }
}

void clsImprimirMovimientos::DividirEnPaginasLSalEent(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for( int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLSalEent(painter, entradas.at(i));
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = 0;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

void clsImprimirMovimientos::ImprimirPaginasLSalEent(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraLSalEent(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLSalEent(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirMovimientos::ImprimirPaginaLSalEent(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLSalEent(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirCabeceraLSalEent(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignCenter,titulo);
    yLinea += rectTitulo.height();
    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->setFont(fuenteInicial);

    int AnchoObjeto = 0;

    QDateTime fecha;

    painter->drawText(0,yLinea,maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Nº ") + strTipo);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Centro"));

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Proyecto"));

    AnchoObjeto += AnchoCol3 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Tarea"));

    AnchoObjeto += AnchoCol4 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Responsable"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirMovimientos::AltoEntradaLSalEent(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centroRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[1]);
    QRect proyRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol3,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    QRect tareaRect = painter->boundingRect(0,
                                            0,
                                            AnchoCol4,
                                            maxAltDispo,
                                            Qt::TextWordWrap,
                                            campos[3]);
    QRect resRect = painter->boundingRect(0,
                                          0,
                                          AnchoCol5,
                                          maxAltDispo,
                                          Qt::TextWordWrap,
                                          campos[4]);
    return qMax(centroRect.height(),qMax(proyRect.height(),qMax(tareaRect.height(),qMax(resRect.height(),rectfuenteInicial.height()))));
}

void clsImprimirMovimientos::ImprimirLineaLSalEent(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setFont(fuenteInicial);

    int AltoEntrada = AltoEntradaLSalEent(painter, cadena);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoEntrada);
    }
    painter->setPen(penInicial);

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol3 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol4 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    colorLinea++;

    yLinea += AltoEntrada;
}


void clsImprimirMovimientos::ImprimirListaEnviosRecepciones(QString tipo,QString strFiltro)
{
    strTipo = tipo;
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);

    if (printDialog.exec())
    {
        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        clsMovimiento_Envio Envio;
        QVector<QStringList> listaEnvios = Envio.ListaEnvio(strFiltro);

        for (int i=0;i<listaEnvios.size();i++)
        {
            QStringList strEnvios = listaEnvios.at(i);

            entrada = strEnvios.at(0) + "-|-" + strEnvios.at(1) + "-|-" + strEnvios.at(2);

            listaEntradas << entrada;
        }

        if (strTipo == QObject::tr("Envío"))
            titulo = QObject::tr("Lista de Envíos de Productos");
        else
            titulo = QObject::tr("Lista de Recepciones de Productos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCol1 = QFontMetrics(painter.font()).width(QObject::tr("Nº ")) + QFontMetrics(painter.font()).width(strTipo);
        AnchoCol2 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha ")) + QFontMetrics(painter.font()).width(strTipo),QFontMetrics(painter.font()).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol3 = (maxAnch - AnchoCol1 - AnchoCol2 - 2*Espacio);

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLEnvRec(&painter,&pages,listaEntradas);
        ImprimirPaginasLEnvRec(&painter,pages);
    }
}

void clsImprimirMovimientos::DividirEnPaginasLEnvRec(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for( int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLEnvRec(painter, entradas.at(i));
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = 0;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

void clsImprimirMovimientos::ImprimirPaginasLEnvRec(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraLEnvRec(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLEnvRec(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirMovimientos::ImprimirPaginaLEnvRec(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLEnvRec(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirCabeceraLEnvRec(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignCenter,titulo);
    yLinea += rectTitulo.height();
    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->setFont(fuenteInicial);

    int AnchoObjeto = 0;

    QDateTime fecha;

    painter->drawText(0,yLinea,maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Nº ") + strTipo);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Fecha ") + strTipo);

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Centro"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirMovimientos::AltoEntradaLEnvRec(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centroRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol3,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[1]);

    return qMax(centroRect.height(),rectfuenteInicial.height());
}

void clsImprimirMovimientos::ImprimirLineaLEnvRec(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setFont(fuenteInicial);

    int AltoEntrada = AltoEntradaLEnvRec(painter, cadena);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoEntrada);
    }
    painter->setPen(penInicial);

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    colorLinea++;

    yLinea += AltoEntrada;
}


void clsImprimirMovimientos::ImprimirListaStock(QString strFiltro)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);

    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        clsMovimiento_Stock Stock;
        QVector<QStringList> listaStock = Stock.ListaStock(strFiltro);

        for (int i=0;i<listaStock.size();i++)
        {
            QStringList strStock = listaStock.at(i);

            if (AnchoCol1 < QFontMetrics(fuenteInicial).width(strStock.at(0)))
                AnchoCol1 = QFontMetrics(fuenteInicial).width(strStock.at(0));

            if (AnchoCol6 < QFontMetrics(fuenteInicial).width(strStock.at(5)))
                AnchoCol6 = QFontMetrics(fuenteInicial).width(strStock.at(5));

            entrada = strStock.at(0) + "-|-" + strStock.at(1) + "-|-" + strStock.at(2) + "-|-" + strStock.at(3) + "-|-" + strStock.at(4) + "-|-" + strStock.at(5);

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Lista de Stock de Productos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCol1 = qMax(AnchoCol1,QFontMetrics(painter.font()).width(QObject::tr("Producto")));
        for(int i=0;i<qlOrigen.size();i++)
            AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(qlOrigen.at(i)),AnchoCol3);
        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Origen")),AnchoCol3);
        AnchoCol4 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha Stock")),QFontMetrics(painter.font()).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol6 = qMax(AnchoCol6,QFontMetrics(painter.font()).width(QObject::tr("Cantidad")));
        AnchoCol2 = (maxAnch - AnchoCol1 - AnchoCol3 - AnchoCol4 - AnchoCol6 - 5*Espacio)/2;
        AnchoCol5 = (maxAnch - AnchoCol1 - AnchoCol3 - AnchoCol4 - AnchoCol6 - 5*Espacio)/2;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLStock(&painter,&pages,listaEntradas);
        ImprimirPaginasLStock(&painter,pages);
    }
}

void clsImprimirMovimientos::DividirEnPaginasLStock(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for( int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLStock(painter, entradas.at(i));
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = 0;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

void clsImprimirMovimientos::ImprimirPaginasLStock(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraLStock(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLStock(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirMovimientos::ImprimirPaginaLStock(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLStock(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirCabeceraLStock(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignCenter,titulo);
    yLinea += rectTitulo.height();
    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->setFont(fuenteInicial);

    int AnchoObjeto = 0;

    QDateTime fecha;

    painter->drawText(0,yLinea,maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Producto"));

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Descripción"));

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Origen"));

    AnchoObjeto += AnchoCol3 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Fecha Stock"));

    AnchoObjeto += AnchoCol4 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Comentario"));

    AnchoObjeto += AnchoCol5 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Cantidad"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirMovimientos::AltoEntradaLStock(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[1]);
    QRect comentRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol5,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[4]);

    return qMax(descRect.height(),qMax(comentRect.height(),rectfuenteInicial.height()));
}

void clsImprimirMovimientos::ImprimirLineaLStock(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setFont(fuenteInicial);

    int AltoEntrada = AltoEntradaLStock(painter, cadena);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoEntrada);
    }
    painter->setPen(penInicial);

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol3 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto += AnchoCol4 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[4]);

    AnchoObjeto += AnchoCol5 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    colorLinea++;

    yLinea += AltoEntrada;
}


void clsImprimirMovimientos::ImprimirListaPedidos(QString strFiltro)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);

    if (printDialog.exec())
    {
        AnchoCol1=0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        clsMovimiento_Pedido Pedido;
        QVector<QStringList> listaPedido = Pedido.ListaPedido(strFiltro);

        for (int i=0;i<listaPedido.size();i++)
        {
            QStringList strPedido = listaPedido.at(i);

            if (AnchoCol1 < QFontMetrics(fuenteInicial).width(strPedido.at(0)))
                AnchoCol1 = QFontMetrics(fuenteInicial).width(strPedido.at(0));

            entrada = strPedido.at(0) + "-|-" + strPedido.at(1);

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Lista de Pedidos de Productos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCol1 = qMax(AnchoCol1,QFontMetrics(painter.font()).width(QObject::tr("Nº Pedido")));
        AnchoCol2 = maxAnch - AnchoCol1 - Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLPed(&painter,&pages,listaEntradas);
        ImprimirPaginasLPed(&painter,pages);
    }
}

void clsImprimirMovimientos::DividirEnPaginasLPed(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for( int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLPed(painter, entradas.at(i));
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = 0;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

void clsImprimirMovimientos::ImprimirPaginasLPed(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraLPed(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLPed(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirMovimientos::ImprimirPaginaLPed(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLPed(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirCabeceraLPed(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignCenter,titulo);
    yLinea += rectTitulo.height();
    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->setFont(fuenteInicial);

    int AnchoObjeto = 0;

    QDateTime fecha;

    painter->drawText(0,yLinea,maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Nº Pedido"));

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Centro"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirMovimientos::AltoEntradaLPed(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centroRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[1]);

    return qMax(centroRect.height(),rectfuenteInicial.height());
}

void clsImprimirMovimientos::ImprimirLineaLPed(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setFont(fuenteInicial);

    int AltoEntrada = AltoEntradaLPed(painter, cadena);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoEntrada);
    }
    painter->setPen(penInicial);

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol1,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCol1 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol2,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    colorLinea++;

    yLinea += AltoEntrada;
}


void clsImprimirMovimientos::ImprimirFirmaF(QPainter *painter)
{
    painter->setBrush(brushInicial);

    painter->drawText(rectFirmaRes.x(),
                      rectFirmaRes.y()-rectFirmaRes.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Responsable")),
                      rectFirmaRes.height(),
                      Qt::AlignLeft | Qt::AlignBottom,
                      QObject::tr("Responsable"));
    painter->drawRect(rectFirmaRes);

    painter->drawText(rectFirmaRRHH.x(),
                      rectFirmaRRHH.y()-rectFirmaRRHH.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Almacén")),
                      rectFirmaRRHH.height(),
                      Qt::AlignLeft | Qt::AlignBottom,
                      QObject::tr("Almacén"));
    painter->drawRect(rectFirmaRRHH);
}

void clsImprimirMovimientos::ImprimirCabeceraF(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setFont(fuenteInicial);

    QDateTime fecha;

    painter->drawText(0,rectTitulo.height(),maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height() + Espacio;
}


void clsImprimirMovimientos::ImprimirLineaFSalEnt(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Salida"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(centRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(centRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = 0;
    yLinea += qMax(centRect.height(),rectfuenteNegrita.height());

    QRect opRect = painter->boundingRect(0,
                                         0,
                                         AnchoCol2-EspacioCorto,
                                         maxAltDispo,
                                         Qt::TextWordWrap,
                                         campos[3]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(opRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(opRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(opRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Operario"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(opRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto = 0;
    yLinea += qMax(opRect.height(),rectfuenteNegrita.height());

    QRect proyRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2,
                                           maxAltDispo,
                                           Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(proyRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(proyRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(proyRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Proyecto"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(proyRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto = 0;
    yLinea += qMax(proyRect.height(),rectfuenteNegrita.height());

    QRect tareaRect = painter->boundingRect(0,
                                            0,
                                            AnchoCol2,
                                            maxAltDispo,
                                            Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(tareaRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(tareaRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(tareaRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tarea"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(tareaRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    AnchoObjeto = 0;
    yLinea += qMax(tareaRect.height(),rectfuenteNegrita.height());

    QRect comentRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(comentRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(comentRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentarios"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[6]);

    yLinea += qMax(comentRect.height(),rectfuenteNegrita.height());

}

int clsImprimirMovimientos::AltoEntradaFSalEnt(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    QRect opRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[3]);
    QRect proyRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[4]);
    QRect tareaRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[5]);
    QRect comentRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[6]);
    return qMax(comentRect.height(),rectfuenteNegrita.height()) +
            qMax(proyRect.height(),rectfuenteNegrita.height()) +
            qMax(tareaRect.height(),rectfuenteNegrita.height()) +
            qMax(centRect.height(),rectfuenteNegrita.height()) +
            qMax(opRect.height(),rectfuenteNegrita.height()) + rectfuenteNegrita.height();
}


void clsImprimirMovimientos::ImprimirFichaSalida(const clsMovimiento_Salida *Sal)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsOperarios Op(Sal->Operario());
        Op.Cargar(Sal->Operario());

        clsCentros Centro;
        Centro.Cargar(Sal->Centro());

        entrada = QObject::tr("Salida")+"-|-" + QString::number(Sal->NoSalida());
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Sal->Operario() + " - " + Op.Nombre() + " " + Op.Apellidos();
        entrada += "-|-" + Sal->Proyecto();
        entrada += "-|-" + Sal->Tarea();
        entrada += "-|-" + Sal->Comentarios();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i = 0;i<Sal->DetallesSalida().size();i++)
        {
            clsMovimiento_DetSal DetSal(Sal->DetallesSalida().at(i));
            clsProductos Prod;
            Prod.Cargar(DetSal.CodProd());

            if (AnchoCol3 < (QFontMetrics(fuenteInicial).width(DetSal.CodProd())))
                AnchoCol3 = QFontMetrics(fuenteInicial).width(DetSal.CodProd());

            if (AnchoCol5 < (QFontMetrics(fuenteInicial).width(QString::number(DetSal.Cantidad()))))
                AnchoCol5 = QFontMetrics(fuenteInicial).width(QString::number(DetSal.Cantidad()));

            entrada = QObject::tr("Linea")+"-|-" + DetSal.CodProd() + "-|-" + Prod.Descripcion() + "-|-" + QString::number(DetSal.Cantidad()) + "-|-" + DetSal.Fecha_Salida().toString(QObject::tr("dd/MM/yyyy"));

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Salida de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Nº Salida")),
                         qMax(QFontMetrics(painter.font()).width(QObject::tr("Centro")),
                              qMax(QFontMetrics(painter.font()).width(QObject::tr("Operario")),
                                   qMax(QFontMetrics(painter.font()).width(QObject::tr("Proyecto")),
                                        qMax(QFontMetrics(painter.font()).width(QObject::tr("Tarea")),
                                             QFontMetrics(painter.font()).width(QObject::tr("Comentarios")))))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1;

        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Código")),AnchoCol3);
        AnchoCol5 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Canticad")),AnchoCol5);
        AnchoCol6 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Salida")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol4 = maxAnch - AnchoCol3 - AnchoCol5 - AnchoCol6 - 3*Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFSal(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFSal(&painter,&pages,listaEntradas);
        ImprimirPaginasFSal(&painter,pages);
    }
}

int clsImprimirMovimientos::CalcularPaginasFSal(QPainter *painter, const QStringList &entradas)
{
    int AltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectCabecera.height() + rectPie.height() + Espacio);

    QList<QStringList> paginas;
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Salida")))
            alto = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFSalL(painter, entradas.at(i));
        if (y + alto > AltDispo && !PaginaActual.empty())
        {
            paginas.append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas.append(PaginaActual);

    return paginas.size();
}

void clsImprimirMovimientos::DividirEnPaginasFSal(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Salida")))
            alto = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFSalL(painter, entradas.at(i));

        if (TotalPaginas - 1 == paginas->size())
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

int clsImprimirMovimientos::AltoEntradaFSalL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol4,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[1]);
    return qMax(descRect.height(),rectfuenteNegrita.height());
}

void clsImprimirMovimientos::ImprimirPaginasFSal(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraF(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFSal(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaF(painter);
        }
}

void clsImprimirMovimientos::ImprimirCabeceraFSalL(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCol3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol4+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol5+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Salida"));

    yLinea += rectCabecera.height();
}

void clsImprimirMovimientos::ImprimirPaginaFSal(QPainter *painter,const QStringList &entradas,int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if(i == 0)
        {
            colorLinea = 0;

            ImprimirLineaFSalEnt(painter, entradaCab);
            if(ent == QObject::tr("Linea"))
                ImprimirLineaFSalL(painter, entradas.at(i));
        }
        else
            ImprimirLineaFSalL(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirLineaFSalL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFSalL(painter);

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFSalL(painter,cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoObjeto);
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol5+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirMovimientos::ImprimirFichaEntrada(const clsMovimiento_Salida *Sal)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;
        AnchoCol7 = 0;
        AnchoCol8 = 0;
        AnchoCol9 = 0;


        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada,entradaEnt;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsOperarios Op(Sal->Operario());
        Op.Cargar(Sal->Operario());

        clsCentros Centro;
        Centro.Cargar(Sal->Centro());

        entrada = QObject::tr("Salida")+"-|-" + QString::number(Sal->NoSalida());
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Sal->Operario() + " - " + Op.Nombre() + " " + Op.Apellidos();
        entrada += "-|-" + Sal->Proyecto();
        entrada += "-|-" + Sal->Tarea();
        entrada += "-|-" + Sal->Comentarios();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i = 0;i<Sal->DetallesSalida().size();i++)
        {
            clsMovimiento_DetSal DetSal(Sal->DetallesSalida().at(i));
            DetSal.Cargar(Sal->DetallesSalida().at(i).Salida(),Sal->DetallesSalida().at(i).CodProd(),Sal->DetallesSalida().at(i).Fecha_Salida());

            clsProductos Prod;
            Prod.Cargar(DetSal.CodProd());

            if (AnchoCol3 < (QFontMetrics(fuenteInicial).width(DetSal.CodProd())))
                AnchoCol3 = QFontMetrics(fuenteInicial).width(DetSal.CodProd());

            if (AnchoCol5 < (QFontMetrics(fuenteInicial).width(QString::number(DetSal.Cantidad()))))
                AnchoCol5 = QFontMetrics(fuenteInicial).width(QString::number(DetSal.Cantidad()));

            entrada = QObject::tr("LineaSal")+"-|-" + QString::number(DetSal.DetallesEntrada().size()) + "-|-" + DetSal.CodProd() +
                    "-|-" + Prod.Descripcion() +
                    "-|-" + QString::number(DetSal.Cantidad()) +
                    "-|-" + DetSal.Fecha_Salida().toString(QObject::tr("dd/MM/yyyy"));

            listaEntradas << entrada;

            if(DetSal.DetallesEntrada().size() > 0)
            {
                for(int j=0;j<DetSal.DetallesEntrada().size();j++)
                {
                    if (AnchoCol7 < (QFontMetrics(fuenteInicial).width(DetSal.DetallesEntrada().at(j).Devueltas())))
                        AnchoCol7 = QFontMetrics(fuenteInicial).width(DetSal.DetallesEntrada().at(j).Devueltas());

                    if (AnchoCol8 < (QFontMetrics(fuenteInicial).width(DetSal.DetallesEntrada().at(j).Perdidas())))
                        AnchoCol8 = QFontMetrics(fuenteInicial).width(DetSal.DetallesEntrada().at(j).Perdidas());

                    entradaEnt = QObject::tr("LineaEnt")+"-|-" + QString::number(DetSal.DetallesEntrada().at(j).Devueltas()) +
                            "-|-" + QString::number(DetSal.DetallesEntrada().at(j).Perdidas()) +
                            "-|-" + DetSal.DetallesEntrada().at(j).Fecha_Entrada().toString(QObject::tr("dd/MM/yyyy"));

                    listaEntradas << entradaEnt;
                }
            }
        }

        titulo = QObject::tr("Entrada de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Nº Salida")),
                         qMax(QFontMetrics(painter.font()).width(QObject::tr("Centro")),
                              qMax(QFontMetrics(painter.font()).width(QObject::tr("Operario")),
                                   qMax(QFontMetrics(painter.font()).width(QObject::tr("Proyecto")),
                                        qMax(QFontMetrics(painter.font()).width(QObject::tr("Tarea")),
                                             QFontMetrics(painter.font()).width(QObject::tr("Comentarios")))))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1;

        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Código")),AnchoCol3);
        AnchoCol5 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Canticad")),AnchoCol5);
        AnchoCol6 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Salida")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol7 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Devueltas")),AnchoCol7);
        AnchoCol8 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Perdidas")),AnchoCol8);
        AnchoCol9 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Entrada")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol4 = maxAnch - AnchoCol3 - AnchoCol5 - AnchoCol6 - AnchoCol7 - AnchoCol8 - AnchoCol9 - 6*Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFEnt(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFEnt(&painter,&pages,listaEntradas);
        ImprimirPaginasFEnt(&painter,pages);
    }
}

int clsImprimirMovimientos::CalcularPaginasFEnt(QPainter *painter, const QStringList &entradas)
{
    int AltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectCabecera.height() + rectPie.height() + Espacio);

    QList<QStringList> paginas;
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if (i == 0)
            alto = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        else
        {
            if (ent == QObject::tr("LineaSal"))
                alto = AltoEntradaFEntL(painter, entradas.at(i));
            else if (ent == QObject::tr("LineaEnt"))
                alto = 0;
        }
        if (y + alto > AltDispo && !PaginaActual.empty())
        {
            paginas.append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas.append(PaginaActual);

    return paginas.size();
}

void clsImprimirMovimientos::DividirEnPaginasFEnt(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if (i == 0)
            alto = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        else
        {
            if (ent == QObject::tr("LineaSal"))
                alto = AltoEntradaFEntL(painter, entradas.at(i));
            else if (ent == QObject::tr("LineaEnt"))
                alto = 0;
        }
        if (TotalPaginas - 1 == paginas->size())
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFSalEnt(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

void clsImprimirMovimientos::ImprimirPaginasFEnt(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraF(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFEnt(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaF(painter);
        }
}

int clsImprimirMovimientos::AltoEntradaFEntL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QString strTamLineas = campos[1];
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol4,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[3]);
    return qMax(descRect.height(),strTamLineas.toInt()*rectfuenteNegrita.height());
}

void clsImprimirMovimientos::ImprimirCabeceraFEntL(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCol3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol4+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Salida"));
    AnchoObjeto += AnchoCol6+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol5+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol7,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Devueltas"));
    AnchoObjeto += AnchoCol7+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol8,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Perdidas"));
    AnchoObjeto += AnchoCol8+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol9,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Entrada"));

    yLinea += rectCabecera.height();
}

void clsImprimirMovimientos::ImprimirPaginaFEnt(QPainter *painter,const QStringList &entradas,int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if(i == 0)
        {
            colorLinea = 0;

            ImprimirLineaFEnt(painter, entradaCab);
            if(ent == QObject::tr("LineaSal"))
                ImprimirLineaFEntSalL(painter, entradas.at(i));
            else if(ent == QObject::tr("LineaEnt"))
                ImprimirLineaFEntL(painter, entradas.at(i));

        }
        else
        {
            if(ent == QObject::tr("LineaSal"))
                ImprimirLineaFEntSalL(painter, entradas.at(i));
            else if(ent == QObject::tr("LineaEnt"))
                ImprimirLineaFEntL(painter, entradas.at(i));
        }
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirLineaFEnt(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Salida"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(centRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(centRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = 0;
    yLinea += qMax(centRect.height(),rectfuenteNegrita.height());

    QRect opRect = painter->boundingRect(0,
                                         0,
                                         AnchoCol2-EspacioCorto,
                                         maxAltDispo,
                                         Qt::TextWordWrap,
                                         campos[3]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(opRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(opRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(opRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Operario"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(opRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto = 0;
    yLinea += qMax(opRect.height(),rectfuenteNegrita.height());

    QRect proyRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2,
                                           maxAltDispo,
                                           Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(proyRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(proyRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(proyRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Proyecto"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(proyRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto = 0;
    yLinea += qMax(proyRect.height(),rectfuenteNegrita.height());

    QRect tareaRect = painter->boundingRect(0,
                                            0,
                                            AnchoCol2,
                                            maxAltDispo,
                                            Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(tareaRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(tareaRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(tareaRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tarea"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(tareaRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    AnchoObjeto = 0;
    yLinea += qMax(tareaRect.height(),rectfuenteNegrita.height());

    QRect comentRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(comentRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(comentRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentarios"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[6]);

    yLinea += qMax(comentRect.height(),rectfuenteNegrita.height());

}

void clsImprimirMovimientos::ImprimirLineaFEntSalL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFEntL(painter);

    altoLinea = yLinea;

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFEntL(painter,cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoObjeto);
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto += AnchoCol3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    AnchoObjeto += AnchoCol6+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    colorLinea++;

    yLinea += AltoObjeto;
}

void clsImprimirMovimientos::ImprimirLineaFEntL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = AnchoCol3 + AnchoCol4 + AnchoCol5 + AnchoCol6 + 4*Espacio;

    painter->setFont(fuenteInicial);

    painter->drawText(AnchoObjeto,
                      altoLinea,
                      AnchoCol7,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol7+Espacio;

    painter->drawText(AnchoObjeto,
                      altoLinea,
                      AnchoCol8,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol8+Espacio;

    painter->drawText(AnchoObjeto,
                      altoLinea,
                      AnchoCol9,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    altoLinea += rectfuenteNegrita.height();
}


void clsImprimirMovimientos::ImprimirLineaFEnvRec(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Envío"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(centRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(centRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = 0;
    yLinea += qMax(centRect.height(),rectfuenteNegrita.height());

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Envío"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect comentRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(comentRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(comentRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentarios"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    yLinea += qMax(comentRect.height(),rectfuenteNegrita.height());

}

int clsImprimirMovimientos::AltoEntradaFEnvRec(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    QRect comentRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[4]);
    return qMax(comentRect.height(),rectfuenteNegrita.height()) +
            qMax(centRect.height(),rectfuenteNegrita.height()) + + 2*rectfuenteNegrita.height();
}

int clsImprimirMovimientos::AltoEntradaFEnvRecL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol4,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[1]);
    return qMax(descRect.height(),rectfuenteNegrita.height());
}

int clsImprimirMovimientos::CalcularPaginasFEnvRec(QPainter *painter, const QStringList &entradas)
{
    int AltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectCabecera.height() + rectPie.height() + Espacio);

    QList<QStringList> paginas;
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Envío")))
            alto = AltoEntradaFEnvRec(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFEnvRecL(painter, entradas.at(i));
        if (y + alto > AltDispo && !PaginaActual.empty())
        {
            paginas.append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFEnvRec(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas.append(PaginaActual);

    return paginas.size();
}

void clsImprimirMovimientos::DividirEnPaginasFEnvRec(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Envío")))
            alto = AltoEntradaFEnvRec(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFEnvRecL(painter, entradas.at(i));

        if (TotalPaginas - 1 == paginas->size())
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFEnvRec(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}


void clsImprimirMovimientos::ImprimirFichaEnvio(const clsMovimiento_Envio *Env)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsCentros Centro;
        Centro.Cargar(Env->CentroDestino());

        entrada = QObject::tr("Envío")+"-|-" + QString::number(Env->NoEnvio());
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Env->FechaCreacion().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Env->Comentarios();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i = 0;i<Env->Detalles().size();i++)
        {
            clsMovimiento_DetEnv DetEnv(Env->Detalles().at(i));
            clsProductos Prod;
            Prod.Cargar(DetEnv.CodProd());

            if (AnchoCol3 < (QFontMetrics(fuenteInicial).width(DetEnv.CodProd())))
                AnchoCol3 = QFontMetrics(fuenteInicial).width(DetEnv.CodProd());

            if (AnchoCol5 < (QFontMetrics(fuenteInicial).width(QString::number(DetEnv.Cantidad()))))
                AnchoCol5 = QFontMetrics(fuenteInicial).width(QString::number(DetEnv.Cantidad()));

            entrada = QObject::tr("Linea")+"-|-" + DetEnv.CodProd() + "-|-" + Prod.Descripcion() + "-|-" + QString::number(DetEnv.Cantidad()) + "-|-" + DetEnv.FechaEnvio().toString(QObject::tr("dd/MM/yyyy"));

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Envío de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Nº Envío")),
                         qMax(QFontMetrics(painter.font()).width(QObject::tr("Centro")),
                              qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha Envío")),
                                   QFontMetrics(painter.font()).width(QObject::tr("Comentarios")))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1;

        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Código")),AnchoCol3);
        AnchoCol5 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Canticad")),AnchoCol5);
        AnchoCol6 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Envío")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol4 = maxAnch - AnchoCol3 - AnchoCol5 - AnchoCol6 - 3*Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFEnvRec(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFEnvRec(&painter,&pages,listaEntradas);
        ImprimirPaginasFEnv(&painter,pages);
    }
}

void clsImprimirMovimientos::ImprimirPaginasFEnv(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraF(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFEnv(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaF(painter);
        }
}

void clsImprimirMovimientos::ImprimirCabeceraFEnvL(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCol3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol4+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol5+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Envío"));

    yLinea += rectCabecera.height();
}

void clsImprimirMovimientos::ImprimirPaginaFEnv(QPainter *painter,const QStringList &entradas,int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if(i == 0)
        {
            colorLinea = 0;

            ImprimirLineaFEnvRec(painter, entradaCab);
            if(ent == QObject::tr("Linea"))
                ImprimirLineaFEnvL(painter, entradas.at(i));
        }
        else
            ImprimirLineaFEnvL(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirLineaFEnvL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFEnvL(painter);

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFEnvRecL(painter,cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoObjeto);
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol5+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirMovimientos::ImprimirFichaRecepcion(const clsMovimiento_Envio *Env)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;
        AnchoCol7 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsCentros Centro;
        Centro.Cargar(Env->CentroDestino());

        entrada = QObject::tr("Envio")+"-|-" + QString::number(Env->NoEnvio());
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Env->FechaCreacion().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Env->Comentarios();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i = 0;i<Env->Detalles().size();i++)
        {
            entrada=QObject::tr("Linea");

            if (AnchoCol3 < (QFontMetrics(fuenteInicial).width(Env->Detalles().at(i).CodProd())))
                AnchoCol3 = QFontMetrics(fuenteInicial).width(Env->Detalles().at(i).CodProd());
            if (AnchoCol5 < (QFontMetrics(fuenteInicial).width(Env->Detalles().at(i).Cantidad())))
                AnchoCol5 = QFontMetrics(fuenteInicial).width(Env->Detalles().at(i).Cantidad());

            clsProductos Prod;
            Prod.Cargar(Env->Detalles().at(i).CodProd());

            entrada += "-|-" + Env->Detalles().at(i).CodProd();
            entrada += "-|-" + Prod.Descripcion();
            entrada += "-|-" + QString::number(Env->Detalles().at(i).Cantidad());
            entrada += "-|-" + Env->Detalles().at(i).FechaEnvio().toString(QObject::tr("dd/MM/yyyy"));
            if(Env->Detalles().at(i).Recibida())
                entrada += "-|-" + QObject::tr("Sí");
            else
                entrada += "-|-"+QObject::tr("No");

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Recepción de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Nº Envío")),
                         qMax(QFontMetrics(painter.font()).width(QObject::tr("Centro")),
                              qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha Envío")),
                                   QFontMetrics(painter.font()).width(QObject::tr("Comentarios")))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1;

        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Código")),AnchoCol3);
        AnchoCol5 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Canticad")),AnchoCol5);
        AnchoCol6 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Envío")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol7 = QFontMetrics(fuenteNegrita).width(QObject::tr("Recibida"));
        AnchoCol4 = maxAnch - AnchoCol3 - AnchoCol5 - AnchoCol6 - AnchoCol7 - 4*Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFEnvRec(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFEnvRec(&painter,&pages,listaEntradas);
        ImprimirPaginasFRec(&painter,pages);
    }
}

void clsImprimirMovimientos::ImprimirPaginasFRec(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraF(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFRec(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaF(painter);
        }
}

void clsImprimirMovimientos::ImprimirCabeceraFRecL(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCol3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol4+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol5+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Envío"));
    AnchoObjeto += AnchoCol6+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol7,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Recibido"));

    yLinea += rectCabecera.height();
}

void clsImprimirMovimientos::ImprimirPaginaFRec(QPainter *painter,const QStringList &entradas,int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if(i == 0)
        {
            colorLinea = 0;

            ImprimirLineaFEnvRec(painter, entradaCab);
            if(ent == QObject::tr("Linea"))
                ImprimirLineaFRecL(painter, entradas.at(i));
        }
        else
            ImprimirLineaFRecL(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirLineaFRecL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFRecL(painter);

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFEnvRecL(painter,cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoObjeto);
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol5+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto += AnchoCol6+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol7,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirMovimientos::ImprimirFichaPedido(const clsMovimiento_Pedido *Ped)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;
        AnchoCol3 = 0;
        AnchoCol4 = 0;
        AnchoCol5 = 0;
        AnchoCol6 = 0;
        AnchoCol7 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsCentros Centro;
        Centro.Cargar(Ped->Centro());

        entrada = QObject::tr("Pedido")+"-|-" + QString::number(Ped->NoPedido());
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Ped->Comentarios();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i = 0;i<Ped->Detalles().size();i++)
        {
            entrada=QObject::tr("Linea");

            if (AnchoCol3 < (QFontMetrics(fuenteInicial).width(Ped->Detalles().at(i).CodProd())))
                AnchoCol3 = QFontMetrics(fuenteInicial).width(Ped->Detalles().at(i).CodProd());
            if (AnchoCol5 < (QFontMetrics(fuenteInicial).width(Ped->Detalles().at(i).Cantidad())))
                AnchoCol5 = QFontMetrics(fuenteInicial).width(Ped->Detalles().at(i).Cantidad());

            clsProductos Prod;
            Prod.Cargar(Ped->Detalles().at(i).CodProd());

            entrada += "-|-" + Ped->Detalles().at(i).CodProd();
            entrada += "-|-" + Prod.Descripcion();
            entrada += "-|-" + QString::number(Ped->Detalles().at(i).Cantidad());
            entrada += "-|-" + Ped->Detalles().at(i).Fecha().toString(QObject::tr("dd/MM/yyyy"));
            if(Ped->Detalles().at(i).Servido())
                entrada += "-|-" + QObject::tr("Sí");
            else
                entrada += "-|-"+QObject::tr("No");

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Pedido de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Nº Pedido")),
                         qMax(QFontMetrics(painter.font()).width(QObject::tr("Centro")),
                                   QFontMetrics(painter.font()).width(QObject::tr("Comentarios"))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1;

        AnchoCol3 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Código")),AnchoCol3);
        AnchoCol5 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Canticad")),AnchoCol5);
        AnchoCol6 = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Fecha Pedido")),QFontMetrics(fuenteNegrita).width(QObject::tr("dd/MM/yyyy")));
        AnchoCol7 = QFontMetrics(fuenteNegrita).width(QObject::tr("Servida"));
        AnchoCol4 = maxAnch - AnchoCol3 - AnchoCol5 - AnchoCol6 - AnchoCol7 - 4*Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFPed(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFPed(&painter,&pages,listaEntradas);
        ImprimirPaginasFPed(&painter,pages);
    }
}

int clsImprimirMovimientos::CalcularPaginasFPed(QPainter *painter, const QStringList &entradas)
{
    int AltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectCabecera.height() + rectPie.height() + Espacio);

    QList<QStringList> paginas;
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Pedido")))
            alto = AltoEntradaFPed(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFPedL(painter, entradas.at(i));
        if (y + alto > AltDispo && !PaginaActual.empty())
        {
            paginas.append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFPed(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas.append(PaginaActual);

    return paginas.size();
}

void clsImprimirMovimientos::DividirEnPaginasFPed(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Pedido")))
            alto = AltoEntradaFPed(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFPedL(painter, entradas.at(i));

        if (TotalPaginas - 1 == paginas->size())
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFPed(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

int clsImprimirMovimientos::AltoEntradaFPed(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    QRect comentRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[3]);
    return qMax(comentRect.height(),rectfuenteNegrita.height()) +
            qMax(centRect.height(),rectfuenteNegrita.height()) + + 2*rectfuenteNegrita.height();
}

int clsImprimirMovimientos::AltoEntradaFPedL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol4,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[1]);
    return qMax(descRect.height(),rectfuenteNegrita.height());
}

void clsImprimirMovimientos::ImprimirPaginasFPed(QPainter *painter,const QList<QStringList> &paginas)
{
    int primeraPagina = printer->fromPage() - 1;
    if (primeraPagina >= paginas.size())
        return;
    if (primeraPagina == -1)
        primeraPagina = 0;
    int ultPagina = printer->toPage() - 1;
    if (ultPagina == -1 || ultPagina >= paginas.size())
        ultPagina = paginas.size() - 1;
    numPaginas = ultPagina - primeraPagina + 1;
    for (int i = 0; i < printer->numCopies(); ++i)
        for (int j = 0; j < numPaginas; ++j)
        {
            if (i != 0 || j != 0)
            {
                printer->newPage();
                yLinea = 0;
            }
            ImprimirCabeceraF(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFPed(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaF(painter);
        }
}

void clsImprimirMovimientos::ImprimirCabeceraFPedL(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCol3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol4+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol5+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Pedido"));
    AnchoObjeto += AnchoCol6+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol7,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Servida"));

    yLinea += rectCabecera.height();
}

void clsImprimirMovimientos::ImprimirPaginaFPed(QPainter *painter,const QStringList &entradas,int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if(i == 0)
        {
            colorLinea = 0;

            ImprimirLineaFPed(painter, entradaCab);
            if(ent == QObject::tr("Linea"))
                ImprimirLineaFPedL(painter, entradas.at(i));
        }
        else
            ImprimirLineaFPedL(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirMovimientos::ImprimirLineaFPedL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFPedL(painter);

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFPedL(painter,cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          AltoObjeto);
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoCol3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol4,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol5,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoCol5+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol6,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto += AnchoCol6+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol7,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    colorLinea++;

    yLinea += AltoObjeto;
}

void clsImprimirMovimientos::ImprimirLineaFPed(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Pedido"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect centRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2-EspacioCorto,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(centRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(centRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(centRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = 0;
    yLinea += qMax(centRect.height(),rectfuenteNegrita.height());

    QRect comentRect = painter->boundingRect(0,
                                             0,
                                             AnchoCol2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,campos[3]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(comentRect.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(comentRect.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentarios"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2-EspacioCorto,
                      qMax(comentRect.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    yLinea += qMax(comentRect.height(),rectfuenteNegrita.height());
}


void clsImprimirMovimientos::ImprimirFichaStock(const clsMovimiento_Stock *Stock)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoCol1 = 0;
        AnchoCol2 = 0;

        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        penInicial = painter.pen();
        brushInicial = painter.brush();
        fuenteInicial = painter.font();
        fuenteTitulo = painter.font();
        fuenteMedTitulo = painter.font();
        fuenteNegrita = painter.font();
        fuenteTitulo.setPointSize(20);
        fuenteNegrita.setBold(true);
        fuenteTitulo.setBold(true);
        fuenteMedTitulo.setBold(true);
        fuenteMedTitulo.setPointSize(15);
        fuenteNegrita.setPointSize(10);
        fuenteInicial.setPointSize(10);

        QStringList listaEntradas;
        QString entrada;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        clsCentros Centro;
        Centro.Cargar(Stock->Centro());

        clsProductos Prod;
        Prod.Cargar(Stock->CodProd());

        entrada = Stock->CodProd();
        entrada += "-|-" + Prod.Descripcion();
        entrada += "-|-" + Stock->Fecha().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + qlOrigen[Stock->Origen()];
        entrada += "-|-" + Stock->Comentario();
        entrada += "-|-" + QString::number(Stock->Cantidad());

        listaEntradas << entrada;

        titulo = qlOrigen[Stock->Origen()] + QObject::tr(" de Producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFicha = QRect(0,0,maxAnch,(maxAlt-(maxAlt*8.5)/257)/2);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(qMax(QFontMetrics(painter.font()).width(QObject::tr("Código")),QFontMetrics(painter.font()).width(QObject::tr("producto"))),
                         qMax(qMax(QFontMetrics(painter.font()).width(QObject::tr("Descripción")),QFontMetrics(painter.font()).width(QObject::tr("producto"))),
                              qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha")),
                                   qMax(QFontMetrics(painter.font()).width(QObject::tr("Origen")),
                                        qMax(QFontMetrics(painter.font()).width(QObject::tr("Cantidad")),
                                             QFontMetrics(painter.font()).width(QObject::tr("Comentario")))))))+EspacioCorto;
        AnchoCol2 = maxAnch - AnchoCol1 - 3*EspacioCorto;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        yLinea = 0;

        ImprimirPaginasFStock(&painter,listaEntradas);
    }
}

void clsImprimirMovimientos::ImprimirPaginasFStock(QPainter *painter,const QStringList &entradas)
{
    for (int i = 0; i < printer->numCopies(); ++i)
        ImprimirPaginaFStock(painter, entradas);
}

void clsImprimirMovimientos::ImprimirPaginaFStock(QPainter *painter,const QStringList &entradas)
{
    painter->drawRect(rectFicha);
    ImprimirLineaFStock(painter,entradas.at(0));
    painter->drawText(5*Espacio,
                      yLinea+3*rectfuenteNegrita.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Responsable")),
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft,
                      QObject::tr("Responsable"));
    painter->drawText(maxAnch-5*Espacio-QFontMetrics(painter->font()).width(QObject::tr("Almacén")),
                      yLinea+3*rectfuenteNegrita.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Almacén")),
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft,
                      QObject::tr("Almacén"));

    QPen penActual(penInicial.brush(),penInicial.width(),Qt::DotLine);
    painter->setPen(penActual);
    painter->drawLine(0,rectFicha.height()+((maxAlt*8.5)/257)/2,maxAnch,rectFicha.height()+((maxAlt*8.5)/257)/2);
    painter->setPen(penInicial);

    painter->drawRect(0,rectFicha.height()+(((maxAlt*17)/257)/2),maxAnch,rectFicha.height());
    yLinea = rectFicha.height()+(((maxAlt*17)/257)/2);
    ImprimirLineaFStock(painter,entradas.at(0));
    painter->drawText(5*Espacio,
                      yLinea+3*rectfuenteNegrita.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Responsable")),
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft,
                      QObject::tr("Responsable"));
    painter->drawText(maxAnch-5*Espacio-QFontMetrics(painter->font()).width(QObject::tr("Almacén")),
                      yLinea+3*rectfuenteNegrita.height(),
                      QFontMetrics(painter->font()).width(QObject::tr("Almacén")),
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft,
                      QObject::tr("Almacén"));
}

void clsImprimirMovimientos::ImprimirLineaFStock(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = EspacioCorto;

    painter->setFont(fuenteMedTitulo);
    QPen penInicial = painter->pen();

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setFont(fuenteInicial);

    QDateTime fecha;

    painter->drawText(EspacioCorto,yLinea,maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height() + Espacio;

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);

    painter->setFont(fuenteNegrita);
    QRect altoRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol1,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           QObject::tr("Código producto"));

    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      altoRect.height()+EspacioCorto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Código producto"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      altoRect.height()+EspacioCorto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[0]);

    AnchoObjeto = EspacioCorto;
    yLinea += altoRect.height()+EspacioCorto;

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      altoRect.height()+EspacioCorto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Descripción producto"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      altoRect.height()+EspacioCorto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    AnchoObjeto = EspacioCorto;
    yLinea += altoRect.height()+EspacioCorto;

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = EspacioCorto;
    yLinea += rectfuenteNegrita.height();

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Origen"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto = EspacioCorto;
    yLinea += rectfuenteNegrita.height();

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      2*rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentario"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      2*rectfuenteNegrita.height(),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto = EspacioCorto;
    yLinea += 2*rectfuenteNegrita.height();

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[5]);
    yLinea += rectfuenteNegrita.height();
}
