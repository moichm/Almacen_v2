#include "clsconfiguracion.h"
#include "./Centros/centro.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QVariant>
#include <QSqlError>
#include <QDir>
#include <QTranslator>


clsConfiguracion::clsConfiguracion(){}

clsConfiguracion::clsConfiguracion(QString _id_usuario,QApplication *app)
{
    qapp = app;
    this->strIdUsuario = _id_usuario;
    CargarArchivoConf();

    ListIdIdiomasDispo = new QMap<QString,QTranslator*>();
}

clsConfiguracion::~clsConfiguracion()
{
    delete ListIdIdiomasDispo;
    delete qapp;
    delete mPermisos;
}

void clsConfiguracion::Cargar(QString _id_usuario,QApplication *app)
{
    this->strIdUsuario = _id_usuario;
    qapp = app;
    CargarArchivoConf();

    ListIdIdiomasDispo = new QMap<QString,QTranslator*>();
}

QString clsConfiguracion::IdUsuarioActual() const
{
    return this->strIdUsuario;
}

void clsConfiguracion::setCentroActual(int _Centro)
{
    this->intCentro = _Centro;
}

int clsConfiguracion::IdCentroActual() const
{
    return this->intCentro;
}

void clsConfiguracion::setUsuarioActual(QString idUsuario)
{
    this->strIdUsuario = idUsuario;
}

bool clsConfiguracion::ExisteArchivoConf() const
{
    QFile file;
    QString rutaArchivo = qapp->applicationDirPath() + "/Conf.config";
    return file.exists(rutaArchivo);
}

void clsConfiguracion::GuardarArchivoConf()
{
    QFile file;
    QString rutaArchivo = qapp->applicationDirPath() + "/Conf.config";
    if (file.exists(rutaArchivo))
    {
        archConf = new QFile(rutaArchivo);
        archConf->remove();
        if (archConf->open(QIODevice::ReadWrite | QFile::Text))
        {
            QXmlStreamWriter writer(archConf);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            writer.writeStartElement("Configuracion");
            writer.writeTextElement("Centro",QString::number(this->intCentro));
            writer.writeTextElement("Idioma",this->strIdioma);
            writer.writeEndElement();
            writer.writeEndDocument();
            archConf->close();
            delete archConf;
        }
        else
            QMessageBox::critical(0,QObject::tr("Error al crear el archivo"),QObject::tr("Ha ocurrido un error al crear el archivo.\n\nNo se ha podido guardar el archivo") + rutaArchivo);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error al guardar el archivo"),QObject::tr("Ha ocurrido un error al guardar el archivo.\n\nNo existe el archivo") + rutaArchivo);
}


void clsConfiguracion::CrearArchivoConf()
{
    QFile file;
    QString rutaArchivo = qapp->applicationDirPath() + "/Conf.config";
    if (!file.exists(rutaArchivo))
    {
        archConf = new QFile(rutaArchivo);
        if (archConf->open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter writer(archConf);
            writer.setAutoFormatting(true);
            writer.writeStartDocument();
            writer.writeStartElement("Configuracion");
            writer.writeTextElement("Centro",QString::number(this->intCentro));
            writer.writeTextElement("Idioma",this->strIdioma);
            writer.writeEndElement();
            writer.writeEndDocument();
            archConf->close();
        }
        else
            QMessageBox::critical(0,QObject::tr("Error al crear el archivo"),QObject::tr("Ha ocurrido un error al crear el archivo.\n\nNo se ha podido crear el archivo ") + rutaArchivo);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error al crear el archivo"),QObject::tr("Ha ocurrido un error al crear el archivo.\n\nYa existe el archivo ") + rutaArchivo);
}

void clsConfiguracion::CargarArchivoConf()
{
    QFile file;
    QXmlStreamReader reader;
    QString rutaArchivo = qapp->applicationDirPath() + "/Conf.config";
    if (file.exists(rutaArchivo))
    {
        archConf = new QFile(rutaArchivo);
        if (archConf->open(QIODevice::ReadOnly | QFile::Text))
        {
            reader.setDevice(archConf);
            while (!reader.atEnd())
            {
                if (reader.readNextStartElement())
                {
                    if (reader.name() == "Centro")
                        intCentro = reader.readElementText().toInt();
                    if (reader.name() == "Idioma")
                        strIdioma = reader.readElementText();
                }
            }
            archConf->close();
        }
        else
            QMessageBox::critical(0,QObject::tr("Error al abrir el archivo"),QObject::tr("Ha ocurrido un error al abrir el archivo.\n\nNo se ha podido abrir el archivo ") + rutaArchivo);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error al abrir el archivo"),QObject::tr("Ha ocurrido un error al abrir el archivo.\n\nNo existe el archivo ") + rutaArchivo);
}


QMap<QPair<QString,QString>,QMap<tPermisos,bool> > clsConfiguracion::Usuarios() const
{
    return this->mUsuarios;
}

void clsConfiguracion::ModUsuarios(QPair<QString,QString> usuario,QMap<tPermisos,bool> _Permisos)
{    
    QString strProductosConsultar,strProductosModificar,strMovimientosConsultar,strMovimientosModificar,strKitsConsultar,strKitsModificar,
            strVehiculosConsultar,strVehiculossModificar,strOperariosConsultar,strOperariosModificar,strCentrosConsultar,strCentrosModificar,
            strConfiguracion;

    if (_Permisos.value(ProdVer) == true)
        strProductosConsultar = "true";
    else if (_Permisos.value(ProdVer) == false)
        strProductosConsultar = "false";

    if (_Permisos.value(ProdMod) == true)
        strProductosModificar = "true";
    else if (_Permisos.value(ProdMod) == false)
        strProductosModificar = "false";

    if (_Permisos.value(MovVer) == true)
        strMovimientosConsultar = "true";
    else if (_Permisos.value(MovVer) == false)
        strMovimientosConsultar = "false";

    if (_Permisos.value(MovMod) == true)
        strMovimientosModificar = "true";
    else if (_Permisos.value(MovMod) == false)
        strMovimientosModificar = "false";

    if (_Permisos.value(KitVer) == true)
        strKitsConsultar = "true";
    else if (_Permisos.value(KitVer) == false)
        strKitsConsultar = "false";

    if (_Permisos.value(KitMod) == true)
        strKitsModificar = "true";
    else if (_Permisos.value(KitMod) == false)
        strKitsModificar = "false";

    if (_Permisos.value(VehVer) == true)
        strVehiculosConsultar = "true";
    else if (_Permisos.value(VehVer) == false)
        strVehiculosConsultar = "false";

    if (_Permisos.value(VehMod) == true)
        strVehiculossModificar = "true";
    else if (_Permisos.value(VehMod) == false)
        strVehiculossModificar = "false";

    if (_Permisos.value(OpVer) == true)
        strOperariosConsultar = "true";
    else if (_Permisos.value(OpVer) == false)
        strOperariosConsultar = "false";

    if (_Permisos.value(OpMod) == true)
        strOperariosModificar = "true";
    else if (_Permisos.value(OpMod) == false)
        strOperariosModificar = "false";

    if (_Permisos.value(CentVer) == true)
        strCentrosConsultar = "true";
    else if (_Permisos.value(CentVer) == false)
        strCentrosConsultar = "false";

    if (_Permisos.value(CentMod) == true)
        strCentrosModificar = "true";
    else if (_Permisos.value(CentMod) == false)
        strCentrosModificar = "false";

    if (_Permisos.value(Confi) == true)
        strConfiguracion = "true";
    else if (_Permisos.value(Confi) == false)
        strConfiguracion = "false";

    QString strConsulta = "UPDATE perm_user SET "
                          "ProdCon = " + strProductosConsultar + ", ProdMod = " + strProductosModificar + ","
                          "movCon = " + strMovimientosConsultar + ", movMod = " + strMovimientosModificar + ","
                          "kitCon = " + strKitsConsultar + ", kitMod = " + strKitsModificar + ","
                          "VehiCon = " + strVehiculosConsultar + ", VehiMod = " + strVehiculossModificar + ","
                          "OpCon = " + strOperariosConsultar + ", OpMod = " + strOperariosModificar + ","
                          "CentCon = " + strCentrosConsultar + ", CentMod = " + strCentrosModificar + ","
                          "Conf = " + strConfiguracion + " WHERE idUsuario = '" + usuario.first + "';";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsConfiguracion::CargarUsuarios()
{
    QMap<tPermisos,bool> *Permisos = new QMap<tPermisos,bool>;

    mUsuarios.clear();

    QString strConsulta = "SELECT idUsuario,pass,ProdCon,ProdMod,MovCon,MovMod,"
                          "KitCon,KitMod,VehiCon,VehiMod,OpCon,OpMod,CentCon,CentMod,Conf "
                          "FROM perm_user;";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        while (consulta.next())
        {
            Permisos->insert(ProdVer,consulta.value(2).toBool());
            Permisos->insert(ProdMod,consulta.value(3).toBool());
            Permisos->insert(MovVer,consulta.value(4).toBool());
            Permisos->insert(MovMod,consulta.value(5).toBool());
            Permisos->insert(KitVer,consulta.value(6).toBool());
            Permisos->insert(KitMod,consulta.value(7).toBool());
            Permisos->insert(VehVer,consulta.value(8).toBool());
            Permisos->insert(VehMod,consulta.value(9).toBool());
            Permisos->insert(OpVer,consulta.value(10).toBool());
            Permisos->insert(OpMod,consulta.value(11).toBool());
            Permisos->insert(CentVer,consulta.value(12).toBool());
            Permisos->insert(CentMod,consulta.value(13).toBool());
            Permisos->insert(Confi,consulta.value(14).toBool());

            QPair<QString,QString> par;
            par.first = consulta.value(0).toString();
            par.second = consulta.value(1).toString();

            mUsuarios.insert(par,*Permisos);
        }
        Permisos->~QMap();
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsConfiguracion::AnyadirUsuario(QPair<QString,QString> user,QMap<tPermisos,bool> _Permisos)
{
    QString strProductosConsultar,strProductosModificar,strMovimientosConsultar,strMovimientosModificar,strKitsConsultar,strKitsModificar,
            strVehiculosConsultar,strVehiculossModificar,strOperariosConsultar,strOperariosModificar,strCentrosConsultar,strCentrosModificar,
            strConfiguracion;

    if (_Permisos[ProdVer] == true)
        strProductosConsultar = "true";
    else if (_Permisos.value(ProdVer) == false)
        strProductosConsultar = "false";

    if (_Permisos.value(ProdMod) == true)
        strProductosModificar = "true";
    else if (_Permisos.value(ProdMod) == false)
        strProductosModificar = "false";

    if (_Permisos.value(MovVer) == true)
        strMovimientosConsultar = "true";
    else if (_Permisos.value(MovVer) == false)
        strMovimientosConsultar = "false";

    if (_Permisos.value(MovMod) == true)
        strMovimientosModificar = "true";
    else if (_Permisos.value(MovMod) == false)
        strMovimientosModificar = "false";

    if (_Permisos.value(KitVer) == true)
        strKitsConsultar = "true";
    else if (_Permisos.value(KitVer) == false)
        strKitsConsultar = "false";

    if (_Permisos.value(KitMod) == true)
        strKitsModificar = "true";
    else if (_Permisos.value(KitMod) == false)
        strKitsModificar = "false";

    if (_Permisos.value(VehVer) == true)
        strVehiculosConsultar = "true";
    else if (_Permisos.value(VehVer) == false)
        strVehiculosConsultar = "false";

    if (_Permisos.value(VehMod) == true)
        strVehiculossModificar = "true";
    else if (_Permisos.value(VehMod) == false)
        strVehiculossModificar = "false";

    if (_Permisos.value(OpVer) == true)
        strOperariosConsultar = "true";
    else if (_Permisos.value(OpVer) == false)
        strOperariosConsultar = "false";

    if (_Permisos.value(OpMod) == true)
        strOperariosModificar = "true";
    else if (_Permisos.value(OpMod) == false)
        strOperariosModificar = "false";

    if (_Permisos.value(CentVer) == true)
        strCentrosConsultar = "true";
    else if (_Permisos.value(CentVer) == false)
        strCentrosConsultar = "false";

    if (_Permisos.value(CentMod) == true)
        strCentrosModificar = "true";
    else if (_Permisos.value(CentMod) == false)
        strCentrosModificar = "false";

    if (_Permisos.value(Confi) == true)
        strConfiguracion = "true";
    else if (_Permisos.value(Confi) == false)
        strConfiguracion = "false";

    QString strConsulta;

    QSqlQuery consulta;

    strConsulta = "INSERT INTO perm_user VALUES ('" + user.first + "','" + user.second + "'," +
                  strProductosConsultar + "," + strProductosModificar + ","  +
                  strVehiculosConsultar + "," + strVehiculossModificar + "," +
                  strOperariosConsultar + "," + strOperariosModificar + ","+
                  strCentrosConsultar + "," + strCentrosModificar + "," +
                  strMovimientosConsultar + "," + strMovimientosModificar + "," +
                  strKitsConsultar + "," + strKitsModificar + "," +
                  strConfiguracion + ");";

    consulta.clear();

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsConfiguracion::EliminarUsuario(QString user)
{
    QString strConsulta = "DELETE FROM perm_user WHERE idUsuario = '" + user +"';";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el usuario. No existe."),0,0);
}

QMap<tPermisos,bool> clsConfiguracion::Permisos()
{    
    mPermisos = new QMap<tPermisos,bool>;

    QString strConsulta = "SELECT ProdCon,ProdMod,MovCon,MovMod,KitCon,KitMod, "
                          "VehiCon,VehiMod,OpCon,OpMod,CentCon,CentMod,Conf "
                          "FROM perm_user WHERE idUsuario = '" + strIdUsuario + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            mPermisos->insert(ProdVer,consulta.value(0).toBool());
            mPermisos->insert(ProdMod,consulta.value(1).toBool());
            mPermisos->insert(MovVer,consulta.value(2).toBool());
            mPermisos->insert(MovMod,consulta.value(3).toBool());
            mPermisos->insert(KitVer,consulta.value(4).toBool());
            mPermisos->insert(KitMod,consulta.value(5).toBool());
            mPermisos->insert(VehVer,consulta.value(6).toBool());
            mPermisos->insert(VehMod,consulta.value(7).toBool());
            mPermisos->insert(OpVer,consulta.value(8).toBool());
            mPermisos->insert(OpMod,consulta.value(9).toBool());
            mPermisos->insert(CentVer,consulta.value(10).toBool());
            mPermisos->insert(CentMod,consulta.value(11).toBool());
            mPermisos->insert(Confi,consulta.value(12).toBool());
        }
        else
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);

    return *mPermisos;
}

void clsConfiguracion::setIdioma(QString _Idioma)
{
    strIdioma = _Idioma;
}

QString clsConfiguracion::Idioma() const
{
    return this->strIdioma;
}

void clsConfiguracion::IdiomasDisponibles()
{
    QDir dir;

    QString filtro = "*_*.qm";
    QDir::Filters filtros = QDir::Files | QDir::Readable;
    QDir::SortFlags ord = QDir::Name;
    QFileInfoList entradas = dir.entryInfoList(QStringList() << filtro, filtros, ord);
    QFileInfo archivo;
    for (int i = 0;i< entradas.size();i++)
    {
        archivo = entradas.at(i);
        QStringList partes = archivo.baseName().split("_");
        QString lenguaje = partes.at(2).toLower();
        QString pais = partes.at(1).toUpper();

        QTranslator* traductor = new QTranslator();
        if (traductor->load(archivo.absoluteFilePath()))
        {
            QString local = lenguaje + "_" + pais;
            ListIdIdiomasDispo->insert(local, traductor);
        }
    }    
}

QStringList clsConfiguracion::Idiomas()
{
    return QStringList(ListIdIdiomasDispo->keys());
}

QMap<QString,QTranslator *> clsConfiguracion::Traducciones() const
{
    return *ListIdIdiomasDispo;
}
