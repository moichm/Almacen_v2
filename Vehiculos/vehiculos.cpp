#include "vehiculos.h"
#include "ui_vehiculos.h"
#include "Vehiculos/vehiculosficha.h"
#include "Vehiculos/vehiculo.h"
#include "clsconfiguracion.h"
#include "clsimprimirvehiculos.h"
#include <QMessageBox>

extern clsConfiguracion *config;

Vehiculos::Vehiculos(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Vehiculos)
{
    ui->setupUi(this);

    bool permiso = config->Permisos().value(Confi);

    ui->tableVehiculos->clear();
    ui->tableVehiculos->setRowCount(0);
    ui->tableVehiculos->setColumnCount(5);
    ui->tableVehiculos->setColumnHidden(4,!permiso);
    ui->tableVehiculos->setHorizontalHeaderLabels(QStringList() << QObject::tr("Matrícula") << QObject::tr("Marca") << QObject::tr("Modelo") << QObject::tr("Fecha Matriculación") << QObject::tr("Centro Actual"));
    ui->tableVehiculos->resizeColumnsToContents();
    ui->tableVehiculos->resizeRowsToContents();
    ui->tableVehiculos->sortByColumn(0);

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Matrícula"));
    ui->cbFiltro->addItem(QObject::tr("Marca"));
    ui->cbFiltro->addItem(QObject::tr("Modelo"));
    if(permiso)
        ui->cbFiltro->addItem(QObject::tr("Centro Actual"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    CargarLineas();

    connect(ui->btnNueva,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevo()));
    connect(ui->btnModificar,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->btnEliminar,SIGNAL(clicked()),this,SLOT(EliminarVehiculo()));
    connect(ui->tableVehiculos,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->btnSalir,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));
}

Vehiculos::~Vehiculos()
{
    delete ui;
}

void Vehiculos::changeEvent(QEvent *e)
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

void Vehiculos::CargarLineas()
{    
    bool permiso = config->Permisos().value(Confi);

    clsVehiculos Vehiculo;
    QVector<QStringList> listaVehiculos = Vehiculo.ListaVehiculos();

    ui->tableVehiculos->setRowCount(0);

    for (int i=0;i<listaVehiculos.size();i++)
    {
        QStringList strVehiculos = listaVehiculos.at(i);
        Vehiculo.Cargar(strVehiculos.at(0));

        clsCentros Centro;
        Centro.Cargar(Vehiculo.Centro());

        QTableWidgetItem *Mat = new QTableWidgetItem;
        Mat->setData(Qt::DisplayRole,strVehiculos.at(0));
        QTableWidgetItem *Marca = new QTableWidgetItem;
        Marca->setData(Qt::DisplayRole,strVehiculos.at(1));
        QTableWidgetItem *Modelo = new QTableWidgetItem;
        Modelo->setData(Qt::DisplayRole,strVehiculos.at(2));
        QTableWidgetItem *FechaMat = new QTableWidgetItem;
        FechaMat->setData(Qt::DisplayRole,strVehiculos.at(3));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,Centro.Nombre() + " - " + Centro.Ciudad());

        if((Vehiculo.Centro() == config->IdCentroActual()) || permiso)
        {
            ui->tableVehiculos->insertRow(ui->tableVehiculos->rowCount());

            ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,0,Mat);
            ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,1,Marca);
            ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,2,Modelo);
            ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,3,FechaMat);
            ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,4,Cent);
        }
    }
    ui->tableVehiculos->resizeColumnsToContents();
    ui->tableVehiculos->resizeRowsToContents();
}

void Vehiculos::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void Vehiculos::Filtrar()
{
    bool permiso = config->Permisos().value(Confi),boFiltroCentro=false;

    QString strFiltro ="";

    ui->tableVehiculos->setRowCount(0);

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "matricula LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "marca LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "modelo LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        boFiltroCentro = true;

    clsVehiculos Vehiculo;
    QVector<QStringList> listaVehiculos = Vehiculo.ListaVehiculos(strFiltro);

    for (int i=0;i<listaVehiculos.size();i++)
    {
        QStringList strVehiculos = listaVehiculos.at(i);
        Vehiculo.Cargar(strVehiculos.at(0));

        clsCentros Centro;
        Centro.Cargar(Vehiculo.Centro());
        QString strCentro = Centro.Nombre() + " - " + Centro.Ciudad();

        ui->tableVehiculos->insertRow(ui->tableVehiculos->rowCount());

        QTableWidgetItem *Mat = new QTableWidgetItem;
        Mat->setData(Qt::DisplayRole,strVehiculos.at(0));
        QTableWidgetItem *Marca = new QTableWidgetItem;
        Marca->setData(Qt::DisplayRole,strVehiculos.at(1));
        QTableWidgetItem *Modelo = new QTableWidgetItem;
        Modelo->setData(Qt::DisplayRole,strVehiculos.at(2));
        QTableWidgetItem *FechaMat = new QTableWidgetItem;
        FechaMat->setData(Qt::DisplayRole,strVehiculos.at(3));
        QTableWidgetItem *Cent = new QTableWidgetItem;
        Cent->setData(Qt::DisplayRole,strCentro);

        if((Vehiculo.Centro() == config->IdCentroActual()) || permiso)
        {
            if (boFiltroCentro)
            {
                if (strCentro.startsWith(ui->edtFiltro->text()))
                {
                    ui->tableVehiculos->insertRow(ui->tableVehiculos->rowCount());

                    ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,0,Mat);
                    ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,1,Marca);
                    ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,2,Modelo);
                    ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,3,FechaMat);
                    ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,4,Cent);
                }
            }
            else
            {
                ui->tableVehiculos->insertRow(ui->tableVehiculos->rowCount());

                ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,0,Mat);
                ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,1,Marca);
                ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,2,Modelo);
                ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,3,FechaMat);
                ui->tableVehiculos->setItem(ui->tableVehiculos->rowCount()-1,4,Cent);
            }
        }
    }
    ui->tableVehiculos->resizeColumnsToContents();
    ui->tableVehiculos->resizeRowsToContents();
}

void Vehiculos::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(VehMod);
    ui->btnEliminar->setEnabled(permiso);
    ui->btnNueva->setEnabled(permiso);
}

void Vehiculos::AbrirVentanaFichaMod()
{
    QString strVehiculo = "";
    QModelIndex indice = ui->tableVehiculos->currentIndex();
    if (indice.isValid())
    {
        strVehiculo = ui->tableVehiculos->item(indice.row(),0)->data(Qt::DisplayRole).toString();

        VehiculosFicha *ventanaFicha = new VehiculosFicha(strVehiculo);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar vehículo"),QObject::tr("Debe seleccionar un vehículo a modificar"),0,0);
}

void Vehiculos::AbrirVentanaFichaNuevo()
{
    VehiculosFicha *ventanaFicha = new VehiculosFicha();
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    if (ventanaFicha->exec() == QDialog::Accepted)
        CargarLineas();
}

void Vehiculos::EliminarVehiculo()
{
    QModelIndex indice = ui->tableVehiculos->currentIndex();
    if (indice.isValid())
    {
        clsVehiculos Vehiculo;
        Vehiculo.Cargar(ui->tableVehiculos->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        Vehiculo.Eliminar();

        CargarLineas();
    }
}

void Vehiculos::Imprimir()
{
    QString strFiltro ="";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "matricula LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "marca LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "modelo LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += "centro-|-" + ui->edtFiltro->text() + "%'";

    clsImprimirVehiculos Inf;
    Inf.ImprimirLista(strFiltro);
}
