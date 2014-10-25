#include "productoslista.h"
#include "ui_productoslista.h"
#include "../clsImagen.h"
#include "producto.h"
#include <QMessageBox>
#include <QFile>

ProductosLista::ProductosLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProductosLista)
{
    ui->setupUi(this);

    ui->tableProductos->clear();
    ui->tableProductos->setRowCount(0);
    ui->tableProductos->setColumnCount(11);
    ui->tableProductos->setHorizontalHeaderLabels(QStringList() << QObject::tr("Código") << QObject::tr("Descripción") << QObject::tr("Categoría") << QObject::tr("Familia")
                                                                << QObject::tr("Certificable") << QObject::tr("Frecuencia Certificación") << QObject::tr("Limitado")
                                                                << QObject::tr("Límite Entrega") << QObject::tr("Disponible") << QObject::tr("Stock Centro") << QObject::tr("Stock Total"));
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
    ui->tableProductos->sortByColumn(0);

    CargarLineas();

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("Código"));
    ui->cbFiltro->addItem(QObject::tr("Descripción"));
    ui->cbFiltro->addItem(QObject::tr("Categoría"));
    ui->cbFiltro->addItem(QObject::tr("Familia"));
    ui->cbFiltro->addItem(QObject::tr("Disponible"));
    ui->cbFiltro->addItem(QObject::tr("Frecuencia Certificación"));
    ui->cbFiltro->addItem(QObject::tr("Certificable"));
    ui->cbFiltro->addItem(QObject::tr("Límite entrega"));
    ui->cbFiltro->addItem(QObject::tr("Límitado"));
    ui->cbFiltro->addItem(QObject::tr("Stock Centro"));
    ui->cbFiltro->addItem(QObject::tr("Stock Total"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    ui->lblCondicion->setVisible(false);
    ui->cbCondicion->setVisible(false);
    ui->cbCondicion->addItem(">");
    ui->cbCondicion->addItem("<");
    ui->cbCondicion->addItem("=");
    ui->cbCondicion->addItem("<=");
    ui->cbCondicion->addItem(">=");
    ui->cbCondicion->setCurrentIndex(0);

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->cbCondicion,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->tableProductos,SIGNAL(clicked(QModelIndex)),this,SLOT(CargarImagen()));
    connect(ui->tableProductos,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

ProductosLista::~ProductosLista()
{
    delete ui;
}

void ProductosLista::changeEvent(QEvent *e)
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

void ProductosLista::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
    {
        ui->edtFiltro->setEnabled(false);
        ui->cbCondicion->setVisible(false);
        ui->lblCondicion->setVisible(false);
        ui->edtFiltro->setText("");
    }
    else if ((ui->cbFiltro->currentIndex() == 10) || (ui->cbFiltro->currentIndex() == 11))
    {
        ui->edtFiltro->setEnabled(true);
        ui->cbCondicion->setVisible(true);
        ui->lblCondicion->setVisible(true);
        ui->edtFiltro->setText("0");
    }
    else
    {
        ui->edtFiltro->setEnabled(true);
        ui->cbCondicion->setVisible(false);
        ui->lblCondicion->setVisible(false);
        ui->edtFiltro->setText("");
    }

    Filtrar();
}

void ProductosLista::Filtrar()
{
    QString strFiltro = "";
    bool boFiltrarStockC = false,boFiltrarStockT = false,boMostrar = false;

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += "productos.idproducto LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += "productos.Descripcion LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += "Categorias.Descripcion LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += "Familias.Descripcion LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 5)
        strFiltro += "IF(productos.disponible=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 6)
        strFiltro += "familias.frec_certif LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 7)
        strFiltro += "IF(familias.certificable=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 8)
        strFiltro += "familias.limite_entrega LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 9)
        strFiltro += "IF(familias.limitado=1,'"+QObject::tr("Sí")+"','"+QObject::tr("No")+"') LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 10)
        boFiltrarStockC = true;
    else if (ui->cbFiltro->currentIndex() == 11)
        boFiltrarStockT = true;

    ui->tableProductos->setRowCount(0);

    clsProductos Prod;
    QVector<QStringList> listProductos = Prod.ListaProductos(strFiltro);

    for (int i=0;i<listProductos.size();i++)
    {
        boMostrar = false;

        QStringList strProductos = listProductos.at(i);

        clsProductos Producto;
        Producto.Cargar(strProductos.at(0));

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strProductos.at(0));
        Cod->setFlags(Cod->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strProductos.at(1));
        Desc->setFlags(Desc->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cat = new QTableWidgetItem;
        Cat->setData(Qt::DisplayRole,strProductos.at(3));
        Cat->setFlags(Cat->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Fam = new QTableWidgetItem;
        Fam->setData(Qt::DisplayRole,strProductos.at(4));
        Fam->setFlags(Fam->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strProductos.at(6));
        Cert->setFlags(Cert->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Frec = new QTableWidgetItem;
        Frec->setData(Qt::DisplayRole,strProductos.at(5));
        Frec->setFlags(Frec->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Lim = new QTableWidgetItem;
        Lim->setData(Qt::DisplayRole,strProductos.at(8));
        Lim->setFlags(Lim->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Limite = new QTableWidgetItem;
        Limite->setData(Qt::DisplayRole,strProductos.at(7));
        Limite->setFlags(Limite->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Disp = new QTableWidgetItem;
        Disp->setData(Qt::DisplayRole,strProductos.at(2));
        Disp->setFlags(Disp->flags() & (~Qt::ItemIsEditable));
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
                    if(Producto.StockCentro() > ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 1)
                {
                    if(Producto.StockCentro() < ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 2)
                {
                    if(Producto.StockCentro() == ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 3)
                {
                    if(Producto.StockCentro() <= ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 4)
                {
                    if(Producto.StockCentro() >= ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
            }
            if(boFiltrarStockT)
            {
                if(ui->cbCondicion->currentIndex() == 0)
                {
                    if(Producto.StockTotal() > ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 1)
                {
                    if(Producto.StockTotal() < ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 2)
                {
                    if(Producto.StockTotal() == ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 3)
                {
                    if(Producto.StockTotal() <= ui->edtFiltro->text().toInt())
                        boMostrar = true;
                }
                else if(ui->cbCondicion->currentIndex() == 4)
                {
                    if(Producto.StockTotal() >= ui->edtFiltro->text().toInt())
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
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,2,Cat);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,3,Fam);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,4,Cert);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,5,Frec);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,6,Lim);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,7,Limite);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,8,Disp);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,9,StockC);
            ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,10,StockT);
        }
    }
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
}

void ProductosLista::CargarLineas()
{
    ui->tableProductos->setRowCount(0);

    clsProductos Prod;
    QVector<QStringList> listProductos = Prod.ListaProductos();

    for (int i=0;i<listProductos.size();i++)
    {
        ui->tableProductos->insertRow(ui->tableProductos->rowCount());

        QStringList strProductos = listProductos.at(i);

        clsProductos Producto;
        Producto.Cargar(strProductos.at(0));

        QTableWidgetItem *Cod = new QTableWidgetItem;
        Cod->setData(Qt::DisplayRole,strProductos.at(0));
        Cod->setFlags(Cod->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strProductos.at(1));
        Desc->setFlags(Desc->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cat = new QTableWidgetItem;
        Cat->setData(Qt::DisplayRole,strProductos.at(3));
        Cat->setFlags(Cat->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Fam = new QTableWidgetItem;
        Fam->setData(Qt::DisplayRole,strProductos.at(4));
        Fam->setFlags(Fam->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Cert = new QTableWidgetItem;
        Cert->setData(Qt::DisplayRole,strProductos.at(6));
        Cert->setFlags(Cert->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Frec = new QTableWidgetItem;
        Frec->setData(Qt::DisplayRole,strProductos.at(5));
        Frec->setFlags(Frec->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Lim = new QTableWidgetItem;
        Lim->setData(Qt::DisplayRole,strProductos.at(8));
        Lim->setFlags(Lim->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Limite = new QTableWidgetItem;
        Limite->setData(Qt::DisplayRole,strProductos.at(7));
        Limite->setFlags(Limite->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *Disp = new QTableWidgetItem;
        Disp->setData(Qt::DisplayRole,strProductos.at(2));
        Disp->setFlags(Disp->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *StockC = new QTableWidgetItem;
        StockC->setData(Qt::DisplayRole,Producto.StockCentro());
        StockC->setFlags(StockC->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *StockT = new QTableWidgetItem;
        StockT->setData(Qt::DisplayRole,Producto.StockTotal());
        StockT->setFlags(StockT->flags() & (~Qt::ItemIsEditable));

        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,0,Cod);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,1,Desc);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,2,Cat);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,3,Fam);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,4,Cert);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,5,Frec);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,6,Lim);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,7,Limite);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,8,Disp);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,9,StockC);
        ui->tableProductos->setItem(ui->tableProductos->rowCount()-1,10,StockT);
    }
    ui->tableProductos->resizeColumnsToContents();
    ui->tableProductos->resizeRowsToContents();
}

QString ProductosLista::CodProdSeleccionado()
{
    QModelIndex indice = ui->tableProductos->currentIndex();
    if (indice.isValid())
        return ui->tableProductos->item(indice.row(),0)->data(Qt::DisplayRole).toString();
    else
        return "";
}

void ProductosLista::CargarImagen()
{
    QModelIndex indice = ui->tableProductos->currentIndex();
    if (indice.isValid())
    {
        clsProductos Prod;
        Prod.Cargar(ui->tableProductos->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        QString rutaimgProd = Prod.Imagen();
        if (rutaimgProd == "")
        {
            QFile file;
            if (file.exists("Imagenes/NoPicture.png"))
            {
                clsImagen *clsImg = new clsImagen("Imagenes/NoPicture.png",ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
                ui->scAreaImagen->setWidget(clsImg);
            }
        }
        else
        {
            QFile file;
            if (file.exists(rutaimgProd))
            {
                clsImagen *clsImg = new clsImagen(rutaimgProd,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
                ui->scAreaImagen->setWidget(clsImg);
            }
        }
    }
}
