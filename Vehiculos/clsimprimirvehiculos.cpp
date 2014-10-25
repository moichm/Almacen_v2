#include "clsimprimirvehiculos.h"

clsImprimirVehiculos::clsImprimirVehiculos(){}

void clsImprimirVehiculos::ImprimirLista(QString strFiltro)
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

        AnchoMat = 0;

        QStringList listaEntradas;
        QString entrada;

        clsVehiculos Vehiculo;
        QVector<QStringList> listaVehiculos = Vehiculo.ListaVehiculos(strFiltro);

        for (int i=0;i<listaVehiculos.size();i++)
        {
            QStringList strVehiculos = listaVehiculos.at(i);

            if (AnchoMat < QFontMetrics(fuenteInicial).width(strVehiculos.at(0)))
                AnchoMat = QFontMetrics(fuenteInicial).width(strVehiculos.at(0));

            entrada = strVehiculos.at(0) + "-|-" + strVehiculos.at(1) + "-|-" + strVehiculos.at(2) + "-|-" + strVehiculos.at(3);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Vehículos");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);

        painter.setFont(fuenteNegrita);

        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoMat = qMax(AnchoMat,QFontMetrics(painter.font()).width(QObject::tr("Código")));
        AnchoFMat = QFontMetrics(painter.font()).width(QObject::tr("Fecha Matricula"));
        AnchoCol = (maxAnch - (AnchoMat + AnchoFMat + 3*Espacio))/2;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height());

        colorLinea=0;

        yLinea = 0;

        DividirEnPaginasLV(&painter,&pages,listaEntradas);
        ImprimirPaginasLV(&painter,pages);
    }
}

void clsImprimirVehiculos::DividirEnPaginasLV(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLV(painter, entradas.at(i));
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

void clsImprimirVehiculos::ImprimirPaginasLV(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLV(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLV(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirVehiculos::ImprimirPaginaLV(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLV(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirVehiculos::ImprimirCabeceraLV(QPainter *painter)
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
                      AnchoMat,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Matrícula"));
    AnchoObjeto += AnchoMat + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Marca"));
    AnchoObjeto += AnchoCol + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Modelo"));
    AnchoObjeto += AnchoCol + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFMat,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Fecha Matrícula"));

    yLinea += rectCabecera.height();
}

int clsImprimirVehiculos::AltoEntradaLV(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect descMarca = painter->boundingRect(0,
                                           0,
                                           AnchoCol,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[1]);
    QRect descMod = painter->boundingRect(0,
                                           0,
                                           AnchoCol,
                                           maxAltDispo,
                                           Qt::TextWordWrap,
                                           campos[2]);

    return qMax(descMod.height(),descMarca.height());
}

void clsImprimirVehiculos::ImprimirLineaLV(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AnchoObjeto = 0;
    int altoEnt = AltoEntradaLV(painter, cadena);

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          altoEnt);
    }
    painter->setPen(penInicial);

    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoMat,
                      altoEnt,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);
    AnchoObjeto += AnchoMat + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      altoEnt,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);
    AnchoObjeto += AnchoCol + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol,
                      altoEnt,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[2]);
    AnchoObjeto += AnchoCol + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFMat,
                      altoEnt,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[3]);

    colorLinea++;

    yLinea += altoEnt;
}


void clsImprimirVehiculos::ImprimirFicha(const clsVehiculos *Vehi)
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

        imagen = new QImage(Vehi->Imagen());
        imagen->scaledToHeight((maxAlt*65)/257,Qt::SmoothTransformation);

        QStringList listaEntradas;
        QString entrada;

        clsCentros Centro;
        Centro.Cargar(Vehi->Centro());

        entrada = "Vehi-|-" + Vehi->Matricula();
        entrada += "-|-" + Vehi->Marca();
        entrada += "-|-" + Vehi->Modelo();
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();
        entrada += "-|-" + Vehi->FechaMatriculacion().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Vehi->FechaITV().toString(QObject::tr("dd/MM/yyyy"));

        listaEntradas << entrada;

        for(int i = 0;i<Vehi->Movimientos().size();i++)
        {
            clsVehiculo_Movs Vehi_Mov(Vehi->Movimientos().at(i));
            Centro.Cargar(Vehi_Mov.Centro());

            entrada = "Mov-|-" + Vehi_Mov.FechaMovimiento().toString(QObject::tr("dd/MM/yyyy")) + "-|-" + Centro.Nombre() + " - " + Centro.Ciudad() + "-|-" + QString::number(Vehi_Mov.KmsFinales());

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;
        EspacioCorto = maxAlt/257;

        titulo = QObject::tr("Ficha de Vehículo");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);
        rectImagen = QRect(maxAnch - ((maxAnch*50)/175),Espacio + rectTitulo.height() + rectFecha.height(),(maxAnch*50)/175,(maxAlt*65)/257);

        painter.setFont(fuenteNegrita);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Matrícula")),QFontMetrics(painter.font()).width(QObject::tr("Matriculación")))+EspacioCorto;

        AnchoCol2 = maxAnch - rectImagen.width() - Espacio - AnchoCol1 - EspacioCorto;

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height() + Espacio);

        colorLinea = 0;

        DividirEnPaginasFV(&painter,&pages,listaEntradas);
        ImprimirPaginasFV(&painter,pages);
    }
}

void clsImprimirVehiculos::DividirEnPaginasFV(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString ent="",entAnt="";

    for(int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == "Vehi"))
            alto = qMax(AltoEntradaFV(painter, entradas.at(i))+Espacio,rectImagen.height()+Espacio);
        else
        {
            campos = entradas.at(i-1).split("-|-");
            entAnt = campos[0];

        if ((i > 0) && (ent == "Mov") && (entAnt == "Vehi"))
            alto = rectfuenteInicial.height() + rectCabecera.height() + rectTitulo.height() + Espacio;
        else
            alto = rectfuenteInicial.height();
        }
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

int clsImprimirVehiculos::AltoEntradaFV(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    QRect textRectMarca = painter->boundingRect(0,
                                                0,
                                                AnchoCol2-EspacioCorto,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[1]);

    QRect textRectModelo = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol2-EspacioCorto,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 campos[2]);

    QRect textRectCentro = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol2-EspacioCorto,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 campos[3]);

    QRect textRectFMatri = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol1,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 QObject::tr("Fecha Matriculación"));

    return intAltoEntrada = 2*rectfuenteInicial.height() +
            qMax(textRectMarca.height(),rectfuenteInicial.height()) +
            qMax(textRectModelo.height(),rectfuenteInicial.height()) +
            qMax(textRectCentro.height(),rectfuenteInicial.height()) +
            qMax(textRectFMatri.height(),rectfuenteInicial.height());
}

void clsImprimirVehiculos::ImprimirImagen(QPainter *painter)
{
    painter->drawImage(rectImagen,*imagen);
}

void clsImprimirVehiculos::ImprimirPaginasFV(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraFV(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFV(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirVehiculos::ImprimirCabeceraFV(QPainter *painter)
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

void clsImprimirVehiculos::ImprimirCabeceraFVM(QPainter *painter)
{
    int AnchoObjeto = 0;

    yLinea = qMax(yLinea,rectImagen.y()+rectImagen.height());

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Movimientos del Vehículo"));

    yLinea += rectTitulo.height();

    painter->setFont(fuenteNegrita);

    int AnchoFMov = QFontMetrics(painter->font()).width(QObject::tr("Fecha Movimiento"));
    int AnchoKMFinales = QFontMetrics(painter->font()).width(QObject::tr("Kms Finales"));
    int AnchoCent = maxAnch - AnchoFMov - AnchoKMFinales - 2*Espacio;

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFMov,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Movimiento"));

    AnchoObjeto += AnchoFMov + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCent,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));

    AnchoObjeto += AnchoCent + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoKMFinales,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Kms Finales"));

    yLinea += rectCabecera.height();
}

void clsImprimirVehiculos::ImprimirPaginaFV(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    QStringList campos;
    QString ent="";

    for (int i = 0;i<entradas.count();i++)
    {
        campos = entradas.at(i).split("-|-");
        ent = campos[0];

        if ((i == 0) && (ent == "Vehi"))
            ImprimirLineaFV(painter, entradas.at(i));
        else
            ImprimirLineaFVM(painter, entradas.at(i));
    }

    painter->drawText(painter->window(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirVehiculos::ImprimirLineaFV(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");

    int AnchoObjeto = 0;

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    yLinea += 0;

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     QObject::tr("Matrícula"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     campos[0]);

    yLinea += rectfuenteNegrita.height();
    AnchoObjeto = 0;

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     QObject::tr("Marca"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     campos[1]);

    yLinea += rectfuenteNegrita.height();
    AnchoObjeto = 0;

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     QObject::tr("Modelo"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     campos[2]);

    yLinea += rectfuenteNegrita.height();
    AnchoObjeto = 0;

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     QObject::tr("Centro"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     campos[3]);

    yLinea += rectfuenteNegrita.height();
    AnchoObjeto = 0;

    QRect textRectFMatri = painter->boundingRect(0,
                                                 0,
                                                 AnchoCol1,
                                                 maxAltDispo,
                                                 Qt::TextWordWrap,
                                                 QObject::tr("Fecha Matriculación"));

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(textRectFMatri.height(),rectfuenteNegrita.height()));
    painter->drawRect(0,yLinea,AnchoCol1,qMax(textRectFMatri.height(),rectfuenteNegrita.height()));

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,qMax(textRectFMatri.height(),rectfuenteNegrita.height()),
                     Qt::AlignLeft|Qt::TextWordWrap,
                     QObject::tr("Fecha Matriculación"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,qMax(textRectFMatri.height(),rectfuenteNegrita.height()),
                     Qt::AlignJustify,
                     campos[4]);

    yLinea += qMax(textRectFMatri.height(),rectfuenteNegrita.height());
    AnchoObjeto = 0;

    painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
    painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

    painter->setFont(fuenteNegrita);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                     Qt::AlignJustify|Qt::TextWordWrap,
                     QObject::tr("Fecha ITV"));
    AnchoObjeto += AnchoCol1;
    painter->setFont(fuenteInicial);
    painter->drawText(AnchoObjeto+EspacioCorto/2,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                     Qt::AlignJustify,
                     campos[5]);
}

void clsImprimirVehiculos::ImprimirLineaFVM(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");

    int AnchoObjeto = 0;

    if(colorLinea == 0)
        ImprimirCabeceraFVM(painter);

    QPen penInicial = painter->pen();

    painter->setFont(fuenteNegrita);

    int AnchoFMov = QFontMetrics(painter->font()).width(QObject::tr("Fecha Movimiento"));
    int AnchoKMFinales = QFontMetrics(painter->font()).width(QObject::tr("Kms Finales"));
    int AnchoCent = maxAnch - AnchoFMov - AnchoKMFinales - 2*Espacio;

    painter->setFont(fuenteInicial);

    if(colorLinea%2!=0)
    {
        painter->setPen(Qt::lightGray);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(0,
                          yLinea,
                          maxAnch,
                          rectfuenteNegrita.height());
    }
    painter->setPen(penInicial);

    AnchoObjeto = 0;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFMov,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[1]);

    AnchoObjeto += AnchoFMov+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCent,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[2]);

    AnchoObjeto += AnchoCent+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoKMFinales,
                      rectfuenteNegrita.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);

    colorLinea++;

    yLinea += rectfuenteNegrita.height();
}
