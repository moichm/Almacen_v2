#include "movimientosenvio.h"
#include "ui_movimientosenvio.h"
#include "Productos/productoslista.h"
#include "Kits/kitslista.h"
#include "Kits/kit.h"
#include "clsconfiguracion.h"
#include "Productos/producto.h"
#include "Centros/centroslista.h"
#include <QVariant>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QCalendarWidget>
#include <QInputDialog>
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosEnvio::MovimientosEnvio(int _Env,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosEnvio)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(4);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad") << QObject::tr("Fecha envío"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    QCalendarWidget *FechaEnvio = new QCalendarWidget();
    FechaEnvio->setFirstDayOfWeek(Qt::Monday);
    ui->datFechaEnvio->setCalendarWidget(FechaEnvio);
    ui->datFechaEnvio->setDate(QDate::currentDate());

    CentroOrigen = new clsCentros;
    CentroDestino = new clsCentros;

    Env = new clsMovimiento_Envio(_Env);

    CargarDatos();

    connect(ui->btnAniadirProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaProd()));
    connect(ui->btnAniadirKit,SIGNAL(clicked()),this,SLOT(AbrirVentanaKits()));
    connect(ui->btnEliminarProd,SIGNAL(clicked()),this,SLOT(EliminarProducto()));
    connect(ui->btnCentro,SIGNAL(clicked()),this,SLOT(AbrirVentanaCentros()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(accept()));
    connect(ui->btnGuardarDatos,SIGNAL(clicked()),this,SLOT(GuardarDatos()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();

    if (_Env == 0)
    {
        ui->btnAniadirProd->setEnabled(false);
        ui->btnAniadirKit->setEnabled(false);
        ui->btnEliminarProd->setEnabled(false);
        ui->btnGuardarDatos->setEnabled(true);
    }
    else
    {
        ui->btnAniadirProd->setEnabled(true);
        ui->btnAniadirKit->setEnabled(true);
        ui->btnEliminarProd->setEnabled(true);
        ui->btnGuardarDatos->setEnabled(false);
    }
}

MovimientosEnvio::~MovimientosEnvio()
{
    delete CentroDestino;
    delete CentroOrigen;
    delete Env;
    delete ui;
}

void MovimientosEnvio::changeEvent(QEvent *e)
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

void MovimientosEnvio::Imprimir()
{    
    Env->Cargar(Env->NoEnvio());

    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaEnvio(Env);
}

void MovimientosEnvio::CargarDatos()
{
    CentroOrigen->Cargar(config->IdCentroActual());

    ui->edtCentroOri->setText(CentroOrigen->Nombre());

    if(Env->NoEnvio() != 0)
    {
        Env->Cargar(Env->NoEnvio());

        CentroDestino->Cargar(Env->CentroDestino());

        ui->edtNEnvio->setText(QString::number(Env->NoEnvio()));
        ui->edtCentDest->setText(CentroDestino->Nombre());
        ui->datFechaEnvio->setDate(Env->FechaCreacion());

        ui->ptedtComentarios->setPlainText(Env->Comentarios());

        CargarDetalles();
    }
}

void MovimientosEnvio::CargarDetalles()
{
    ui->tableDetalles->setRowCount(0);

    Env->Cargar(Env->NoEnvio());

    for (int i=0;i<Env->Detalles().size();i++)
    {
        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        clsProductos Prod;
        Prod.Cargar(Env->Detalles().at(i).CodProd());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,Env->Detalles().at(i).CodProd());
        Cod->setFlags(Cod->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,Prod.Descripcion());
        Desc->setFlags(Desc->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cant = new QTableWidgetItem;
        Cant->setData(Qt::DisplayRole,Env->Detalles().at(i).Cantidad());
        Cant->setFlags(Cant->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,Env->Detalles().at(i).FechaEnvio().toString(QObject::tr("dd/MM/yyyy")));
        FEnv->setFlags(FEnv->flags() & (~Qt::ItemIsEditable));

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Cod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Cant);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,3,FEnv);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void MovimientosEnvio::InsertarKit(int codKit)
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

void MovimientosEnvio::InsertarProducto(QString CodProd,int Cantidad)
{
    clsProductos Prod;

    if (Cantidad > 0)
    {
        Prod.Cargar(CodProd);
        if (Prod.Estado() == eDisponible)
        {
            if (Prod.StockCentro() < Cantidad)
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No hay suficiente cantidad del producto ") + CodProd + QObject::tr(" para entregar."));
            else
            {
                clsMovimiento_DetEnv EnvDet;
                EnvDet.Cargar(ui->edtNEnvio->text().toInt(),CodProd,QDate::currentDate());
                int CantActual = 0;

                if (EnvDet.Existe())
                {
                    CantActual = EnvDet.Cantidad();
                    EnvDet.setCantidad(CantActual+Cantidad);
                    if(!EnvDet.Modificar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a enviar del producto ") + CodProd + ("."));
                }
                else
                {
                    EnvDet.setCantidad(Cantidad);
                    if(!EnvDet.Insertar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido insertar el producto ") + CodProd + QObject::tr(" a enviar."));
                }
            }
        }
        else
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("El producto ") + CodProd + QObject::tr(" no está disponible."),0,0);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
}

void MovimientosEnvio::EliminarProducto()
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

        clsMovimiento_DetEnv EnvDet;
        EnvDet.Cargar(ui->edtNEnvio->text().toInt(),Prod,datFecha);
        if(!EnvDet.Eliminar())
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido eliminar el producto ") + Prod + ("."));

        CargarDetalles();
    }
}

void MovimientosEnvio::Guardar()
{
    Env->setFechaCreacion(ui->datFechaEnvio->date());
    Env->setCentroDestino(CentroDestino->Codigo());
    Env->setComentarios(ui->ptedtComentarios->toPlainText());

    if (!Env->Existe())
    {
        if(Env->Insertar())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(Env->Modificar())
            this->accept();
        else
            this->reject();
    }
}

void MovimientosEnvio::GuardarDatos()
{
    Env->setFechaCreacion(ui->datFechaEnvio->date());
    Env->setCentroDestino(CentroDestino->Codigo());
    Env->setComentarios(ui->ptedtComentarios->toPlainText());

    if (ui->edtCentDest->text() != "")
    {
        if (!Env->Existe())
            Env->Insertar();
        else
            Env->Modificar();

        CargarDatos();

        ui->btnEliminarProd->setEnabled(true);
        ui->btnGuardarDatos->setEnabled(false);
    }
    else
        QMessageBox::critical(this,QObject::tr("Error al introducir datos"),QObject::tr("Debe introducir un centro de destino."),0,0);
}

void MovimientosEnvio::AbrirVentanaProd()
{
    ProductosLista *ventanaProd = new ProductosLista();
    ventanaProd->setWindowModality(Qt::ApplicationModal);
    ventanaProd->show();
    if (ventanaProd->exec() == QDialog::Accepted)
    {
        bool ok;
        int intCantidad = QInputDialog::getInt(this,QObject::tr("Introducción de datos"),QObject::tr("Introduzca la cantidad a incluir del producto:"),1,1,100000000,1,&ok);
        if(ok)
        {
            if (intCantidad > 0)
            {
                QString strProducto = ventanaProd->CodProdSeleccionado();
                InsertarProducto(strProducto,intCantidad);
                CargarDetalles();
            }
            else
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
                ui->btnAniadirProd->click();
            }
        }
    }
}

void MovimientosEnvio::AbrirVentanaKits()
{
    KitsLista *ventanaKits = new KitsLista();
    ventanaKits->setWindowModality(Qt::ApplicationModal);
    ventanaKits->show();
    if (ventanaKits->exec() == QDialog::Accepted)
        InsertarKit(ventanaKits->CodKitSeleccionado());
}

void MovimientosEnvio::AbrirVentanaCentros()
{
    CentrosLista *ventanaCentro= new CentrosLista();
    ventanaCentro->setWindowModality(Qt::ApplicationModal);
    ventanaCentro->show();
    if (ventanaCentro->exec() == QDialog::Accepted)
    {
        CentroDestino->Cargar(ventanaCentro->CodigoCentroSeleccionado());
        ui->edtCentDest->setText(CentroDestino->Nombre());
    }
}

void MovimientosEnvio::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnAniadirKit->setEnabled(permiso);
    ui->btnEliminarProd->setEnabled(permiso);
    ui->btnAniadirProd->setEnabled(permiso);
    ui->btnGuardarDatos->setEnabled(permiso);
}

void MovimientosEnvio::ServirPedido(int Pedido, QString _Prod, QDate _Fecha)
{
    clsMovimiento_Pedido Ped;
    Ped.Cargar(Pedido);

    CentroDestino->Cargar(Ped.Centro());
    ui->edtCentDest->setText(CentroDestino->Nombre());

    clsMovimiento_DetPed PedDet;
    PedDet.Cargar(Pedido,_Prod,_Fecha);

    GuardarDatos();

    if(!PedDet.Servir(Env->NoEnvio(),Env->FechaCreacion()))
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido servir el producto ") + _Prod + ("."));

    ui->btnCentro->setEnabled(false);
    ui->btnAniadirProd->setEnabled(true);
    ui->btnAniadirKit->setEnabled(true);

    InsertarProducto(_Prod,PedDet.Cantidad());
    CargarDetalles();
}
