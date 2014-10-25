#include <QVariant>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QPushButton>
#include "Productos/productoslista.h"
#include "Productos/producto.h"
#include "Kits/kitslista.h"
#include "Kits/kit.h"
#include "Operarios/operarioslista.h"
#include "Operarios/operario.h"
#include "Centros/centro.h"
#include "clsconfiguracion.h"
#include "movimientossalidas.h"
#include "ui_movimientossalidas.h"
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosSalidas::MovimientosSalidas(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosSalidas)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(4);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad") << QObject::tr("Fecha salida"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);
    
    Sal = new clsMovimiento_Salida(codigo);

    CargarDatos();

    connect(ui->btnAniadirProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaProd()));
    connect(ui->btnAniadirKit,SIGNAL(clicked()),this,SLOT(AbrirVentanaKits()));
    connect(ui->btnEliminarProd,SIGNAL(clicked()),this,SLOT(EliminarProducto()));
    connect(ui->btnOperario,SIGNAL(clicked()),this,SLOT(AbrirVentanaOperarios()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(accept()));
    connect(ui->btnGuardarDatos,SIGNAL(clicked()),this,SLOT(GuardarDatos()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));


    DeshabilitarControles();

    if (codigo == 0)
    {
        ui->btnAniadirProd->setEnabled(false);
        ui->btnAniadirKit->setEnabled(false);
        ui->btnEliminarProd->setEnabled(false);
        ui->btnOperario->setEnabled(true);
        ui->btnGuardarDatos->setEnabled(true);
    }
    else
    {
        ui->btnAniadirProd->setEnabled(true);
        ui->btnAniadirKit->setEnabled(true);
        ui->btnEliminarProd->setEnabled(true);
        ui->btnOperario->setEnabled(false);
        ui->btnGuardarDatos->setEnabled(false);
    }
}

MovimientosSalidas::~MovimientosSalidas()
{
    delete Sal;
    delete ui;
}

void MovimientosSalidas::changeEvent(QEvent *e)
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

void MovimientosSalidas::Imprimir()
{
    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaSalida(Sal);
}

void MovimientosSalidas::CargarDatos()
{
    clsCentros Centro;
    Centro.Cargar(config->IdCentroActual());

    ui->edtCentro->setText(Centro.Nombre());

    if(Sal->NoSalida() != 0)
    {
        Sal->Cargar(Sal->NoSalida());

        clsOperarios Op(Sal->Operario());
        Op.Cargar(Sal->Operario());

        ui->edtNSalida->setText(QString::number(Sal->NoSalida()));
        ui->edtNomOperario->setText(Op.Nombre() + " " + Op.Apellidos());
        ui->edtOperario->setText(Op.NIF());
        ui->edtProyecto->setText(Sal->Proyecto());
        ui->edtTarea->setText(Sal->Tarea());
        ui->ptedtComentarios->setPlainText(Sal->Comentarios());

        CargarDetalles();
    }
}

void MovimientosSalidas::CargarDetalles()
{
    Sal->Cargar(Sal->NoSalida());
    ui->tableDetalles->setRowCount(0);

    for (int i=0;i<Sal->DetallesSalida().size();i++)
    {
        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        clsProductos Prod;
        Prod.Cargar(Sal->DetallesSalida().at(i).CodProd());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).CodProd());
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,Prod.Descripcion());
        QTableWidgetItem *Cant = new QTableWidgetItem;
        Cant->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).Cantidad());
        QTableWidgetItem *FSal = new QTableWidgetItem;
        FSal->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).Fecha_Salida().toString(QObject::tr("dd/MM/yyyy")));

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Cod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Cant);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,3,FSal);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void MovimientosSalidas::InsertarKit(int codKit)
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

void MovimientosSalidas::InsertarProducto(QString CodProd,int Cantidad)
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
                clsMovimiento_DetSal SalDet;
                SalDet.Cargar(ui->edtNSalida->text().toInt(),CodProd,QDate::currentDate());
                int CantActual = SalDet.Cantidad();

                SalDet.setCantidad(CantActual+Cantidad);

                if (SalDet.Existe())
                {
                    if(!SalDet.Modificar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad del producto ") + CodProd + QObject::tr(" en la salida."));
                }
                else
                {
                    if(!SalDet.Insertar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido insertar el producto ") + CodProd + QObject::tr(" a la salida."));
                }
            }
        }
        else
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("El producto ") + CodProd + QObject::tr(" no está disponible."),0,0);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
}

void MovimientosSalidas::EliminarProducto()
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

        clsMovimiento_DetSal SalDet;
        SalDet.Cargar(ui->edtNSalida->text().toInt(),Prod,datFecha);
        if(!SalDet.Eliminar())
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido eliminar el producto ") + Prod + ("."));

        CargarDetalles();
    }
}

bool MovimientosSalidas::InsertarSalida()
{
    bool error=false;

    Sal->setOperario(ui->edtOperario->text());
    Sal->setProyecto(ui->edtProyecto->text());
    Sal->setTarea(ui->edtTarea->text());
    Sal->setComentarios(ui->ptedtComentarios->toPlainText());

    if(Sal->Insertar())
        error = true;

    return error;
}

bool MovimientosSalidas::ModificarSalida()
{
    bool error=false;

    Sal->setOperario(ui->edtOperario->text());
    Sal->setProyecto(ui->edtProyecto->text());
    Sal->setTarea(ui->edtTarea->text());
    Sal->setComentarios(ui->ptedtComentarios->toPlainText());

    if(Sal->Modificar())
        error = true;

    return error;
}

void MovimientosSalidas::Guardar()
{
    if (!Sal->Existe())
    {
        if(InsertarSalida())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(ModificarSalida())
            this->accept();
        else
            this->reject();
    }
}

void MovimientosSalidas::GuardarDatos()
{
    if (!Sal->Existe())
    {
        if (InsertarSalida())
        {
            ui->btnAniadirKit->setEnabled(true);
            ui->btnEliminarProd->setEnabled(true);
            ui->btnAniadirProd->setEnabled(true);
            ui->btnGuardarDatos->setEnabled(false);
        }
    }
    else
    {
        if (ModificarSalida())
        {
            ui->btnAniadirKit->setEnabled(true);
            ui->btnEliminarProd->setEnabled(true);
            ui->btnAniadirProd->setEnabled(true);
            ui->btnGuardarDatos->setEnabled(false);
        }
    }

    CargarDatos();    
}

void MovimientosSalidas::AbrirVentanaProd()
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

void MovimientosSalidas::AbrirVentanaKits()
{
    KitsLista *ventanaKits = new KitsLista();
    ventanaKits->setWindowModality(Qt::ApplicationModal);
    ventanaKits->show();
    if (ventanaKits->exec() == QDialog::Accepted)
        InsertarKit(ventanaKits->CodKitSeleccionado());
}

void MovimientosSalidas::AbrirVentanaOperarios()
{
    OperariosLista *ventanaOp = new OperariosLista();
    ventanaOp->setWindowModality(Qt::ApplicationModal);
    ventanaOp->show();
    if (ventanaOp->exec() == QDialog::Accepted)
    {
        clsOperarios Op(ventanaOp->NIFOpSeleccionado());
        ui->edtOperario->setText(Op.NIF());
        ui->edtNomOperario->setText(Op.Nombre() + " " + Op.Apellidos());
    }
}

void MovimientosSalidas::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnAniadirKit->setEnabled(permiso);
    ui->btnEliminarProd->setEnabled(permiso);
    ui->btnAniadirProd->setEnabled(permiso);
    ui->btnGuardarDatos->setEnabled(permiso);
}
