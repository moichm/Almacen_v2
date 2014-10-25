#include "vehiculosficha.h"
#include "ui_vehiculosficha.h"
#include "../Centros/centroslista.h"
#include <QCalendarWidget>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include "clsconfiguracion.h"
#include "clsimprimirvehiculos.h"
#include "../clsImagen.h"

extern clsConfiguracion *config;

VehiculosFicha::VehiculosFicha(QString _Matricula,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehiculosFicha)
{
    ui->setupUi(this);

    ui->tableDetalle->clear();
    ui->tableDetalle->setRowCount(0);
    ui->tableDetalle->setColumnCount(3);
    ui->tableDetalle->setHorizontalHeaderLabels(QStringList() << QObject::tr("Fecha") << QObject::tr("Centro") << QObject::tr("Kms Finales"));
    ui->tableDetalle->resizeColumnsToContents();
    ui->tableDetalle->resizeRowsToContents();
    ui->tableDetalle->sortByColumn(0);

    QCalendarWidget *FechaCompra = new QCalendarWidget();
    FechaCompra->setFirstDayOfWeek(Qt::Monday);
    ui->datFechaMatriculacion->setCalendarWidget(FechaCompra);
    ui->datFechaMatriculacion->setDate(QDate::currentDate());

    ui->datFechaITV->setReadOnly(true);
    ui->datFechaITV->setDate(QDate::currentDate());

    Vehiculo = new clsVehiculos(_Matricula);

    Centro = new clsCentros();

    CargarDatos();

    ModCentro = false;

    connect(ui->btnCentro,SIGNAL(clicked()),this,SLOT(AbrirVentanaCentros()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));
    connect(ui->btnExaminar,SIGNAL(clicked()),this,SLOT(AbrirImagenNueva()));

    DeshabilitarControles();
}

VehiculosFicha::~VehiculosFicha()
{
    delete Centro;
    delete Vehiculo;
    delete ui;
}

void VehiculosFicha::changeEvent(QEvent *e)
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

void VehiculosFicha::CargarDatos()
{
    if (Vehiculo->Matricula() == "")
        ui->edtMatricula->setReadOnly(false);
    else
    {
        ui->edtMatricula->setReadOnly(true);

        Vehiculo->Cargar(Vehiculo->Matricula());

        Centro->Cargar(Vehiculo->Centro());

        ui->edtMatricula->setText(Vehiculo->Matricula());
        ui->edtMarca->setText(Vehiculo->Marca());
        ui->edtModelo->setText(Vehiculo->Modelo());
        ui->datFechaMatriculacion->setDate(Vehiculo->FechaMatriculacion());
        ui->datFechaITV->setDate(Vehiculo->FechaITV());
        ui->edtCentro->setText(Centro->Nombre());

        CargarLineas();

        QString rutaimgVehi = Vehiculo->Imagen();
        if (rutaimgVehi == "")
        {
            QFile file;
            if (file.exists(qApp->applicationDirPath()+"/Imagenes/NoPicture.png"))
            {
                clsImagen *clsImg = new clsImagen(qApp->applicationDirPath()+"/Imagenes/NoPicture.png",ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
                ui->scAreaImagen->setWidget(clsImg);
            }
        }
        else
        {
            QFile file;
            if (file.exists(rutaimgVehi))
            {
                clsImagen *clsImg = new clsImagen(rutaimgVehi,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
                ui->scAreaImagen->setWidget(clsImg);
            }
        }
    }
}

void VehiculosFicha::AbrirVentanaCentros()
{
    CentrosLista *ventanaCentro = new CentrosLista();
    ventanaCentro->setWindowModality(Qt::ApplicationModal);
    ventanaCentro->show();
    if (ventanaCentro->exec() == QDialog::Accepted)
    {
        if (ventanaCentro->CodigoCentroSeleccionado() != Vehiculo->Centro())
        {
            ModCentro = true;
            Vehiculo->setCentro(ventanaCentro->CodigoCentroSeleccionado());
            Centro->Cargar(Vehiculo->Centro());
            ui->edtCentro->setText(Centro->Nombre());
        }
    }
}

void VehiculosFicha::Guardar()
{
    Vehiculo->setMatricula(ui->edtMatricula->text());
    Vehiculo->setMarca(ui->edtMarca->text());
    Vehiculo->setModelo(ui->edtModelo->text());
    Vehiculo->setFechaMatriculacion(ui->datFechaMatriculacion->date());

    clsVehiculo_Movs MovVehiculo(Vehiculo->Centro(),ui->edtMatricula->text());

    if (!Vehiculo->Existe())
    {
        if (Vehiculo->Insertar())
        {
            if (ModCentro)
            {
                if(MovVehiculo.Insertar())
                    this->accept();
                else
                    this->reject();
            }
            else
                this->accept();
        }
        else
            this->reject();
    }
    else
    {
        if (Vehiculo->Modificar())
        {
            if (ModCentro)
            {
                if(MovVehiculo.Insertar())
                    this->accept();
                else
                    this->reject();
            }
            else
                this->accept();
        }
        else
            this->reject();
    }
}

void VehiculosFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(VehMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
    ui->btnCentro->setEnabled(permiso);
}

void VehiculosFicha::CargarLineas()
{
    clsCentros Centro;

    for(int i = 0;i<Vehiculo->Movimientos().size();i++)
    {
        ui->tableDetalle->insertRow(ui->tableDetalle->rowCount());

        clsVehiculo_Movs Vehi_Mov(Vehiculo->Movimientos().at(i));
        Centro.Cargar(Vehi_Mov.Centro());

        QTableWidgetItem *FechaMov = new QTableWidgetItem;
        FechaMov->setData(Qt::DisplayRole,Vehi_Mov.FechaMovimiento().toString(QObject::tr("dd/MM/yyyy")));
        FechaMov->setFlags(FechaMov->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,Centro.Nombre() + " - " + Centro.Ciudad());
        Cent->setFlags(Cent->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *KmFin = new QTableWidgetItem;
        KmFin->setData(Qt::DisplayRole,Vehi_Mov.KmsFinales());
        KmFin->setFlags(KmFin->flags() & (~Qt::ItemIsEditable));

        ui->tableDetalle->setItem(ui->tableDetalle->rowCount()-1,0,FechaMov);
        ui->tableDetalle->setItem(ui->tableDetalle->rowCount()-1,1,Cent);
        ui->tableDetalle->setItem(ui->tableDetalle->rowCount()-1,2,KmFin);
    }

    ui->tableDetalle->resizeColumnsToContents();
    ui->tableDetalle->resizeRowsToContents();
}

void VehiculosFicha::Imprimir()
{
    clsImprimirVehiculos Inf;
    Inf.ImprimirFicha(Vehiculo);
}

void VehiculosFicha::AbrirImagenNueva()
{
    QString rutaAplicacion = qApp->applicationDirPath();
    QString nomArchivo = QFileDialog::getOpenFileName(this,QObject::tr("Abrir imagen"), rutaAplicacion+"/Imagenes", QObject::tr("Archivos de imagen (*.png *.jpg *.bmp)"));
    Vehiculo->setImagen(nomArchivo);
    clsImagen *clsImg = new clsImagen(nomArchivo,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
    ui->scAreaImagen->setWidget(clsImg);
}
