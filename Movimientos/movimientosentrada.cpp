#include "movimientosentrada.h"
#include "ui_movimientosentrada.h"
#include "movimientosdevoluciones.h"
#include "../clsconfiguracion.h"
#include "../Operarios/operario.h"
#include "../Centros/centro.h"
#include <QVariant>
#include <QMessageBox>
#include "clsimprimirmovimientos.h"

extern clsConfiguracion *config;

MovimientosEntrada::MovimientosEntrada(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovimientosEntrada)
{
    ui->setupUi(this);

    ui->tableDetalles->clear();
    ui->tableDetalles->setRowCount(0);
    ui->tableDetalles->setColumnCount(6);
    ui->tableDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad") << QObject::tr("Fecha salida") << QObject::tr("Devueltas") << QObject::tr("Perdidas"));
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
    ui->tableDetalles->sortByColumn(0);

    Sal = new clsMovimiento_Salida(codigo);

    CargarDatos();

    connect(ui->btnDevolverProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaDevolucion()));
    connect(ui->tableDetalles,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaDevolucion()));
    connect(ui->btnDevolverTodo,SIGNAL(clicked()),this,SLOT(DevolverTodo()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();
}

MovimientosEntrada::~MovimientosEntrada()
{
    delete Sal;
    delete ui;
}

void MovimientosEntrada::changeEvent(QEvent *e)
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

void MovimientosEntrada::Imprimir()
{
    clsImprimirMovimientos Inf;
    Inf.ImprimirFichaEntrada(Sal);
}

void MovimientosEntrada::CargarDatos()
{
    clsCentros Centro;
    Centro.Cargar(config->IdCentroActual());

    ui->edtCentro->setText(Centro.Nombre());

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

void MovimientosEntrada::CargarDetalles()
{
    ui->tableDetalles->setRowCount(0);

    for (int i=0;i<Sal->DetallesSalida().size();i++)
    {
        ui->tableDetalles->insertRow(ui->tableDetalles->rowCount());

        clsProductos Prod;
        Prod.Cargar(Sal->DetallesSalida().at(i).CodProd());

        int Devueltas=0,Perdidas=0;
        clsMovimiento_DetSal DetSal(Sal->DetallesSalida().at(i));
        DetSal.Cargar(Sal->NoSalida(),Prod.Codigo(),DetSal.Fecha_Salida());
        for(int j=0;j<DetSal.DetallesEntrada().size();j++)
        {
            Devueltas += DetSal.DetallesEntrada().at(j).Devueltas();
            Perdidas += DetSal.DetallesEntrada().at(j).Perdidas();
        }

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).CodProd());
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,Prod.Descripcion());
        QTableWidgetItem *Cant = new QTableWidgetItem;
        Cant->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).Cantidad());
        QTableWidgetItem *FSal = new QTableWidgetItem;
        FSal->setData(Qt::DisplayRole,Sal->DetallesSalida().at(i).Fecha_Salida().toString(QObject::tr("dd/MM/yyyy")));
        QTableWidgetItem *Dev = new QTableWidgetItem;
        Dev->setData(Qt::DisplayRole,Devueltas);
        QTableWidgetItem *Perd = new QTableWidgetItem;
        Perd->setData(Qt::DisplayRole,Perdidas);

        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,0,Cod);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,1,Desc);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,2,Cant);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,3,FSal);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,4,Dev);
        ui->tableDetalles->setItem(ui->tableDetalles->rowCount()-1,5,Perd);
    }
    ui->tableDetalles->resizeColumnsToContents();
    ui->tableDetalles->resizeRowsToContents();
}

void MovimientosEntrada::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(MovMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnDevolverTodo->setEnabled(permiso);
    ui->btnDevolverProd->setEnabled(permiso);
}

void MovimientosEntrada::AbrirVentanaDevolucion()
{
    QString CodProd = "";
    QDate datFechaSal;
    QModelIndex indice = ui->tableDetalles->currentIndex();
    if (indice.isValid())
    {
        CodProd = ui->tableDetalles->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        QStringList strFechaSal = ui->tableDetalles->item(indice.row(),3)->data(Qt::DisplayRole).toString().split("/");
        if(strFechaSal.at(2).length() == 4)
            datFechaSal = QDate::fromString(strFechaSal.at(0)+strFechaSal.at(1)+strFechaSal.at(2),"ddMMyyyy");
        else
            datFechaSal = QDate::fromString(strFechaSal.at(2)+strFechaSal.at(1)+strFechaSal.at(0),"yyyyMMdd");

        MovimientosDevoluciones *ventanaDev = new MovimientosDevoluciones(ui->edtNSalida->text().toInt(),CodProd,datFechaSal);
        ventanaDev->setWindowModality(Qt::ApplicationModal);
        ventanaDev->show();
        if (ventanaDev->exec() == QDialog::Accepted)
        {
            int intCantDev = ventanaDev->CantidadDevuelta();
            int intCantPerd = ventanaDev->CantidadPerdida();

            if(intCantDev + intCantPerd > 0)
            {
                clsMovimiento_DetEnt DetEnt(ui->edtNSalida->text().toInt(),intCantDev,intCantPerd,CodProd,QDate::currentDate(),datFechaSal);

                DetEnt.Cargar(ui->edtNSalida->text().toInt(),CodProd,datFechaSal,QDate::currentDate());
                if (!DetEnt.Existe())
                {
                    DetEnt.setDevueltas(intCantDev);
                    DetEnt.setPerdidas(intCantPerd);
                    if(!DetEnt.Insertar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a devolver del producto ") + CodProd + ("."));
                }
                else
                {
                    DetEnt.setPerdidas(DetEnt.Perdidas()+intCantPerd);
                    DetEnt.setDevueltas(DetEnt.Devueltas()+intCantDev);
                    if(!DetEnt.Modificar())
                        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a devolver del producto ") + CodProd + ("."));
                }
            }
        }
    }
}

void MovimientosEntrada::DevolverTodo()
{
    for (int i=0;i<ui->tableDetalles->rowCount();i++)
    {
        QString producto = ui->tableDetalles->item(i,0)->data(Qt::DisplayRole).toString();
        int Cant = ui->tableDetalles->item(i,2)->data(Qt::DisplayRole).toInt();
        QDate datFechaSal;
        QStringList strFecha = ui->tableDetalles->item(i,3)->data(Qt::DisplayRole).toString().split("/");
        if(strFecha.at(2).length() == 4)
            datFechaSal = QDate::fromString(strFecha.at(0)+strFecha.at(1)+strFecha.at(2),"ddMMyyyy");
        else
            datFechaSal = QDate::fromString(strFecha.at(2)+strFecha.at(1)+strFecha.at(0),"yyyyMMdd");

        clsMovimiento_DetEnt DetEnt;

        DetEnt.Cargar(ui->edtNSalida->text().toInt(),producto,datFechaSal,QDate::currentDate());
        int Dev = Cant - DetEnt.Perdidas();
        DetEnt.setDevueltas(Dev);
        if (DetEnt.Existe())
        {
            if(!DetEnt.Modificar())
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a devolver del producto ") + producto + ("."));
        }
        else
        {
            clsMovimiento_DetEnt DetEnt(ui->edtNSalida->text().toInt(),Cant,0,producto,datFechaSal,QDate::currentDate());
            if(!DetEnt.Insertar())
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("No se ha podido modificar la cantidad a devolver del producto ") + producto + ("."));
        }
    }
}
