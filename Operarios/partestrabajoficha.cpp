#include "partestrabajoficha.h"
#include "ui_partestrabajoficha.h"
#include "operarioslista.h"
#include "partestrabajodet.h"
#include <QCalendarWidget>
#include <QMessageBox>
#include <QPushButton>
#include "clsconfiguracion.h"
#include "clsimprimirpartes.h"

extern clsConfiguracion *config;

PartesTrabajoFicha::PartesTrabajoFicha(int _Parte,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PartesTrabajoFicha)
{
    ui->setupUi(this);

    ui->tableHoras->clear();
    ui->tableHoras->setRowCount(0);
    ui->tableHoras->setColumnCount(6);
    ui->tableHoras->setHorizontalHeaderLabels(QStringList() << QObject::tr("NIF") << QObject::tr("Operario") << QObject::tr("Tipo de hora") << QObject::tr("Nº de horas") << QObject::tr("Proyecto") << QObject::tr("Tarea"));
    ui->tableHoras->resizeColumnsToContents();
    ui->tableHoras->resizeRowsToContents();
    ui->tableHoras->sortByColumn(0);

    QCalendarWidget *FechaParte = new QCalendarWidget();
    FechaParte->setFirstDayOfWeek(Qt::Monday);
    ui->datFechaParte->setCalendarWidget(FechaParte);
    ui->datFechaParte->setDate(QDate::currentDate());

    Parte = new clsPartes_Trabajo(_Parte);

    CargarDatos();

    QPushButton *btnGuardar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(ui->btnOperarios,SIGNAL(clicked()),this,SLOT(AbrirVentanaOperarios()));
    connect(ui->btnGuardarDatos,SIGNAL(clicked()),this,SLOT(GuardarDatos()));
    connect(btnGuardar,SIGNAL(clicked()),this,SLOT(Guardar()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnAniadir,SIGNAL(clicked()),this,SLOT(AbrirVentanaAniadir()));
    connect(ui->btnModificar,SIGNAL(clicked()),this,SLOT(AbrirVentanaFicha()));
    connect(ui->btnEliminar,SIGNAL(clicked()),this,SLOT(EliminarLinea()));
    connect(ui->tableHoras,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFicha()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();

    if (_Parte == 0)
        ui->btnGuardarDatos->setEnabled(true);
    else
        ui->btnGuardarDatos->setEnabled(false);
}

PartesTrabajoFicha::~PartesTrabajoFicha()
{
    delete Operario;
    delete Parte;
    delete ui;
}

void PartesTrabajoFicha::changeEvent(QEvent *e)
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

void PartesTrabajoFicha::CargarDatos()
{
    if (Parte->NoParte() != 0)
    {
        Parte->Cargar(Parte->NoParte());

        Operario = new clsOperarios(Parte->Responsable());
        Operario->Cargar(Parte->Responsable());

        ui->edtNoParte->setText(QString::number(Parte->NoParte()));
        ui->edtDescripcion->setText(Parte->Descripcion());
        ui->edtResponsable->setText(Operario->Nombre() + " " + Operario->Apellidos());
        ui->datFechaParte->setDate(Parte->FechaParte());

        CargarLineas();

        ui->btnAniadir->setEnabled(true);
        ui->btnEliminar->setEnabled(true);
        ui->btnModificar->setEnabled(true);
    }
    else
    {
        ui->btnAniadir->setEnabled(false);
        ui->btnEliminar->setEnabled(false);
        ui->btnModificar->setEnabled(false);
    }
}

void PartesTrabajoFicha::AbrirVentanaOperarios()
{
    OperariosLista *ventanaOperario = new OperariosLista();
    ventanaOperario->setWindowModality(Qt::ApplicationModal);
    ventanaOperario->show();
    if (ventanaOperario->exec() == QDialog::Accepted)
    {
        Operario = new clsOperarios(ventanaOperario->NIFOpSeleccionado());
        Operario->Cargar(ventanaOperario->NIFOpSeleccionado());
        ui->edtResponsable->setText(Operario->Nombre() + " " + Operario->Apellidos());
    }
}

void PartesTrabajoFicha::Guardar()
{    
    if (!Parte->Existe())
    {
        if(InsertarParte())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(ModificarParte())
            this->accept();
        else
            this->reject();
    }
}

void PartesTrabajoFicha::CargarLineas()
{
    ui->tableHoras->setRowCount(0);

    for (int i=0;i<Parte->Detalles().size();i++)
    {
        ui->tableHoras->insertRow(ui->tableHoras->rowCount());

        clsOperarios Op;
        Op.Cargar(Parte->Detalles().at(i).Operario());

        QTableWidgetItem *idOp = new QTableWidgetItem;
        idOp->setData(Qt::DisplayRole,Parte->Detalles().at(i).Operario());
        QTableWidgetItem *NomAp = new QTableWidgetItem;
        NomAp->setData(Qt::DisplayRole,Op.Nombre() + " " + Op.Apellidos());
        QTableWidgetItem *THora = new QTableWidgetItem;
        THora->setData(Qt::DisplayRole,qltHora[Parte->Detalles().at(i).TipoHora()]);
        QTableWidgetItem *NHoras = new QTableWidgetItem;
        NHoras->setData(Qt::DisplayRole,QString::number(Parte->Detalles().at(i).Horas()));
        QTableWidgetItem *Proy = new QTableWidgetItem;
        Proy->setData(Qt::DisplayRole,Parte->Detalles().at(i).Proyecto());
        QTableWidgetItem *Tarea = new QTableWidgetItem;
        Tarea->setData(Qt::DisplayRole,Parte->Detalles().at(i).Tarea());

        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,0,idOp);
        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,1,NomAp);
        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,2,THora);
        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,3,NHoras);
        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,4,Proy);
        ui->tableHoras->setItem(ui->tableHoras->rowCount()-1,5,Tarea);
    }
    ui->tableHoras->resizeColumnsToContents();
    ui->tableHoras->resizeRowsToContents();
}

void PartesTrabajoFicha::GuardarDatos()
{
    if (ui->edtResponsable->text() != "")
    {
        if (!Parte->Existe())
        {
            if(InsertarParte())
                this->accept();
            else
                this->reject();
        }
        else
        {
            if(ModificarParte())
                this->accept();
            else
                this->reject();
        }

        CargarDatos();

        ui->btnGuardarDatos->setEnabled(false);
        ui->btnEliminar->setEnabled(true);
        ui->btnAniadir->setEnabled(true);
        ui->btnModificar->setEnabled(true);
    }
    else
        QMessageBox::critical(this,QObject::tr("Error al introducir datos"),QObject::tr("Debe introducir un responsable."),0,0);
}

void PartesTrabajoFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(OpMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnOperarios->setEnabled(permiso);
    ui->btnGuardarDatos->setEnabled(permiso);
    if(ui->edtNoParte->text().toInt() != 0)
    {
        ui->btnAniadir->setEnabled(permiso);
        ui->btnEliminar->setEnabled(permiso);
        ui->btnModificar->setEnabled(permiso);
    }
}

bool PartesTrabajoFicha::InsertarParte()
{
    Parte->setDescripcion(ui->edtDescripcion->text());
    Parte->setResponsable(Operario->NIF());
    Parte->setFechaParte(ui->datFechaParte->date());
    if(Parte->Insertar())
    {
        ui->edtNoParte->setText(QString::number(Parte->NoParte()));
        return true;
    }
    else
        return false;
}

bool PartesTrabajoFicha::ModificarParte()
{
    Parte->setDescripcion(ui->edtDescripcion->text());
    Parte->setResponsable(Operario->NIF());
    Parte->setFechaParte(ui->datFechaParte->date());
    if(Parte->Modificar())
        return true;
    else
        return false;
}

void PartesTrabajoFicha::AbrirVentanaAniadir()
{
    PartesTrabajoDet *ventanaDetalle = new PartesTrabajoDet(ui->edtNoParte->text().toInt());
    ventanaDetalle->setWindowModality(Qt::ApplicationModal);
    ventanaDetalle->show();
    while (ventanaDetalle->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void PartesTrabajoFicha::AbrirVentanaFicha()
{
    QString Op = "", Tarea = "", Proyecto = "";
    tHora TipoHora = Normal;
    double Horas=0;
    QModelIndex indice = ui->tableHoras->currentIndex();
    if (indice.isValid())
    {
        Op = ui->tableHoras->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        Tarea = ui->tableHoras->item(indice.row(),5)->data(Qt::DisplayRole).toString();
        Proyecto = ui->tableHoras->item(indice.row(),4)->data(Qt::DisplayRole).toString();
        if (ui->tableHoras->item(indice.row(),2)->data(Qt::DisplayRole).toString() == QObject::tr("Normal"))
            TipoHora = Normal;
        else if (ui->tableHoras->item(indice.row(),2)->data(Qt::DisplayRole).toString() == QObject::tr("Extra"))
            TipoHora = Extra;
        else
            TipoHora = Normal;
        Horas = ui->tableHoras->item(indice.row(),3)->data(Qt::DisplayRole).toDouble();
    }    

    PartesTrabajoDet *ventanaDetalle = new PartesTrabajoDet(ui->edtNoParte->text().toInt(),Op,TipoHora,Horas,Proyecto,Tarea);
    ventanaDetalle->setWindowModality(Qt::ApplicationModal);
    ventanaDetalle->show();

    while (ventanaDetalle->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void PartesTrabajoFicha::EliminarLinea()
{
    QString Op = "", Tarea = "", Proyecto = "";
    tHora TipoHora = Normal;
    QModelIndex indice = ui->tableHoras->currentIndex();
    if (indice.isValid())
    {
        Op = ui->tableHoras->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        Tarea = ui->tableHoras->item(indice.row(),5)->data(Qt::DisplayRole).toString();
        Proyecto = ui->tableHoras->item(indice.row(),4)->data(Qt::DisplayRole).toString();
        if (ui->tableHoras->item(indice.row(),2)->data(Qt::DisplayRole).toString() == QObject::tr("Normal"))
            TipoHora = Normal;
        else if (ui->tableHoras->item(indice.row(),2)->data(Qt::DisplayRole).toString() == QObject::tr("Extra"))
            TipoHora = Extra;
        else
            TipoHora = Normal;

        clsPartes_Trabajo_Det PartesDet;
        PartesDet.Cargar(ui->edtNoParte->text().toInt(),Op,TipoHora,Proyecto,Tarea);
        PartesDet.Eliminar();

        CargarLineas();
    }
}

void PartesTrabajoFicha::Imprimir()
{
    clsImprimirPartes Inf;
    Inf.ImprimirFichaPartes(Parte);
}
