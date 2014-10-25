#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include "clsimprimirproductos.h"
#include "Productos/producto.h"
#include "Movimientos/movimientos.h"
#include "Operarios/operario.h"
#include "clsconfiguracion.h"
#include "Centros/centro.h"

extern clsConfiguracion *config;

clsImprimirProductos::clsImprimirProductos(){}

void clsImprimirProductos::ImprimirListaProd(QString strFiltro)
{
    AnchoCod = 0;

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

        QStringList strFiltroInventario = strFiltro.split("-|-");

        clsProductos Producto,Prod;
        QVector<QStringList> listaProductos = Producto.ListaProductos(strFiltro);

        for (int i=0;i<listaProductos.size();i++)
        {
            QStringList strProductos = listaProductos.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strProductos.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strProductos.at(0));

            Prod.Cargar(strProductos.at(0));

            entrada = strProductos.at(0) + "-|-" + strProductos.at(1) + "-|-" + strProductos.at(3) + "-|-" + strProductos.at(4) + "-|-" + strProductos.at(2) + "-|-" + QString::number(Prod.StockCentro()) + "-|-" + QString::number(Prod.StockTotal());

            if(strFiltroInventario.length() > 1)
            {
                if(strFiltroInventario.at(0) == "StockCentro")
                {
                    if(strFiltroInventario.at(1) == ">")
                    {
                        if(QString::number(Prod.StockCentro()) > strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "<")
                    {
                        if(QString::number(Prod.StockCentro()) < strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "=")
                    {
                        if(QString::number(Prod.StockCentro()) == strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "<=")
                    {
                        if(QString::number(Prod.StockCentro()) <= strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == ">=")
                    {
                        if(QString::number(Prod.StockCentro()) >= strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                }
                if(strFiltroInventario.at(0) == "StockTotal")
                {
                    if(strFiltroInventario.at(1) == ">")
                    {
                        if(QString::number(Prod.StockTotal()) > strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "<")
                    {
                        if(QString::number(Prod.StockTotal()) < strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "=")
                    {
                        if(QString::number(Prod.StockTotal()) == strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == "<=")
                    {
                        if(QString::number(Prod.StockTotal()) <= strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                    else if(strFiltroInventario.at(1) == ">=")
                    {
                        if(QString::number(Prod.StockTotal()) >= strFiltroInventario.at(2))
                            listaEntradas << entrada;
                    }
                }
            }
            else
                listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Productos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,2*QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoStock = qMax(QFontMetrics(painter.font()).width(QObject::tr("Stock ")),qMax(QFontMetrics(painter.font()).width(QObject::tr("Total")),QFontMetrics(painter.font()).width(QObject::tr("Centro"))));
        AnchoDispo = QFontMetrics(painter.font()).width(QObject::tr("Disponible"));
        AnchoDesc = (maxAnch - (AnchoCod + AnchoDispo + 2*AnchoStock + 6*Espacio))/2;
        AnchoCat = (maxAnch - (AnchoCod + AnchoDispo + 2*AnchoStock + 6*Espacio))/4;
        AnchoFam = (maxAnch - (AnchoCod + AnchoDispo + 2*AnchoStock + 6*Espacio))/4;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLP(&painter,&pages,listaEntradas);
        ImprimirPaginasLP(&painter,pages);
    }
}

void clsImprimirProductos::DividirEnPaginasLP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLP(painter, entradas.at(i));
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

void clsImprimirProductos::ImprimirPaginasLP(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLP(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLP(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirProductos::ImprimirPaginaLP(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLP(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirProductos::ImprimirCabeceraLP(QPainter *painter)
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
                      AnchoCod,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoDesc + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCat,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Categorí­a"));
    AnchoObjeto += AnchoCat + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFam,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Familia"));
    AnchoObjeto += AnchoFam + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDispo,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Disponible"));
    AnchoObjeto += AnchoDispo + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoStock,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Stock Centro"));
    AnchoObjeto += AnchoStock + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoStock,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Stock Total"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirProductos::AltoEntradaLP(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    painter->setFont(fuenteInicial);

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);
    QRect textRectCat = painter->boundingRect(0,0,AnchoCat, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[2]);
    QRect textRectFam = painter->boundingRect(0,0,AnchoFam, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[3]);

    return qMax(textRectDesc.height(),qMax(textRectFam.height(),qMax(textRectCat.height(),rectfuenteInicial.height())));
}

void clsImprimirProductos::ImprimirLineaLP(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0,AnchoObjeto=0;
    QString booleano = "";

    AltoObjeto = AltoEntradaLP(painter, cadena);

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

    painter->drawText(0,
                      yLinea,
                      AnchoCod,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCod + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    AnchoObjeto += AnchoDesc + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCat,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCat + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFam,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoFam + Espacio;

    if (campos[4] == "true")
        booleano = QObject::tr("Sí­");
    else
        booleano = QObject::tr("No");
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDispo,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      booleano);

    AnchoObjeto += AnchoDispo + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoStock,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[5]);

    AnchoObjeto += AnchoStock + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoStock,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[6]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirProductos::ObtenerSalidas(const QString Prod, QStringList &entradasSal)
{
    QString strConsulta = "SELECT DISTINCT(salidas.idsalida) "
            "FROM salidas INNER JOIN linea_salidas ON salidas.idsalida=linea_salidas.idsalida "
            "WHERE linea_salidas.idProducto = '" + Prod + "' AND salidas.idCentro ='" + QString::number(config->IdCentroActual()) + "' ";

    clsMovimiento_Salida Sal;
    clsOperarios Op;

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            Sal.Cargar(sqlConsulta.value(0).toInt());
            Op.Cargar(Sal.Operario());

            QString entrada="Sal";
            entrada += "-|-" + sqlConsulta.value(0).toString();
            entrada += "-|-" + Sal.Proyecto();
            entrada += "-|-" + Sal.Tarea();
            entrada += "-|-" + Sal.Fecha_Creacion().toString(QObject::tr("dd/MM/yyyy"));
            entrada += "-|-" + Op.Nombre() + " " + Op.Apellidos();

            entradasSal << entrada;
        }
}

void clsImprimirProductos::ObtenerEnvios(const QString Prod, QStringList &entradasEnv)
{
    QString strConsulta = "SELECT DISTINCT(envios.idenvio) "
            "FROM envios INNER JOIN linea_envios ON envios.idenvio=linea_envios.idenvio "
            "WHERE linea_envios.idProducto = '" + Prod + "' AND envios.idCentroOrigen ='" + QString::number(config->IdCentroActual()) + "' ";

    clsMovimiento_Envio Env;
    clsCentros Centro;

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            Env.Cargar(sqlConsulta.value(0).toInt());
            Centro.Cargar(Env.CentroDestino());

            QString entrada="Env";
            entrada += "-|-" + sqlConsulta.value(0).toString();
            entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
            entrada += "-|-" + Env.FechaCreacion().toString(QObject::tr("dd/MM/yyyy"));

            entradasEnv << entrada;
    }
}

void clsImprimirProductos::ObtenerStock(const QString Prod, QStringList &entradasSto)
{
    QString strFiltro = "stock.idProducto = '" + Prod + "' AND stock.idCentro ='" + QString::number(config->IdCentroActual()) + "' ";

    clsMovimiento_Stock Stock;
    QVector<QStringList> listaStock = Stock.ListaStock(strFiltro);

    for (int i=0;i<listaStock.size();i++)
    {
        QStringList strStock = listaStock.at(i);

        QString entrada="";

        entrada = "Stock";
        entrada += "-|-" + strStock.at(3);
        entrada += "-|-" + strStock.at(2);
        entrada += "-|-" + strStock.at(4);
        entrada += "-|-" + strStock.at(5);

        entradasSto << entrada;
    }
}

void clsImprimirProductos::ImprimirFichaProd(const clsProductos *Prod)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        QStringList listaEntradas;
        QString entrada;
        clsFamilias Fam;
        clsCategorias Cat;
        Fam.Cargar(Prod->Familia());
        Cat.Cargar(Prod->Categoria());
        entrada = "Prod";
        entrada += "-|-" + Prod->Codigo();
        entrada += "-|-" + Prod->Descripcion();
        entrada += "-|-" + Cat.Descripcion();
        entrada += "-|-" + Fam.Descripcion();
        if (Prod->Disponible())
            entrada += "-|-" + QObject::tr("Sí");
        else
            entrada += "-|-" + QObject::tr("No");
        entrada += "-|-" + qlEstado[Prod->Estado()];
        entrada += "-|-" + QString::number(Prod->StockTotal());
        entrada += "-|-" + QString::number(Prod->StockCentro());
        if (Fam.Certificable())
            entrada += "-|-" + QObject::tr("Sí");
        else
            entrada += "-|-" + QObject::tr("No");
        entrada += "-|-" + Prod->FechaProxCertificacion().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Prod->FechaUltimaCertificacion().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + QString::number(Fam.FrecuenciaCertificacion());
        if (Fam.Limitada())
            entrada += "-|-" + QObject::tr("Sí");
        else
            entrada += "-|-" + QObject::tr("No");
        entrada += "-|-" + QString::number(Fam.Limite());

        imagen = new QImage(Prod->Imagen());
        imagen->scaledToHeight((maxAlt*65)/257,Qt::SmoothTransformation);

        listaEntradas << entrada;

        QPainter painter(printer);
        painter.setRenderHint(QPainter::Antialiasing);
        QList<QStringList> pages;

        yLinea = 0;

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

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        titulo = QObject::tr("Ficha de producto");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        painter.setFont(fuenteNegrita);        

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Stock Centro")),QFontMetrics(painter.font()).width(QObject::tr("Stock Centro")))+EspacioCorto;

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height() + Espacio);

        rectImagen = QRect(maxAnch - ((maxAnch*50)/175),
                           Espacio + rectTitulo.height() + rectFecha.height(),
                           (maxAnch*50)/175,
                           (maxAlt*65)/257);

        colorLinea = 0;

        ObtenerSalidas(Prod->Codigo(),listaEntradas);
        ObtenerEnvios(Prod->Codigo(),listaEntradas);
        ObtenerStock(Prod->Codigo(),listaEntradas);
        DividirEnPaginasFP(&painter,&pages,listaEntradas);
        ImprimirPaginasFP(&painter,pages);
    }
}

void clsImprimirProductos::DividirEnPaginasFP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString entAnt="",ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        if (i == 0)
            alto = qMax(AltoEntradaFPProd(painter, entradas.at(i))+Espacio,rectImagen.height()+Espacio);
        else
        {
            campos = entradas.at(i).split("-|-");
            ent = campos[0];

            campos = entradas.at(i-1).split("-|-");
            entAnt = campos[0];

            if ((i > 0) && (entAnt == "Prod") && (ent == "Sal"))
                alto = AltoEntradaFPSal(painter, entradas.at(i)) + rectCabecera.height() + rectTitulo.height() + Espacio;
            else if((i > 0) && ((entAnt == "Prod") || (entAnt == "Sal")) && (ent == "Env"))
                alto = AltoEntradaFPEnv(painter, entradas.at(i)) + rectCabecera.height() + rectTitulo.height() + Espacio;
            else if((i > 0) && ((entAnt == "Prod") || (entAnt == "Sal") || (entAnt == "Env")) && (ent == "Stock"))
                alto = AltoEntradaFPStock(painter, entradas.at(i)) + rectCabecera.height() + rectTitulo.height() + Espacio;
            else if ((i > 0) && (ent == "Sal"))
                alto = AltoEntradaFPSal(painter, entradas.at(i));
            else if((i > 0) && (ent == "Env"))
                alto = AltoEntradaFPEnv(painter, entradas.at(i));
            else if((i > 0)  && (ent == "Stock"))
                alto = AltoEntradaFPStock(painter, entradas.at(i));
        }
        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = rectCabecera.height() + rectTitulo.height() + Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

int clsImprimirProductos::AltoEntradaFPProd(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    painter->setFont(fuenteNegrita);

    QRect textRectFrecCalib = painter->boundingRect(0,0,AnchoCol1, maxAltDispo,Qt::TextWordWrap,QObject::tr("Frec. Certificación"));

    QRect textRectFechaCalib = painter->boundingRect(0,0,AnchoCol1, maxAltDispo,Qt::TextWordWrap,QObject::tr("Fecha Próx. Certificación"));

    int AnchoDescrip = QFontMetrics(painter->font()).width(QObject::tr("Descripción"));

    painter->setFont(fuenteInicial);

    QRect textRectDescr = painter->boundingRect(0,
                                                0,
                                                maxAnch - rectImagen.width() - Espacio - AnchoDescrip - EspacioCorto,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[2]);

    QRect textRectCat = painter->boundingRect(0,0,(maxAnch - rectImagen.width() - EspacioCorto - 2*AnchoCol1)/2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[3]);

    QRect textRectFam = painter->boundingRect(0,0,(maxAnch - rectImagen.width() - EspacioCorto - 2*AnchoCol1)/2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[4]);

    QRect textRectEstado = painter->boundingRect(0,0,AnchoCol2, maxAltDispo,Qt::TextWordWrap,campos[6]);

    return intAltoEntrada = 3*rectfuenteInicial.height() +
            qMax(textRectDescr.height(),rectfuenteInicial.height()) +
            qMax(textRectCat.height(),qMax(textRectFam.height(),rectfuenteInicial.height())) +
            qMax(textRectFrecCalib.height(),rectfuenteInicial.height()) +
            qMax(textRectFechaCalib.height(),rectfuenteInicial.height()) +
            qMax(textRectEstado.height(),rectfuenteInicial.height());
}

int clsImprimirProductos::AltoEntradaFPSal(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    painter->setFont(fuenteNegrita);

    int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
    int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
    int AnchoCol = maxAnch - AnchoNSal - AnchoFSal -4*Espacio;

    painter->setFont(fuenteInicial);

    QRect textRectProy = painter->boundingRect(0,
                                               0,
                                               AnchoCol/4,
                                               maxAltDispo,
                                               Qt::TextWordWrap,
                                               campos[2]);
    QRect textRectTarea = painter->boundingRect(0,
                                                0,
                                                AnchoCol/4,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[3]);
    QRect textRectOp = painter->boundingRect(0,
                                             0,
                                             AnchoCol/2,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[5]);

    return intAltoEntrada = qMax(qMax(textRectProy.height(),qMax(textRectTarea.height(),textRectOp.height())),rectfuenteInicial.height());
}

int clsImprimirProductos::AltoEntradaFPEnv(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    painter->setFont(fuenteNegrita);

    int AnchoNEnv = QFontMetrics(painter->font()).width(QObject::tr("Nº Envío"));
    int AnchoFEnv = QFontMetrics(painter->font()).width(QObject::tr("Fecha Envío"));
    int AnchoCol = maxAnch - AnchoNEnv - AnchoFEnv - 2*Espacio;

    painter->setFont(fuenteInicial);

    QRect textRectCentro = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 campos[2]);

    return intAltoEntrada = qMax(textRectCentro.height(),rectfuenteInicial.height());
}

int clsImprimirProductos::AltoEntradaFPStock(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    painter->setFont(fuenteNegrita);

    int AnchoOrigen = qMax(QFontMetrics(painter->font()).width(QObject::tr("Entrada")),QFontMetrics(painter->font()).width(QObject::tr("Salida")));
    int AnchoFStock = QFontMetrics(painter->font()).width(QObject::tr("Fecha Stock"));
    int AnchoCantidad = QFontMetrics(painter->font()).width(QObject::tr("Cantidad"));
    int AnchoCol = maxAnch - AnchoOrigen - AnchoFStock - AnchoCantidad -3*Espacio;

    painter->setFont(fuenteInicial);

    QRect textRectComen = painter->boundingRect(0,
                                                0,
                                                AnchoCol,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[3]);

    return intAltoEntrada = qMax(textRectComen.height(),rectfuenteInicial.height());
}

void clsImprimirProductos::ImprimirPaginasFP(QPainter *painter,const QList<QStringList> &paginas)
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
            if (j == 0)
                ImprimirImagen(painter);
            ImprimirCabeceraFP(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFP(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirProductos::ImprimirImagen(QPainter *painter)
{
    painter->drawImage(rectImagen,*imagen);
}

void clsImprimirProductos::ImprimirCabeceraFP(QPainter *painter)
{
    painter->setFont(fuenteTitulo);

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setFont(fuenteInicial);

    QDateTime fecha;

    painter->drawText(0,rectTitulo.height(),maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height() + Espacio;
}

void clsImprimirProductos::ImprimirCabeceraFPS(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Salidas del Producto"));

    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->setFont(fuenteNegrita);

    int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
    int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
    int AnchoCol = maxAnch - AnchoNSal - AnchoFSal -4*Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNSal,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Salida"));

    AnchoObjeto += AnchoNSal+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Proyecto"));

    AnchoObjeto += AnchoCol/4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/4,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tarea"));

    AnchoObjeto += AnchoCol/4+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFSal,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Salida"));

    AnchoObjeto += AnchoFSal+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/2,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Operario"));

    yLinea += rectCabecera.height();
}

void clsImprimirProductos::ImprimirCabeceraFPEnv(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Envios del Producto"));

    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->setFont(fuenteNegrita);

    int AnchoNEnv = QFontMetrics(painter->font()).width(QObject::tr("Nº Envío"));
    int AnchoFEnv = QFontMetrics(painter->font()).width(QObject::tr("Fecha Envío"));
    int AnchoCol = maxAnch - AnchoNEnv - AnchoFEnv - 2*Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNEnv,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Envío"));

    AnchoObjeto += AnchoNEnv+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));

    AnchoObjeto += AnchoCol+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFEnv,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Envío"));

    yLinea += rectCabecera.height();
}

void clsImprimirProductos::ImprimirCabeceraFPStock(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Stock del Producto"));

    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setBrush(brushInicial);
    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->setFont(fuenteNegrita);

    int AnchoOrigen = qMax(QFontMetrics(painter->font()).width(QObject::tr("Entrada")),QFontMetrics(painter->font()).width(QObject::tr("Salida")));
    int AnchoFStock = QFontMetrics(painter->font()).width(QObject::tr("Fecha Stock"));
    int AnchoCantidad = QFontMetrics(painter->font()).width(QObject::tr("Cantidad"));
    int AnchoCol = maxAnch - AnchoOrigen - AnchoFStock - AnchoCantidad -3*Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFStock,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Stock"));

    AnchoObjeto += AnchoFStock+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoOrigen,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Origen"));

    AnchoObjeto += AnchoOrigen+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Comentarios"));

    AnchoObjeto += AnchoCol+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCantidad,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));

    yLinea += rectCabecera.height();
}

void clsImprimirProductos::ImprimirPaginaFP(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    QStringList campos;
    QString entAnt="",ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        if (i > 0)
        {
            campos = entradas.at(i).split("-|-");
            ent = campos[0];

            campos = entradas.at(i-1).split("-|-");
            entAnt = campos[0];

            if ((entAnt == "Prod") && (ent == "Sal"))
                colorLinea = 0;
            else if(((entAnt == "Prod") || (entAnt == "Sal")) && (ent == "Env"))
                colorLinea = 0;
            else if(((entAnt == "Prod") || (entAnt == "Sal") || (entAnt == "Env")) && (ent == "Stock"))
                colorLinea = 0;
        }
        else
            colorLinea = 0;

        ImprimirLineaFP(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirProductos::ImprimirLineaFP(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");

    int AnchoObjeto = 0;

    int maxAnchoDispo = 0;

    if (campos[0] == "Prod")
    {
        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        painter->drawRect(AnchoCol1,yLinea,AnchoCol1 + 2*AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        yLinea += 0;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Código"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1 + 2*AnchoCol2 - EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[1]);

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        QRect textRectDescr = painter->boundingRect(0,0,AnchoCol1 + 2*AnchoCol2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[2]);

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectDescr.height()),
                          Qt::AlignJustify,
                          QObject::tr("Descripción"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,textRectDescr.width(),qMax(rectfuenteNegrita.height(),textRectDescr.height()),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[2]);

        painter->drawRect(AnchoCol1,yLinea,maxAnchoDispo-AnchoCol1,qMax(rectfuenteNegrita.height(),textRectDescr.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectDescr.height()));

        yLinea += qMax(rectfuenteNegrita.height(),textRectDescr.height());
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Categoría"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        QRect textRectCat = painter->boundingRect(0,0,AnchoCol2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[3]);
        painter->drawText(AnchoObjeto+EspacioCorto/2,
                          yLinea,
                          textRectCat.width(),
                          textRectCat.height(),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[3]);

        AnchoObjeto += AnchoCol2;
        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,QFontMetrics(painter->font()).height(),
                          Qt::AlignJustify,
                          QObject::tr("Familia"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        QRect textRectFam = painter->boundingRect(0,0,AnchoCol2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[4]);
        painter->drawText(AnchoObjeto+EspacioCorto/2,
                          yLinea,
                          textRectFam.width(),
                          textRectFam.height(),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[4]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(qMax(rectfuenteNegrita.height(),textRectCat.height()),textRectFam.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(qMax(rectfuenteNegrita.height(),textRectCat.height()),textRectFam.height()));

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,qMax(qMax(rectfuenteNegrita.height(),textRectCat.height()),textRectFam.height()));
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,qMax(qMax(rectfuenteNegrita.height(),textRectCat.height()),textRectFam.height()));

        yLinea += qMax(qMax(rectfuenteNegrita.height(),textRectCat.height()),textRectFam.height());
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        QRect textRectEstado = painter->boundingRect(0,0,AnchoCol2, maxAltDispo,Qt::TextWordWrap,campos[6]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(rectfuenteNegrita.height(),textRectEstado.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectEstado.height()));

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,qMax(rectfuenteNegrita.height(),textRectEstado.height()));
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectEstado.height()));

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectEstado.height()),
                          Qt::AlignJustify,
                          QObject::tr("Disponible"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,qMax(rectfuenteNegrita.height(),textRectEstado.height()),
                          Qt::AlignJustify,
                          campos[5]);

        AnchoObjeto += AnchoCol2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectEstado.height()),
                          Qt::AlignJustify,
                          QObject::tr("Estado"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,textRectEstado.width(),qMax(rectfuenteNegrita.height(),textRectEstado.height()),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[6]);

        yLinea += qMax(rectfuenteNegrita.height(),textRectEstado.height());
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        QRect lineaFrecCalib = painter->boundingRect(0,0,AnchoCol1, maxAltDispo,Qt::TextWordWrap,QObject::tr("Frec. Certificación"));

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,lineaFrecCalib.height());
        painter->drawRect(0,yLinea,AnchoCol1,lineaFrecCalib.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,lineaFrecCalib.height(),
                          Qt::AlignJustify,
                          QObject::tr("Certificable"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,lineaFrecCalib.height(),
                          Qt::AlignJustify,
                          campos[9]);

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,lineaFrecCalib.height());
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,lineaFrecCalib.height());

        AnchoObjeto += AnchoCol2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,lineaFrecCalib.height(),
                          Qt::AlignLeft|Qt::TextWordWrap,
                          QObject::tr("Frec. Certificación"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,lineaFrecCalib.height(),
                          Qt::AlignJustify,
                          campos[12] + QObject::tr(" meses"));

        yLinea += qMax(lineaFrecCalib.height(),rectfuenteNegrita.height());
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        QRect lineaFechaCalib = painter->boundingRect(0,0,AnchoCol1, maxAltDispo,Qt::TextWordWrap,QObject::tr("Fecha Próx. Certificación"));

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,lineaFechaCalib.height());
        painter->drawRect(0,yLinea,AnchoCol1,lineaFechaCalib.height());

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,lineaFechaCalib.height());
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,lineaFechaCalib.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,lineaFechaCalib.height(),
                          Qt::AlignLeft|Qt::TextWordWrap,
                          QObject::tr("Fecha Próx. Certificación"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,lineaFechaCalib.height(),
                          Qt::AlignJustify,
                          campos[10]);

        AnchoObjeto += AnchoCol2;

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,lineaFechaCalib.height());
        painter->drawRect(0,yLinea,AnchoCol1,lineaFechaCalib.height());

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,lineaFechaCalib.height());
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,lineaFechaCalib.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,lineaFechaCalib.height(),
                          Qt::AlignLeft|Qt::TextWordWrap,
                          QObject::tr("Fecha Últ. Certificación"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,lineaFechaCalib.height(),
                          Qt::AlignJustify,
                          campos[11]);

        yLinea += qMax(lineaFrecCalib.height(),rectfuenteNegrita.height());

        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,rectfuenteNegrita.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Limitado"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[13]);
        AnchoObjeto += AnchoCol2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Límite"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[14]);

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = 0;

        if((rectImagen.height() + rectImagen.y()+ EspacioCorto/2) > yLinea)
            maxAnchoDispo = maxAnch - rectImagen.width() - EspacioCorto;
        else
            maxAnchoDispo = maxAnch - EspacioCorto;

        AnchoCol2 = (maxAnchoDispo - 2*AnchoCol1)/2;

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Stock Total"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[7]);

        AnchoObjeto += AnchoCol2;

        painter->drawRect(2*AnchoCol1+AnchoCol2,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(AnchoCol1+AnchoCol2,yLinea,AnchoCol1,rectfuenteNegrita.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Stock Centro"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[8]);

        yLinea = qMax(yLinea,rectImagen.y()+rectImagen.height());
    }
    else if(campos[0] == "Sal")
    {
        if(colorLinea == 0)
            ImprimirCabeceraFPS(painter);

        painter->setFont(fuenteNegrita);

        int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
        int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
        int AnchoCol = maxAnch - AnchoNSal - AnchoFSal -4*Espacio;

        painter->setFont(fuenteInicial);

        QRect textRectProy = painter->boundingRect(0,
                                                   0,
                                                   AnchoCol/4,
                                                   maxAltDispo,
                                                   Qt::TextWordWrap,
                                                   campos[2]);
        QRect textRectTarea = painter->boundingRect(0,
                                                    0,
                                                    AnchoCol/4,
                                                    maxAltDispo,
                                                    Qt::TextWordWrap,
                                                    campos[3]);
        QRect textRectOp = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol/2,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 campos[5]);

        int AltoObjeto = qMax(textRectProy.height(),qMax(textRectTarea.height(),qMax(textRectOp.height(),rectfuenteInicial.height())));

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
                          AnchoNSal,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter,
                          campos[1]);

        AnchoObjeto += AnchoNSal+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/4,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[2]);

        AnchoObjeto += AnchoCol/4+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/4,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[3]);

        AnchoObjeto += AnchoCol/4+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoFSal,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[4]);

        AnchoObjeto += AnchoFSal+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/2,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter,
                          campos[5]);

        colorLinea++;

        yLinea += AltoObjeto;
    }
    else if(campos[0] == "Env")
    {
        if(colorLinea == 0)
            ImprimirCabeceraFPEnv(painter);

        QPen penInicial = painter->pen();

        painter->setFont(fuenteNegrita);

        int AnchoNEnv = QFontMetrics(painter->font()).width(QObject::tr("Nº Envío"));
        int AnchoFEnv = QFontMetrics(painter->font()).width(QObject::tr("Fecha Envío"));
        int AnchoCol = maxAnch - AnchoNEnv - AnchoFEnv -3*Espacio;

        painter->setFont(fuenteInicial);

        int AltoObjeto = AltoEntradaFPEnv(painter,cadena);

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
                          AnchoNEnv,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter,
                          campos[1]);

        AnchoObjeto += AnchoNEnv+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[2]);

        AnchoObjeto += AnchoCol+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoFEnv,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[3]);

        colorLinea++;

        yLinea += AltoObjeto;
    }
    else if(campos[0] == "Stock")
    {
        if(colorLinea == 0)
            ImprimirCabeceraFPStock(painter);

        QPen penInicial = painter->pen();

        painter->setFont(fuenteNegrita);

        int AnchoOrigen = qMax(QFontMetrics(painter->font()).width(QObject::tr("Entrada")),QFontMetrics(painter->font()).width(QObject::tr("Salida")));
        int AnchoFStock = QFontMetrics(painter->font()).width(QObject::tr("Fecha Stock"));
        int AnchoCantidad = QFontMetrics(painter->font()).width(QObject::tr("Cantidad"));
        int AnchoCol = maxAnch - AnchoOrigen - AnchoFStock - AnchoCantidad -3*Espacio;

        painter->setFont(fuenteInicial);

        QRect textRectComen = painter->boundingRect(0,
                                                    0,
                                                    AnchoCol,
                                                    maxAltDispo,
                                                    Qt::TextWordWrap,
                                                    campos[3]);

        int AltoObjeto = qMax(textRectComen.height(),rectfuenteInicial.height());

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
                          AnchoFStock,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter,
                          campos[1]);

        AnchoObjeto += AnchoFStock+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoOrigen,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[2]);

        AnchoObjeto += AnchoOrigen+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[3]);

        AnchoObjeto += AnchoCol+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCantidad,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[4]);

        colorLinea++;

        yLinea += AltoObjeto;
    }
}


void clsImprimirProductos::ImprimirListaCategoria(QString strFiltro)
{
    AnchoCod = 0;

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

        clsCategorias Categoria;
        QVector<QStringList> listaCategorias = Categoria.ListaCategorias(strFiltro);

        for (int i=0;i<listaCategorias.size();i++)
        {
            QStringList strCategorias = listaCategorias.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strCategorias.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strCategorias.at(0));

            entrada = strCategorias.at(0) + "-|-" + strCategorias.at(1);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Categorías");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCod = qMax(AnchoCod,QFontMetrics(painter.font()).width(QObject::tr("Código")));
        AnchoDesc = maxAnch - AnchoCod - Espacio;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;

        yLinea = 0;

        DividirEnPaginasLCat(&painter,&pages,listaEntradas);
        ImprimirPaginasLCat(&painter,pages);
    }
}

void clsImprimirProductos::DividirEnPaginasLCat(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLCat(painter, entradas.at(i));
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

void clsImprimirProductos::ImprimirPaginasLCat(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLCat(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLCat(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirProductos::ImprimirPaginaLCat(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLCat(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirProductos::ImprimirCabeceraLCat(QPainter *painter)
{
    painter->setFont(fuenteTitulo);

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
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
                      AnchoCod,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));

    yLinea += rectCabecera.height();
}

int clsImprimirProductos::AltoEntradaLCat(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);

    return qMax(textRectDesc.height(),rectfuenteInicial.height());
}

void clsImprimirProductos::ImprimirLineaLCat(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0, AnchoObjeto = 0;

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);

    AltoObjeto = qMax(textRectDesc.height(),rectfuenteInicial.height());

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

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCod,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCod + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirProductos::ImprimirListaFamilia(QString strFiltro)
{
    AnchoCod = 0;

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

        clsFamilias Familia;
        QVector<QStringList> listaFamilias = Familia.ListaFamilias(strFiltro);

        for (int i=0;i<listaFamilias.size();i++)
        {
            QStringList strFamilias = listaFamilias.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strFamilias.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strFamilias.at(0));

            entrada = strFamilias.at(0) + "-|-" + strFamilias.at(1) + "-|-" + strFamilias.at(2) + "-|-" + strFamilias.at(3) + "-|-" + strFamilias.at(4) + "-|-" + strFamilias.at(5);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Familias");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,2*QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCod = qMax(AnchoCod,QFontMetrics(painter.font()).width(QObject::tr("Código")));
        AnchoFrec = qMax(QFontMetrics(painter.font()).width(QObject::tr("Frecuencia ")),QFontMetrics(painter.font()).width(QObject::tr("certificación")));
        AnchoLim = QFontMetrics(painter.font()).width(QObject::tr("Limitada "));
        AnchoDispo = QFontMetrics(painter.font()).width(QObject::tr("Disponible"));
        AnchoCerti = QFontMetrics(painter.font()).width(QObject::tr("Certificable"));
        AnchoDesc = maxAnch - (AnchoCod + AnchoDispo + AnchoCerti + AnchoFrec + AnchoLim + 5*Espacio);

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLFam(&painter,&pages,listaEntradas);
        ImprimirPaginasLFam(&painter,pages);
    }
}

void clsImprimirProductos::DividirEnPaginasLFam(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLFam(painter, entradas.at(i));
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

void clsImprimirProductos::ImprimirPaginasLFam(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLFam(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLFam(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirProductos::ImprimirPaginaLFam(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLFam(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirProductos::ImprimirCabeceraLFam(QPainter *painter)
{
    painter->setFont(fuenteTitulo);

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
                      AnchoCod,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoDesc + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoLim,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Limitada"));
    AnchoObjeto += AnchoLim + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoLim,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Límite entrega"));
    AnchoObjeto += AnchoLim + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCerti,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Certificable"));
    AnchoObjeto += AnchoCerti + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFrec,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Frecuencia certificación"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirProductos::AltoEntradaLFam(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);

    return textRectDesc.height();
}

void clsImprimirProductos::ImprimirLineaLFam(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0,AnchoObjeto=0;
    QString booleano = "";

    QRect textRectDesc = painter->boundingRect(AnchoCod + Espacio,0,AnchoDesc, rectfuenteInicial.height(),Qt::TextWordWrap,campos[1]);

    AltoObjeto = textRectDesc.height();

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

    painter->drawText(0,
                      yLinea,
                      AnchoCod,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);
    AnchoObjeto += AnchoDesc + Espacio;

    if (campos[2] == "true")
        booleano = QObject::tr("Sí­");
    else
        booleano = QObject::tr("No");
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoLim,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      booleano);
    AnchoObjeto += AnchoLim + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoLim,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);
    AnchoObjeto += AnchoLim + Espacio;

    if (campos[4] == "true")
        booleano = QObject::tr("Sí­");
    else
        booleano = QObject::tr("No");
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCerti,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      booleano);
    AnchoObjeto += AnchoCerti + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFrec,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[5]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirProductos::ImprimirListaHistorico(QString strFiltro)
{
    AnchoCod = 0;

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

        clsHistoricoCertificaciones HistCert;
        QVector<QStringList> listaHistCert = HistCert.ListasHistCert(strFiltro);

        for (int i=0;i<listaHistCert.size();i++)
        {
            QStringList strHistCert = listaHistCert.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strHistCert.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strHistCert.at(0));

            entrada = strHistCert.at(0) + "-|-" + strHistCert.at(1) + "-|-" + strHistCert.at(2) + "-|-" + strHistCert.at(3);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Histórico de Certificaciones");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,2*QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCod = qMax(AnchoCod,QFontMetrics(painter.font()).width(QObject::tr("Código")));
        AnchoFechCert = qMax(qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha ")),QFontMetrics(painter.font()).width(QObject::tr("dd/MM/yyyy"))),QFontMetrics(painter.font()).width(QObject::tr("certificación")));
        AnchoEmisor = (maxAnch - (AnchoCod + AnchoFechCert + 3*Espacio))/3;
        AnchoDesc = 2*(maxAnch - (AnchoCod + AnchoFechCert + 3*Espacio))/3;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLHist(&painter,&pages,listaEntradas);
        ImprimirPaginasLHist(&painter,pages);
    }
}

void clsImprimirProductos::DividirEnPaginasLHist(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLHist(painter, entradas.at(i));
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

void clsImprimirProductos::ImprimirPaginasLHist(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLHist(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLHist(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirProductos::ImprimirPaginaLHist(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLHist(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirProductos::ImprimirCabeceraLHist(QPainter *painter)
{
    painter->setFont(fuenteTitulo);

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
                      AnchoCod,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoDesc + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFechCert,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Fecha certificación"));
    AnchoObjeto += AnchoFechCert + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoEmisor,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Emisor del certificado"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirProductos::AltoEntradaLHist(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);

    return textRectDesc.height();
}

void clsImprimirProductos::ImprimirLineaLHist(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0,AnchoObjeto=0;

    QRect textRectDesc = painter->boundingRect(AnchoCod + Espacio,0,AnchoDesc, rectfuenteInicial.height(),Qt::TextWordWrap,campos[1]);

    AltoObjeto = textRectDesc.height();

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

    painter->drawText(0,
                      yLinea,
                      AnchoCod,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);
    AnchoObjeto += AnchoDesc + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFechCert,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);
    AnchoObjeto += AnchoFechCert + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoEmisor,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    colorLinea++;

    yLinea += AltoObjeto;
}
