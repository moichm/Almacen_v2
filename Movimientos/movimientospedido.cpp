#include "movimientospedido.h"
#include "ui_movimientospedido.h"
#include "movimientosenvio.h"
#include "Productos/productoslista.h"
#include "Kits/kitslista.h"
#include "Kits/kit.h"
#include "clsconfiguracion.h"
#include "Productos/producto.h"
#include "Centros/centro.h"
#include <QVariant>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QInputDialog>
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosPedido::MovimientosPedido(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosPedido)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(5);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad") << QObject::tr("Fecha pedido") << QObject::tr("Servido"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    Ped = new clsMovimiento_Pedido(codigo);
    if(Ped->NoPedido() != 0)
        Ped->Cargar(codigo);

    CargarDatos();

    connect(ui->btnAniadirProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaProd()));
    connect(ui->btnAniadirKit,SIGNAL(clicked()),this,SLOT(AbrirVentanaKits()));
    connect(ui->btnEliminarProd,SIGNAL(clicked()),this,SLOT(EliminarProducto()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(accept()));
    connect(ui->btnServir,SIGNAL(clicked()),this,SLOT(Servir()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();

    if(codigo!= 0)
    {
        if(config->IdCentroActual() != Ped->Centro())
        {
            ui->btnAniadirProd->setEnabled(false);
            ui->btnAniadirKit->setEnabled(false);
            ui->btnEliminarProd->setEnabled(false);
            ui->btnServir->setEnabled(true);
        }
        else
        {
            ui->btnAniadirProd->setEnabled(true);
            ui->btnAniadirKit->setEnabled(true);
            ui->btnEliminarProd->setEnabled(true);
            ui->btnServir->setEnabled(false);
        }
    }
    else
        ui->btnServir->setEnabled(false);
}

MovimientosPedido::~MovimientosPedido()
{
    delete Ped;
    delete ui;
}

void MovimientosPedido::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MovimientosPedido::Imprimir()
{    
    Ped->Cargar(Ped->NoPedido());

    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaPedido(Ped);
}

void MovimientosPedido::CargarDatos()
{
    clsCentros Centro;

    if(Ped->NoPedido() != 0)
    {
        Centro.Cargar(Ped->Centro());

        ui->edtCentro->setText(Centro.Nombre());
        ui->edtNPedido->setText(QString::number(Ped->NoPedido()));

        ui->ptedtComentarios->setPlainText(Ped->Comentarios());

        CargarDetalles();
    }
    else
    {
        Centro.Cargar(config->IdCentroActual());

        ui->edtCentro->setText(Centro.Nombre());
    }
}

void MovimientosPedido::CargarDetalles()
{
    ui->tableDetalles->setRowCount(0);

    Ped->Cargar(Ped->NoPedido());

    for (int i=0;i<Ped->Detalles().size();i++)
    {
        clsMovimiento_DetPed PedDet(Ped->Detalles().at(i));

        QString strProd = PedDet.CodProd();

        clsProductos product;
        product.Cargar(strProd);

        bool boServido = Ped->Detalles().at(i).Servido();

        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        QTableWidgetItem *Prod = new QTableWidgetItem;
        Prod->setData(Qt::DisplayRole,strProd);
        Prod->setFlags(Prod->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,product.Descripcion());
        Desc->setFlags(Desc->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Canti = new QTableWidgetItem;
        Canti->setData(Qt::DisplayRole,PedDet.Cantidad());
        Canti->setFlags(Canti->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *FPed = new QTableWidgetItem;
        FPed->setData(Qt::DisplayRole,PedDet.Fecha().toString(QObject::tr("dd/MM/yyyy")));
        FPed->setFlags(FPed->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Serv = new QTableWidgetItem;
        if(boServido)
            Serv->setData(Qt::DisplayRole,QObject::tr("Sí"));
        else
            Serv->setData(Qt::DisplayRole,QObject::tr("No"));
        Serv->setFlags(Serv->flags() & (~Qt::ItemIsEditable));

        ui->tableDetalles->blockSignals(true);

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Prod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Canti);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,3,FPed);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,4,Serv);

        ui->tableDetalles->blockSignals(false);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void MovimientosPedido::InsertarKit(int codKit)
{
    clsKits Kit;
    Kit.Cargar(codKit);
    for(int i = 0;i<Kit.Detalles().size();i++)
    {
        clsKitDetalle KitDet(Kit.Detalles().at(i));

        QString codProd = KitDet.CodProd();
        int intCantidad = KitDet.Cantidad();

        InsertarProducto(codProd,intCantidad);
        CargarDetalles();
    }
}

void MovimientosPedido::InsertarProducto(QString CodProd,int Cantidad)
{
    clsProductos Prod;

    if (Cantidad > 0)
    {
        Prod.Cargar(CodProd);

        clsMovimiento_DetPed PedDet;
        PedDet.Cargar(Ped->NoPedido(),CodProd,QDate::currentDate());
        int CantActual = PedDet.Cantidad();

        PedDet.setCantidad(CantActual+Cantidad);

        if (PedDet.Existe())
        {
            if(!PedDet.Modificar())
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a pedir del producto ") + CodProd + ("."));
        }
        else
        {
            if(!PedDet.Insertar())
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido insertar el producto ") + CodProd + QObject::tr(" a pedir."));
        }
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
}

void MovimientosPedido::EliminarProducto()
{
    QModelIndex indice = ui->tableDetalles->currentIndex();
    if (indice.isValid())
    {
        QString Prod =  ui->tableDetalles->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        QDate datFecha;
        QStringList strFecha = ui->tableDetalles->item(indice.row(),3)->data(Qt::DisplayRole).toString().split("/");
        if(strFecha.at(2).length() == 4)
            datFecha = QDate::fromString(strFecha.at(0)+strFecha.at(1)+strFecha.at(2),"ddMMyyyy");
        else
            datFecha = QDate::fromString(strFecha.at(2)+strFecha.at(1)+strFecha.at(0),"yyyyMMdd");

        clsMovimiento_DetPed PedDet;
        PedDet.Cargar(Ped->NoPedido(),Prod,datFecha);
        if(!PedDet.Eliminar())
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido eliminar el producto ") + Prod + ("."));

        CargarDetalles();
    }
}

void MovimientosPedido::Guardar()
{
    Ped->setComentarios(ui->ptedtComentarios->toPlainText());

    if (!Ped->Existe())
    {
        if(Ped->Insertar())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(Ped->Modificar())
            this->accept();
        else
            this->reject();
    }
}

void MovimientosPedido::AbrirVentanaProd()
{
    ProductosLista *ventanaProd = new ProductosLista();
    ventanaProd->setWindowModality(Qt::ApplicationModal);
    ventanaProd->show();
    if (ventanaProd->exec() == QDialog::Accepted)
    {
        bool ok;
        int Cant = QInputDialog::getInt(this,QObject::tr("Introducción de datos"),QObject::tr("Introduzca la cantidad a pedir del producto:"),1,1,100000000,1,&ok);
        if(ok)
        {
            if (Cant > 0)
            {
                QString strProducto = ventanaProd->CodProdSeleccionado();
                InsertarProducto(strProducto,Cant);
                CargarDetalles();
            }
            else
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
                ui->btnAniadirProd->click();
            }
        }

        CargarDetalles();
    }
}

void MovimientosPedido::AbrirVentanaKits()
{
    KitsLista *ventanaKits = new KitsLista();
    ventanaKits->setWindowModality(Qt::ApplicationModal);
    ventanaKits->show();
    if (ventanaKits->exec() == QDialog::Accepted)
        InsertarKit(ventanaKits->CodKitSeleccionado());
}

void MovimientosPedido::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnAniadirKit->setEnabled(permiso);
    ui->btnEliminarProd->setEnabled(permiso);
    ui->btnAniadirProd->setEnabled(permiso);
    ui->btnServir->setEnabled(permiso);
}

void MovimientosPedido::Servir()
{
    clsMovimiento_DetPed PedDet;    
    QDate datFecha;
    QStringList strFecha = ui->tableDetalles->item(ui->tableDetalles->currentRow(),3)->data(Qt::DisplayRole).toString().split("/");
    if(strFecha.at(2).length() == 4)
        datFecha = QDate::fromString(strFecha.at(0)+strFecha.at(1)+strFecha.at(2),"ddMMyyyy");
    else
        datFecha = QDate::fromString(strFecha.at(2)+strFecha.at(1)+strFecha.at(0),"yyyyMMdd");

    PedDet.Cargar(ui->edtNPedido->text().toInt(),
                  ui->tableDetalles->item(ui->tableDetalles->currentRow(),0)->data(Qt::DisplayRole).toString(),
                  datFecha);
    if(PedDet.Servido() == false)
    {
        MovimientosEnvio *ventanaEnvio = new MovimientosEnvio(0);
        ventanaEnvio->setWindowModality(Qt::ApplicationModal);
        ventanaEnvio->show();
        ventanaEnvio->ServirPedido(ui->edtNPedido->text().toInt(),
                                   ui->tableDetalles->item(ui->tableDetalles->currentRow(),0)->data(Qt::DisplayRole).toString(),
                                   datFecha);
    }
    else
        QMessageBox::information(0,QObject::tr("Pedidos Internos"),QObject::tr("Ya se ha servido la línea del pedido interno"),0,0);
}
