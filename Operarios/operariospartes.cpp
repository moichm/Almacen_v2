#include "operariospartes.h"
#include "ui_operariospartes.h"
#include "operariosficha.h"
#include "operario.h"
#include "clsconfiguracion.h"
#include "clsimprimiroperarios.h"
#include "partestrabajoficha.h"
#include "clsimprimirpartes.h"
#include <QMessageBox>

extern clsConfiguracion *config;

OperariosPartes::OperariosPartes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OperariosPartes)
{
    ui->setupUi(this);

    bool permiso = config->Permisos().value(Confi);

    ui->tabOperariosPartes->setCurrentIndex(0);

    ui->tableOperarios->clear();
    ui->tableOperarios->setRowCount(0);
    ui->tableOperarios->setColumnCount(8);
    ui->tableOperarios->setColumnHidden(7,!permiso);
    ui->tableOperarios->setHorizontalHeaderLabels(QStringList() << QObject::tr("NIF") << QObject::tr("Nombre") << QObject::tr("Apellidos") << QObject::tr("Fecha Nacimiento") << QObject::tr("Dirección") << QObject::tr("Correo electrónico") << QObject::tr("Nº CCC") << QObject::tr("Centro"));
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
    ui->tableOperarios->sortByColumn(0);

    ui->tablePartes->clear();
    ui->tablePartes->setRowCount(0);
    ui->tablePartes->setColumnCount(4);
    ui->tablePartes->setHorizontalHeaderLabels(QStringList() << QObject::tr("Nº Parte") << QObject::tr("Fecha Parte") << QObject::tr("Responsable") <<  QObject::tr("Descripción"));
    ui->tablePartes->resizeColumnsToContents();
    ui->tablePartes->resizeRowsToContents();
    ui->tablePartes->sortByColumn(0);

    ui->cbFiltroOp->addItem(QObject::tr("Todos"));
    ui->cbFiltroOp->addItem(QObject::tr("NIF"));
    ui->cbFiltroOp->addItem(QObject::tr("Nombre"));
    ui->cbFiltroOp->addItem(QObject::tr("Apellidos"));
    ui->cbFiltroOp->addItem(QObject::tr("Fecha nacimiento"));
    ui->cbFiltroOp->addItem(QObject::tr("Dirección"));
    ui->cbFiltroOp->addItem(QObject::tr("Correo electrónico"));
    ui->cbFiltroOp->addItem(QObject::tr("Nº CCC"));
    if(permiso)
        ui->cbFiltroOp->addItem(QObject::tr("Centro"));
    ui->cbFiltroOp->setCurrentIndex(0);
    ui->edtFiltroOp->setEnabled(false);

    ui->cbFiltroPT->addItem(QObject::tr("Todos"));
    ui->cbFiltroPT->addItem(QObject::tr("Nº Parte"));
    ui->cbFiltroPT->addItem(QObject::tr("Responsable"));
    ui->cbFiltroPT->addItem(QObject::tr("Descripción"));
    ui->cbFiltroPT->setCurrentIndex(0);
    ui->edtFiltroPT->setEnabled(false);

    CargarLineas();

    connect(ui->edtFiltroOp,SIGNAL(textChanged(QString)),this,SLOT(FiltrarOp()));
    connect(ui->cbFiltroOp,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroOp()));
    connect(ui->btnSalirOp,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->btnNuevaOp,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevoOp()));
    connect(ui->btnModificarOp,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModOp()));
    connect(ui->tableOperarios,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModOp()));
    connect(ui->btnEliminarOp,SIGNAL(clicked()),this,SLOT(EliminarOperario()));
    connect(ui->btnImprimirOp,SIGNAL(clicked()),this,SLOT(ImprimirOp()));

    connect(ui->edtFiltroPT,SIGNAL(textChanged(QString)),this,SLOT(FiltrarPT()));
    connect(ui->cbFiltroPT,SIGNAL(currentIndexChanged(QString)),this,SLOT(ActivarFiltroPT()));
    connect(ui->btnNuevaPT,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaNuevoPT()));
    connect(ui->btnModificarPT,SIGNAL(clicked()),this,SLOT(AbrirVentanaFichaModPT()));
    connect(ui->btnSalirPT,SIGNAL(clicked()),this,SIGNAL(closeMDI()));
    connect(ui->tablePartes,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(AbrirVentanaFichaModPT()));
    connect(ui->btnEliminarPT,SIGNAL(clicked()),this,SLOT(EliminarPT()));
    connect(ui->btnImprimirPT,SIGNAL(clicked()),this,SLOT(ImprimirPT()));

    connect(ui->tabOperariosPartes,SIGNAL(currentChanged(int)),this,SLOT(CargarLineas()));

    DeshabilitarControles();
}

OperariosPartes::~OperariosPartes()
{
    delete ui;
}

void OperariosPartes::changeEvent(QEvent *e)
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

void OperariosPartes::CargarLineas()
{
    if(ui->tabOperariosPartes->currentIndex() == 0)
    {
        CargarLineasOp();
        this->setWindowTitle(QObject::tr("Operarios"));
    }
    else if(ui->tabOperariosPartes->currentIndex() == 1)
    {
        CargarLineasPT();
        this->setWindowTitle(QObject::tr("Partes de Trabajo"));
    }
    else
    {
        ui->tableOperarios->setRowCount(0);
        ui->tablePartes->setRowCount(0);
    }
}

void OperariosPartes::ImprimirOp()
{
    QString strFiltro = "";

    if (ui->cbFiltroOp->currentIndex() == 1)
        strFiltro += "operarios.nif LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 2)
        strFiltro += "operarios.nombre LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 3)
        strFiltro += "operarios.apellidos LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 4)
        strFiltro += "DATE_FORMAT(operarios.fecha_nacimiento, '%d/%m/%Y') LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 5)
        strFiltro += "operarios.direccion LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 6)
        strFiltro += "operarios.email LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 7)
        strFiltro += "operarios.ccc LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 8)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltroOp->text() + "%'";

    clsImprimirOperarios Inf;
    Inf.ImprimirListaOperario(strFiltro);
}

void OperariosPartes::CargarLineasOp()
{
    clsOperarios Operario;
    QVector<QStringList> listaOperarios = Operario.ListaOperarios();

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
        QTableWidgetItem *Fecha_Nac = new QTableWidgetItem;
        Fecha_Nac->setData(Qt::DisplayRole,strOperarios.at(3));
        QTableWidgetItem *Direc = new QTableWidgetItem;
        Direc->setData(Qt::DisplayRole,strOperarios.at(4));
        QTableWidgetItem *Email = new QTableWidgetItem;
        Email->setData(Qt::DisplayRole,strOperarios.at(5));
        QTableWidgetItem *CCC = new QTableWidgetItem;
        CCC->setData(Qt::DisplayRole,strOperarios.at(6));
        QTableWidgetItem *Centro = new QTableWidgetItem;
        Centro->setData(Qt::DisplayRole,strOperarios.at(7));

        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,0,NIF);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,1,Nom);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,2,Apell);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,3,Fecha_Nac);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,4,Direc);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,5,Email);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,6,CCC);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,7,Centro);
    }
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
}

void OperariosPartes::ActivarFiltroOp()
{
    if (ui->cbFiltroOp->currentIndex() == 0)
        ui->edtFiltroOp->setEnabled(false);
    else
        ui->edtFiltroOp->setEnabled(true);
    FiltrarOp();
}

void OperariosPartes::FiltrarOp()
{
    clsOperarios Operario;
    QString strFiltro ="";

    if (ui->cbFiltroOp->currentIndex() == 1)
        strFiltro += "operarios.nif LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 2)
        strFiltro += "operarios.nombre LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 3)
        strFiltro += "operarios.apellidos LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 4)
        strFiltro += "DATE_FORMAT(operarios.fecha_nacimiento, '%d/%m/%Y') LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 5)
        strFiltro += "operarios.direccion LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 6)
        strFiltro += "operarios.email LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 7)
        strFiltro += "operarios.ccc LIKE '%" + ui->edtFiltroOp->text() + "%'";
    else if (ui->cbFiltroOp->currentIndex() == 8)
        strFiltro += "centros.nombre LIKE '%" + ui->edtFiltroOp->text() + "%'";

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
        QTableWidgetItem *Fecha_Nac = new QTableWidgetItem;
        Fecha_Nac->setData(Qt::DisplayRole,strOperarios.at(3));
        QTableWidgetItem *Direc = new QTableWidgetItem;
        Direc->setData(Qt::DisplayRole,strOperarios.at(4));
        QTableWidgetItem *Email = new QTableWidgetItem;
        Email->setData(Qt::DisplayRole,strOperarios.at(5));
        QTableWidgetItem *CCC = new QTableWidgetItem;
        CCC->setData(Qt::DisplayRole,strOperarios.at(6));
        QTableWidgetItem *Centro = new QTableWidgetItem;
        Centro->setData(Qt::DisplayRole,strOperarios.at(7));

        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,0,NIF);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,1,Nom);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,2,Apell);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,3,Fecha_Nac);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,4,Direc);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,5,Email);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,6,CCC);
        ui->tableOperarios->setItem(ui->tableOperarios->rowCount()-1,7,Centro);
    }
    ui->tableOperarios->resizeColumnsToContents();
    ui->tableOperarios->resizeRowsToContents();
}

void OperariosPartes::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(OpMod);
    ui->btnEliminarOp->setEnabled(permiso);
    ui->btnNuevaOp->setEnabled(permiso);
}

void OperariosPartes::AbrirVentanaFichaModOp()
{
    QString Operario = "";
    QModelIndex indice = ui->tableOperarios->currentIndex();
    if (indice.isValid())
    {
        Operario = ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString();
        OperariosFicha *ventanaFicha = new OperariosFicha(Operario);
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();
        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar operario"),QObject::tr("Debe seleccionar un operario a modificar"),0,0);
}

void OperariosPartes::AbrirVentanaFichaNuevoOp()
{
    OperariosFicha *ventanaFicha = new OperariosFicha("");
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();
    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void OperariosPartes::EliminarOperario()
{
    QModelIndex indice = ui->tableOperarios->currentIndex();
    if (indice.isValid())
    {
        clsOperarios Op(ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        Op.Cargar(ui->tableOperarios->item(indice.row(),0)->data(Qt::DisplayRole).toString());
        Op.Eliminar();

        CargarLineas();
    }
}

void OperariosPartes::AbrirVentanaFichaNuevoPT()
{
    PartesTrabajoFicha *ventanaFicha = new PartesTrabajoFicha(0);
    ventanaFicha->setWindowModality(Qt::ApplicationModal);
    ventanaFicha->show();

    while (ventanaFicha->exec() == QDialog::Rejected)
        ;
    CargarLineas();
}

void OperariosPartes::AbrirVentanaFichaModPT()
{
    QModelIndex indice = ui->tablePartes->currentIndex();
    if (indice.isValid())
    {
        PartesTrabajoFicha *ventanaFicha = new PartesTrabajoFicha(ui->tablePartes->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        ventanaFicha->setWindowModality(Qt::ApplicationModal);
        ventanaFicha->show();

        while (ventanaFicha->exec() == QDialog::Rejected)
            ;
        CargarLineas();
    }
    else
        QMessageBox::warning(0,QObject::tr("Modificar parte de trabajo"),QObject::tr("Debe seleccionar un parte de trabajo a modificar"),0,0);
}

void OperariosPartes::ActivarFiltroPT()
{
    if (ui->cbFiltroPT->currentIndex() == 0)
    {
        ui->edtFiltroPT->setEnabled(false);
        FiltrarPT();
    }
    else
    {
        ui->edtFiltroPT->setEnabled(true);
        FiltrarPT();
    }
}

void OperariosPartes::FiltrarPT()
{
    QString strFiltro = "";

    if (ui->cbFiltroPT->currentIndex() == 1)
        strFiltro += "idparte LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 2)
        strFiltro += "CONCAT(nombre,CONCAT(' ',apellidos)) LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 3)
        strFiltro += "descripcion LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 4)
        strFiltro += "DATE_FORMAT(fecha_parte, '%d-%m-%Y') LIKE '%" + ui->edtFiltroPT->text() + "%'";

    clsPartes_Trabajo PartesTrabajo;
    QVector<QStringList> listaPartesTrabajo = PartesTrabajo.ListaPartes_Trabajo(strFiltro);

    ui->tablePartes->setRowCount(0);

    for (int i=0;i<listaPartesTrabajo.size();i++)
    {
        QStringList strPartesTrabajo = listaPartesTrabajo.at(i);

        ui->tablePartes->insertRow(ui->tablePartes->rowCount());

        QTableWidgetItem *IdParte = new QTableWidgetItem;
        IdParte->setData(Qt::DisplayRole,strPartesTrabajo.at(0));
        QTableWidgetItem *FParte = new QTableWidgetItem;
        FParte->setData(Qt::DisplayRole,strPartesTrabajo.at(1));
        QTableWidgetItem *Resp = new QTableWidgetItem;
        Resp->setData(Qt::DisplayRole,strPartesTrabajo.at(2));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strPartesTrabajo.at(3));

        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,0,IdParte);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,1,FParte);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,2,Resp);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,3,Desc);
    }
    ui->tablePartes->resizeColumnsToContents();
    ui->tablePartes->resizeRowsToContents();
}

void OperariosPartes::CargarLineasPT()
{
    clsPartes_Trabajo PartesTrabajo;
    QVector<QStringList> listaPartesTrabajo = PartesTrabajo.ListaPartes_Trabajo();

    ui->tablePartes->setRowCount(0);

    for (int i=0;i<listaPartesTrabajo.size();i++)
    {
        QStringList strPartesTrabajo = listaPartesTrabajo.at(i);

        ui->tablePartes->insertRow(ui->tablePartes->rowCount());

        QTableWidgetItem *IdParte = new QTableWidgetItem;
        IdParte->setData(Qt::DisplayRole,strPartesTrabajo.at(0));
        QTableWidgetItem *FParte = new QTableWidgetItem;
        FParte->setData(Qt::DisplayRole,strPartesTrabajo.at(1));
        QTableWidgetItem *Resp = new QTableWidgetItem;
        Resp->setData(Qt::DisplayRole,strPartesTrabajo.at(2));
        QTableWidgetItem *Desc = new QTableWidgetItem;
        Desc->setData(Qt::DisplayRole,strPartesTrabajo.at(3));

        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,0,IdParte);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,1,FParte);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,2,Resp);
        ui->tablePartes->setItem(ui->tablePartes->rowCount()-1,3,Desc);
    }
    ui->tablePartes->resizeColumnsToContents();
    ui->tablePartes->resizeRowsToContents();
}

void OperariosPartes::EliminarPT()
{
    QModelIndex indice = ui->tablePartes->currentIndex();
    if (indice.isValid())
    {
        clsPartes_Trabajo ParteTrab;
        ParteTrab.Cargar(ui->tablePartes->item(indice.row(),0)->data(Qt::DisplayRole).toInt());
        ParteTrab.Eliminar();
        CargarLineas();
    }
}

void OperariosPartes::ImprimirPT()
{
    QString strFiltro = "";

    if (ui->cbFiltroPT->currentIndex() == 1)
        strFiltro += "idparte LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 2)
        strFiltro += "CONCAT(nombre,CONCAT(' ',apellidos)) LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 3)
        strFiltro += "descripcion LIKE '%" + ui->edtFiltroPT->text() + "%'";
    else if (ui->cbFiltroPT->currentIndex() == 4)
        strFiltro += "DATE_FORMAT(fecha_parte, '%d-%m-%Y') LIKE '%" + ui->edtFiltroPT->text() + "%'";

    clsImprimirPartes Inf;
    Inf.ImprimirListaPartes(strFiltro);
}
