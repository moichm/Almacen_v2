#include "kits.h"
#include "ui_kits.h"
#include "kitficha.h"
#include "kit.h"
#include "clsconfiguracion.h"
#include "clsimprimirkits.h"
#include <QMessageBox>

extern clsConfiguracion *config;

Kits::Kits(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kits)
{
    ui->setupUi(this);

    ui->tableKits->clear();
    ui->tableKits->setRowCount(0);
    ui->tableKits->setColumnCount(2);
    ui->tableKits->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción"));
    ui->tableKits->resizeColumnsToContents();
    ui->tableKits->resizeRowsToContents();
    ui->tableKits->sortByColumn(0);

    CargarLineas();

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Descripción"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));

    connect(ui->btnNuevo,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevo()));
    connect(ui->btnModificar,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->btnSalir,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->tableKits,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaMod()));
    connect(ui->btnEliminar,SIGNAL(clicked()),this,SLOT(Eliminar()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(Imprimir()));

    DeshabilitarControles();
}

Kits::~Kits()
{
    delete ui;
}

void Kits::changeEvent(QEvent *e)
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

void Kits::AbrirVentanaFichaMod()
{
    int Kit = 0;
    QModelIndex indice = ui->tableKits->currentIndex();
    if (indice.isValid())
    {
        Kit = ui->tableKits->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        KitFicha *ventanaFicha = new KitFicha(Kit);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar kit"),QObject::tr("Debe seleccionar un kit a modificar"),0,0);
}

void Kits::AbrirVentanaFichaNuevo()
{
    KitFicha *ventanaFicha = new KitFicha(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void Kits::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void Kits::Filtrar()
{
    QString strFiltro ="";

    ui->tableKits->setRowCount(0);

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "kits.descripcion LIKE '%" + ui->edtFiltro->text() + "%'";

    clsKits clsKit;
    QVector<QStringList> listaKit = clsKit.ListaKits(strFiltro);

    ui->tableKits->setRowCount(0);

    for (int i=0;i<listaKit.size();i++)
    {
        QStringList strKits = listaKit.at(i);

        ui->tableKits->insertRow(ui->tableKits->rowCount());

        QTableWidgetItem *Codigo = new QTableWidgetItem;
        Codigo->setData(Qt::DisplayRole,strKits.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strKits.at(1));

        ui->tableKits->setItem(ui->tableKits->rowCount()-1,0,Codigo);
        ui->tableKits->setItem(ui->tableKits->rowCount()-1,1,Desc);
    }
    ui->tableKits->resizeColumnsToContents();
    ui->tableKits->resizeRowsToContents();
}

void Kits::CargarLineas()
{
    clsKits clsKit;
    QVector<QStringList> listaKit = clsKit.ListaKits();

    ui->tableKits->setRowCount(0);

    for (int i=0;i<listaKit.size();i++)
    {
        QStringList strKits = listaKit.at(i);

        ui->tableKits->insertRow(ui->tableKits->rowCount());

        QTableWidgetItem *Codigo = new QTableWidgetItem;
        Codigo->setData(Qt::DisplayRole,strKits.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strKits.at(1));

        ui->tableKits->setItem(ui->tableKits->rowCount()-1,0,Codigo);
        ui->tableKits->setItem(ui->tableKits->rowCount()-1,1,Desc);
    }
    ui->tableKits->resizeColumnsToContents();
    ui->tableKits->resizeRowsToContents();
}

void Kits::Eliminar()
{
    QModelIndex indice = ui->tableKits->currentIndex();
    if (indice.isValid())
    {
        clsKits Kit;
        Kit.Cargar(ui->tableKits->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        Kit.Eliminar();
        CargarLineas();
    }
}

void Kits::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(KitMod);
    ui->btnEliminar->setEnabled(permiso);
    ui->btnNuevo->setEnabled(permiso);
}

void Kits::Imprimir()
{
    QString strFiltro ="";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "kits.descripcion LIKE '%" + ui->edtFiltro->text() + "%'";

    clsImprimirKits Inf;
    Inf.ImprimirLista(strFiltro);
}
