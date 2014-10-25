#include "categoriaslista.h"
#include "ui_categoriaslista.h"
#include "producto.h"
#include <QMessageBox>

CategoriasLista::CategoriasLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoriasLista)
{
    ui->setupUi(this);

    ui->tableCategorias->clear();
    ui->tableCategorias->setRowCount(0);
    ui->tableCategorias->setColumnCount(2);
    ui->tableCategorias->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción"));
    ui->tableCategorias->resizeColumnsToContents();
    ui->tableCategorias->resizeRowsToContents();
    ui->tableCategorias->sortByColumn(0);

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Descripción"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    CargarLineas();

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
}

CategoriasLista::~CategoriasLista()
{
    delete ui;
}

void CategoriasLista::changeEvent(QEvent *e)
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

int CategoriasLista::CodCatSeleccionado()
{
    QModelIndex indice = ui->tableCategorias->currentIndex();
    if (indice.isValid())
        return ui->tableCategorias->item(indice.row(),0)->data(Qt::DisplayRole).toInt();
    else
        return 0;
}

void CategoriasLista::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void CategoriasLista::Filtrar()
{
    QString strFiltro = "";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "categorias.Descripcion LIKE '%" + ui->edtFiltro->text() + "%'";

    clsCategorias Categoria;
    QVector<QStringList> listaCategorias = Categoria.ListaCategorias(strFiltro);

    ui->tableCategorias->setRowCount(0);

    for (int i=0;i<listaCategorias.size();i++)
    {
        QStringList strCategorias = listaCategorias.at(i);

        ui->tableCategorias->insertRow(ui->tableCategorias->rowCount());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strCategorias.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strCategorias.at(1));

        ui->tableCategorias->setItem(ui->tableCategorias->rowCount()-1,0,Cod);
        ui->tableCategorias->setItem(ui->tableCategorias->rowCount()-1,1,Desc);
    }
    ui->tableCategorias->resizeColumnsToContents();
    ui->tableCategorias->resizeRowsToContents();
}

void CategoriasLista::CargarLineas()
{
    clsCategorias Categoria;
    QVector<QStringList> listaCategorias = Categoria.ListaCategorias();

    ui->tableCategorias->setRowCount(0);

    for (int i=0;i<listaCategorias.size();i++)
    {
        QStringList strCategorias = listaCategorias.at(i);

        ui->tableCategorias->insertRow(ui->tableCategorias->rowCount());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strCategorias.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strCategorias.at(1));

        ui->tableCategorias->setItem(ui->tableCategorias->rowCount()-1,0,Cod);
        ui->tableCategorias->setItem(ui->tableCategorias->rowCount()-1,1,Desc);
    }
    ui->tableCategorias->resizeColumnsToContents();
    ui->tableCategorias->resizeRowsToContents();
}
