#include "movimientosdevoluciones.h"
#include "ui_movimientosdevoluciones.h"
#include <QVariant>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>

MovimientosDevoluciones::MovimientosDevoluciones(int NSalida,QString codProd,QDate _Fecha,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosDevoluciones)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(3);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Devueltas") << QObject::tr("Perdidas") << QObject::tr("Fecha entrada"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    ui->datFechaSalida->setReadOnly(true);

    CargarDatos(NSalida,codProd,_Fecha);

    QPushButton *btnAceptar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Close);
    connect(btnAceptar,SIGNAL(clicked()),this,SLOT(Aceptar()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));
}

MovimientosDevoluciones::~MovimientosDevoluciones()
{
    delete SalDet;
    delete ui;
}

void MovimientosDevoluciones::changeEvent(QEvent *e)
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

void MovimientosDevoluciones::CargarDatos(int NSal,QString _CodProd,QDate _FechaSal)
{
    clsProductos Prod;
    Prod.Cargar(_CodProd);

    ui->edtNSalida->setText(QString::number(NSal));
    ui->edtNomProd->setText(Prod.Descripcion());
    ui->edtCodProd->setText(_CodProd);
    ui->datFechaSalida->setDate(_FechaSal);

    clsMovimiento_Salida Sal;

    Sal.Cargar(NSal);
    SalDet = new clsMovimiento_DetSal();
    SalDet->Cargar(NSal,_CodProd,_FechaSal);

    int intPerdidas=0,intDevueltas=0;

    ui->edtCantidad->setText(QString::number(SalDet->Cantidad()));

    for (int i=0;i<SalDet->DetallesEntrada().count();i++)
    {
        intDevueltas += SalDet->DetallesEntrada().at(i).Devueltas();
        intPerdidas += SalDet->DetallesEntrada().at(i).Perdidas();
    }

    ui->edtDevueltas->setText(QString::number(intDevueltas));
    ui->edtPerdidas->setText(QString::number(intPerdidas));
    ui->edtCantPend->setText(QString::number(SalDet->Cantidad()-intPerdidas-intDevueltas));
    CargarDetalles();
}

void MovimientosDevoluciones::CargarDetalles()
{
    ui->tableDetalles->setRowCount(0);

    for (int i=0;i<SalDet->DetallesEntrada().size();i++)
    {
        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        QTableWidgetItem *Dev = new QTableWidgetItem;
        Dev->setData(Qt::DisplayRole,SalDet->DetallesEntrada().at(i).Devueltas());
        QTableWidgetItem *Perd = new QTableWidgetItem;
        Perd->setData(Qt::DisplayRole,SalDet->DetallesEntrada().at(i).Perdidas());
        QTableWidgetItem *FEnt = new QTableWidgetItem;
        FEnt->setData(Qt::DisplayRole,SalDet->DetallesEntrada().at(i).Fecha_Entrada().toString(QObject::tr("dd/MM/yyyy")));

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Dev);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Perd);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,FEnt);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

int MovimientosDevoluciones::CantidadPerdida()
{
    return ui->spbCantPerd->value();
}

int MovimientosDevoluciones::CantidadDevuelta()
{
    return ui->spbCantDev->value();
}

void MovimientosDevoluciones::Aceptar()
{
    if (ui->edtCantidad->text().toInt() >= ui->spbCantDev->value()+ui->spbCantPerd->value()+ui->edtDevueltas->text().toInt()+ui->edtPerdidas->text().toInt())
        this->accept();
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("La cantidad pendiente de devolución debe ser mayor o igual que la cantidad a devolver más la cantidad perdida."));
        this->reject();
    }
}
