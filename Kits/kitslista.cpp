#include "kitslista.h"
#include "ui_kitslista.h"
#include "clsconfiguracion.h"
#include "Productos/producto.h"
#include <QMessageBox>

KitsLista::KitsLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KitsLista)
{
    ui->setupUi(this);

    ui->tableKits->clear();
    ui->tableKits->setRowCount(0);
    ui->tableKits->setColumnCount(2);
    ui->tableKits->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción"));
    ui->tableKits->resizeColumnsToContents();
    ui->tableKits->resizeRowsToContents();
    ui->tableKits->sortByColumn(0);

    ui->tableKitsDetalles->clear();
    ui->tableKitsDetalles->setRowCount(0);
    ui->tableKitsDetalles->setColumnCount(3);
    ui->tableKitsDetalles->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Cantidad"));
    ui->tableKitsDetalles->resizeColumnsToContents();
    ui->tableKitsDetalles->resizeRowsToContents();
    ui->tableKitsDetalles->sortByColumn(0);

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Descripción"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    CargarLineas();

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->tableKits,SIGNAL(clicked(QModelIndex)),this,SLOT(CargarLineasDetalles()));
    connect(ui->tableKits,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(accept()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));
}

KitsLista::~KitsLista()
{
    delete ui;
}

void KitsLista::changeEvent(QEvent *e)
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

int KitsLista::CodKitSeleccionado()
{
    QModelIndex indice = ui->tableKits->currentIndex();
    if (indice.isValid())
        return ui->tableKits->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
    else
        return 0;
}

void KitsLista::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void KitsLista::Filtrar()
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

void KitsLista::CargarLineas()
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

void KitsLista::CargarLineasDetalles()
{
    clsKits Kit;
    clsProductos Prod;

    ui->tableKitsDetalles->setRowCount(0);

    QModelIndex indice = ui->tableKits->currentIndex();    
    if (indice.isValid())
    {
        Kit.Cargar(ui->tableKits->item(indice.row(),0)->data(Qt::DisplayRole).toInt());

        for (int i=0;i<Kit.Detalles().size();i++)
        {
            ui->tableKitsDetalles->insertRow(ui->tableKitsDetalles->rowCount());

            Prod.Cargar(Kit.Detalles().at(i).CodProd());

            QTableWidgetItem *Codigo = new QTableWidgetItem;
            Codigo->setData(Qt::DisplayRole,Kit.Detalles().at(i).CodProd());
            QTableWidgetItem *Desc = new QTableWidgetItem;
            Desc->setData(Qt::DisplayRole,Prod.Descripcion());
            QTableWidgetItem *Cant = new QTableWidgetItem;
            Cant->setData(Qt::DisplayRole,Kit.Detalles().at(i).Cantidad());

            ui->tableKitsDetalles->setItem(ui->tableKitsDetalles->rowCount()-1,0,Codigo);
            ui->tableKitsDetalles->setItem(ui->tableKitsDetalles->rowCount()-1,1,Desc);
            ui->tableKitsDetalles->setItem(ui->tableKitsDetalles->rowCount()-1,2,Cant);
        }
        ui->tableKitsDetalles->resizeColumnsToContents();
        ui->tableKitsDetalles->resizeRowsToContents();
    }
}
