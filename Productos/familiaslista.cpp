#include "familiaslista.h"
#include "ui_familiaslista.h"
#include "producto.h"
#include <QMessageBox>

FamiliasLista::FamiliasLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FamiliasLista)
{
    ui->setupUi(this);

    ui->tableFamilias->clear();
    ui->tableFamilias->setRowCount(0);
    ui->tableFamilias->setColumnCount(6);
    ui->tableFamilias->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Limitada") << QObject::tr("Límite entrega") << QObject::tr("Certificable") << QObject::tr("Frecuencia certificación"));
    ui->tableFamilias->resizeColumnsToContents();
    ui->tableFamilias->resizeRowsToContents();
    ui->tableFamilias->sortByColumn(0);

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Descripción"));
    ui->cbFiltro->addItem(QObject::tr("Limitada"));
    ui->cbFiltro->addItem(QObject::tr("Límite entrega"));
    ui->cbFiltro->addItem(QObject::tr("Certificable"));
    ui->cbFiltro->addItem(QObject::tr("Frecuencia certificación"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    CargarLineas();

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
}

FamiliasLista::~FamiliasLista()
{
    delete ui;
}

void FamiliasLista::changeEvent(QEvent *e)
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

int FamiliasLista::CodFamiliaSeleccionada()
{
    QModelIndex indice = ui->tableFamilias->currentIndex();
    if (indice.isValid())
        return ui->tableFamilias->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
    else
        return 0;
}

void FamiliasLista::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void FamiliasLista::Filtrar()
{
    QString strFiltro = "";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "familias.Descripcion LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','No') LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','No') LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 5)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltro->text() + "%'";

    clsFamilias Familia;
    QVector<QStringList> listaFamilias = Familia.ListaFamilias();

    ui->tableFamilias->setRowCount(0);

    for (int i=0;i<listaFamilias.size();i++)
    {
        QStringList strFamilias = listaFamilias.at(i);

        ui->tableFamilias->insertRow(ui->tableFamilias->rowCount());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strFamilias.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strFamilias.at(1));
        QTableWidgetItem *Limi = new QTableWidgetItem;
        Limi->setData(Qt::DisplayRole,strFamilias.at(2));
        QTableWidgetItem *LimEnt = new QTableWidgetItem;
        LimEnt->setData(Qt::DisplayRole,strFamilias.at(3));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strFamilias.at(4));
        QTableWidgetItem *FrecCert = new QTableWidgetItem;
        FrecCert->setData(Qt::DisplayRole,strFamilias.at(5));

        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,0,Cod);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,1,Desc);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,2,Limi);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,3,LimEnt);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,4,Cert);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,5,FrecCert);
    }
    ui->tableFamilias->resizeColumnsToContents();
    ui->tableFamilias->resizeRowsToContents();
}

void FamiliasLista::CargarLineas()
{
    clsFamilias Familia;
    QVector<QStringList> listaFamilias = Familia.ListaFamilias();

    ui->tableFamilias->setRowCount(0);

    for (int i=0;i<listaFamilias.size();i++)
    {
        QStringList strFamilias = listaFamilias.at(i);

        ui->tableFamilias->insertRow(ui->tableFamilias->rowCount());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strFamilias.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strFamilias.at(1));
        QTableWidgetItem *Limi = new QTableWidgetItem;
        Limi->setData(Qt::DisplayRole,strFamilias.at(2));
        QTableWidgetItem *LimEnt = new QTableWidgetItem;
        LimEnt->setData(Qt::DisplayRole,strFamilias.at(3));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strFamilias.at(4));
        QTableWidgetItem *FrecCert = new QTableWidgetItem;
        FrecCert->setData(Qt::DisplayRole,strFamilias.at(5));

        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,0,Cod);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,1,Desc);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,2,Limi);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,3,LimEnt);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,4,Cert);
        ui->tableFamilias->setItem(ui->tableFamilias->rowCount()-1,5,FrecCert);
    }
    ui->tableFamilias->resizeColumnsToContents();
    ui->tableFamilias->resizeRowsToContents();
}
