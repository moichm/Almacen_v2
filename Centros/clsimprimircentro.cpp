#include "clsimprimircentro.h"

clsImprimirCentro::clsImprimirCentro(){}

void clsImprimirCentro::ImprimirLista(QString strFiltro)
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

        clsCentros Centro;
        QVector<QStringList> listaCentros = Centro.ListaCentros(strFiltro);

        for (int i=0;i<listaCentros.size();i++)
        {
            QStringList strCentros = listaCentros.at(i);

            if (AnchoCod < QFontMetrics(fuenteInicial).width(strCentros.at(0)))
                AnchoCod = QFontMetrics(fuenteInicial).width(strCentros.at(0));

            entrada = strCentros.at(0) + "-|-" + strCentros.at(1) + "-|-" + strCentros.at(2) + "-|-" + strCentros.at(3);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Centros");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);

        painter.setFont(fuenteNegrita);

        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoCod = qMax(AnchoCod,QFontMetrics(painter.font()).width(QObject::tr("Código")));
        AnchoNombre = (maxAnch - (AnchoCod + 3*Espacio))/4;
        AnchoCiudad = (maxAnch - (AnchoCod + 3*Espacio))/4;
        AnchoDirec = (maxAnch - (AnchoCod + 3*Espacio))/2;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;

        yLinea = 0;

        DividirEnPaginasLC(&painter,&pages,listaEntradas);
        ImprimirPaginasLC(&painter,pages);
    }
}

void clsImprimirCentro::DividirEnPaginasLC(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLC(painter, entradas.at(i));
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

void clsImprimirCentro::ImprimirPaginasLC(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLC(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLC(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirCentro::ImprimirPaginaLC(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLC(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirCentro::ImprimirCabeceraLC(QPainter *painter)
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
                      AnchoNombre,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nombre"));
    AnchoObjeto += AnchoNombre + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCiudad,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Ciudad"));
    AnchoObjeto += AnchoCiudad + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCiudad,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Dirección"));

    yLinea += rectCabecera.height();
}

int clsImprimirCentro::AltoEntradaLC(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectNom = painter->boundingRect(0,0,AnchoNombre, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1]);
    QRect textRectCiu = painter->boundingRect(0,0,AnchoCiudad, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[2]);
    QRect textRectDir = painter->boundingRect(0,0,AnchoDirec, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[3]);

    return qMax(textRectNom.height(),qMax(textRectCiu.height(),qMax(textRectDir.height(),rectfuenteInicial.height())));
}

void clsImprimirCentro::ImprimirLineaLC(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0, AnchoObjeto = 0;

    AltoObjeto = AltoEntradaLC(painter,cadena);

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
                      AnchoNombre,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1]);

    AnchoObjeto += AnchoNombre + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCiudad,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCiudad + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDirec,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    colorLinea++;

    yLinea += AltoObjeto;
}
