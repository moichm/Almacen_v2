#include "principal.h"
#include "ui_principal.h"
#include "Movimientos/movimientoslistas.h"
#include "Kits/kits.h"
#include "Productos/listas.h"
#include "Operarios/operariospartes.h"
#include "Centros/centros.h"
#include "Vehiculos/vehiculos.h"
#include "ventanaLogin.h"
#include "Productos/producto.h"
#include "configuracion.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include "clsconfiguracion.h"
#include <QLocale>
#include <QTranslator>
#include <QActionGroup>
#include <QMdiSubWindow>
#include <QInputDialog>
#include <QSqlQuery>

extern clsConfiguracion *config;

Principal::Principal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Principal)
{
    ui->setupUi(this);

    conexion = new conexionBD();

    if(!conexion->conectarBD())
        boEntra=false;
    else
        boEntra=CargarSesion();

    if (boEntra)
    {
        connect(ui->actionSalir,SIGNAL(triggered()),this,SLOT(Salir()));
        connect(ui->actionCerrar_sesi_n,SIGNAL(triggered()),this,SLOT(CerrarSesion()));
        connect(ui->actionAbrir_sesi_n,SIGNAL(triggered()),this,SLOT(AbrirSesion()));
        connect(ui->actionProductos,SIGNAL(triggered()),this,SLOT(AbrirProductos()));
        connect(ui->actionMovimientos,SIGNAL(triggered()),this,SLOT(AbrirVentanaMovs()));
        connect(ui->actionKits,SIGNAL(triggered()),this,SLOT(AbrirVentanaKits()));
        connect(ui->actionVehiculos,SIGNAL(triggered()),this,SLOT(AbrirVehiculos()));
        connect(ui->actionOperarios,SIGNAL(triggered()),this,SLOT(AbrirOperariosPartes()));
        connect(ui->actionCentros,SIGNAL(triggered()),this,SLOT(AbrirCentros()));
        connect(ui->action_Configuracion,SIGNAL(triggered()),this,SLOT(AbrirVentanaOpciones()));
        connect(ui->actionSalir_Menu,SIGNAL(triggered()),this,SLOT(Salir()));
        connect(ui->actionCambiar_Contrase_a,SIGNAL(triggered()),this,SLOT(CambiarPass()));
        connect(ui->actionCascada_2,SIGNAL(triggered()),this,SLOT(VCascada()));
        connect(ui->actionMosaico_2,SIGNAL(triggered()),this,SLOT(VMosaico()));
        connect(ui->actionCerrar_todas,SIGNAL(triggered()),this,SLOT(CerrarTodas()));

        transActual = 0;
        IdiomasDisponibles();
    }
}

Principal::~Principal()
{
    ui->contMDI->closeAllSubWindows();
    delete transActual;
    delete conexion;
    delete ui;
}

void Principal::closeEvent(QCloseEvent *event)
{
    ui->contMDI->closeAllSubWindows();
    if (ui->contMDI->currentSubWindow())
        event->ignore();
    else
        event->accept();
}

void Principal::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);

    QMainWindow::changeEvent(e);
}

void Principal::AbrirProductos()
{
    listas *mdiHijo = new listas();
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

void Principal::AbrirVentanaMovs()
{
    MovimientosListas *mdiHijo = new MovimientosListas();
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

void Principal::AbrirVentanaKits()
{
    Kits *mdiHijo = new Kits();
    ui->contMDI->setAttribute(Qt::WA_DeleteOnClose);
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

void Principal::AbrirOperariosPartes()
{
    OperariosPartes *mdiHijo = new OperariosPartes();
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

void Principal::AbrirVehiculos()
{
    Vehiculos *mdiHijo = new Vehiculos();
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

void Principal::AbrirCentros()
{
    Centros *mdiHijo = new Centros();
    QMdiSubWindow *sw = ui->contMDI->addSubWindow(mdiHijo);
    connect(mdiHijo,SIGNAL(closeMDI()),sw,SLOT(close()));
    ui->contMDI->setActiveSubWindow(sw);
    mdiHijo->showMaximized();
}

bool Principal::CargarSesion()
{
    QDesktopWidget *desktop = QApplication::desktop();
    ventanaLogin *Login=new ventanaLogin();
    int x, y,intentos=0;
    bool para=false;

    x = (desktop->width() - Login->width()) / 2;
    y = (desktop->height() - Login->height()) / 2;

    Login->move(x,y);
    Login->setWindowModality(Qt::ApplicationModal);

    while (!para)
    {
        Login->show();

        if((Login->exec()==QDialog::Accepted))
        {
            if(Login->ComprobarUsuario() && Login->ComprobarPass())
            {                
                config->Cargar(Login->Id_Usuario(),qApp);
                if (!config->ExisteArchivoConf())
                    AbrirVentanaOpciones();

                config->IdiomasDisponibles();

                para = true;

                OcultarAcciones();

                ComprobarProductos();

                return true;
            }
            else
            {
                intentos++;
                if(intentos == 1)
                    QMessageBox::critical(0,QObject::tr("ERROR"),QObject::tr("Usuario o contraseña incorrectos\nIntentos 1 de 3"),0,0);
                else if(intentos == 2)
                    QMessageBox::critical(0,QObject::tr("ERROR"),QObject::tr("Usuario o contraseña incorrectos\nIntentos 2 de 3"),0,0);
                else
                    QMessageBox::critical(0,QObject::tr("ERROR"),QObject::tr("Usuario o contraseña incorrectos\nIntentos 3 de 3"),0,0);
            }
            if (intentos == 3)
            {
                para = true;
                return false;
            }
        }
        else
        {
            para =true;
            return false;
        }
    }
    return false;
}

void Principal::ComprobarProductos()
{
    QString strConsulta = "SELECT productos.idproducto "
            "FROM productos "
            "ORDER BY productos.idproducto;";


    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        while(consulta.next())
        {
            clsProductos Prod;
            Prod.Cargar(consulta.value(0).toString());
            if (Prod.Estado() != eDisponible)
                Prod.setDisponible(false);
            else
                Prod.setDisponible(true);
            Prod.Modificar();
        }
    }
}

void Principal::AbrirSesion()
{
    CerrarSesion();
    if(ui->contMDI->subWindowList().count() == 0)
    {
        if (!CargarSesion())
            qApp->quit();
        else
        {
            ui->tbarGeneral->setEnabled(true);
            ui->tbarGeneral->setHidden(false);
            ui->menuCambiar_idioma->setEnabled(true);
            ui->action_Configuracion->setEnabled(true);
        }
    }
}

void Principal::CerrarSesion()
{
    ui->contMDI->closeAllSubWindows();
    DeshabilitarControles();
}

void Principal::DeshabilitarControles()
{
    ui->tbarGeneral->setHidden(true);
    ui->action_Configuracion->setEnabled(false);
    ui->menuCambiar_idioma->setEnabled(false);
}

void Principal::AbrirVentanaOpciones()
{
    Configuracion *ventanaOpciones = new Configuracion();
    ventanaOpciones->setWindowModality(Qt::ApplicationModal);
    ventanaOpciones->show();

    connect(ventanaOpciones,SIGNAL(Guardado()),this,SLOT(CerrarTodas()));
}

void Principal::OcultarAcciones()
{
    QMap<tPermisos,bool> Permisos;
    Permisos = config->Permisos();
    if (Permisos.size() > 0)
    {
        ui->actionKits->setVisible(Permisos.value(KitVer));
        ui->actionMovimientos->setVisible(Permisos.value(MovVer));
        ui->actionCentros->setVisible(Permisos.value(CentVer));
        ui->actionProductos->setVisible(Permisos.value(ProdVer));
        ui->actionOperarios->setVisible(Permisos.value(OpVer));
        ui->actionVehiculos->setVisible(Permisos.value(VehVer));
        ui->action_Configuracion->setVisible(Permisos.value(Confi));
    }
}

void Principal::Traducir(QAction* action)
{
    if (transActual)
        qApp->removeTranslator(transActual);
    transActual = config->Traducciones().value((action->data().toString()), 0);
    if (transActual)
    {
        if(ui->contMDI->subWindowList().size() > 0)
        {
            if (QMessageBox::question(this,QObject::tr("Traducir"),QObject::tr("Se van a cerrar las ventanas abiertas, ¿desea continuar?"),QMessageBox::Yes | QMessageBox::No,QMessageBox::Yes) == QMessageBox::Yes)
            {
                ui->contMDI->closeAllSubWindows();
                qApp->installTranslator(transActual);
                config->setIdioma(action->data().toString());
            }
        }
        else
        {
            qApp->installTranslator(transActual);
            config->setIdioma(action->data().toString());
        }
    }
}

void Principal::IdiomasDisponibles()
{
    QStringList listaIdiomas = config->Idiomas();

    QActionGroup* actions = new QActionGroup(this);
    connect(actions, SIGNAL(triggered(QAction*)), this, SLOT(Traducir(QAction*)));

    QStringList partes = config->Idioma().split("_");
    QString lenguaje = partes.at(1).toLower();
    QString pais = partes.at(0).toUpper();
    QString lenguajeactual = "";
    QString paisactual = "";
    QString idioma = "";

    for (int i=0;i<listaIdiomas.size();i++)
    {
        QLocale local(listaIdiomas.at(i));

        idioma = listaIdiomas.at(i);
        QStringList partes = idioma.split("_");
        lenguajeactual = partes.at(1).toLower();
        paisactual = partes.at(0).toUpper();

        QAction* action = ui->menuCambiar_idioma->addAction(QLocale::languageToString(local.language()) + " (" + QLocale::countryToString(local.country()) + ")");
        action->setData(listaIdiomas.at(i));
        action->setCheckable(true);
        if ((lenguaje == lenguajeactual) && (pais == paisactual))
        {
            action->setChecked(true);
            Traducir(action);
        }
        actions->addAction(action);
    }
}

void Principal::Salir()
{
    config->GuardarArchivoConf();
    qApp->quit();
}

void Principal::CambiarPass()
{
    bool ok;
    QString pass1 = QInputDialog::getText(this, QObject::tr("Cambio de contraseña"),QObject::tr("Introduzca la nueva contraseña:"), QLineEdit::Normal,"", &ok);
    if (ok && !pass1.isEmpty())
    {
        QString pass2 = QInputDialog::getText(this, QObject::tr("Cambio de contraseña"),QObject::tr("Introduzca la nueva contraseña:"), QLineEdit::Normal,"", &ok);
        if (ok && !pass2.isEmpty())
        {
            if (pass1 != pass2)
                QMessageBox::information(0,QObject::tr("Cambio de contraseña"),QObject::tr("La contraseña no ha sido modificada.\n Las dos contraseñas introducidas no coinciden."));
            else
            {
                QMap<tPermisos,bool> Permisos;
                Permisos = config->Permisos();
                QPair<QString,QString> parUser;
                parUser.first = config->IdUsuarioActual();
                parUser.second = pass1;
                config->ModUsuarios(parUser,Permisos);
                QMessageBox::information(0,QObject::tr("Cambio de contraseña"),QObject::tr("La contraseña se ha modificado correctamente."));
            }
        }
        else
            QMessageBox::information(0,QObject::tr("Cambio de contraseña"),QObject::tr("La contraseña no ha sido modificada."));
    }
    else
        QMessageBox::information(0,QObject::tr("Cambio de contraseña"),QObject::tr("La contraseña no ha sido modificada."));
}

void Principal::VCascada()
{
    ui->contMDI->cascadeSubWindows();
}

void Principal::VMosaico()
{
    ui->contMDI->tileSubWindows();
}

void Principal::CerrarTodas()
{
    ui->contMDI->closeAllSubWindows();
}
