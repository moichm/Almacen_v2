#include <QVariant>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QInputDialog>
#include "kitficha.h"
#include "kit.h"
#include "ui_kitficha.h"
#include "Productos/productoslista.h"
#include "Productos/producto.h"
#include "clsconfiguracion.h"
#include "clsimprimirkits.h"

extern clsConfiguracion *config;

KitFicha::KitFicha(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KitFicha)
{
    ui->setupUi(this);

    Kit = new clsKits();
    Kit->Cargar(codigo);

    CargarDatos();

    QPushButton *btnGuardar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(ui->btnEliminarProd,SIGNAL(clicked()),this,SLOT(EliminarProducto()));
    connect(ui->btnAniadirProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaProd()));
    connect(btnGuardar,SIGNAL(clicked()),this,SLOT(GuardarKit()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();
}

KitFicha::~KitFicha()
{
    delete Kit;
    delete ui;
}

void KitFicha::changeEvent(QEvent *e)
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

void KitFicha::Imprimir()
{    
    Kit->Cargar(Kit->Codigo());

    clsImprimirKits Inf;
    Inf.ImprimirFicha(Kit);
}

void KitFicha::AbrirVentanaProd()
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
            }
            else
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
                ui->btnAniadirProd->click();
            }
        }
    }
}

void KitFicha::CargarDatos()
{
    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(3);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    if (Kit->Codigo() != 0)
    {
        ui->edtCodKit->setText(QString::number(Kit->Codigo()));
        ui->edtNombre->setText(Kit->Descripcion());

        CargarDetalles();
    }
}

void KitFicha::CargarDetalles()
{
    ui->tableDetalles->setRowCount(0);

    Kit->Cargar(Kit->Codigo());

    for (int i=0;i<Kit->Detalles().size();i++)
    {
        clsKitDetalle KitDet(Kit->Detalles().at(i));

        clsProductos product;
        product.Cargar(KitDet.CodProd());

        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        QTableWidgetItem *Prod = new QTableWidgetItem;
        Prod->setData(Qt::DisplayRole,KitDet.CodProd());
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,product.Descripcion());
        QTableWidgetItem *Canti = new QTableWidgetItem;
        Canti->setData(Qt::DisplayRole,KitDet.Cantidad());

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Prod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Canti);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void KitFicha::InsertarProducto(QString CodProd,int Cantidad)
{
    clsProductos Prod;

    if (Cantidad > 0)
    {
        Prod.Cargar(CodProd);

        clsKitDetalle KitDet(Kit->Codigo(),CodProd,0);
        KitDet.Cargar(Kit->Codigo(),CodProd);
        int CantActual = KitDet.Cantidad();

        KitDet.setCantidad(CantActual+Cantidad);

        if (KitDet.Existe())
            KitDet.Modificar();
        else
            KitDet.Insertar();

        CargarDetalles();
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad debe ser mayor que cero."));
}

void KitFicha::EliminarProducto()
{
    QModelIndex indice = ui->tableDetalles->currentIndex();
    if (indice.isValid())
    {
        QString Prod =  ui->tableDetalles->item(indice.row(),0)->data(Qt::DisplayRole).toString();

        clsKitDetalle KitDet(Kit->Codigo(),Prod,0);
        KitDet.Cargar(Kit->Codigo(),Prod);
        KitDet.Eliminar();

        CargarDetalles();
    }
}

void KitFicha::GuardarKit()
{
    Kit->setDescripcion(ui->edtNombre->text());    
    Kit->Cargar(Kit->Codigo());
    if (!Kit->Existe())
    {
        if(Kit->Insertar())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(Kit->Modificar())
            this->accept();
        else
            this->reject();
    }
}

void KitFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(KitMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnAniadirProd->setEnabled(permiso);
    ui->btnEliminarProd->setEnabled(permiso);
    ui->tableDetalles->setEnabled(permiso);
}
