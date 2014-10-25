#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QMessageBox>
#include "clsimprimirpartes.h"
#include "Operarios/operario.h"

clsImprimirPartes::clsImprimirPartes(){}

clsImprimirPartes::~clsImprimirPartes(){}

void clsImprimirPartes::ImprimirListaPartes(QString strFiltro)
{
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

        clsPartes_Trabajo PartesTrabajo;
        QVector<QStringList> listaPartesTrabajo = PartesTrabajo.ListaPartes_Trabajo(strFiltro);

        for (int i=0;i<listaPartesTrabajo.size();i++)
        {
            QStringList strPartesTrabajo = listaPartesTrabajo.at(i);

            entrada = strPartesTrabajo.at(0) + "-|-" + strPartesTrabajo.at(1) + "-|-" + strPartesTrabajo.at(2) + "-|-" + strPartesTrabajo.at(3);

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Lista de Partes de Trabajo");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoNum = QFontMetrics(painter.font()).width(QObject::tr("Nº Parte"));
        AnchoFecha = QFontMetrics(painter.font()).width(QObject::tr("Fecha Parte"));
        AnchoRes = (maxAnch - AnchoNum - AnchoFecha - 3*Espacio)/3;
        AnchoDesc = 2*(maxAnch - AnchoNum - AnchoFecha - 3*Espacio)/3;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLP(&painter,&pages,listaEntradas);
        ImprimirPaginasLP(&painter,pages);
    }
}

void clsImprimirPartes::DividirEnPaginasLP(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for( int i = 0;i<entradas.count();i++)
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

void clsImprimirPartes::ImprimirPaginasLP(QPainter *painter,const QList<QStringList> &paginas)
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

void clsImprimirPartes::ImprimirPaginaLP(QPainter *painter,const QStringList &entradas, int NumPagina)
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

void clsImprimirPartes::ImprimirCabeceraLP(QPainter *painter)
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
                      AnchoNum,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Nº Parte"));

    AnchoObjeto += AnchoNum + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFecha,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Fecha Parte"));

    AnchoObjeto += AnchoFecha + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoRes,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Responsable"));

    AnchoObjeto += AnchoRes + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      rectCabecera.height(),
                      Qt::AlignLeft,
                      QObject::tr("Descripción"));

    yLinea += rectCabecera.height();

    colorLinea = 0;
}

int clsImprimirPartes::AltoEntradaLP(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoDesc,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[3]);
    QRect resRect = painter->boundingRect(0,
                                          0,
                                          AnchoRes,
                                          maxAltDispo,
                                          Qt::TextWordWrap,
                                          campos[2]);
    return qMax(descRect.height(),resRect.height());
}

void clsImprimirPartes::ImprimirLineaLP(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    painter->setFont(fuenteInicial);

    int AltoEntrada = AltoEntradaLP(painter,cadena);

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
                      AnchoNum,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);

    AnchoObjeto += AnchoNum + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFecha,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoFecha + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoRes,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoRes + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoDesc,
                      AltoEntrada,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    colorLinea++;

    yLinea += AltoEntrada;
}


void clsImprimirPartes::ImprimirFichaPartes(const clsPartes_Trabajo *Partes)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        AnchoNumH = 0;

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

        clsOperarios clsOp(Partes->Responsable());
        clsOp.Cargar(Partes->Responsable());

        entrada = QObject::tr("Parte")+"-|-" + QString::number(Partes->NoParte());
        entrada += "-|-" + Partes->FechaParte().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Partes->Responsable() + " - " + clsOp.Nombre() + " " + clsOp.Apellidos();
        entrada += "-|-" + Partes->Descripcion();

        entradaCab = entrada;

        listaEntradas << entrada;

        for (int i=0;i<Partes->Detalles().size();i++)
        {
            clsPartes_Trabajo_Det PartesDetalle = Partes->Detalles().at(i);
            clsOperarios Op(PartesDetalle.Operario());
            Op.Cargar(PartesDetalle.Operario());

            if (AnchoNumH < (QFontMetrics(fuenteNegrita).width(QString::number(PartesDetalle.Horas()))))
                AnchoNumH = QFontMetrics(fuenteNegrita).width(QString::number(PartesDetalle.Horas()));

            entrada = QObject::tr("Linea")+"-|-" + PartesDetalle.Operario() + "-|-" + Op.Nombre() + " " + Op.Apellidos() + "-|-" + qltHora[PartesDetalle.TipoHora()] + "-|-" + QString::number(PartesDetalle.Horas()) + "-|-" + PartesDetalle.Proyecto() + "-|-" + PartesDetalle.Tarea();

            listaEntradas << entrada;
        }

        titulo = QObject::tr("Parte de Trabajo");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectFirmaRes = QRect(maxAnch/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);
        rectFirmaRRHH = QRect(maxAnch-(maxAnch*60)/175-(maxAnch)/175,maxAlt-rectPie.height()-maxAnch/175-(maxAlt*40)/257,(maxAnch*60)/175,(maxAlt*40)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoNIF = QFontMetrics(fuenteNegrita).width("00000000X");
        AnchoTHora = 0;
        for(int i=0;i<qltHora.size();i++)
            AnchoTHora = qMax(QFontMetrics(fuenteNegrita).width(qltHora.at(i)),AnchoTHora);
        AnchoTHora = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Tipo Hora")),AnchoTHora);
        AnchoNumH = qMax(QFontMetrics(fuenteNegrita).width(QObject::tr("Nº Horas")),AnchoNumH);

        AnchoCol = maxAnch - AnchoNIF - AnchoTHora - AnchoNumH - 5*Espacio;

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Responsable")),qMax(QFontMetrics(painter.font()).width(QObject::tr("Fecha Parte")),QFontMetrics(painter.font()).width(QObject::tr("Descripción"))))+EspacioCorto;

        AnchoCol2 = maxAnch - AnchoCol1;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        TotalPaginas = CalcularPaginasFP(&painter,listaEntradas);
        if (TotalPaginas == 1)
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        colorLinea = 0;
        yLinea = 0;

        DividirEnPaginasFP(&painter,&pages,listaEntradas);
        ImprimirPaginasFP(&painter,pages);
    }
}

int clsImprimirPartes::CalcularPaginasFP(QPainter *painter, const QStringList &entradas)
{
    int AltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

    QList<QStringList> paginas;
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Parte")))
            alto = AltoEntradaFP(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFPL(painter, entradas.at(i));
        if (y + alto > AltDispo && !PaginaActual.empty())
        {
            paginas.append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFP(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas.append(PaginaActual);

    return paginas.size();
}

void clsImprimirPartes::DividirEnPaginasFP(QPainter *painter, QList<QStringList> *paginas, const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == QObject::tr("Parte")))
            alto = AltoEntradaFP(painter, entradaCab)+Espacio;
        else
            alto = AltoEntradaFPL(painter, entradas.at(i));

        if (TotalPaginas - 1 == paginas->size())
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + rectFirmaRes.height() + rectfuenteNegrita.height() + Espacio);
        else
            maxAltDispo = maxAlt - (rectTitulo.height() + rectFecha.height() + rectPie.height() + rectCabecera.height() + Espacio);

        if (y + alto > maxAltDispo && !PaginaActual.empty())
        {
            paginas->append(PaginaActual);
            PaginaActual.clear();
            y = AltoEntradaFP(painter, entradaCab)+Espacio;
        }
        PaginaActual.append(entradas.at(i));
        y += alto;
    }
    if (!PaginaActual.empty())
        paginas->append(PaginaActual);
}

int clsImprimirPartes::AltoEntradaFP(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    QRect resRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[3]);
    QRect descRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol2,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[4]);
    return qMax(descRect.height(),rectfuenteNegrita.height())+qMax(resRect.height(),rectfuenteNegrita.height())+2*rectfuenteNegrita.height();
}

int clsImprimirPartes::AltoEntradaFPL(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    painter->setFont(fuenteInicial);

    QRect NomOpRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol/3,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);
    QRect ProyRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol/3,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[5]);
    QRect TareaRect = painter->boundingRect(0,
                                           0,
                                           AnchoCol/3,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[6]);

    return qMax(qMax(NomOpRect.height(),qMax(ProyRect.height(),TareaRect.height())),rectfuenteNegrita.height());
}

void clsImprimirPartes::ImprimirPaginasFP(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraFP(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFP(painter, paginas[indice], indice + 1);
            if (j == numPaginas-1)
                ImprimirFirmaFP(painter);
        }
}

void clsImprimirPartes::ImprimirFirmaFP(QPainter *painter)
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
                      QFontMetrics(painter->font()).width(QObject::tr("RRHH")),
                      rectFirmaRRHH.height(),
                      Qt::AlignLeft | Qt::AlignBottom,
                      QObject::tr("RRHH"));
    painter->drawRect(rectFirmaRRHH);
}

void clsImprimirPartes::ImprimirCabeceraFP(QPainter *painter)
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

void clsImprimirPartes::ImprimirCabeceraFPL(QPainter *painter)
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
                      AnchoNIF,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("NIF"));
    AnchoObjeto += AnchoNIF+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nombre Operario"));
    AnchoObjeto += AnchoCol/3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoTHora,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tipo Hora"));
    AnchoObjeto += AnchoTHora+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNumH,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Horas"));
    AnchoObjeto += AnchoNumH+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Proyecto"));
    AnchoObjeto += AnchoCol/3+Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tarea"));

    yLinea += rectCabecera.height();
}

void clsImprimirPartes::ImprimirPaginaFP(QPainter *painter,const QStringList &entradas,int NumPagina)
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

            ImprimirLineaFP(painter, entradaCab);
            if(ent == QObject::tr("Linea"))
                ImprimirLineaFPL(painter, entradas.at(i));
        }
        else
            ImprimirLineaFPL(painter, entradas.at(i));
    }

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirPartes::ImprimirLineaFP(QPainter *painter, const QString &cadena)
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
                      QObject::tr("Nº Parte"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Parte"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Responsable"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    AnchoObjeto = 0;
    yLinea += rectfuenteNegrita.height();

    QRect textRectDescr = painter->boundingRect(0,
                                                0,
                                                AnchoCol2,
                                                maxAltDispo,
                                                Qt::TextWordWrap,campos[4]);

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(textRectDescr.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(textRectDescr.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol1,
                      qMax(textRectDescr.height(),rectfuenteNegrita.height()),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,
                      yLinea,
                      AnchoCol2,
                      qMax(textRectDescr.height(),rectfuenteNegrita.height()),
                      Qt::AlignJustify|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    yLinea += qMax(textRectDescr.height(),rectfuenteNegrita.height());
}

void clsImprimirPartes::ImprimirLineaFPL(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFPL(painter);

    QPen penInicial = painter->pen();

    int AltoObjeto = AltoEntradaFPL(painter,cadena);

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
                      AnchoNIF,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoNIF+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCol/3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoTHora,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    AnchoObjeto += AnchoTHora+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNumH,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    AnchoObjeto += AnchoNumH+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[5]);

    AnchoObjeto += AnchoCol/3+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/3,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[6]);

    colorLinea++;

    yLinea += AltoObjeto;
}
