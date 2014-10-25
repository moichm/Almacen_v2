#include <QMessageBox>
#include "centros.h"
#include "ui_centros.h"
#include "centrosficha.h"
#include "centro.h"
#include "clsconfiguracion.h"
#include "clsimprimircentro.h"

extern clsConfiguracion *config;

Centros::Centros(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Centros)
{
    ui->setupUi(this);

    ui->tableCentros->clear();
    ui->tableCentros->setRowCount(0);
    ui->tableCentros->setColumnCount(4);
    ui->tableCentros->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Nombre") << QObject::tr("Ciudad") << QObject::tr("Dirección"));
    ui->tableCentros->resizeColumnsToContents();
    ui->tableCentros->resizeRowsToContents();
    ui->tableCentros->sortByColumn(0);

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Código"));
    ui->cbFiltro->addItem(QObject::tr("Nombre"));
    ui->cbFiltro->addItem(QObject::tr("Ciudad"));
    ui->cbFiltro->addItem(QObject::tr("Dirección"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    CargarLineas();

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->btnNueva,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevo()));
    connect(ui->btnModificar,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->tableCentros,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->btnEliminar,SIGNAL(clicked()),this,SLOT(EliminarCentro()));
    connect(ui->btnSalir,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();
}

Centros::~Centros()
{
    delete ui;
}

void Centros::changeEvent(QEvent *e)
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

void Centros::CargarLineas()
{
    clsCentros Centro;
    QVector<QStringList> listaCentros = Centro.ListaCentros();

    ui->tableCentros->setRowCount(0);

    for (int i=0;i<listaCentros.size();i++)
    {
        QStringList strCentros = listaCentros.at(i);

        ui->tableCentros->insertRow(ui->tableCentros->rowCount());

        QTableWidgetItem *Codigo = new QTableWidgetItem;
        Codigo->setData(Qt::DisplayRole,strCentros.at(0));
        QTableWidgetItem *Nombre = new QTableWidgetItem;
        Nombre->setData(Qt::DisplayRole,strCentros.at(1));
        QTableWidgetItem *Ciudad = new QTableWidgetItem;
        Ciudad->setData(Qt::DisplayRole,strCentros.at(2));
        QTableWidgetItem *Direccion = new QTableWidgetItem;
        Direccion->setData(Qt::DisplayRole,strCentros.at(3));

        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,0,Codigo);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,1,Nombre);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,2,Ciudad);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,3,Direccion);
    }
    ui->tableCentros->resizeColumnsToContents();
    ui->tableCentros->resizeRowsToContents();
}

void Centros::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void Centros::Filtrar()
{
    clsCentros Centro;
    QString strFiltro ="";

    ui->tableCentros->setRowCount(0);

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "centros.idCentro LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "centros.ciudad LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += "centros.Direccion LIKE '%" + ui->edtFiltro->text() + "%'";

    QVector<QStringList> listaCentros = Centro.ListaCentros(strFiltro);

    for (int i=0;i<listaCentros.size();i++)
    {
        QStringList strCentros = listaCentros.at(i);

        ui->tableCentros->insertRow(ui->tableCentros->rowCount());

        QTableWidgetItem *Codigo = new QTableWidgetItem;
        Codigo->setData(Qt::DisplayRole,strCentros.at(0));
        QTableWidgetItem *Nombre = new QTableWidgetItem;
        Nombre->setData(Qt::DisplayRole,strCentros.at(1));
        QTableWidgetItem *Ciudad = new QTableWidgetItem;
        Ciudad->setData(Qt::DisplayRole,strCentros.at(2));
        QTableWidgetItem *Direccion = new QTableWidgetItem;
        Direccion->setData(Qt::DisplayRole,strCentros.at(3));

        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,0,Codigo);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,1,Nombre);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,2,Ciudad);
        ui->tableCentros->setItem(ui->tableCentros->rowCount()-1,3,Direccion);
    }
    ui->tableCentros->resizeColumnsToContents();
    ui->tableCentros->resizeRowsToContents();
}

void Centros::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(CentMod);
    ui->btnEliminar->setEnabled(permiso);
    ui->btnNueva->setEnabled(permiso);
}

void Centros::EliminarCentro()
{
    QModelIndex indice = ui->tableCentros->currentIndex();
    if (indice.isValid())
    {
        clsCentros Centro;
        Centro.Cargar(ui->tableCentros->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        Centro.Eliminar();

        CargarLineas();
    }
}

void Centros::AbrirVentanaFichaMod()
{
    QModelIndex indice = ui->tableCentros->currentIndex();
    if (indice.isValid())
    {
        CentrosFicha *ventanaFicha = new CentrosFicha(ui->tableCentros->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar centro"),QObject::tr("Debe seleccionar un centro a modificar"),0,0);
}

void Centros::AbrirVentanaFichaNuevo()
{
    CentrosFicha *ventanaFicha = new CentrosFicha(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void Centros::Imprimir()
{
    QString strFiltro ="";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "centros.idCentro LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "centros.ciudad LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += "centros.Direccion LIKE '%" + ui->edtFiltro->text() + "%'";

    clsImprimirCentro Inf;
    Inf.ImprimirLista(strFiltro);
}
