#include "movimientoslistas.h"
#include "ui_movimientoslistas.h"
#include "movimientossalidas.h"
#include "movimientosentrada.h"
#include "movimientospedido.h"
#include "movimientosenvio.h"
#include "movimientosrecepcion.h"
#include "movimientosstock.h"
#include <QMessageBox>
#include "clsconfiguracion.h"
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosListas::MovimientosListas(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MovimientosListas)
{    
    ui->setupUi(this);

    ui->tabMovimientos->setCurrentIndex(0);
    CargarLineas();

    ui->cbFiltro_Ent->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Ent->addItem(QObject::tr("Nº Salida"));
    ui->cbFiltro_Ent->addItem(QObject::tr("Proyecto"));
    ui->cbFiltro_Ent->addItem(QObject::tr("Tarea"));
    ui->cbFiltro_Ent->addItem(QObject::tr("Operario"));
    if(config->Permisos().value(Confi))
        ui->cbFiltro_Ent->addItem(QObject::tr("Centro"));
    ui->cbFiltro_Ent->setCurrentIndex(0);
    ui->edtFiltro_Ent->setEnabled(false);

    ui->cbFiltro_Env->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Env->addItem(QObject::tr("Nº Envío"));
    ui->cbFiltro_Env->addItem(QObject::tr("Centro Destino"));
    ui->cbFiltro_Env->addItem(QObject::tr("Fecha envío"));
    if(config->Permisos().value(Confi))
        ui->cbFiltro_Env->addItem(QObject::tr("Centro Origen"));
    ui->cbFiltro_Env->setCurrentIndex(0);
    ui->edtFiltro_Env->setEnabled(false);

    ui->cbFiltro_Ped->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Ped->addItem(QObject::tr("Nº Pedido"));
    ui->cbFiltro_Ped->addItem(QObject::tr("Centro"));
    ui->cbFiltro_Ped->setCurrentIndex(0);
    ui->edtFiltro_Ped->setEnabled(false);

    ui->cbFiltro_Rec->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Rec->addItem(QObject::tr("Nº Recepción"));
    ui->cbFiltro_Rec->addItem(QObject::tr("Centro Origen"));
    ui->cbFiltro_Rec->addItem(QObject::tr("Fecha envío"));
    if(config->Permisos().value(Confi))
        ui->cbFiltro_Rec->addItem(QObject::tr("Centro Destino"));
    ui->cbFiltro_Rec->setCurrentIndex(0);
    ui->edtFiltro_Rec->setEnabled(false);

    ui->cbFiltro_Sal->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Sal->addItem(QObject::tr("Nº Salida"));
    ui->cbFiltro_Sal->addItem(QObject::tr("Proyecto"));
    ui->cbFiltro_Sal->addItem(QObject::tr("Tarea"));
    ui->cbFiltro_Sal->addItem(QObject::tr("Operario"));
    if(config->Permisos().value(Confi))
        ui->cbFiltro_Sal->addItem(QObject::tr("Centro"));
    ui->cbFiltro_Sal->setCurrentIndex(0);
    ui->edtFiltro_Sal->setEnabled(false);

    ui->cbFiltro_Stock->addItem(QObject::tr("Todos"));
    ui->cbFiltro_Stock->addItem(QObject::tr("Producto"));
    ui->cbFiltro_Stock->addItem(QObject::tr("Descripción"));
    ui->cbFiltro_Stock->addItem(QObject::tr("Origen"));
    ui->cbFiltro_Stock->addItem(QObject::tr("Comentario"));
    ui->cbFiltro_Stock->addItem(QObject::tr("Cantidad"));
    if(config->Permisos().value(Confi))
        ui->cbFiltro_Stock->addItem(QObject::tr("Centro"));
    ui->cbFiltro_Stock->setCurrentIndex(0);
    ui->edtFiltro_Stock->setEnabled(false);

    connect(ui->edtFiltro_Ent,SIGNAL(textChanged(QString)),this,SLOT(FiltrarEntradas()));
    connect(ui->cbFiltro_Ent,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroEntradas()));
    connect(ui->btnSalir_Ent,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnFicha_Ent,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaEnt()));
    connect(ui->tableEntradas,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaEnt()));
    connect(ui->rbtnPorDevolver,SIGNAL(clicked()),this,SLOT(FiltrarEntradas()));
    connect(ui->rbtnTodas,SIGNAL(clicked()),this,SLOT(FiltrarEntradas()));
    connect(ui->btnImprimir_Ent,SIGNAL(clicked()),this,SLOT(ImprimirEnt()));

    connect(ui->edtFiltro_Env,SIGNAL(textChanged(QString)),this,SLOT(FiltrarEnvios()));
    connect(ui->cbFiltro_Env,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroEnvios()));
    connect(ui->btnSalir_Env,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnEliminar_Env,SIGNAL(clicked()),this,SLOT(EliminarEnvio()));
    connect(ui->btnNueva_Env,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevaEnvio()));
    connect(ui->btnModificar_Env,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModEnvio()));
    connect(ui->tableEnvios,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModEnvio()));
    connect(ui->btnImprimir_Env,SIGNAL(clicked()),this,SLOT(ImprimirEnv()));

    connect(ui->btnNueva_Ped,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevaPedido()));
    connect(ui->btnEliminar_Ped,SIGNAL(clicked()),this,SLOT(EliminarPedido()));
    connect(ui->btnModificar_Ped,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModPedido()));
    connect(ui->btnSalir_Ped,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->tablePedidos,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModPedido()));
    connect(ui->edtFiltro_Ped,SIGNAL(textChanged(QString)),this,SLOT(FiltrarPedidos()));
    connect(ui->cbFiltro_Ped,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroPedidos()));
    connect(ui->btnImprimir_Ped,SIGNAL(clicked()),this,SLOT(ImprimirPedidos()));

    connect(ui->edtFiltro_Rec,SIGNAL(textChanged(QString)),this,SLOT(FiltrarRecepciones()));
    connect(ui->cbFiltro_Rec,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroRecepciones()));
    connect(ui->btnSalir_Rec,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnFicha_Rec,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaRec()));
    connect(ui->tableRecepcion,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaRec()));
    connect(ui->btnImprimir_Rec,SIGNAL(clicked()),this,SLOT(ImprimirRec()));

    connect(ui->edtFiltro_Sal,SIGNAL(textChanged(QString)),this,SLOT(FiltrarSalidas()));
    connect(ui->cbFiltro_Sal,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroSalidas()));
    connect(ui->btnSalir_Sal,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnNueva_Sal,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevaSal()));
    connect(ui->btnModificar_Sal,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModSal()));
    connect(ui->btnEliminar_Sal,SIGNAL(clicked()),this,SLOT(EliminarSalida()));
    connect(ui->tableSalidas,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModSal()));
    connect(ui->btnImprimir_Sal,SIGNAL(clicked()),this,SLOT(ImprimirSal()));

    connect(ui->btnNueva_Stock,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevoStock()));
    connect(ui->tableStockProd,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModStock()));
    connect(ui->cbFiltro_Stock,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroStock()));
    connect(ui->edtFiltro_Stock,SIGNAL(textChanged(QString)),this,SLOT(FiltrarStock()));
    connect(ui->btnSalir_Stock,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnImprimir_Stock,SIGNAL(clicked()),this,SLOT(ImprimirStock()));
    connect(ui->btnFicha,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModStock()));

    connect(ui->tabMovimientos,SIGNAL(currentChanged(int)),this,SLOT(CargarLineas()));

    DeshabilitarControles();
}

MovimientosListas::~MovimientosListas()
{
    delete ui;
}

void MovimientosListas::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MovimientosListas::CargarLineas()
{
    if(ui->tabMovimientos->currentIndex() == 0)
    {
        CargarLineasSalidas();
        this->setWindowTitle(QObject::tr("Salidas"));
    }
    else if(ui->tabMovimientos->currentIndex() == 1)
    {
        CargarLineasEntradas();
        this->setWindowTitle(QObject::tr("Entradas"));
    }
    else if(ui->tabMovimientos->currentIndex() == 2)
    {
        CargarLineasPedidos();
        this->setWindowTitle(QObject::tr("Pedidos Internos"));
    }
    else if(ui->tabMovimientos->currentIndex() == 3)
    {
        CargarLineasEnvios();
        this->setWindowTitle(QObject::tr("Envios"));
    }
    else if(ui->tabMovimientos->currentIndex() == 4)
    {
        CargarLineasRecepciones();
        this->setWindowTitle(QObject::tr("Recepciones"));
    }
    else if(ui->tabMovimientos->currentIndex() == 5)
    {
        CargarLineasStock();
        this->setWindowTitle(QObject::tr("Movimientos de Stock"));
    }
    else
    {
        ui->tableEntradas->setRowCount(0);
        ui->tableEnvios->setRowCount(0);
        ui->tablePedidos->setRowCount(0);
        ui->tableRecepcion->setRowCount(0);
        ui->tableSalidas->setRowCount(0);
        ui->tableStockProd->setRowCount(0);
    }
}

void MovimientosListas::AbrirVentanaFichaNuevaSal()
{
    MovimientosSalidas *ventanaFicha = new MovimientosSalidas(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineasSalidas();
}

void MovimientosListas::AbrirVentanaFichaModSal()
{
    int Salida = 0;
    QModelIndex indice = ui->tableSalidas->currentIndex();
    if (indice.isValid())
    {
        Salida = ui->tableSalidas->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        MovimientosSalidas *ventanaFicha = new MovimientosSalidas(Salida);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineasSalidas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar salida"),QObject::tr("Debe seleccionar una salida a modificar"),0,0);
}

void MovimientosListas::AbrirVentanaFichaEnt()
{
    int Salida = 0;
    QModelIndex indice = ui->tableEntradas->currentIndex();
    if (indice.isValid())
    {
        Salida = ui->tableEntradas->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        MovimientosEntrada *ventanaFicha = new MovimientosEntrada(Salida);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineasEntradas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Ficha salida"),QObject::tr("Debe seleccionar una salida a consultar."),0,0);
}

void MovimientosListas::AbrirVentanaFichaNuevaPedido()
{
    MovimientosPedido *ventanaFicha = new MovimientosPedido(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineasPedidos();
}

void MovimientosListas::AbrirVentanaFichaModPedido()
{
    int Pedido = 0;
    QModelIndex indice = ui->tablePedidos->currentIndex();
    if (indice.isValid())
    {
        Pedido = ui->tablePedidos->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        MovimientosPedido *ventanaFicha = new MovimientosPedido(Pedido);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineasPedidos();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar pedido"),QObject::tr("Debe seleccionar un pedido a modificar"),0,0);
}

void MovimientosListas::AbrirVentanaFichaRec()
{
    int Envio = 0;
    QModelIndex indice = ui->tableRecepcion->currentIndex();
    if (indice.isValid())
    {
        Envio = ui->tableRecepcion->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        MovimientosRecepcion *ventanaFicha = new MovimientosRecepcion(Envio);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineasRecepciones();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar envío"),QObject::tr("Debe seleccionar un envío a modificar"),0,0);
}

void MovimientosListas::AbrirVentanaFichaNuevaEnvio()
{
    MovimientosEnvio *ventanaFicha = new MovimientosEnvio(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineasEnvios();
}

void MovimientosListas::AbrirVentanaFichaModEnvio()
{
    int Envio = 0;
    QModelIndex indice = ui->tableEnvios->currentIndex();
    if (indice.isValid())
    {
        Envio = ui->tableEnvios->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        MovimientosEnvio *ventanaFicha = new MovimientosEnvio(Envio);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineasEnvios();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar envío"),QObject::tr("Debe seleccionar un envío a modificar"),0,0);
}

void MovimientosListas::AbrirVentanaFichaNuevoStock()
{
    MovimientosStock *ventanaStock = new MovimientosStock();
    ventanaStock->setWindowModality(Qt::ApplicationModal);
    ventanaStock->show();
    while (ventanaStock->exec() == QDialog::Rejected)
        ;
    CargarLineasStock();
}

void MovimientosListas::AbrirVentanaFichaModStock()
{
    QModelIndex indice = ui->tableStockProd->currentIndex();
    if (indice.isValid())
    {
        QString Prod = ui->tableStockProd->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        QDate datStock;
        QStringList strFechaStock = ui->tableStockProd->item(indice.row(),3)->data(Qt::DisplayRole).toString().split("/");
        if(strFechaStock.at(2).length() == 4)
            datStock = QDate::fromString(strFechaStock.at(0)+strFechaStock.at(1)+strFechaStock.at(2),"ddMMyyyy");
        else
            datStock = QDate::fromString(strFechaStock.at(2)+strFechaStock.at(1)+strFechaStock.at(0),"yyyyMMdd");

        int intCentro = config->IdCentroActual();
        tOrigen Origen;
        if(ui->tableStockProd->item(indice.row(),2)->data(Qt::DisplayRole).toString() == QObject::tr("Entrada"))
            Origen = Entrada;
        else
            Origen = Salida;

        clsMovimiento_Stock Stock(Prod,datStock,intCentro,Origen);

        MovimientosStock *ventanaFicha = new MovimientosStock(Stock);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineasStock();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar stock"),QObject::tr("Debe seleccionar una línea"),0,0);
}

void MovimientosListas::ImprimirSal()
{
    QString strFiltro = "";

    if (ui->cbFiltro_Sal->currentIndex() == 1)
        strFiltro += "CAST(salidas.idsalida AS varchar(10)) LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 2)
        strFiltro += "salidas.proyecto LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 3)
        strFiltro += "salidas.tarea LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 4)
        strFiltro += "concat_ws(' ',operarios.nombre,operarios.apellidos) LIKE '%" + ui->edtFiltro_Sal->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaSalidasEntradas(QObject::tr("Salida"),strFiltro);
}

void MovimientosListas::ImprimirEnt()
{    
    QString strFiltro = "";
    bool bPorDevolver;

    if(ui->rbtnPorDevolver->isChecked())
        bPorDevolver = true;
    else
        bPorDevolver = false;

    if (ui->cbFiltro_Ent->currentIndex() == 1)
        strFiltro += "CAST(salidas.idsalida AS varchar(10)) LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 2)
        strFiltro += "salidas.proyecto LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 3)
        strFiltro += "salidas.tarea LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 4)
        strFiltro += "concat_ws(' ',operarios.nombre,operarios.apellidos) LIKE '%" + ui->edtFiltro_Ent->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaSalidasEntradas(QObject::tr("Entrada"),strFiltro,bPorDevolver);
}

void MovimientosListas::ImprimirRec()
{
    QString strFiltro = "";

    if (ui->cbFiltro_Rec->currentIndex() == 1)
        strFiltro += "CAST(envios.idenvio AS varchar(10)) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 2)
        strFiltro += "concat_ws(' - ',centO.nombre,centO.ciudad) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 3)
        strFiltro += "CAST(envios.fecha_creacion AS varchar(10)) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaEnviosRecepciones(QObject::tr("Recepción"),strFiltro);
}

void MovimientosListas::ImprimirStock()
{
    QString strFiltro = "";

    if (ui->cbFiltro_Stock->currentIndex() == 1)
        strFiltro += "stock.idproducto LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 2)
        strFiltro += "productos.descripcion LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 3)
        strFiltro += "CAST(stock.origen AS varchar(10)) LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 4)
        strFiltro += "stock.comentario LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 5)
        strFiltro += "CAST(stock.cantidad AS varchar(10)) LIKE '%" + ui->edtFiltro_Stock->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaStock(strFiltro);
}

void MovimientosListas::ImprimirPedidos()
{
    QString strFiltro = "";

    if (ui->cbFiltro_Ped->currentIndex() == 1)
        strFiltro += "CAST(pedidos_internos.idPedido AS varchar(20)) LIKE '%" + ui->edtFiltro_Ped->text() + "%'";
    else if (ui->cbFiltro_Ped->currentIndex() == 2)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltro_Ped->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaPedidos(strFiltro);
}

void MovimientosListas::ImprimirEnv()
{
    QString strFiltro = "";

    if (ui->cbFiltro_Env->currentIndex() == 1)
        strFiltro += "CAST(envios.idenvio AS varchar(20)) LIKE '%" + ui->edtFiltro_Env->text() + "%'";
    else if (ui->cbFiltro_Env->currentIndex() == 2)
        strFiltro += "concat_ws(' - ',centD.nombre,centD.ciudad) LIKE '%" + ui->edtFiltro_Env->text() + "%'";
    else if (ui->cbFiltro_Env->currentIndex() == 3)
        strFiltro += "CAST(envios.fecha_creacion AS varchar(10)) LIKE '%" + ui->edtFiltro_Env->text() + "%'";

    clsImprimirMovimientos Inf;
    Inf.ImprimirListaEnviosRecepciones(QObject::tr("Envío"),strFiltro);
}

void MovimientosListas::CargarLineasSalidas()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableSalidas->clear();
    ui->tableSalidas->setRowCount(0);
    ui->tableSalidas->setColumnCount(5);
    ui->tableSalidas->setColumnHidden(4,!permiso);
    ui->tableSalidas->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Salida") << QObject::tr("Proyecto") << QObject::tr("Tarea") << QObject::tr("Operario") << QObject::tr("Centro"));
    ui->tableSalidas->resizeColumnsToContents();
    ui->tableSalidas->resizeRowsToContents();
    ui->tableSalidas->sortByColumn(0);

    clsMovimiento_Salida Sal;
    QVector<QStringList> listaSalida = Sal.ListaSalida();

    ui->tableSalidas->setRowCount(0);

    for (int i=0;i<listaSalida.size();i++)
    {
        QStringList strSalida = listaSalida.at(i);

        ui->tableSalidas->insertRow(ui->tableSalidas->rowCount());

        QTableWidgetItem *idSal = new QTableWidgetItem;
        idSal->setData(Qt::DisplayRole,strSalida.at(0));
        QTableWidgetItem *Proy = new QTableWidgetItem;
        Proy->setData(Qt::DisplayRole,strSalida.at(2));
        QTableWidgetItem *Tarea = new QTableWidgetItem;
        Tarea->setData(Qt::DisplayRole,strSalida.at(3));
        QTableWidgetItem *NomApOp = new QTableWidgetItem;
        NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strSalida.at(1));

        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,0,idSal);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,1,Proy);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,2,Tarea);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,3,NomApOp);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,4,Cent);
    }
    ui->tableSalidas->resizeColumnsToContents();
    ui->tableSalidas->resizeRowsToContents();
}

void MovimientosListas::CargarLineasEntradas()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableEntradas->clear();
    ui->tableEntradas->setRowCount(0);
    ui->tableEntradas->setColumnCount(5);
    ui->tableEntradas->setColumnHidden(4,!permiso);
    ui->tableEntradas->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Salida") << QObject::tr("Proyecto") << QObject::tr("Tarea") << QObject::tr("Operario") << QObject::tr("Centro"));
    ui->tableEntradas->resizeColumnsToContents();
    ui->tableEntradas->resizeRowsToContents();
    ui->tableEntradas->sortByColumn(0);

    clsMovimiento_Salida Sal;
    QVector<QStringList> listaSalida = Sal.ListaSalida();

    ui->tableEntradas->setRowCount(0);

    for (int i=0;i<listaSalida.size();i++)
    {        
        QStringList strSalida = listaSalida.at(i);

        bool bMuestra = false;

        if(ui->rbtnPorDevolver->isChecked())
        {
            int intCantidad = 0;

            Sal.Cargar(strSalida.at(0).toInt());
            int j = 0;
            while((j < Sal.DetallesSalida().size()) && !bMuestra)
            {
                clsMovimiento_DetSal DetSal(Sal.DetallesSalida().at(j));
                DetSal.Cargar(Sal.DetallesSalida().at(j).Salida(),Sal.DetallesSalida().at(j).CodProd(),Sal.DetallesSalida().at(j).Fecha_Salida());

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
                ui->tableEntradas->insertRow(ui->tableEntradas->rowCount());

                QTableWidgetItem *idSal = new QTableWidgetItem;
                idSal->setData(Qt::DisplayRole,strSalida.at(0));
                QTableWidgetItem *Proy = new QTableWidgetItem;
                Proy->setData(Qt::DisplayRole,strSalida.at(2));
                QTableWidgetItem *Tarea = new QTableWidgetItem;
                Tarea->setData(Qt::DisplayRole,strSalida.at(3));
                QTableWidgetItem *NomApOp = new QTableWidgetItem;
                NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
                QTableWidgetItem *Cent = new QTableWidgetItem;
                Cent->setData(Qt::DisplayRole,strSalida.at(1));

                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,0,idSal);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,1,Proy);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,2,Tarea);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,3,NomApOp);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,4,Cent);
            }
        }
        else
        {
            ui->tableEntradas->insertRow(ui->tableEntradas->rowCount());

            QTableWidgetItem *idSal = new QTableWidgetItem;
            idSal->setData(Qt::DisplayRole,strSalida.at(0));
            QTableWidgetItem *Proy = new QTableWidgetItem;
            Proy->setData(Qt::DisplayRole,strSalida.at(2));
            QTableWidgetItem *Tarea = new QTableWidgetItem;
            Tarea->setData(Qt::DisplayRole,strSalida.at(3));
            QTableWidgetItem *NomApOp = new QTableWidgetItem;
            NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
            QTableWidgetItem *Cent = new QTableWidgetItem;
            Cent->setData(Qt::DisplayRole,strSalida.at(1));

            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,0,idSal);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,1,Proy);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,2,Tarea);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,3,NomApOp);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,4,Cent);
        }
    }
    ui->tableEntradas->resizeColumnsToContents();
    ui->tableEntradas->resizeRowsToContents();
}

void MovimientosListas::CargarLineasStock()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableStockProd->clear();
    ui->tableStockProd->setRowCount(0);
    ui->tableStockProd->setColumnCount(7);
    ui->tableStockProd->setColumnHidden(6,!permiso);
    ui->tableStockProd->setHorizontalHeaderLabels(QStringList() << QObject::tr("Producto") << QObject::tr("Descripción") << QObject::tr("Origen") << QObject::tr("Fecha Stock") << QObject::tr("Comentario") << QObject::tr("Cantidad") << QObject::tr("Centro"));
    ui->tableStockProd->resizeColumnsToContents();
    ui->tableStockProd->resizeRowsToContents();
    ui->tableStockProd->sortByColumn(0);

    clsMovimiento_Stock Stock;
    QVector<QStringList> listaStock = Stock.ListaStock();

    ui->tableStockProd->setRowCount(0);

    for (int i=0;i<listaStock.size();i++)
    {
        QStringList strStock = listaStock.at(i);

        ui->tableStockProd->insertRow(ui->tableStockProd->rowCount());

        QTableWidgetItem *idprod = new QTableWidgetItem;
        idprod->setData(Qt::DisplayRole,strStock.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strStock.at(1));
        QTableWidgetItem *Orig = new QTableWidgetItem;
        Orig->setData(Qt::DisplayRole,strStock.at(2));
        QTableWidgetItem *FStock = new QTableWidgetItem;
        FStock->setData(Qt::DisplayRole,strStock.at(3));
        QTableWidgetItem *Coment = new QTableWidgetItem;
        Coment->setData(Qt::DisplayRole,strStock.at(4));
        QTableWidgetItem *Cant = new QTableWidgetItem;
        Cant->setData(Qt::DisplayRole,strStock.at(5));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strStock.at(6));

        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,0,idprod);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,1,Desc);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,2,Orig);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,3,FStock);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,4,Coment);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,5,Cant);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,6,Cent);
    }
    ui->tableStockProd->resizeColumnsToContents();
    ui->tableStockProd->resizeRowsToContents();
}

void MovimientosListas::CargarLineasRecepciones()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableRecepcion->clear();
    ui->tableRecepcion->setRowCount(0);
    ui->tableRecepcion->setColumnCount(4);
    ui->tableRecepcion->setColumnHidden(3,!permiso);
    ui->tableRecepcion->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Recepción") << QObject::tr("Centro Origen") << QObject::tr("Fecha Envío") << QObject::tr("Centro Destino"));
    ui->tableRecepcion->resizeColumnsToContents();
    ui->tableRecepcion->resizeRowsToContents();
    ui->tableRecepcion->sortByColumn(0);

    QString strFiltro = "";

    if(!permiso)
        strFiltro = "envios.idcentroDestino = '" + QString::number(config->IdCentroActual()) + "'";

    clsMovimiento_Envio Env;
    QVector<QStringList> listaEnvio = Env.ListaEnvio(strFiltro);

    ui->tableRecepcion->setRowCount(0);

    for (int i=0;i<listaEnvio.size();i++)
    {
        QStringList strEnvio = listaEnvio.at(i);

        ui->tableRecepcion->insertRow(ui->tableRecepcion->rowCount());

        QTableWidgetItem *idEnv = new QTableWidgetItem;
        idEnv->setData(Qt::DisplayRole,strEnvio.at(0));
        QTableWidgetItem *CentO = new QTableWidgetItem;
        CentO->setData(Qt::DisplayRole,strEnvio.at(3));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,strEnvio.at(2));
        QTableWidgetItem *CentD = new QTableWidgetItem;
        CentD->setData(Qt::DisplayRole,strEnvio.at(1));

        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,0,idEnv);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,1,CentO);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,2,FEnv);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,3,CentD);
    }
    ui->tableRecepcion->resizeColumnsToContents();
    ui->tableRecepcion->resizeRowsToContents();
}

void MovimientosListas::CargarLineasPedidos()
{
    ui->tablePedidos->clear();
    ui->tablePedidos->setRowCount(0);
    ui->tablePedidos->setColumnCount(2);
    ui->tablePedidos->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Pedido") << QObject::tr("Centro"));
    ui->tablePedidos->resizeColumnsToContents();
    ui->tablePedidos->resizeRowsToContents();
    ui->tablePedidos->sortByColumn(0);

    clsMovimiento_Pedido Ped;
    QVector<QStringList> listaPedido = Ped.ListaPedido();

    ui->tablePedidos->setRowCount(0);

    for (int i=0;i<listaPedido.size();i++)
    {
        QStringList strPedido = listaPedido.at(i);

        ui->tablePedidos->insertRow(ui->tablePedidos->rowCount());

        QTableWidgetItem *idPed = new QTableWidgetItem;
        idPed->setData(Qt::DisplayRole,strPedido.at(0));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strPedido.at(1));

        ui->tablePedidos->setItem(ui->tablePedidos->rowCount()-1,0,idPed);
        ui->tablePedidos->setItem(ui->tablePedidos->rowCount()-1,1,Cent);
    }
    ui->tablePedidos->resizeColumnsToContents();
    ui->tablePedidos->resizeRowsToContents();
}

void MovimientosListas::CargarLineasEnvios()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableEnvios->clear();
    ui->tableEnvios->setRowCount(0);
    ui->tableEnvios->setColumnCount(4);
    ui->tableEnvios->setColumnHidden(3,!permiso);
    ui->tableEnvios->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Envío") << QObject::tr("Centro Destino") << QObject::tr("Fecha Envío") << QObject::tr("Centro Origen"));
    ui->tableEnvios->resizeColumnsToContents();
    ui->tableEnvios->resizeRowsToContents();
    ui->tableEnvios->sortByColumn(0);

    QString strFiltro = "";

    if(!permiso)
        strFiltro = "envios.idcentroOrigen = '" + QString::number(config->IdCentroActual()) + "'";

    clsMovimiento_Envio Env;
    QVector<QStringList> listaEnvio = Env.ListaEnvio(strFiltro);

    ui->tableEnvios->setRowCount(0);

    for (int i=0;i<listaEnvio.size();i++)
    {
        QStringList strEnvio = listaEnvio.at(i);

        ui->tableEnvios->insertRow(ui->tableEnvios->rowCount());

        QTableWidgetItem *idEnv = new QTableWidgetItem;
        idEnv->setData(Qt::DisplayRole,strEnvio.at(0));
        QTableWidgetItem *CentD = new QTableWidgetItem;
        CentD->setData(Qt::DisplayRole,strEnvio.at(1));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,strEnvio.at(2));
        QTableWidgetItem *CentO = new QTableWidgetItem;
        CentO->setData(Qt::DisplayRole,strEnvio.at(3));

        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,0,idEnv);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,1,CentD);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,2,FEnv);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,3,CentO);
    }
    ui->tableEnvios->resizeColumnsToContents();
    ui->tableEnvios->resizeRowsToContents();
}

void MovimientosListas::ActivarFiltroSalidas()
{
    if (ui->cbFiltro_Sal->currentIndex() == 0)
        ui->edtFiltro_Sal->setEnabled(false);
    else
        ui->edtFiltro_Sal->setEnabled(true);
    FiltrarSalidas();
}

void MovimientosListas::ActivarFiltroEntradas()
{
    if (ui->cbFiltro_Ent->currentIndex() == 0)
        ui->edtFiltro_Ent->setEnabled(false);
    else
        ui->edtFiltro_Ent->setEnabled(true);
    FiltrarEntradas();
}

void MovimientosListas::ActivarFiltroRecepciones()
{
    if (ui->cbFiltro_Rec->currentIndex() == 0)
        ui->edtFiltro_Rec->setEnabled(false);
    else
        ui->edtFiltro_Rec->setEnabled(true);
    FiltrarRecepciones();
}

void MovimientosListas::ActivarFiltroPedidos()
{
    if (ui->cbFiltro_Ped->currentIndex() == 0)
        ui->edtFiltro_Ped->setEnabled(false);
    else
        ui->edtFiltro_Ped->setEnabled(true);
    FiltrarPedidos();
}

void MovimientosListas::ActivarFiltroEnvios()
{
    if (ui->cbFiltro_Env->currentIndex() == 0)
        ui->edtFiltro_Env->setEnabled(false);
    else
        ui->edtFiltro_Env->setEnabled(true);
    FiltrarEnvios();
}

void MovimientosListas::ActivarFiltroStock()
{
    if (ui->cbFiltro_Stock->currentIndex() == 0)
        ui->edtFiltro_Stock->setEnabled(false);
    else
        ui->edtFiltro_Stock->setEnabled(true);
    FiltrarStock();
}

void MovimientosListas::FiltrarSalidas()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableSalidas->clear();
    ui->tableSalidas->setRowCount(0);
    ui->tableSalidas->setColumnCount(5);
    ui->tableSalidas->setColumnHidden(4,!permiso);
    ui->tableSalidas->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Salida") << QObject::tr("Proyecto") << QObject::tr("Tarea") << QObject::tr("Operario") << QObject::tr("Centro"));
    ui->tableSalidas->resizeColumnsToContents();
    ui->tableSalidas->resizeRowsToContents();
    ui->tableSalidas->sortByColumn(0);

    QString strFiltro = "";

    if (ui->cbFiltro_Sal->currentIndex() == 1)
        strFiltro += "CAST(salidas.idsalida AS varchar(10)) LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 2)
        strFiltro += "salidas.proyecto LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 3)
        strFiltro += "salidas.tarea LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 4)
        strFiltro += "concat_ws(' ',operarios.nombre,operarios.apellidos) LIKE '%" + ui->edtFiltro_Sal->text() + "%'";
    else if (ui->cbFiltro_Sal->currentIndex() == 5)
        strFiltro += "concat_ws(' - ',centros.nombre,centros.ciudad) LIKE '%" + ui->edtFiltro_Sal->text() + "%'";

    clsMovimiento_Salida Sal;
    QVector<QStringList> listaSalida = Sal.ListaSalida(strFiltro);

    ui->tableSalidas->setRowCount(0);

    for (int i=0;i<listaSalida.size();i++)
    {
        QStringList strSalida = listaSalida.at(i);

        ui->tableSalidas->insertRow(ui->tableSalidas->rowCount());

        QTableWidgetItem *idSal = new QTableWidgetItem;
        idSal->setData(Qt::DisplayRole,strSalida.at(0));
        QTableWidgetItem *Proy = new QTableWidgetItem;
        Proy->setData(Qt::DisplayRole,strSalida.at(2));
        QTableWidgetItem *Tarea = new QTableWidgetItem;
        Tarea->setData(Qt::DisplayRole,strSalida.at(3));
        QTableWidgetItem *NomApOp = new QTableWidgetItem;
        NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strSalida.at(1));

        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,0,idSal);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,1,Proy);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,2,Tarea);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,3,NomApOp);
        ui->tableSalidas->setItem(ui->tableSalidas->rowCount()-1,4,Cent);
    }
    ui->tableSalidas->resizeColumnsToContents();
    ui->tableSalidas->resizeRowsToContents();
}

void MovimientosListas::FiltrarEntradas()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableEntradas->clear();
    ui->tableEntradas->setRowCount(0);
    ui->tableEntradas->setColumnCount(5);
    ui->tableEntradas->setColumnHidden(4,!permiso);
    ui->tableEntradas->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Salida") << QObject::tr("Proyecto") << QObject::tr("Tarea") << QObject::tr("Operario") << QObject::tr("Centro"));
    ui->tableEntradas->resizeColumnsToContents();
    ui->tableEntradas->resizeRowsToContents();
    ui->tableEntradas->sortByColumn(0);

    QString strFiltro = "";

    if (ui->cbFiltro_Ent->currentIndex() == 1)
        strFiltro += "CAST(salidas.idsalida AS varchar(10)) LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 2)
        strFiltro += "salidas.proyecto LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 3)
        strFiltro += "salidas.tarea LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 4)
        strFiltro += "concat_ws(' ',operarios.nombre,operarios.apellidos) LIKE '%" + ui->edtFiltro_Ent->text() + "%'";
    else if (ui->cbFiltro_Ent->currentIndex() == 5)
        strFiltro += "concat_ws(' - ',centros.nombre,centros.ciudad) LIKE '%" + ui->edtFiltro_Ent->text() + "%'";

    clsMovimiento_Salida Sal;
    QVector<QStringList> listaSalida = Sal.ListaSalida(strFiltro);

    ui->tableEntradas->setRowCount(0);

    for (int i=0;i<listaSalida.size();i++)
    {
        QStringList strSalida = listaSalida.at(i);

        bool bMuestra = false;

        if(ui->rbtnPorDevolver->isChecked())
        {
            int intCantidad = 0;

            Sal.Cargar(strSalida.at(0).toInt());
            int j = 0;
            while((j < Sal.DetallesSalida().size()) && !bMuestra)
            {
                clsMovimiento_DetSal DetSal(Sal.DetallesSalida().at(j));
                DetSal.Cargar(Sal.DetallesSalida().at(j).Salida(),Sal.DetallesSalida().at(j).CodProd(),Sal.DetallesSalida().at(j).Fecha_Salida());

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
                ui->tableEntradas->insertRow(ui->tableEntradas->rowCount());

                QTableWidgetItem *idSal = new QTableWidgetItem;
                idSal->setData(Qt::DisplayRole,strSalida.at(0));
                QTableWidgetItem *Proy = new QTableWidgetItem;
                Proy->setData(Qt::DisplayRole,strSalida.at(2));
                QTableWidgetItem *Tarea = new QTableWidgetItem;
                Tarea->setData(Qt::DisplayRole,strSalida.at(3));
                QTableWidgetItem *NomApOp = new QTableWidgetItem;
                NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
                QTableWidgetItem *Cent = new QTableWidgetItem;
                Cent->setData(Qt::DisplayRole,strSalida.at(1));

                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,0,idSal);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,1,Proy);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,2,Tarea);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,3,NomApOp);
                ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,4,Cent);
            }
        }
        else
        {
            ui->tableEntradas->insertRow(ui->tableEntradas->rowCount());

            QTableWidgetItem *idSal = new QTableWidgetItem;
            idSal->setData(Qt::DisplayRole,strSalida.at(0));
            QTableWidgetItem *Proy = new QTableWidgetItem;
            Proy->setData(Qt::DisplayRole,strSalida.at(2));
            QTableWidgetItem *Tarea = new QTableWidgetItem;
            Tarea->setData(Qt::DisplayRole,strSalida.at(3));
            QTableWidgetItem *NomApOp = new QTableWidgetItem;
            NomApOp->setData(Qt::DisplayRole,strSalida.at(4));
            QTableWidgetItem *Cent = new QTableWidgetItem;
            Cent->setData(Qt::DisplayRole,strSalida.at(1));

            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,0,idSal);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,1,Proy);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,2,Tarea);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,3,NomApOp);
            ui->tableEntradas->setItem(ui->tableEntradas->rowCount()-1,4,Cent);
        }
    }
    ui->tableEntradas->resizeColumnsToContents();
    ui->tableEntradas->resizeRowsToContents();
}

void MovimientosListas::FiltrarRecepciones()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableRecepcion->clear();
    ui->tableRecepcion->setRowCount(0);
    ui->tableRecepcion->setColumnCount(4);
    ui->tableRecepcion->setColumnHidden(3,!permiso);
    ui->tableRecepcion->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Recepción") << QObject::tr("Centro Origen") << QObject::tr("Fecha Envío") << QObject::tr("Centro Destino"));
    ui->tableRecepcion->resizeColumnsToContents();
    ui->tableRecepcion->resizeRowsToContents();
    ui->tableRecepcion->sortByColumn(0);

    QString strFiltro = "";

    if(!permiso)
        strFiltro = "envios.idcentroDestino = '" + QString::number(config->IdCentroActual()) + "' AND ";

    if (ui->cbFiltro_Rec->currentIndex() == 1)
        strFiltro += "CAST(envios.idenvio AS varchar(10)) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 2)
        strFiltro += "concat_ws(' - ',centO.nombre,centO.ciudad) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 3)
        strFiltro += "CAST(envios.fecha_creacion AS varchar(10)) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 4)
        strFiltro += "concat_ws(' - ',centD.nombre,centD.ciudad) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";

    clsMovimiento_Envio Env;
    QVector<QStringList> listaEnvio = Env.ListaEnvio(strFiltro);

    ui->tableRecepcion->setRowCount(0);

    for (int i=0;i<listaEnvio.size();i++)
    {
        QStringList strEnvio = listaEnvio.at(i);

        ui->tableRecepcion->insertRow(ui->tableRecepcion->rowCount());

        QTableWidgetItem *idEnv = new QTableWidgetItem;
        idEnv->setData(Qt::DisplayRole,strEnvio.at(0));
        QTableWidgetItem *CentO = new QTableWidgetItem;
        CentO->setData(Qt::DisplayRole,strEnvio.at(3));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,strEnvio.at(2));
        QTableWidgetItem *CentD = new QTableWidgetItem;
        CentD->setData(Qt::DisplayRole,strEnvio.at(1));

        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,0,idEnv);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,1,CentO);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,2,FEnv);
        ui->tableRecepcion->setItem(ui->tableRecepcion->rowCount()-1,3,CentD);
    }
    ui->tableRecepcion->resizeColumnsToContents();
    ui->tableRecepcion->resizeRowsToContents();
}

void MovimientosListas::FiltrarEnvios()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableEnvios->clear();
    ui->tableEnvios->setRowCount(0);
    ui->tableEnvios->setColumnCount(4);
    ui->tableEnvios->setColumnHidden(3,!permiso);
    ui->tableEnvios->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Envío") << QObject::tr("Centro Destino") << QObject::tr("Fecha Envío") << QObject::tr("Centro Origen"));
    ui->tableEnvios->resizeColumnsToContents();
    ui->tableEnvios->resizeRowsToContents();
    ui->tableEnvios->sortByColumn(0);

    QString strFiltro = "";

    if(!permiso)
        strFiltro = "envios.idcentroOrigen = '" + QString::number(config->IdCentroActual()) + "' AND ";

    if (ui->cbFiltro_Env->currentIndex() == 1)
        strFiltro += "CAST(envios.idenvio AS varchar(20)) LIKE '%" + ui->edtFiltro_Env->text() + "%'";
    else if (ui->cbFiltro_Env->currentIndex() == 2)
        strFiltro += "concat_ws(' - ',centD.nombre,centD.ciudad) LIKE '%" + ui->edtFiltro_Env->text() + "%'";
    else if (ui->cbFiltro_Env->currentIndex() == 3)
        strFiltro += "CAST(envios.fecha_creacion AS varchar(10)) LIKE '%" + ui->edtFiltro_Env->text() + "%'";
    else if (ui->cbFiltro_Rec->currentIndex() == 4)
        strFiltro += "concat_ws(' - ',centO.nombre,centO.ciudad) LIKE '%" + ui->edtFiltro_Rec->text() + "%'";

    clsMovimiento_Envio Env;
    QVector<QStringList> listaEnvio = Env.ListaEnvio(strFiltro);

    ui->tableEnvios->setRowCount(0);

    for (int i=0;i<listaEnvio.size();i++)
    {
        QStringList strEnvio = listaEnvio.at(i);

        ui->tableEnvios->insertRow(ui->tableEnvios->rowCount());

        QTableWidgetItem *idEnv = new QTableWidgetItem;
        idEnv->setData(Qt::DisplayRole,strEnvio.at(0));
        QTableWidgetItem *CentD = new QTableWidgetItem;
        CentD->setData(Qt::DisplayRole,strEnvio.at(1));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,strEnvio.at(2));
        QTableWidgetItem *CentO = new QTableWidgetItem;
        CentO->setData(Qt::DisplayRole,strEnvio.at(3));

        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,0,idEnv);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,1,CentD);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,2,FEnv);
        ui->tableEnvios->setItem(ui->tableEnvios->rowCount()-1,3,CentO);
    }
    ui->tableEnvios->resizeColumnsToContents();
    ui->tableEnvios->resizeRowsToContents();
}

void MovimientosListas::FiltrarStock()
{
    bool permiso = config->Permisos().value(Confi);

    ui->tableStockProd->clear();
    ui->tableStockProd->setRowCount(0);
    ui->tableStockProd->setColumnCount(7);
    ui->tableStockProd->setColumnHidden(6,!permiso);
    ui->tableStockProd->setHorizontalHeaderLabels(QStringList() << QObject::tr("Producto") << QObject::tr("Descripción") << QObject::tr("Origen") << QObject::tr("Fecha Stock") << QObject::tr("Comentario") << QObject::tr("Cantidad") << QObject::tr("Centro"));
    ui->tableStockProd->resizeColumnsToContents();
    ui->tableStockProd->resizeRowsToContents();
    ui->tableStockProd->sortByColumn(0);

    QString strFiltro = "";

    if (ui->cbFiltro_Stock->currentIndex() == 1)
        strFiltro += "stock.idproducto LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 2)
        strFiltro += "productos.descripcion LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 3)
        strFiltro += "CAST(stock.origen AS varchar(10)) LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 4)
        strFiltro += "stock.comentario LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 5)
        strFiltro += "CAST(stock.cantidad AS varchar(10)) LIKE '%" + ui->edtFiltro_Stock->text() + "%'";
    else if (ui->cbFiltro_Stock->currentIndex() == 6)
        strFiltro += "concat_ws(' - ',centros.nombre,centros.ciudad) LIKE '%" + ui->edtFiltro_Stock->text() + "%'";

    clsMovimiento_Stock Stock;
    QVector<QStringList> listaStock = Stock.ListaStock(strFiltro);

    ui->tableStockProd->setRowCount(0);

    for (int i=0;i<listaStock.size();i++)
    {
        QStringList strStock = listaStock.at(i);

        ui->tableStockProd->insertRow(ui->tableStockProd->rowCount());

        QTableWidgetItem *idprod = new QTableWidgetItem;
        idprod->setData(Qt::DisplayRole,strStock.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strStock.at(1));
        QTableWidgetItem *Orig = new QTableWidgetItem;
        Orig->setData(Qt::DisplayRole,strStock.at(2));
        QTableWidgetItem *FStock = new QTableWidgetItem;
        FStock->setData(Qt::DisplayRole,strStock.at(3));
        QTableWidgetItem *Coment = new QTableWidgetItem;
        Coment->setData(Qt::DisplayRole,strStock.at(4));
        QTableWidgetItem *Cant = new QTableWidgetItem;
        Cant->setData(Qt::DisplayRole,strStock.at(5));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strStock.at(6));

        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,0,idprod);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,1,Desc);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,2,Orig);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,3,FStock);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,4,Coment);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,5,Cant);
        ui->tableStockProd->setItem(ui->tableStockProd->rowCount()-1,6,Cent);
    }
    ui->tableStockProd->resizeColumnsToContents();
    ui->tableStockProd->resizeRowsToContents();
}

void MovimientosListas::FiltrarPedidos()
{
    ui->tablePedidos->clear();
    ui->tablePedidos->setRowCount(0);
    ui->tablePedidos->setColumnCount(2);
    ui->tablePedidos->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Pedido") << QObject::tr("Centro"));
    ui->tablePedidos->resizeColumnsToContents();
    ui->tablePedidos->resizeRowsToContents();
    ui->tablePedidos->sortByColumn(0);

    QString strFiltro = "";

    if (ui->cbFiltro_Ped->currentIndex() == 1)
        strFiltro += "CAST(pedidos_internos.idPedido AS varchar(20)) LIKE '%" + ui->edtFiltro_Ped->text() + "%'";
    else if (ui->cbFiltro_Ped->currentIndex() == 2)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltro_Ped->text() + "%'";

    clsMovimiento_Pedido Ped;
    QVector<QStringList> listaPedido = Ped.ListaPedido(strFiltro);

    ui->tablePedidos->setRowCount(0);

    for (int i=0;i<listaPedido.size();i++)
    {
        QStringList strPedido = listaPedido.at(i);

        ui->tablePedidos->insertRow(ui->tablePedidos->rowCount());

        QTableWidgetItem *idPed = new QTableWidgetItem;
        idPed->setData(Qt::DisplayRole,strPedido.at(0));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strPedido.at(1));

        ui->tablePedidos->setItem(ui->tablePedidos->rowCount()-1,0,idPed);
        ui->tablePedidos->setItem(ui->tablePedidos->rowCount()-1,1,Cent);
    }
    ui->tablePedidos->resizeColumnsToContents();
    ui->tablePedidos->resizeRowsToContents();
}

void MovimientosListas::EliminarSalida()
{
    int Salida = 0;
    QModelIndex indice = ui->tableSalidas->currentIndex();
    if (indice.isValid())
    {
        Salida = ui->tableSalidas->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
        clsMovimiento_Salida Sal;
        Sal.Cargar(Salida);
        Sal.Eliminar();

        CargarLineasSalidas();
    }
}

void MovimientosListas::EliminarPedido()
{
    int Pedido = 0;
    QModelIndex indice = ui->tablePedidos->currentIndex();
    if (indice.isValid())
    {
        Pedido  = ui->tablePedidos->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
        clsMovimiento_Pedido Ped;
        Ped.Cargar(Pedido);
        if (Ped.Centro() == config->IdCentroActual() && !Ped.SeHaServido())
            Ped.Eliminar();

        CargarLineasPedidos();
    }
}

void MovimientosListas::EliminarEnvio()
{
    int intEnvio = 0;
    QModelIndex indice = ui->tableEnvios->currentIndex();
    if (indice.isValid())
    {
        intEnvio = ui->tableEnvios->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
        clsMovimiento_Envio Env;
        Env.Cargar(intEnvio);
        Env.Eliminar();

        CargarLineasEnvios();
    }
}

void MovimientosListas::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->btnNueva_Sal->setEnabled(permiso);
    ui->btnNueva_Env->setEnabled(permiso);
}
