#include "operarioslista.h"
#include "ui_operarioslista.h"
#include "../clsImagen.h"
#include "./Operarios/operario.h"
#include <QMessageBox>
#include <QFile>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

OperariosLista::OperariosLista(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperariosLista)
{
    ui->setupUi(this);

    ui->tableOperarios->clear();
    ui->tableOperarios->setRowCount(0);
    ui->tableOperarios->setColumnCount(4);
    ui->tableOperarios->setHorizontalHeaderLabels(QStringList() << QObject::tr("NIF") << QObject::tr("Nombre") << QObject::tr("Apellidos") <<  QObject::tr("Centro"));
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
    ui->tableOperarios->sortByColumn(0);

    CargarLineas();

    ui->cbFiltro->addItem(QObject::tr("Todos"));
    ui->cbFiltro->addItem(QObject::tr("NIF"));
    ui->cbFiltro->addItem(QObject::tr("Nombre"));
    ui->cbFiltro->addItem(QObject::tr("Apellidos"));
    ui->cbFiltro->addItem(QObject::tr("Centro"));
    ui->cbFiltro->setCurrentIndex(0);
    ui->edtFiltro->setEnabled(false);

    connect(ui->edtFiltro,SIGNAL(textChanged(QString)),this,SLOT(Filtrar()));
    connect(ui->cbFiltro,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltro()));
    connect(ui->tableOperarios,SIGNAL(clicked(QModelIndex)),this,SLOT(CargarImagen()));
    connect(ui->tableOperarios,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(accept()));
}

OperariosLista::~OperariosLista()
{
    delete ui;
}

void OperariosLista::changeEvent(QEvent *e)
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

void OperariosLista::ActivarFiltro()
{
    if (ui->cbFiltro->currentIndex() == 0)
        ui->edtFiltro->setEnabled(false);
    else
        ui->edtFiltro->setEnabled(true);
    Filtrar();
}

void OperariosLista::Filtrar()
{
    QString strFiltro ="";

    if (ui->cbFiltro->currentIndex() == 1)
        strFiltro += " operarios.nif LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 2)
        strFiltro += " operarios.Nombre LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 3)
        strFiltro += " operarios.apellidos LIKE '%" + ui->edtFiltro->text() + "%'";
    else if (ui->cbFiltro->currentIndex() == 4)
        strFiltro += " centros.nombre LIKE '%" + ui->edtFiltro->text() + "%'";

    clsOperarios Operario;
    QVector<QStringList> listaOperarios = Operario.ListaOperarios(strFiltro);

    ui->tableOperarios->setRowCount(0);

    for (int i=0;i<listaOperarios.size();i++)
    {
        QStringList strOperarios = listaOperarios.at(i);

        ui->tableOperarios->insertRow(ui->tableOperarios->rowCount());

        QTableWidgetItem *NIF = new QTableWidgetItem;
        NIF->setData(Qt::DisplayRole,strOperarios.at(0));
        QTableWidgetItem *Nom = new QTableWidgetItem;
        Nom->setData(Qt::DisplayRole,strOperarios.at(1));
        QTableWidgetItem *Apell = new QTableWidgetItem;
        Apell->setData(Qt::DisplayRole,strOperarios.at(2));
        QTableWidgetItem *Centro = new QTableWidgetItem;
        Centro->setData(Qt::DisplayRole,strOperarios.at(7));

        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,0,NIF);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,1,Nom);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,2,Apell);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,3,Centro);
    }
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
}

void OperariosLista::CargarLineas()
{
    clsOperarios Operario;
    QVector<QStringList> listaOperarios = Operario.ListaOperarios();

    ui->tableOperarios->setRowCount(0);

    for (int i=0;i<listaOperarios.size();i++)
    {
        QStringList strOperarios = listaOperarios.at(i);
        Operario.Cargar(strOperarios.at(0));

        QTableWidgetItem *NIF = new QTableWidgetItem;
        NIF->setData(Qt::DisplayRole,strOperarios.at(0));
        QTableWidgetItem *Nom = new QTableWidgetItem;
        Nom->setData(Qt::DisplayRole,strOperarios.at(1));
        QTableWidgetItem *Apell = new QTableWidgetItem;
        Apell->setData(Qt::DisplayRole,strOperarios.at(2));
        QTableWidgetItem *Centro = new QTableWidgetItem;
        Centro->setData(Qt::DisplayRole,strOperarios.at(7));

        if(Operario.Centro() == config->IdCentroActual())
        {
            ui->tableOperarios->insertRow(ui->tableOperarios->rowCount());

            ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,0,NIF);
            ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,1,Nom);
            ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,2,Apell);
            ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,3,Centro);
        }
    }
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
}

QString OperariosLista::NIFOpSeleccionado()
{
    QModelIndex indice = ui->tableOperarios->currentIndex();
    if (indice.isValid())
        return ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString();
    else
        return "";
}

void OperariosLista::CargarImagen()
{
    QModelIndex indice = ui->tableOperarios->currentIndex();
    if (indice.isValid())
    {
        clsOperarios Op(ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        Op.Cargar(ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        QString rutaimgOp = Op.Imagen();
        if (rutaimgOp == "")
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
            if (file.exists(rutaimgOp))
            {
                clsImagen *clsImg = new clsImagen(rutaimgOp,ui->scAreaImagen->width(),ui->scAreaImagen->height(),ui->scAreaImagen);
                ui->scAreaImagen->setWidget(clsImg);
            }
        }
    }
}
