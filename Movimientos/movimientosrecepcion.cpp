#include "movimientosrecepcion.h"
#include "ui_movimientosrecepcion.h"
#include "Productos/producto.h"
#include <QPushButton>
#include "clsconfiguracion.h"
#include <QMessageBox>
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosRecepcion::MovimientosRecepcion(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosRecepcion)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(5);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad")  << QObject::tr("Fecha Envío") << QObject::tr("Recibido"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    Envio = new clsMovimiento_Envio(codigo);

    Centro = new clsCentros;

    CargarDatos();

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));
    connect(ui->btnRecibir,SIGNAL(clicked()),this,SLOT(Recibir()));
    connect(ui->btnRecibirTodo,SIGNAL(clicked()),this,SLOT(RecibirTodo()));

    DeshabilitarControles();
}

MovimientosRecepcion::~MovimientosRecepcion()
{
    delete Centro;
    delete Envio;
    delete ui;
}

void MovimientosRecepcion::changeEvent(QEvent *e)
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

void MovimientosRecepcion::Imprimir()
{
    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaRecepcion(Envio);
}

void MovimientosRecepcion::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnRecibir->setEnabled(permiso);
    ui->btnRecibirTodo->setEnabled(permiso);
}

void MovimientosRecepcion::CargarDatos()
{
    Envio->Cargar(Envio->NoEnvio());

    Centro->Cargar(Envio->CentroDestino());
    ui->edtCentDest->setText(Centro->Nombre());

    Centro->Cargar(Envio->CentroOrigen());
    ui->edtCentroOri->setText(Centro->Nombre());

    ui->edtNRecep->setText(QString::number(Envio->NoEnvio()));
    ui->edtFechaEnvio->setText(Envio->FechaCreacion().toString(QObject::tr("dd/MM/yyyy")));

    ui->ptedtComentarios->setPlainText(Envio->Comentarios());

    CargarDetalles();
}

void MovimientosRecepcion::CargarDetalles()
{
    for (int i=0;i<Envio->Detalles().size();i++)
    {
        QString strProd = Envio->Detalles().at(i).CodProd();

        int Cant = Envio->Detalles().at(i).Cantidad();
        QDate FechaEnv = Envio->Detalles().at(i).FechaEnvio();
        bool boRecep = Envio->Detalles().at(i).Recibida();

        clsProductos product;
        product.Cargar(strProd);

        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        QTableWidgetItem *Prod = new QTableWidgetItem;
        Prod->setData(Qt::DisplayRole,strProd);
        Prod->setFlags(Prod->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,product.Descripcion());
        Desc->setFlags(Desc->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Canti = new QTableWidgetItem;
        Canti->setData(Qt::DisplayRole,Cant);
        Canti->setFlags(Canti->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *FEnv = new QTableWidgetItem;
        FEnv->setData(Qt::DisplayRole,FechaEnv.toString(QObject::tr("dd/MM/yyyy")));
        FEnv->setFlags(FEnv->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Recep = new QTableWidgetItem;
        if(boRecep)
            Recep->setData(Qt::DisplayRole,QObject::tr("Sí"));
        else
            Recep->setData(Qt::DisplayRole,QObject::tr("No"));
        Recep->setFlags(Recep->flags() & (~Qt::ItemIsEditable));

        ui->tableDetalles->blockSignals(true);

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Prod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Canti);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,3,FEnv);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,4,Recep);

        ui->tableDetalles->blockSignals(false);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void MovimientosRecepcion::Recibir()
{
    QModelIndex indice = ui->tableDetalles->currentIndex();
    if (indice.isValid())
    {
        QString CodProd = "";
        QDate datFechaEnv;
        CodProd = ui->tableDetalles->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        QStringList strFechaEnv = ui->tableDetalles->item(indice.row(),3)->data(Qt::DisplayRole).toString().split("/");
        if(strFechaEnv.at(2).length() == 4)
            datFechaEnv = QDate::fromString(strFechaEnv.at(0)+strFechaEnv.at(1)+strFechaEnv.at(2),"ddMMyyyy");
        else
            datFechaEnv = QDate::fromString(strFechaEnv.at(2)+strFechaEnv.at(1)+strFechaEnv.at(0),"yyyyMMdd");

        clsMovimiento_DetEnv DetEnv;
        DetEnv.Cargar(ui->edtNRecep->text().toInt(),CodProd,datFechaEnv);
        if(!DetEnv.Recibida())
        {
            if(!DetEnv.Recibir())
                QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("No se ha podido recibir el producto ") + CodProd + ("."));
        }
        else
            QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("El producto ") + CodProd + QObject::tr(" ya se ha recibido."));

        CargarDetalles();
    }
}

void MovimientosRecepcion::RecibirTodo()
{
    for (int i=0;i<ui->tableDetalles->rowCount();i++)
    {
        QString CodProd = "";
        QDate datFechaEnv;
        CodProd = ui->tableDetalles->item(i,0)->data(Qt::DisplayRole).toString();
        QStringList strFechaEnv = ui->tableDetalles->item(i,3)->data(Qt::DisplayRole).toString().split("/");
        if(strFechaEnv.at(2).length() == 4)
            datFechaEnv = QDate::fromString(strFechaEnv.at(0)+strFechaEnv.at(1)+strFechaEnv.at(2),"ddMMyyyy");
        else
            datFechaEnv = QDate::fromString(strFechaEnv.at(2)+strFechaEnv.at(1)+strFechaEnv.at(0),"yyyyMMdd");

        clsMovimiento_DetEnv DetEnv;
        DetEnv.Cargar(ui->edtNRecep->text().toInt(),CodProd,datFechaEnv);
        if(!DetEnv.Recibida())
        {
            if(!DetEnv.Recibir())
                QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("No se ha podido recibir el producto ") + CodProd + ("."));
        }
        else
            QMessageBox::critical(0,QObject::tr("Error"),QObject::tr("El producto ") + CodProd + QObject::tr(" ya se ha recibido."));
    }

    CargarDetalles();
}
