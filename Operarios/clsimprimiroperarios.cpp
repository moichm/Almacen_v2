#include <QSqlQuery>
#include <QPrintDialog>
#include <QPainter>
#include <QPrinter>
#include <QDateTime>
#include <QVariant>
#include <QMessageBox>
#include "clsimprimiroperarios.h"
#include "Movimientos/movimientos.h"
#include "Centros/centro.h"

clsImprimirOperarios::clsImprimirOperarios(){}

void clsImprimirOperarios::ImprimirListaOperario(QString strFiltro)
{
    AnchoNIF = 0;

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

        clsOperarios Operario;
        QVector<QStringList> listaOperarios = Operario.ListaOperarios(strFiltro);

        for (int i=0;i<listaOperarios.size();i++)
        {
            QStringList strOperarios = listaOperarios.at(i);

            if (AnchoNIF < QFontMetrics(fuenteInicial).width(strOperarios.at(0)))
                AnchoNIF = QFontMetrics(fuenteInicial).width(strOperarios.at(0));

            entrada = strOperarios.at(0) + "-|-" + strOperarios.at(1) + "-|-" + strOperarios.at(2) + "-|-" + strOperarios.at(3) + "-|-" + strOperarios.at(7);

            listaEntradas << entrada;
        }

        maxAlt = painter.window().height();
        maxAnch = painter.window().width();
        Espacio = (maxAlt*3)/257;

        titulo = QObject::tr("Listado de Operario");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,2*QFontMetrics(painter.font()).height()+(maxAlt/257));

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        AnchoFecha = qMax(QFontMetrics(painter.font()).width(QObject::tr("dd/MM/yyyy")),QFontMetrics(painter.font()).width(QObject::tr("nacimiento")));
        AnchoCentro = (maxAnch - (AnchoNIF + AnchoFecha + 3*Espacio))/3;
        AnchoNomAp = 2*(maxAnch - (AnchoNIF + AnchoFecha + 3*Espacio))/3;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+(maxAlt/257));

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height()+rectCabecera.height()+2*Espacio);

        colorLinea=0;
        yLinea = 0;

        DividirEnPaginasLOp(&painter,&pages,listaEntradas);
        ImprimirPaginasLOp(&painter,pages);
    }
}

void clsImprimirOperarios::DividirEnPaginasLOp(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual;
    int y = 0;
    for(int i = 0;i<entradas.count();i++)
    {
        int alto = AltoEntradaLOp(painter, entradas.at(i));
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

void clsImprimirOperarios::ImprimirPaginasLOp(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabeceraLOp(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaLOp(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirOperarios::ImprimirPaginaLOp(QPainter *painter,const QStringList &entradas, int NumPagina)
{
    for (int i = 0;i<entradas.count();i++)
        ImprimirLineaLOp(painter, entradas.at(i));

    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirOperarios::ImprimirCabeceraLOp(QPainter *painter)
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
    yLinea += rectFecha.height() + Espacio;

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
    AnchoObjeto += AnchoNIF + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNomAp,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Nombre y Apellidos"));
    AnchoObjeto += AnchoNomAp + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFecha,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      QObject::tr("Fecha Nacimiento"));
    AnchoObjeto += AnchoFecha + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCentro,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));

    yLinea += rectCabecera.height();
}

int clsImprimirOperarios::AltoEntradaLOp(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");

    QRect textRectNomAp = painter->boundingRect(0,0,AnchoNomAp, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[1] + " " + campos[2]);
    QRect textRectCentro = painter->boundingRect(0,0,AnchoCentro, rectfuenteInicial.height(),Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,campos[4]);

    return qMax(textRectNomAp.height(),qMax(textRectCentro.height(),rectfuenteInicial.height()));
}

void clsImprimirOperarios::ImprimirLineaLOp(QPainter *painter, const QString &cadena)
{
    QPen penInicial = painter->pen();

    QStringList campos = cadena.split("-|-");
    int AltoObjeto=0,AnchoObjeto=0;

    AltoObjeto = AltoEntradaLOp(painter,cadena);

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
                      AnchoNIF,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter,
                      campos[0]);
    AnchoObjeto += AnchoNIF + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNomAp,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[1] + " " + campos[2]);
    AnchoObjeto += AnchoNomAp + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFecha,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[3]);
    AnchoObjeto += AnchoFecha + Espacio;
    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCentro,
                      AltoObjeto,
                      Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                      campos[4]);

    colorLinea++;

    yLinea += AltoObjeto;
}


void clsImprimirOperarios::ImprimirCabecera(QPainter *painter)
{
    painter->setFont(fuenteTitulo);
    QPen penInicial = painter->pen();
    painter->setBrush(brushInicial);

    painter->drawText(0,0,maxAnch,rectTitulo.height(),Qt::AlignHCenter|Qt::AlignVCenter,titulo);
    yLinea += rectTitulo.height();

    painter->setPen(penInicial);
    painter->setFont(fuenteInicial);

    QDateTime fecha;

    painter->drawText(0,rectTitulo.height(),maxAnch,rectFecha.height(),Qt::AlignLeft|Qt::AlignVCenter,fecha.currentDateTime().toString(QObject::tr("dd-MM-yyyy hh:mm")));
    yLinea += rectFecha.height() + Espacio;
}


void clsImprimirOperarios::ObtenerSalidas(const QString Op, QStringList &entradasSal)
{
    QString strFiltro = "responsable = '" + Op + "'";

    clsMovimiento_Salida Sal;
    QVector<QStringList> listaSalida = Sal.ListaSalida(strFiltro);

    for (int i=0;i<listaSalida.size();i++)
    {
        QStringList strSalida = listaSalida.at(i);

        QString entrada="";

        entrada = "Sal";
        entrada += "-|-" + strSalida.at(0);
        entrada += "-|-" + strSalida.at(2);
        entrada += "-|-" + strSalida.at(3);
        entrada += "-|-" + strSalida.at(5);

        entradasSal << entrada;
    }
}

void clsImprimirOperarios::ObtenerPartesRes(const QString Op, QStringList &entradasParteRes)
{
    QString strFiltro = "partes_trabajo.responsable = '" + Op + "'";

    clsPartes_Trabajo ParteRes;
    QVector<QStringList> listaParteRes = ParteRes.ListaPartes_Trabajo(strFiltro);

    for (int i=0;i<listaParteRes.size();i++)
    {
        QStringList strParteRes = listaParteRes.at(i);

        QString entrada="";

        entrada = "ParRes";
        entrada += "-|-" + strParteRes.at(0);
        entrada += "-|-" + strParteRes.at(1);
        entrada += "-|-" + strParteRes.at(3);
        entrada += "-|-" + strParteRes.at(4);

        entradasParteRes << entrada;
    }
}

void clsImprimirOperarios::ObtenerPartesLin(const QString Op, QStringList &entradasParteLin)
{
    QString strConsulta = "SELECT DISTINCT(partes_trabajo.idParte),partes_trabajo.fecha_parte,partes_trabajo.descripcion,concat_ws(' - ',centros.nombre,centros.ciudad) as centro "
            "FROM partes_trabajo INNER JOIN linea_partes ON linea_partes.idParte = partes_trabajo.idParte INNER JOIN centros ON centros.idCentro = partes_trabajo.idCentro "
            "WHERE linea_partes.idOperario = '" + Op + "' "
            "ORDER BY partes_trabajo.idParte;";

    QSqlQuery consulta;

    QString entrada="";

    if (consulta.exec(strConsulta))
    {
        while(consulta.next())
        {
            entrada = "ParLin";
            entrada += "-|-" + consulta.value(0).toString();
            entrada += "-|-" + consulta.value(1).toDate().toString(QObject::tr("dd/MM/yyyy"));
            entrada += "-|-" + consulta.value(2).toString();
            entrada += "-|-" + consulta.value(3).toString();

            entradasParteLin << entrada;
        }
    }
}

void clsImprimirOperarios::ImprimirFichaOp(const clsOperarios *Op)
{
    printer = new QPrinter(QPrinter::PrinterResolution);
    printer->setPageSize(QPrinter::A4);

    QPrintDialog printDialog(printer);
    if (printDialog.exec())
    {
        printer->setPageMargins(20,20,15,20,QPrinter::Millimeter);

        clsCentros Centro;
        Centro.Cargar(Op->Centro());

        QStringList listaEntradas;
        QString entrada;
        entrada = "Op";
        entrada += "-|-" + Op->NIF();
        entrada += "-|-" + Op->Nombre();
        entrada += "-|-" + Op->Apellidos();
        entrada += "-|-" + Op->FechaNacimiento().toString(QObject::tr("dd/MM/yyyy"));
        entrada += "-|-" + Op->Direccion();
        entrada += "-|-" + Op->Email();
        entrada += "-|-" + Op->CCC();
        entrada += "-|-" + Centro.Nombre() + " - " + Centro.Ciudad();

        imagen = new QImage(Op->Imagen());
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

        titulo = QObject::tr("Ficha de Operario");

        rectTitulo = QRect(0,0,maxAnch,(maxAlt*15)/257);
        rectFecha = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectPie = QRect(0,0,maxAnch,(maxAlt*5)/257);
        rectCabecera = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);        
        rectImagen = QRect(maxAnch - ((maxAnch*50)/175),Espacio + rectTitulo.height() + rectFecha.height(),(maxAnch*50)/175,(maxAlt*65)/257);

        painter.setFont(fuenteNegrita);

        rectfuenteNegrita = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        AnchoCol1 = qMax(QFontMetrics(painter.font()).width(QObject::tr("Correo electrónico")),QFontMetrics(painter.font()).width(QObject::tr("Fecha Nacimiento")))+EspacioCorto;

        AnchoCol2 = maxAnch - (rectImagen.width() + Espacio + AnchoCol1) + EspacioCorto;

        painter.setFont(fuenteInicial);

        rectfuenteInicial = QRect(0,0,maxAnch,QFontMetrics(painter.font()).height()+EspacioCorto);

        maxAltDispo = maxAlt - (rectTitulo.height()+rectFecha.height()+rectPie.height() + 2*Espacio);

        colorLinea = 0;

        ObtenerSalidas(Op->NIF(),listaEntradas);
        ObtenerPartesRes(Op->NIF(),listaEntradas);
        ObtenerPartesLin(Op->NIF(),listaEntradas);
        DividirEnPaginasFOp(&painter,&pages,listaEntradas);
        ImprimirPaginasFOp(&painter,pages);
    }
}

void clsImprimirOperarios::DividirEnPaginasFOp(QPainter *painter, QList<QStringList> *paginas,const QStringList &entradas)
{
    QStringList PaginaActual,campos;
    int y = 0, alto = 0;
    QString entAnt="",ent="";

    for(int i = 0;i<entradas.count();i++)
    {
        if (i == 0)
            alto = qMax(AltoEntradaFOp(painter, entradas.at(i))+Espacio,rectImagen.height()+Espacio);
        else
        {
            campos = entradas.at(i).split("-|-");
            ent = campos[0];

            campos = entradas.at(i-1).split("-|-");
            entAnt = campos[0];

            if((i > 0) && (entAnt == "Op") && (ent == "Sal"))
                alto = AltoEntradaFOpSal(painter, entradas.at(i)) + rectCabecera.height() + rectTitulo.height() + Espacio;
            else if(((i > 0) && ((entAnt == "Op") || (entAnt == "Sal")) && (ent == "ParRes")) || ((i > 0) && ((entAnt == "Op") || (entAnt == "Sal") || (entAnt == "ParRes")) && (ent == "ParLin")))
                alto = AltoEntradaFOpPartes(painter, entradas.at(i)) + rectCabecera.height() + rectTitulo.height() + Espacio;
            else if ((i > 0) && (ent == "Sal"))
                alto = AltoEntradaFOpSal(painter, entradas.at(i));
            else if((i > 0) && (ent == "ParRes"))
                alto = AltoEntradaFOpPartes(painter, entradas.at(i));
            else if((i > 0) && (ent == "ParLin"))
                alto = AltoEntradaFOpPartes(painter, entradas.at(i));
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

int clsImprimirOperarios::AltoEntradaFOp(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    QRect textRectNom = painter->boundingRect(0,
                                              0,
                                              AnchoCol2-EspacioCorto,
                                              maxAltDispo,
                                              Qt::TextWordWrap,
                                              campos[2]);

    QRect textRectAp = painter->boundingRect(0,
                                             0,
                                             AnchoCol2-EspacioCorto,
                                             maxAltDispo,
                                             Qt::TextWordWrap,
                                             campos[3]);

    QRect textRectDirec = painter->boundingRect(0,
                                                0,
                                                AnchoCol2-EspacioCorto,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[5]);

    return intAltoEntrada = 5*rectfuenteInicial.height() +
            qMax(textRectNom.height(),rectfuenteInicial.height()) +
            qMax(textRectAp.height(),rectfuenteInicial.height()) +
            qMax(textRectDirec.height(),rectfuenteInicial.height());
}

int clsImprimirOperarios::AltoEntradaFOpSal(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    painter->setFont(fuenteNegrita);

    int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
    int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
    int AnchoCol = maxAnch - AnchoNSal - AnchoFSal - 3*Espacio;

    painter->setFont(fuenteInicial);

    QRect textRectProy = painter->boundingRect(0,
                                               0,
                                               AnchoCol/2,
                                               maxAltDispo,
                                               Qt::TextWordWrap,
                                               campos[2]);
    QRect textRectTarea = painter->boundingRect(0,
                                                0,
                                                AnchoCol/2,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[3]);

    return intAltoEntrada = qMax(textRectProy.height(),qMax(textRectTarea.height(),rectfuenteInicial.height()));
}

int clsImprimirOperarios::AltoEntradaFOpPartes(QPainter *painter, const QString &entrada)
{
    QStringList campos = entrada.split("-|-");
    int intAltoEntrada = 0;

    QRect textRectDesc = painter->boundingRect(0,
                                               0,
                                               (maxAnch - 3*Espacio - QFontMetrics(painter->font()).width(QObject::tr("Nº Parte")) - QFontMetrics(painter->font()).width(QObject::tr("Fecha Parte")))/2,
                                               maxAltDispo,
                                               Qt::TextWordWrap,
                                               campos[3]);
    QRect textRectNom = painter->boundingRect(0,
                                                0,
                                                (maxAnch - 3*Espacio - QFontMetrics(painter->font()).width(QObject::tr("Nº Parte")) - QFontMetrics(painter->font()).width(QObject::tr("Fecha Parte")))/2,
                                                maxAltDispo,
                                                Qt::TextWordWrap,
                                                campos[4]);

    return intAltoEntrada = qMax(textRectDesc.height(),qMax(textRectNom.height(),rectfuenteInicial.height()));
}

void clsImprimirOperarios::ImprimirPaginasFOp(QPainter *painter,const QList<QStringList> &paginas)
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
            ImprimirCabecera(painter);
            int indice;
            if (printer->pageOrder() == QPrinter::FirstPageFirst)
                indice = primeraPagina + j;
            else
                indice = primeraPagina - j;
            ImprimirPaginaFOp(painter, paginas[indice], indice + 1);
        }
}

void clsImprimirOperarios::ImprimirImagen(QPainter *painter)
{
    painter->drawImage(rectImagen,*imagen);
}

void clsImprimirOperarios::ImprimirCabeceraFOpS(QPainter *painter)
{
    painter->setBrush(brushInicial);

    int AnchoObjeto = 0;

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Salidas del Operario"));

    yLinea += rectTitulo.height();

    painter->setFont(fuenteNegrita);

    int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
    int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
    int AnchoCol = maxAnch - AnchoNSal - AnchoFSal - 3*Espacio;

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNSal,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Salida"));

    AnchoObjeto += AnchoNSal + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/2,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Proyecto"));

    AnchoObjeto += AnchoCol/2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/2,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Tarea"));

    AnchoObjeto += AnchoCol/2 + Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFSal,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Salida"));

    yLinea += rectCabecera.height();
}

void clsImprimirOperarios::ImprimirCabeceraFOpPRes(QPainter *painter)
{
    painter->setBrush(brushInicial);

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Partes del Operario como Responsable"));

    yLinea += rectTitulo.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    ImprimirCabecera(painter);
}

void clsImprimirOperarios::ImprimirCabeceraFOpPLin(QPainter *painter)
{
    painter->setBrush(brushInicial);

    yLinea += Espacio;

    painter->setFont(fuenteMedTitulo);

    painter->drawText(0,yLinea,maxAnch,rectTitulo.height(),Qt::AlignCenter,QObject::tr("Partes en los que está incluido el Operario"));

    yLinea += rectTitulo.height();

    painter->setFont(fuenteNegrita);

    painter->drawRect(0,
                      yLinea,
                      maxAnch,
                      rectCabecera.height());

    ImprimirCabecera(painter);
}

void clsImprimirOperarios::ImprimirCabeceraFOpP(QPainter *painter)
{
    int AnchoObjeto = 0;

    painter->setFont(fuenteNegrita);
    painter->setBrush(brushInicial);

    int AnchoNPar = QFontMetrics(painter->font()).width(QObject::tr("Nº Parte"));
    int AnchoFPar = QFontMetrics(painter->font()).width(QObject::tr("Fecha Parte"));
    int AnchoCol = maxAnch - AnchoNPar - AnchoFPar - 3*Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoNPar,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Nº Parte"));

    AnchoObjeto += AnchoNPar+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoFPar,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Fecha Parte"));

    AnchoObjeto += AnchoFPar+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/2,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Descripción"));

    AnchoObjeto += AnchoCol/2+Espacio;

    painter->drawText(AnchoObjeto,
                      yLinea,
                      AnchoCol/2,
                      rectCabecera.height(),
                      Qt::AlignLeft|Qt::AlignVCenter,
                      QObject::tr("Centro"));

    yLinea += rectCabecera.height();
}

void clsImprimirOperarios::ImprimirPaginaFOp(QPainter *painter,const QStringList &entradas, int NumPagina)
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

            if ((entAnt == "Op") && (ent == "Sal"))
                colorLinea = 0;
            else if(((entAnt == "Op") || (entAnt == "Sal")) && (ent == "ParRes"))
                colorLinea = 0;
            else if(((entAnt == "Op") || (entAnt == "Sal") || (entAnt == "ParRes")) && (ent == "ParLin"))
                colorLinea = 0;
        }
        else
            colorLinea = 0;

        ImprimirLineaFOp(painter, entradas.at(i));
    }
    painter->drawText(0,
                      maxAlt-rectPie.height(),
                      rectPie.width(),
                      rectPie.height(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(NumPagina) + "/" + QString::number(numPaginas));
}

void clsImprimirOperarios::ImprimirLineaFOp(QPainter *painter, const QString &cadena)
{
    QStringList campos = cadena.split("-|-");

    int AnchoObjeto = EspacioCorto/2;

    if (campos[0] == "Op")
    {
        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("NIF"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[1]);

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = EspacioCorto/2;

        QRect textRectNom = painter->boundingRect(0,0,AnchoCol2-EspacioCorto,maxAltDispo,Qt::TextWordWrap,campos[2]);

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectNom.height()),
                          Qt::AlignJustify,
                          QObject::tr("Nombre"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,qMax(rectfuenteNegrita.height(),textRectNom.height()),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[2]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(rectfuenteNegrita.height(),textRectNom.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectNom.height()));

        yLinea += qMax(rectfuenteNegrita.height(),textRectNom.height());
        AnchoObjeto = EspacioCorto/2;

        QRect textRectAp = painter->boundingRect(0,0,AnchoCol2 - EspacioCorto, maxAltDispo,Qt::TextWordWrap,campos[3]);

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectAp.height()),
                          Qt::AlignJustify,
                          QObject::tr("Apellidos"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,textRectAp.height(),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[3]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(rectfuenteNegrita.height(),textRectAp.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectAp.height()));

        yLinea += qMax(rectfuenteNegrita.height(),textRectAp.height());
        AnchoObjeto = EspacioCorto/2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,QFontMetrics(painter->font()).height(),
                          Qt::AlignJustify,
                          QObject::tr("Fecha Nacimiento"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[4]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = EspacioCorto/2;

        QRect textRectDirec = painter->boundingRect(0,0,AnchoCol2-EspacioCorto,maxAltDispo,Qt::TextWordWrap,campos[5]);

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,QFontMetrics(painter->font()).height(),
                          Qt::AlignJustify,
                          QObject::tr("Dirección"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[5]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,qMax(rectfuenteNegrita.height(),textRectDirec.height()));
        painter->drawRect(0,yLinea,AnchoCol1,qMax(rectfuenteNegrita.height(),textRectDirec.height()));

        yLinea += qMax(rectfuenteNegrita.height(),textRectDirec.height());
        AnchoObjeto = EspacioCorto/2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,QFontMetrics(painter->font()).height(),
                          Qt::AlignJustify,
                          QObject::tr("Correo electrónico"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify|Qt::TextWordWrap,
                          campos[6]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = EspacioCorto/2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          QObject::tr("Nº CCC"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[7]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        yLinea += rectfuenteNegrita.height();
        AnchoObjeto = EspacioCorto/2;

        painter->setFont(fuenteNegrita);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol1,rectfuenteNegrita.height(),
                          Qt::AlignLeft|Qt::TextWordWrap,
                          QObject::tr("Centro"));
        AnchoObjeto += AnchoCol1;
        painter->setFont(fuenteInicial);
        painter->drawText(AnchoObjeto,yLinea,AnchoCol2-EspacioCorto,rectfuenteNegrita.height(),
                          Qt::AlignJustify,
                          campos[8]);

        painter->drawRect(AnchoCol1,yLinea,AnchoCol2,rectfuenteNegrita.height());
        painter->drawRect(0,yLinea,AnchoCol1,rectfuenteNegrita.height());

        yLinea = qMax(yLinea,rectImagen.y()+rectImagen.height());
    }
    else if(campos[0] == "Sal")
    {
        if(colorLinea == 0)
            ImprimirCabeceraFOpS(painter);

        QPen penInicial = painter->pen();

        painter->setFont(fuenteNegrita);

        int AnchoNSal = QFontMetrics(painter->font()).width(QObject::tr("Nº Salida"));
        int AnchoFSal = QFontMetrics(painter->font()).width(QObject::tr("Fecha Salida"));
        int AnchoCol = maxAnch - AnchoNSal - AnchoFSal - 3*Espacio;

        painter->setFont(fuenteInicial);

        QRect textRectProy = painter->boundingRect(0,
                                                   0,
                                                   AnchoCol/2,
                                                   maxAltDispo,
                                                   Qt::TextWordWrap,
                                                   campos[2]);
        QRect textRectTarea = painter->boundingRect(0,
                                                    0,
                                                    AnchoCol/2,
                                                    maxAltDispo,
                                                    Qt::TextWordWrap,
                                                    campos[3]);

        int AltoObjeto = qMax(textRectProy.height(),qMax(textRectTarea.height(),rectfuenteInicial.height()));

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
                          AnchoCol/2,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[2]);

        AnchoObjeto += AnchoCol/2+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/2,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[3]);

        AnchoObjeto += AnchoCol/2+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoFSal,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[4]);

        colorLinea++;

        yLinea += AltoObjeto;
    }
    else if((campos[0] == "ParRes") || (campos[0] == "ParLin"))
    {
        if((colorLinea == 0) && (campos[0] == "ParRes"))
            ImprimirCabeceraFOpPRes(painter);
        else if((colorLinea == 0) && (campos[0] == "ParLin"))
            ImprimirCabeceraFOpPLin(painter);

        QPen penInicial = painter->pen();

        painter->setFont(fuenteNegrita);

        int AnchoNPar = QFontMetrics(painter->font()).width(QObject::tr("Nº Parte"));
        int AnchoFPar = QFontMetrics(painter->font()).width(QObject::tr("Fecha Parte"));
        int AnchoCol = maxAnch - AnchoNPar - AnchoFPar - 3*Espacio;

        int AltoObjeto = AltoEntradaFOpPartes(painter,cadena);

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
                          AnchoNPar,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter,
                          campos[1]);

        AnchoObjeto += AnchoNPar+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoFPar,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[2]);

        AnchoObjeto += AnchoFPar+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/2,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[3]);

        AnchoObjeto += AnchoCol/2+Espacio;

        painter->drawText(AnchoObjeto,
                          yLinea,
                          AnchoCol/2,
                          AltoObjeto,
                          Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWordWrap,
                          campos[4]);

        colorLinea++;

        yLinea += AltoObjeto;
    }
}
