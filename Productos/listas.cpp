#include "listas.h"
#include "ui_listas.h"
#include <QMessageBox>
#include "categoriasficha.h"
#include "familiasficha.h"
#include "productosficha.h"
#include "producto.h"
#include "clsconfiguracion.h"
#include "clsimprimirproductos.h"

extern clsConfiguracion *config;

listas::listas(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::listas)
{
    ui->setupUi(this);

    ui->tabListas->setCurrentIndex(0);

    ui->tableProductos->clear();
    ui->tableProductos->setRowCount(0);
    ui->tableProductos->setColumnCount(11);
    ui->tableProductos->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Disponible")
                                                  << QObject::tr("Categoría") << QObject::tr("Familia") << QObject::tr("Frecuencia Certificación")
                                                  << QObject::tr("Certificable") << QObject::tr("Límite entrega") << QObject::tr("Limitado") << QObject::tr("Stock Centro") << QObject::tr("Stock Total"));
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
    ui->tableProductos->sortByColumn(0);

    ui->tableCategorias->clear();
    ui->tableCategorias->setRowCount(0);
    ui->tableCategorias->setColumnCount(2);
    ui->tableCategorias->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción"));
    ui->tableCategorias->resizeColumnsToContents();
    ui->tableCategorias->resizeRowsToContents();
    ui->tableCategorias->sortByColumn(0);

    ui->tableFamilias->clear();
    ui->tableFamilias->setRowCount(0);
    ui->tableFamilias->setColumnCount(6);
    ui->tableFamilias->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Limitada") << QObject::tr("Límite entrega") << QObject::tr("Certificable") << QObject::tr("Frecuencia certificación"));
    ui->tableFamilias->resizeColumnsToContents();
    ui->tableFamilias->resizeRowsToContents();
    ui->tableFamilias->sortByColumn(0);

    ui->tableHistCert->clear();
    ui->tableHistCert->setRowCount(0);
    ui->tableHistCert->setColumnCount(4);
    ui->tableHistCert->setHorizontalHeaderLabels(QStringList() << QObject::tr("Producto") << QObject::tr("Descripción") << QObject::tr("Fecha Certificación") << QObject::tr("Emisor del certificado"));
    ui->tableHistCert->resizeColumnsToContents();
    ui->tableHistCert->resizeRowsToContents();
    ui->tableHistCert->sortByColumn(0);

    ui->cbFiltroProd->addItem(QObject::tr("Todos"));
    ui->cbFiltroProd->addItem(QObject::tr("Código"));
    ui->cbFiltroProd->addItem(QObject::tr("Descripción"));
    ui->cbFiltroProd->addItem(QObject::tr("Categoría"));
    ui->cbFiltroProd->addItem(QObject::tr("Familia"));
    ui->cbFiltroProd->addItem(QObject::tr("Disponible"));
    ui->cbFiltroProd->addItem(QObject::tr("Frecuencia Certificación"));
    ui->cbFiltroProd->addItem(QObject::tr("Certificable"));
    ui->cbFiltroProd->addItem(QObject::tr("Límite"));
    ui->cbFiltroProd->addItem(QObject::tr("Limitado"));
    ui->cbFiltroProd->addItem(QObject::tr("Stock Centro"));
    ui->cbFiltroProd->addItem(QObject::tr("Stock Total"));
    ui->cbFiltroProd->setCurrentIndex(0);
    ui->edtFiltroProd->setEnabled(false);

    ui->cbFiltroCat->addItem(QObject::tr("Todos"));
    ui->cbFiltroCat->addItem(QObject::tr("Descripción"));
    ui->cbFiltroCat->setCurrentIndex(0);
    ui->edtFiltroCat->setEnabled(false);

    ui->cbFiltroFam->addItem(QObject::tr("Todos"));
    ui->cbFiltroFam->addItem(QObject::tr("Descripción"));
    ui->cbFiltroFam->addItem(QObject::tr("Limitada"));
    ui->cbFiltroFam->addItem(QObject::tr("Límite entrega"));
    ui->cbFiltroFam->addItem(QObject::tr("Certificable"));
    ui->cbFiltroFam->addItem(QObject::tr("Frecuencia certificación"));
    ui->cbFiltroFam->setCurrentIndex(0);
    ui->edtFiltroFam->setEnabled(false);

    ui->cbFiltroHistCert->addItem(QObject::tr("Todos"));
    ui->cbFiltroHistCert->addItem(QObject::tr("Código"));
    ui->cbFiltroHistCert->addItem(QObject::tr("Descripción"));
    ui->cbFiltroHistCert->addItem(QObject::tr("Emisor"));
    ui->cbFiltroHistCert->setCurrentIndex(0);
    ui->edtFiltroHistCert->setEnabled(false);

    ui->lblCondicion->setVisible(false);
    ui->cbCondicion->setVisible(false);
    ui->cbCondicion->addItem(">");
    ui->cbCondicion->addItem("<");
    ui->cbCondicion->addItem("=");
    ui->cbCondicion->addItem("<=");
    ui->cbCondicion->addItem(">=");
    ui->cbCondicion->setCurrentIndex(0);

    CargarLineas();

    connect(ui->edtFiltroProd,SIGNAL(textChanged(QString)),this,SLOT(FiltrarProd()));
    connect(ui->cbFiltroProd,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroProd()));
    connect(ui->btnNuevaProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevoProd()));
    connect(ui->btnModificarProd,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModProd()));
    connect(ui->tableProductos,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModProd()));
    connect(ui->btnSalirProd,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnEliminarProd,SIGNAL(clicked()),this,SLOT(EliminarProducto()));
    connect(ui->btnImprimirProd,SIGNAL(clicked()),this,SLOT(ImprimirProd()));
    connect(ui->cbCondicion,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroProd()));

    connect(ui->edtFiltroCat,SIGNAL(textChanged(QString)),this,SLOT(FiltrarCat()));
    connect(ui->cbFiltroCat,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroCat()));
    connect(ui->btnNuevoCat,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevaCat()));
    connect(ui->btnModificarCat,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModCat()));
    connect(ui->tableCategorias,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModCat()));
    connect(ui->btnSalirCat,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnEliminarCat,SIGNAL(clicked()),this,SLOT(EliminarCat()));
    connect(ui->btnImprimirCat,SIGNAL(clicked()),this,SLOT(ImprimirCat()));

    connect(ui->edtFiltroFam,SIGNAL(textChanged(QString)),this,SLOT(FiltrarFam()));
    connect(ui->cbFiltroFam,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroFam()));
    connect(ui->btnNuevoFam,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevaFam()));
    connect(ui->btnModificarFam,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModFam()));
    connect(ui->tableFamilias,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModFam()));
    connect(ui->btnSalirFam,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnEliminarFam,SIGNAL(clicked()),this,SLOT(EliminarFam()));
    connect(ui->btnImprimirFam,SIGNAL(clicked()),this,SLOT(ImprimirFam()));

    connect(ui->edtFiltroHistCert,SIGNAL(textChanged(QString)),this,SLOT(FiltrarHistCert()));
    connect(ui->cbFiltroHistCert,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroHistCert()));
    connect(ui->btnSalirHistCert,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnImprimirHistCert,SIGNAL(clicked()),this,SLOT(ImprimirHistCert()));

    connect(ui->tabListas,SIGNAL(currentChanged(int)),this,SLOT(CargarLineas()));

    DeshabilitarControles();
}

listas::~listas()
{
    delete ui;
}

void listas::changeEvent(QEvent *e)
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

void listas::ImprimirProd()
{
    QString strFiltro = "";

    if (ui->cbFiltroProd->currentIndex() == 1)
        strFiltro += "productos.idproducto LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 2)
        strFiltro += "productos.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 3)
        strFiltro += "Categorias.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 4)
        strFiltro += "Familias.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 5)
        strFiltro += "IF(productos.disponible=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 6)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 7)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 8)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 9)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 10)
        strFiltro += "StockCentro-|-" + ui->cbCondicion->currentText() + "-|-" + ui->edtFiltroProd->text();
    else if (ui->cbFiltroProd->currentIndex() == 11)
        strFiltro += "StockTotal-|-" + ui->cbCondicion->currentText() + "-|-" + ui->edtFiltroProd->text();

    clsImprimirProductos Inf;
    Inf.ImprimirListaProd(strFiltro);
}

void listas::ImprimirFam()
{
    QString strFiltro = "";

    if (ui->cbFiltroFam->currentIndex() == 1)
        strFiltro += "familias.Descripcion LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 2)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 3)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 4)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 5)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltroFam->text() + "%'";

    clsImprimirProductos Inf;
    Inf.ImprimirListaFamilia(strFiltro);
}

void listas::ImprimirCat()
{
    QString strFiltro = "";

    if (ui->cbFiltroCat->currentIndex() == 1)
        strFiltro += "categorias.Descripcion LIKE '%" + ui->edtFiltroCat->text() + "%'";

    clsImprimirProductos Inf;
    Inf.ImprimirListaCategoria(strFiltro);
}

void listas::ImprimirHistCert()
{
    QString strFiltro = "";

    if (ui->cbFiltroHistCert->currentIndex() == 1)
        strFiltro += "certificaciones.idproducto LIKE '%" + ui->edtFiltroHistCert->text() + "%'";
    else if (ui->cbFiltroHistCert->currentIndex() == 2)
        strFiltro += "productos.Descripcion LIKE '%" + ui->edtFiltroHistCert->text() + "%'";
    else if (ui->cbFiltroHistCert->currentIndex() == 3)
        strFiltro += "certificaciones.emisor LIKE '%" + ui->edtFiltroHistCert->text() + "%'";

    clsImprimirProductos Inf;
    Inf.ImprimirListaHistorico(strFiltro);
}

void listas::AbrirVentanaFichaNuevoProd()
{
    ProductosFicha *ventanaFicha = new ProductosFicha("");
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    if (ventanaFicha->exec() == QDialog::Accepted)
        CargarLineas();
}

void listas::AbrirVentanaFichaModProd()
{
    QString Producto = "";
    QModelIndex indice = ui->tableProductos->currentIndex();
    if (indice.isValid())
    {
        Producto = ui->tableProductos->item(indice.row(),0)->data(Qt::DisplayRole).toString();

        ProductosFicha *ventanaFicha = new ProductosFicha(Producto);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar producto"),QObject::tr("Debe seleccionar un producto a modificar"),0,0);
}

void listas::AbrirVentanaFichaNuevaCat()
{
    CategoriasFicha *ventanaFicha = new CategoriasFicha(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    if (ventanaFicha->exec() == QDialog::Accepted)
        CargarLineas();
}

void listas::AbrirVentanaFichaModCat()
{
    int Categoria = 0;
    QModelIndex indice = ui->tableCategorias->currentIndex();
    if (indice.isValid())
    {
        Categoria = ui->tableCategorias->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        CategoriasFicha *ventanaFicha = new CategoriasFicha(Categoria);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar categoría"),QObject::tr("Debe seleccionar una categoría a modificar"),0,0);
}

void listas::AbrirVentanaFichaNuevaFam()
{
    FamiliasFicha *ventanaFicha = new FamiliasFicha(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    if (ventanaFicha->exec() == QDialog::Accepted)
        CargarLineas();
}

void listas::AbrirVentanaFichaModFam()
{
    int Familia = 0;
    QModelIndex indice = ui->tableFamilias->currentIndex();
    if (indice.isValid())
    {
        Familia = ui->tableFamilias->item(indice.row(),0)->data(Qt::DisplayRole).toInt();

        FamiliasFicha *ventanaFicha = new FamiliasFicha(Familia);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        if (ventanaFicha->exec() == QDialog::Accepted)
            CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar familia"),QObject::tr("Debe seleccionar una familia a modificar"),0,0);
}

void listas::ActivarFiltroCat()
{
    if (ui->cbFiltroCat->currentIndex() == 0)
        ui->edtFiltroCat->setEnabled(false);
    else
        ui->edtFiltroCat->setEnabled(true);
    FiltrarCat();
}

void listas::ActivarFiltroFam()
{
    if (ui->cbFiltroFam->currentIndex() == 0)
        ui->edtFiltroFam->setEnabled(false);
    else
        ui->edtFiltroFam->setEnabled(true);
    FiltrarFam();
}

void listas::ActivarFiltroHistCert()
{
    if (ui->cbFiltroHistCert->currentIndex() == 0)
        ui->edtFiltroHistCert->setEnabled(false);
    else
        ui->edtFiltroHistCert->setEnabled(true);
    FiltrarHistCert();
}

void listas::ActivarFiltroProd()
{
    if (ui->cbFiltroProd->currentIndex() == 0)
    {
        ui->edtFiltroProd->setEnabled(false);
        ui->cbCondicion->setVisible(false);
        ui->lblCondicion->setVisible(false);
        ui->edtFiltroProd->setText("");
    }
    else if ((ui->cbFiltroProd->currentIndex() == 10) || (ui->cbFiltroProd->currentIndex() == 11))
    {
        ui->edtFiltroProd->setEnabled(true);
        ui->cbCondicion->setVisible(true);
        ui->lblCondicion->setVisible(true);
        ui->edtFiltroProd->setText("0");
    }
    else
    {
        ui->edtFiltroProd->setEnabled(true);
        ui->cbCondicion->setVisible(false);
        ui->lblCondicion->setVisible(false);
        ui->edtFiltroProd->setText("");
    }
    FiltrarProd();
}

void listas::FiltrarProd()
{
    QString strFiltro = "";
    bool boFiltrarStockC = false,boFiltrarStockT = false,boMostrar = false;

    if (ui->cbFiltroProd->currentIndex() == 1)
        strFiltro += "productos.idproducto LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 2)
        strFiltro += "productos.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 3)
        strFiltro += "Categorias.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 4)
        strFiltro += "Familias.Descripcion LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 5)
        strFiltro += "IF(productos.disponible=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 6)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 7)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 8)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 9)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroProd->text() + "%'";
    else if (ui->cbFiltroProd->currentIndex() == 10)
        boFiltrarStockC = true;
    else if (ui->cbFiltroProd->currentIndex() == 11)
        boFiltrarStockT = true;

    clsProductos Producto;
    QVector<QStringList> listaProductos = Producto.ListaProductos(strFiltro);

    ui->tableProductos->setRowCount(0);

    for (int i=0;i<listaProductos.size();i++)
    {
        boMostrar = false;

        QStringList strProductos = listaProductos.at(i);

        clsProductos Producto;
        Producto.Cargar(strProductos.at(0));

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strProductos.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strProductos.at(1));
        QTableWidgetItem *Dispo = new QTableWidgetItem;
        Dispo->setData(Qt::DisplayRole,strProductos.at(2));
        QTableWidgetItem *Cat = new QTableWidgetItem;
        Cat->setData(Qt::DisplayRole,strProductos.at(3));
        QTableWidgetItem *Fam = new QTableWidgetItem;
        Fam->setData(Qt::DisplayRole,strProductos.at(4));
        QTableWidgetItem *FrecCert = new QTableWidgetItem;
        FrecCert->setData(Qt::DisplayRole,strProductos.at(5));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strProductos.at(6));
        QTableWidgetItem *Lim = new QTableWidgetItem;
        Lim->setData(Qt::DisplayRole,strProductos.at(7));
        QTableWidgetItem *Limitado = new QTableWidgetItem;
        Limitado->setData(Qt::DisplayRole,strProductos.at(8));
        QTableWidgetItem *StockC = new QTableWidgetItem;
        StockC->setData(Qt::DisplayRole,Producto.StockCentro());
        StockC->setFlags(StockC->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *StockT = new QTableWidgetItem;
        StockT->setData(Qt::DisplayRole,Producto.StockTotal());
        StockT->setFlags(StockT->flags() & (~Qt::ItemIsEditable));

        if(boFiltrarStockC || boFiltrarStockT)
        {
            if(boFiltrarStockC)
            {
                if(ui->cbCondicion->currentIndex() == 0)
                {
                    if(Producto.StockCentro() > ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 1)
                {
                    if(Producto.StockCentro() < ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 2)
                {
                    if(Producto.StockCentro() == ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 3)
                {
                    if(Producto.StockCentro() <= ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 4)
                {
                    if(Producto.StockCentro() >= ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
            }
            if(boFiltrarStockT)
            {
                if(ui->cbCondicion->currentIndex() == 0)
                {
                    if(Producto.StockTotal() > ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 1)
                {
                    if(Producto.StockTotal() < ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 2)
                {
                    if(Producto.StockTotal() == ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 3)
                {
                    if(Producto.StockTotal() <= ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 4)
                {
                    if(Producto.StockTotal() >= ui->edtFiltroProd->text().toInt())
                        boMostrar = true;
                }
            }
        }
        else
            boMostrar = true;

        if (boMostrar)
        {
            ui->tableProductos->insertRow(ui->tableProductos->rowCount());

            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,0,Cod);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,1,Desc);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,2,Dispo);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,3,Cat);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,4,Fam);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,5,FrecCert);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,6,Cert);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,7,Lim);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,8,Limitado);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,9,StockC);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,10,StockT);
        }
    }
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
}

void listas::FiltrarHistCert()
{
    QString strFiltro = "";

    if (ui->cbFiltroHistCert->currentIndex() == 1)
        strFiltro += "certificaciones.idproducto LIKE '%" + ui->edtFiltroHistCert->text() + "%'";
    else if (ui->cbFiltroHistCert->currentIndex() == 2)
        strFiltro += "productos.Descripcion LIKE '%" + ui->edtFiltroHistCert->text() + "%'";
    else if (ui->cbFiltroHistCert->currentIndex() == 3)
        strFiltro += "certificaciones.emisor LIKE '%" + ui->edtFiltroHistCert->text() + "%'";

    clsHistoricoCertificaciones HistCert;
    QVector<QStringList> listaHistCert = HistCert.ListasHistCert(strFiltro);

    ui->tableHistCert->setRowCount(0);

    for (int i=0;i<listaHistCert.size();i++)
    {
        QStringList strHistCert = listaHistCert.at(i);

        ui->tableHistCert->insertRow(ui->tableHistCert->rowCount());

        QTableWidgetItem *Prod = new QTableWidgetItem;
        Prod->setData(Qt::DisplayRole,strHistCert.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strHistCert.at(1));
        QTableWidgetItem *FEmi = new QTableWidgetItem;
        FEmi->setData(Qt::DisplayRole,strHistCert.at(2));
        QTableWidgetItem *Emi = new QTableWidgetItem;
        Emi->setData(Qt::DisplayRole,strHistCert.at(3));

        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,0,Prod);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,1,Desc);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,2,FEmi);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,3,Emi);
    }
    ui->tableHistCert->resizeColumnsToContents();
    ui->tableHistCert->resizeRowsToContents();
}

void listas::FiltrarFam()
{
    QString strFiltro = "";

    if (ui->cbFiltroFam->currentIndex() == 1)
        strFiltro += "familias.Descripcion LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 2)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 3)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 4)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltroFam->text() + "%'";
    else if (ui->cbFiltroFam->currentIndex() == 5)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltroFam->text() + "%'";

    clsFamilias Familia;
    QVector<QStringList> listaFamilias = Familia.ListaFamilias(strFiltro);

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

void listas::FiltrarCat()
{
    QString strFiltro = "";

    if (ui->cbFiltroCat->currentIndex() == 1)
        strFiltro += "categorias.Descripcion LIKE '%" + ui->edtFiltroCat->text() + "%'";

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

void listas::CargarLineasProd()
{
    clsProductos Producto;
    QVector<QStringList> listaProductos = Producto.ListaProductos();

    ui->tableProductos->setRowCount(0);

    for (int i=0;i<listaProductos.size();i++)
    {
        QStringList strProductos = listaProductos.at(i);

        clsProductos Producto;
        Producto.Cargar(strProductos.at(0));

        ui->tableProductos->insertRow(ui->tableProductos->rowCount());

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strProductos.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strProductos.at(1));
        QTableWidgetItem *Dispo = new QTableWidgetItem;
        Dispo->setData(Qt::DisplayRole,strProductos.at(2));
        QTableWidgetItem *Cat = new QTableWidgetItem;
        Cat->setData(Qt::DisplayRole,strProductos.at(3));
        QTableWidgetItem *Fam = new QTableWidgetItem;
        Fam->setData(Qt::DisplayRole,strProductos.at(4));
        QTableWidgetItem *FrecCert = new QTableWidgetItem;
        FrecCert->setData(Qt::DisplayRole,strProductos.at(5));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strProductos.at(6));
        QTableWidgetItem *Lim = new QTableWidgetItem;
        Lim->setData(Qt::DisplayRole,strProductos.at(7));
        QTableWidgetItem *Limitado = new QTableWidgetItem;
        Limitado->setData(Qt::DisplayRole,strProductos.at(8));
        QTableWidgetItem *StockC = new QTableWidgetItem;
        StockC->setData(Qt::DisplayRole,Producto.StockCentro());
        StockC->setFlags(StockC->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *StockT = new QTableWidgetItem;
        StockT->setData(Qt::DisplayRole,Producto.StockTotal());
        StockT->setFlags(StockT->flags() & (~Qt::ItemIsEditable));

        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,0,Cod);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,1,Desc);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,2,Dispo);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,3,Cat);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,4,Fam);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,5,FrecCert);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,6,Cert);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,7,Lim);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,8,Limitado);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,9,StockC);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,10,StockT);
    }
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
}

void listas::CargarLineasCat()
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

void listas::CargarLineasFam()
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

void listas::CargarLineaHistCert()
{
    clsHistoricoCertificaciones HistCert;
    QVector<QStringList> listaHistCert = HistCert.ListasHistCert();

    ui->tableHistCert->setRowCount(0);

    for (int i=0;i<listaHistCert.size();i++)
    {
        QStringList strHistCert = listaHistCert.at(i);

        ui->tableHistCert->insertRow(ui->tableHistCert->rowCount());

        QTableWidgetItem *Prod = new QTableWidgetItem;
        Prod->setData(Qt::DisplayRole,strHistCert.at(0));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strHistCert.at(1));
        QTableWidgetItem *FEmi = new QTableWidgetItem;
        FEmi->setData(Qt::DisplayRole,strHistCert.at(2));
        QTableWidgetItem *Emi = new QTableWidgetItem;
        Emi->setData(Qt::DisplayRole,strHistCert.at(3));

        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,0,Prod);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,1,Desc);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,2,FEmi);
        ui->tableHistCert->setItem(ui->tableHistCert->rowCount()-1,3,Emi);
    }
    ui->tableHistCert->resizeColumnsToContents();
    ui->tableHistCert->resizeRowsToContents();
}

void listas::EliminarProducto()
{
    QModelIndex indice = ui->tableProductos->currentIndex();
    if (indice.isValid())
    {
        clsProductos Prod;
        Prod.Cargar(ui->tableProductos->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        Prod.Eliminar();

        CargarLineas();
    }
}

void listas::EliminarCat()
{
    QModelIndex indice = ui->tableCategorias->currentIndex();
    if (indice.isValid())
    {
        clsCategorias Cat;
        Cat.Cargar(ui->tableCategorias->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        Cat.Eliminar();
        CargarLineas();
    }
}

void listas::EliminarFam()
{
    QModelIndex indice = ui->tableFamilias->currentIndex();
    if (indice.isValid())
    {
        clsFamilias Fam;
        Fam.Cargar(ui->tableFamilias->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        Fam.Eliminar();
        CargarLineas();
    }
}

void listas::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(ProdMod);
    ui->btnEliminarCat->setEnabled(permiso);
    ui->btnNuevoCat->setEnabled(permiso);
    ui->btnEliminarFam->setEnabled(permiso);
    ui->btnNuevoFam->setEnabled(permiso);
    ui->btnEliminarProd->setEnabled(permiso);
    ui->btnNuevaProd->setEnabled(permiso);
}

void listas::CargarLineas()
{
    if(ui->tabListas->currentIndex() == 0)
    {
        CargarLineasProd();
        this->setWindowTitle(QObject::tr("Productos"));
    }
    else if(ui->tabListas->currentIndex() == 1)
    {
        CargarLineasCat();
        this->setWindowTitle(QObject::tr("Categorías"));
    }
    else if(ui->tabListas->currentIndex() == 2)
    {
        CargarLineasFam();
        this->setWindowTitle(QObject::tr("Familias"));
    }
    else if(ui->tabListas->currentIndex() == 3)
    {
        CargarLineaHistCert();
        this->setWindowTitle(QObject::tr("Historico de Certificaciones"));
    }
    else
    {
        ui->tableProductos->setRowCount(0);
        ui->tableCategorias->setRowCount(0);
        ui->tableFamilias->setRowCount(0);
        ui->tableHistCert->setRowCount(0);
    }
}
