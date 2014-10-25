#include "clsimprimirkits.h"
#include "Productos/producto.h"

clsImprimirKits::clsImprimirKits(){}

void clsImprimirKits::ImprimirLista(QString strFiltro)
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

        clsKits Kits;
        QVector<QStringList> listaKits = Kits.ListaKits(strFiltro);

        for (int i=0;i<listaKits.size();i++)
        {
            QStringList strKits = listaKits.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strKits.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strKits.at(0));

            entrada = strKits.at(0) + "-|-" + strKits.at(1);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Kits");

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

        DividirEnPaginasLK(&painter,&pages,listaEntradas);
        ImprimirPaginasLK(&painter,pages);
    }
}

void clsImprimirKits::DividirEnPaginasLK(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLK(painter, entradas.at(i));
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

void clsImprimirKits::ImprimirPaginasLK(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLK(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLK(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirKits::ImprimirPaginaLK(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLK(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirKits::ImprimirCabeceraLK(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();

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

int clsImprimirKits::AltoEntradaLK(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectDesc = painter->boundingRect(0,0,AnchoDesc, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);

    return qMax(textRectDesc.height(),rectfuenteInicial.height());
}

void clsImprimirKits::ImprimirLineaLK(QPainter *painter, const QString &cadena)
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


void clsImprimirKits::ImprimirFicha(const clsKits *Kit)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        strCodKit = QString::number(Kit->Codigo());
        strDescKit = Kit->Descripcion();

        AnchoCod = 0;
        AnchoCant = 0;

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

        QStringList listaEntradas;
        QString entrada;

        clsProductos Prod;

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        titulo = QObject::tr("Ficha de Kit");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        painter.setFont(fuenteNegrita);

        for(int i = 0;i < Kit->Detalles().size(); i++)
        {
            Prod.Cargar(Kit->Detalles().at(i).CodProd());

            entrada = Kit->Detalles().at(i).CodProd();
            if (AnchoCod < QFontMetrics(fuenteInicial).width(Kit->Detalles().at(i).CodProd()))
                AnchoCod = QFontMetrics(fuenteInicial).width(Kit->Detalles().at(i).CodProd());

            entrada += "-|-" + Prod.Descripcion();

            entrada += "-|-" + QString::number(Kit->Detalles().at(i).Cantidad());
            if (AnchoCant < QFontMetrics(fuenteInicial).width(QString::number(Kit->Detalles().at(i).Cantidad())))
                AnchoCant = QFontMetrics(fuenteInicial).width(QString::number(Kit->Detalles().at(i).Cantidad()));

            listaEntradas << entrada;
        }

        AnchoCant = qMax(AnchoCant,QFontMetrics(painter.font()).width(QObject::tr("Cantidad")));

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        QRect textRectDescr = painter.boundingRect(0,
                                                   0,
                                                   maxAnch-(QFontMetrics(painter.font()).width(QObject::tr("Descripción"))+2*EspacioCorto),
                                                   maxAltDispo,
                                                   Qt::TextWordWrap,
                                                   strDescKit);

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height() + rectfuenteNegrita.height() + rectCabecera.height() + qMax(rectfuenteNegrita.height(),textRectDescr.height()) + 2*Espacio);

        colorLinea = 0;

        DividirEnPaginasFK(&painter,&pages,listaEntradas);
        ImprimirPaginasFK(&painter,pages);
    }
}

void clsImprimirKits::DividirEnPaginasFK(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {        
        int alto = AltoEntradaFKL(painter, entradas.at(i));
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

int clsImprimirKits::AltoEntradaFKL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           maxAnch-AnchoCod-AnchoCant,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    return qMax(descRect.height(),rectfuenteNegrita.height());
}

void clsImprimirKits::ImprimirPaginasFK(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraFK(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFK(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirKits::ImprimirCabeceraFK(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    painter->setBrush(brushInicial);

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setFont(fuenteInicial);

    QDateTime fecha;

    painter->drawText(0,rectTitulo.height(),maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height() + Espacio;

    painter->setFont(fuenteNegrita);
    int AnchoDesc = QFontMetrics(painter->font()).width(QObject::tr("Descripción"));
    painter->setFont(fuenteInicial);

    QRect textRectDescr = painter->boundingRect(0,
                                                0,
                                                maxAnch-(AnchoDesc-2*EspacioCorto),
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                strDescKit);

    painter->drawRect(0,yLinea,AnchoDesc+EspacioCorto,rectfuenteNegrita.height() + qMax(rectfuenteNegrita.height(),textRectDescr.height()));
    painter->drawRect(AnchoDesc+EspacioCorto,yLinea,maxAnch-AnchoDesc-EspacioCorto,rectfuenteNegrita.height() + qMax(rectfuenteNegrita.height(),textRectDescr.height()));

    yLinea += 0;

    int AnchoObjeto = 0;

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoDesc+EspacioCorto,rectfuenteNegrita.height(),
                      Qt::AlignJustify|Qt::AlignVCenter,
                      QObject::tr("ID Kit"));
    AnchoObjeto += AnchoDesc+EspacioCorto;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,maxAnch-AnchoDesc-EspacioCorto,rectfuenteNegrita.height(),
                      Qt::AlignJustify|Qt::AlignVCenter,
                      strCodKit);

    AnchoObjeto = 0;

    yLinea += rectfuenteNegrita.height();

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoDesc+EspacioCorto,qMax(rectfuenteNegrita.height(),textRectDescr.height()),
                      Qt::AlignJustify|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += QFontMetrics(painter->font()).width(QObject::tr("Descripción"))+EspacioCorto;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,maxAnch-AnchoDesc-EspacioCorto,qMax(rectfuenteNegrita.height(),textRectDescr.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      strDescKit);

    yLinea += qMax(rectfuenteNegrita.height(),textRectDescr.height()) + Espacio;

    colorLinea = 0;
}

void clsImprimirKits::ImprimirCabeceraFKL(QPainter *painter)
{
    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    int AnchoCol = maxAnch - AnchoCod - AnchoCant - 2*Espacio;
    int AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCod,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Código"));
    AnchoObjeto += AnchoCod + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCant,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Cantidad"));

    yLinea += rectCabecera.height();
}

void clsImprimirKits::ImprimirPaginaFK(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaFK(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirKits::ImprimirLineaFK(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");

    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFKL(painter);

    painter->setFont(fuenteNegrita);

    int AnchoCol = maxAnch - AnchoCod - AnchoCant - 2*Espacio;

    QRect descRect = painter->boundingRect(0,
                                           0,
                                           maxAnch-AnchoCod-AnchoCant,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[1]);

    int AltoObjeto = qMax(descRect.height(),rectfuenteInicial.height());

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
                      AnchoCod,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoCod+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    AnchoObjeto += AnchoCol + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCant,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    colorLinea++;

    yLinea += AltoObjeto;
}
