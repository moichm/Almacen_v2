#include "configuracion.h"
#include "ui_configuracion.h"
#include "clsconfiguracion.h"
#include "./Centros/centro.h"
#include <QSqlQuery>
#include <QMap>
#include <QVector>
#include <iterator>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QModelIndex>
#include <QInputDialog>
#include <QFileDialog>
#include <QSqlError>
#include <QProcess>

extern clsConfiguracion *config;

Configuracion::Configuracion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuracion)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);

    ui->tableUsuarios->clear();
    ui->tableUsuarios->setRowCount(0);
    ui->tableUsuarios->setColumnCount(15);
    ui->tableUsuarios->setHorizontalHeaderLabels(QStringList() << QObject::tr("Usuario") << QObject::tr("Contraseña")
                                                 << QObject::tr("Ver Productos") << QObject::tr("Modificar Productos")
                                                 << QObject::tr("Ver Movimientos") << QObject::tr("Modificar Movimientos")
                                                 << QObject::tr("Ver Kits") << QObject::tr("Modificar Kits")
                                                 << QObject::tr("Ver Vehículos") << QObject::tr("Modificar Vehículos")
                                                 << QObject::tr("Ver Operarios") << QObject::tr("Modificar Operarios")
                                                 << QObject::tr("Ver Centros") << QObject::tr("Modificar Centros")
                                                 << QObject::tr("Configuración"));

    ui->tableUsuarios->resizeColumnsToContents();
    ui->tableUsuarios->resizeRowsToContents();
    ui->tableUsuarios->sortByColumn(0);

    CargarCentros();
    if (config->ExisteArchivoConf())
    {
        config->CargarArchivoConf();

        clsCentros Centro;
        Centro.Cargar(config->IdCentroActual());
        ui->edtCentro->setText(Centro.Nombre());

        ui->cbCentros->setCurrentIndex(ui->cbCentros->findText(Centro.Nombre(),Qt::MatchExactly));
    }
    CargarLineasUsuarios();

    Inserts = new QVector<int>;
    Eliminadas = new QVector<int>;

    QPushButton *Aplicar = ui->buttonBox->button(QDialogButtonBox::Apply);
    QPushButton *Aceptar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *Cancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(Aplicar,SIGNAL(clicked()),this,SLOT(Aplicar()));
    connect(Aceptar,SIGNAL(clicked()),this,SLOT(Guardar()));
    connect(Cancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->tableUsuarios,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(Modificacion()));
    connect(ui->btnAnyadirUser,SIGNAL(clicked()),this,SLOT(AniadirUsuario()));
    connect(ui->btnEliminarUser,SIGNAL(clicked()),this,SLOT(EliminarUsuario()));
    connect(ui->btnBackup,SIGNAL(clicked()),this,SLOT(Copia_Seguridad()));
    connect(ui->btnBinBachup,SIGNAL(clicked()),this,SLOT(SeleccionarArchivo()));
    connect(ui->btnResPass,SIGNAL(clicked()),this,SLOT(RestablecerPass()));
}

Configuracion::~Configuracion()
{
    delete Mods;
    delete Inserts;
    delete Eliminadas;
    delete ui;
}

void Configuracion::changeEvent(QEvent *e)
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

void Configuracion::CargarCentros()
{
    mapCentros.clear();

    QString strConsulta = "SELECT idCentro,nombre FROM centros;";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        while (consulta.next())
            mapCentros.insert(consulta.value(1).toString(),consulta.value(0).toInt());

        for (QMap<QString,int>::iterator i=mapCentros.begin();i!=mapCentros.end();i++)
            ui->cbCentros->addItem(i.key());
    }
}

void Configuracion::Guardar()
{
    Almacenar();

    this->accept();
}

void Configuracion::Aplicar()
{
    Almacenar();
}

void Configuracion::Almacenar()
{
    if (config->ExisteArchivoConf())
    {
        clsCentros Centro;
        Centro.Cargar(config->IdCentroActual());

        if (ui->cbCentros->currentText() != Centro.Nombre())
        {
            QMap<QString,int>::iterator i = mapCentros.find(ui->cbCentros->currentText());
            config->setCentroActual(i.value());
            config->GuardarArchivoConf();

            if (QMessageBox::question(this,QObject::tr("Almacén"),QObject::tr("Se van a cerrar las ventanas abiertas, ¿desea continuar?"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes) == QMessageBox::Yes)
                Guardado();
        }
    }
    else
    {
        QMap<QString,int>::iterator i = mapCentros.find(ui->cbCentros->currentText());
        config->setCentroActual(i.value());
        config->CrearArchivoConf();
    }

    for (int i = 0;i < Mods->size();i++)
    {
        if (Mods->at(i) == true)
        {
            QPair<QString,QString> par;
            par.first = ui->tableUsuarios->item(i,0)->data(Qt::DisplayRole).toString();
            par.second = ui->tableUsuarios->item(i,1)->data(Qt::DisplayRole).toString();
            QMap<tPermisos,bool> permisos;
            permisos.insert(ProdVer,ui->tableUsuarios->item(i,2)->data(Qt::DisplayRole).toBool());
            permisos.insert(ProdMod,ui->tableUsuarios->item(i,3)->data(Qt::DisplayRole).toBool());
            permisos.insert(MovVer,ui->tableUsuarios->item(i,4)->data(Qt::DisplayRole).toBool());
            permisos.insert(MovMod,ui->tableUsuarios->item(i,5)->data(Qt::DisplayRole).toBool());
            permisos.insert(KitVer,ui->tableUsuarios->item(i,6)->data(Qt::DisplayRole).toBool());
            permisos.insert(KitMod,ui->tableUsuarios->item(i,7)->data(Qt::DisplayRole).toBool());
            permisos.insert(VehVer,ui->tableUsuarios->item(i,8)->data(Qt::DisplayRole).toBool());
            permisos.insert(VehMod,ui->tableUsuarios->item(i,9)->data(Qt::DisplayRole).toBool());
            permisos.insert(OpVer,ui->tableUsuarios->item(i,10)->data(Qt::DisplayRole).toBool());
            permisos.insert(OpMod,ui->tableUsuarios->item(i,11)->data(Qt::DisplayRole).toBool());
            permisos.insert(CentVer,ui->tableUsuarios->item(i,12)->data(Qt::DisplayRole).toBool());
            permisos.insert(CentMod,ui->tableUsuarios->item(i,13)->data(Qt::DisplayRole).toBool());
            permisos.insert(Confi,ui->tableUsuarios->item(i,14)->data(Qt::DisplayRole).toBool());
            config->ModUsuarios(par,permisos);
        }
    }
    for (int i = 0;i < Inserts->size();i++)
    {
        QPair<QString,QString> par;
        par.first = ui->tableUsuarios->item(Inserts->at(i),0)->data(Qt::DisplayRole).toString();
        par.second = mapPassUser.value(ui->tableUsuarios->item(Inserts->at(i),0)->data(Qt::DisplayRole).toString());
        QMap<tPermisos,bool> permisos;
        permisos.insert(ProdVer,ui->tableUsuarios->item(Inserts->at(i),2)->data(Qt::DisplayRole).toBool());
        permisos.insert(ProdMod,ui->tableUsuarios->item(Inserts->at(i),3)->data(Qt::DisplayRole).toBool());
        permisos.insert(MovVer,ui->tableUsuarios->item(Inserts->at(i),4)->data(Qt::DisplayRole).toBool());
        permisos.insert(MovMod,ui->tableUsuarios->item(Inserts->at(i),5)->data(Qt::DisplayRole).toBool());
        permisos.insert(KitVer,ui->tableUsuarios->item(Inserts->at(i),6)->data(Qt::DisplayRole).toBool());
        permisos.insert(KitMod,ui->tableUsuarios->item(Inserts->at(i),7)->data(Qt::DisplayRole).toBool());
        permisos.insert(VehVer,ui->tableUsuarios->item(Inserts->at(i),8)->data(Qt::DisplayRole).toBool());
        permisos.insert(VehMod,ui->tableUsuarios->item(Inserts->at(i),9)->data(Qt::DisplayRole).toBool());
        permisos.insert(OpVer,ui->tableUsuarios->item(Inserts->at(i),10)->data(Qt::DisplayRole).toBool());
        permisos.insert(OpMod,ui->tableUsuarios->item(Inserts->at(i),11)->data(Qt::DisplayRole).toBool());
        permisos.insert(CentVer,ui->tableUsuarios->item(Inserts->at(i),12)->data(Qt::DisplayRole).toBool());
        permisos.insert(CentMod,ui->tableUsuarios->item(Inserts->at(i),13)->data(Qt::DisplayRole).toBool());
        permisos.insert(Confi,ui->tableUsuarios->item(Inserts->at(i),14)->data(Qt::DisplayRole).toBool());

        config->AnyadirUsuario(par,permisos);

        Inserts->remove(i);
    }
    for (int i = 0;i < Eliminadas->size();i++)
        config->EliminarUsuario(ui->tableUsuarios->item(Eliminadas->at(i),0)->data(Qt::DisplayRole).toString());
}

void Configuracion::CargarLineasUsuarios()
{
    config->CargarUsuarios();

    const QMap<QPair<QString,QString>,QMap<tPermisos,bool> > Usuarios = config->Usuarios();

    Mods = new QVector<bool>(Usuarios.size());
    for (int i = 0;i < Mods->size();i++)
        Mods->replace(i,false);

    for (QMap<QPair<QString,QString>,QMap<tPermisos,bool> >::const_iterator i=Usuarios.begin();i!=Usuarios.end();i++)
    {
        QPair<QString,QString> user = i.key();
        QString id_user = user.first;
        QString pass_user = user.second;
        QMap<tPermisos,bool> Permisos = i.value();

        ui->tableUsuarios->insertRow(ui->tableUsuarios->rowCount());

        QTableWidgetItem *idUser = new QTableWidgetItem;
        idUser->setData(Qt::DisplayRole,id_user);
        idUser->setFlags(idUser->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *PassUser = new QTableWidgetItem;
        PassUser->setData(Qt::DisplayRole,pass_user);
        QTableWidgetItem *VerProd = new QTableWidgetItem;
        VerProd->setData(Qt::DisplayRole,Permisos.value(ProdVer));
        QTableWidgetItem *ModProd = new QTableWidgetItem;
        ModProd->setData(Qt::DisplayRole,Permisos.value(ProdMod));
        QTableWidgetItem *VerMov = new QTableWidgetItem;
        VerMov->setData(Qt::DisplayRole,Permisos.value(MovVer));
        QTableWidgetItem *ModMov = new QTableWidgetItem;
        ModMov->setData(Qt::DisplayRole,Permisos.value(MovMod));
        QTableWidgetItem *VerKit = new QTableWidgetItem;
        VerKit->setData(Qt::DisplayRole,Permisos.value(KitVer));
        QTableWidgetItem *ModKit = new QTableWidgetItem;
        ModKit->setData(Qt::DisplayRole,Permisos.value(KitMod));
        QTableWidgetItem *VerVehi = new QTableWidgetItem;
        VerVehi->setData(Qt::DisplayRole,Permisos.value(VehVer));
        QTableWidgetItem *ModVehi = new QTableWidgetItem;
        ModVehi->setData(Qt::DisplayRole,Permisos.value(VehMod));
        QTableWidgetItem *VerOp = new QTableWidgetItem;
        VerOp->setData(Qt::DisplayRole,Permisos.value(OpVer));
        QTableWidgetItem *ModOp = new QTableWidgetItem;
        ModOp->setData(Qt::DisplayRole,Permisos.value(OpMod));
        QTableWidgetItem *VerCentro = new QTableWidgetItem;
        VerCentro->setData(Qt::DisplayRole,Permisos.value(CentVer));
        QTableWidgetItem *ModCentro = new QTableWidgetItem;
        ModCentro->setData(Qt::DisplayRole,Permisos.value(CentMod));
        QTableWidgetItem *Conf = new QTableWidgetItem;
        Conf->setData(Qt::DisplayRole,Permisos.value(Confi));

        ui->tableUsuarios->blockSignals(true);

        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,0,idUser);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,1,PassUser);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,2,VerProd);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,3,ModProd);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,4,VerMov);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,5,ModMov);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,6,VerKit);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,7,ModKit);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,8,VerVehi);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,9,ModVehi);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,10,VerOp);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,11,ModOp);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,12,VerCentro);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,13,ModCentro);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,14,Conf);

        ui->tableUsuarios->setColumnHidden(1,true);

        ui->tableUsuarios->blockSignals(false);
    }
    ui->tableUsuarios->resizeColumnsToContents();
    ui->tableUsuarios->resizeRowsToContents();
}

void Configuracion::RestablecerPass()
{
    QString passUser = ui->tableUsuarios->item(ui->tableUsuarios->currentItem()->row(),0)->data(Qt::DisplayRole).toString();
    if (passUser.at(0).isLower())
        passUser.replace(0,1,passUser.at(0).toUpper());

    passUser=asignarPass(passUser);
    ui->tableUsuarios->item(ui->tableUsuarios->currentItem()->row(),1)->setData(Qt::DisplayRole,passUser);
    QMessageBox::about(0,QObject::tr("Cambio de contraseña"),QObject::tr("La nueva contraseña es: ") + passUser);
}

QString Configuracion::asignarPass(QString idUsuario)
{
    for (int i = 1;i < idUsuario.length();i++)
    {
        if(idUsuario.at(i).toLower() == 'a')
            idUsuario.replace(i,1,"4");
        else if(idUsuario.at(i).toLower() == 'e')
            idUsuario.replace(i,1,"3");
        else if(idUsuario.at(i).toLower() == 'i')
            idUsuario.replace(i,1,"1");
        else if(idUsuario.at(i).toLower() == 'o')
            idUsuario.replace(i,1,"0");
    }
    return idUsuario;
}

void Configuracion::Modificacion()
{
    if (Mods->at(ui->tableUsuarios->currentRow()) == false &&
       (!Inserts->contains(ui->tableUsuarios->currentRow())) &&
       (!Eliminadas->contains(ui->tableUsuarios->currentRow())))
        Mods->replace(ui->tableUsuarios->currentRow(),true);
}

void Configuracion::AniadirUsuario()
{
    bool ok;
    QString pass;
    QString user = QInputDialog::getText(this,QObject::tr("Introducción de datos"),QObject::tr("Introduzca el usuario:"),QLineEdit::Normal,"",&ok);
    if (ok)
    {
        ui->tableUsuarios->insertRow(ui->tableUsuarios->rowCount());

        pass=asignarPass(user);

        QTableWidgetItem *idUser = new QTableWidgetItem;
        idUser->setData(Qt::DisplayRole,user);
        idUser->setFlags(idUser->flags() & (~Qt::ItemIsEditable));
        QTableWidgetItem *PassUser = new QTableWidgetItem;
        PassUser->setData(Qt::DisplayRole,pass);
        QTableWidgetItem *VerProd = new QTableWidgetItem;
        VerProd->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModProd = new QTableWidgetItem;
        ModProd->setData(Qt::DisplayRole,false);
        QTableWidgetItem *VerMov = new QTableWidgetItem;
        VerMov->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModMov = new QTableWidgetItem;
        ModMov->setData(Qt::DisplayRole,false);
        QTableWidgetItem *VerKit = new QTableWidgetItem;
        VerKit->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModKit = new QTableWidgetItem;
        ModKit->setData(Qt::DisplayRole,false);
        QTableWidgetItem *VerVehi = new QTableWidgetItem;
        VerVehi->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModVehi = new QTableWidgetItem;
        ModVehi->setData(Qt::DisplayRole,false);
        QTableWidgetItem *VerOp = new QTableWidgetItem;
        VerOp->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModOp = new QTableWidgetItem;
        ModOp->setData(Qt::DisplayRole,false);
        QTableWidgetItem *VerCentro = new QTableWidgetItem;
        VerCentro->setData(Qt::DisplayRole,false);
        QTableWidgetItem *ModCentro = new QTableWidgetItem;
        ModCentro->setData(Qt::DisplayRole,false);
        QTableWidgetItem *Conf = new QTableWidgetItem;
        Conf->setData(Qt::DisplayRole,false);

        ui->tableUsuarios->blockSignals(true);

        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,0,idUser);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,1,PassUser);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,2,VerProd);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,3,ModProd);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,4,VerMov);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,5,ModMov);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,6,VerKit);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,7,ModKit);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,8,VerVehi);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,9,ModVehi);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,10,VerOp);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,11,ModOp);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,12,VerCentro);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,13,ModCentro);
        ui->tableUsuarios->setItem(ui->tableUsuarios->rowCount()-1,14,Conf);

        ui->tableUsuarios->setColumnHidden(1,true);

        ui->tableUsuarios->blockSignals(false);

        Inserts->append(ui->tableUsuarios->rowCount()-1);
        Mods->append(false);
        mapPassUser.insert(user,pass);

        ui->tableUsuarios->resizeColumnsToContents();
        ui->tableUsuarios->resizeRowsToContents();
    }
}

void Configuracion::EliminarUsuario()
{
    mapPassUser.remove(ui->tableUsuarios->item(ui->tableUsuarios->currentRow(),0)->data(Qt::DisplayRole).toString());
    ui->tableUsuarios->setRowHidden(ui->tableUsuarios->currentRow(),true);
    Mods->replace(ui->tableUsuarios->currentRow(),false);
    Eliminadas->append(ui->tableUsuarios->currentRow());

    if (Inserts->contains(ui->tableUsuarios->currentRow()))
        Inserts->remove(Inserts->indexOf(ui->tableUsuarios->currentRow()));
}

void Configuracion::Copia_Seguridad()
{
    /*QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Guardar copia seguridad"),QDir::homePath(),QObject::tr("Copias de seguridad sql (*.sql)"));

    if (fileName != "")
    {
        QObject *parent;

        QString program = ui->ledtBinBackup->text();
        QStringList arguments;

        arguments << "-Fc" << "-h" << "localhost" << "-U" << "moi" << "almacen" << "-f" << fileName + ".sql";

        if (program != "")
        {
            QProcess *myProcess = new QProcess(parent);
            myProcess->start(program, arguments);

            if (!myProcess->waitForStarted())
                QMessageBox::information(0,QObject::tr("Copia de seguridad"),QObject::tr("No se ha podido realizar la copia de seguridad.\n"));
            else
            {
                if (myProcess->waitForFinished(600000))
                    QMessageBox::information(0,QObject::tr("Copia de seguridad"),QObject::tr("Copia de seguridad realizada con éxito."));
                else
                    QMessageBox::information(0,QObject::tr("Copia de seguridad"),QObject::tr("No se ha podido realizar la copia de seguridad.\nSe ha sobrepasado el tiempo de espera."));
            }
        }
        else
        {
            QMessageBox::information(0,QObject::tr("Copia de seguridad"),QObject::tr("Seleccione el archivo pg_dump contenido en la carpeta de instalación de PostgreSQL."));
        }
    }*/
}

void Configuracion::SeleccionarArchivo()
{
    /*QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Seleccionar archivo"),QDir::homePath(),QObject::tr("Archivo pg_dump (*.*)"));

    if (fileName == "")
        fileName = "/opt/PostgreSQL/8.4/bin/pg_dump";
    ui->ledtBinBackup->setText(fileName);*/
}
