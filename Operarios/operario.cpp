#include "Centros/centro.h"
#include "operario.h"
#include <QSqlRecord>
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include "clsconfiguracion.h"
#include "Centros/centro.h"

extern clsConfiguracion *config;

clsOperarios::clsOperarios(QString _NIF,QString _Nom,QString _Ape,QDate _FechaNacimiento,QString _Direcc,QString _Email,QString _NCCC,int _Centro,QString _rutaImage)
{
    this->strNIF = _NIF;
    this->strNombre = _Nom;
    this->strApellidos = _Ape;
    this->datFechaNacimiento = _FechaNacimiento;
    this->strDireccion = _Direcc;
    this->strEmail = _Email;
    this->strNCCC = _NCCC;
    this->intCentro = _Centro;
    this->strRutaImagen = _rutaImage;
}

clsOperarios::~clsOperarios(){}

bool clsOperarios::Insertar()
{
    if (!Existe())
    {
        QString strConsulta;

        if (strNIF == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un NIF para el operario"),0,0);
            return false;
        }
        else if (strNombre == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nombre para el operario"),0,0);
            return false;
        }
        else if (strApellidos == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir unos apellidos para el operario"),0,0);
            return false;
        }
        else if (datFechaNacimiento.toString("yyyy/MM/dd") == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una fecha de nacimiento para el operario"),0,0);
            return false;
        }
        else if (strDireccion == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una dirección para el operario"),0,0);
            return false;
        }
        else if (strNCCC == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de ccc para el operario"),0,0);            
            return false;
        }
        else if (this->intCentro == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro para el operario"),0,0);
            return false;
        }
        else
        {
            strConsulta =  "INSERT INTO operarios VALUES ('" + strNIF + "', '" + strNombre + "', '" + strApellidos + "', '" +
                           datFechaNacimiento.toString("yyyy/MM/dd") + "', '" + strDireccion + "', '" + strEmail + "', '" + strNCCC + "', " + QString::number(intCentro) +
                           + ", '" + strRutaImagen + "');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el operario. Ya existe."),0,0);
        return false;
    }
}

bool clsOperarios::Modificar()
{
    if (Existe())
    {
        QString strConsulta;

        if (strNIF == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un NIF para el operario"),0,0);
            return false;
        }
        else if (strNombre == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nombre para el operario"),0,0);
            return false;
        }
        else if (strApellidos == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir unos apellidos para el operario"),0,0);
            return false;
        }
        else if (datFechaNacimiento.toString("yyyy/MM/dd") == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una fecha de nacimiento para el operario"),0,0);
            return false;
        }
        else if (strDireccion == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una dirección para el operario"),0,0);
            return false;
        }
        else if (strNCCC == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de ccc para el operario"),0,0);
            return false;
        }
        else if (this->intCentro == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro para el operario"),0,0);
            return false;
        }
        else
        {
            strConsulta =  "UPDATE operarios SET Nombre= '" + strNombre + "', apellidos= '" + strApellidos + "', fecha_nacimiento='" + datFechaNacimiento.toString("yyyy/MM/dd") +
                           "', direccion='" + strDireccion + "', email='" + strEmail + "', ccc='" + strNCCC + "', idcentro=" + QString::number(intCentro) +
                           + ", Imagen='" + strRutaImagen + "' WHERE NIF ='"+ strNIF +"';";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede modificar el operario. No existe."),0,0);
        return false;
    }
}

bool clsOperarios::Eliminar()
{
    if (Existe())
    {
        QString strConsulta;

        QSqlQuery consulta;

        strConsulta = "DELETE FROM operarios WHERE NIF='" + strNIF + "';";

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
            return true;
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el operario. No existe."),0,0);
        return false;
    }
}

void clsOperarios::Cargar(QString NIF)
{
    QString strConsulta;

    strConsulta = "SELECT nombre,apellidos,fecha_nacimiento,direccion,email,ccc,idcentro,imagen "
                  "FROM operarios WHERE nif = '" + NIF + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->strNIF = NIF;
            this->strNombre = consulta.value(0).toString();
            this->strApellidos = consulta.value(1).toString();
            this->datFechaNacimiento = consulta.value(2).toDate();
            this->strDireccion = consulta.value(3).toString();
            this->strEmail = consulta.value(4).toString();
            this->strNCCC = consulta.value(5).toString();
            this->intCentro = consulta.value(6).toInt();
            this->strRutaImagen = consulta.value(7).toString();
        }
    }
}

bool clsOperarios::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM operarios WHERE nif = '" + strNIF + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QString clsOperarios::NIF() const
{
    return this->strNIF;
}

QString clsOperarios::Nombre() const
{
    return this->strNombre;
}

QString clsOperarios::Apellidos() const
{
    return this->strApellidos;
}

QString clsOperarios::Direccion() const
{
    return this->strDireccion;
}

QString clsOperarios::Email() const
{
    return this->strEmail;
}

QString clsOperarios::CCC() const
{
    return this->strNCCC;
}

QString clsOperarios::Imagen() const
{
    return this->strRutaImagen;
}

int clsOperarios::Centro() const
{
    return this->intCentro;
}

void clsOperarios::setCentro(int _Centro)
{
    intCentro = _Centro;
}

void clsOperarios::setNombre(QString _Nombre)
{
    strNombre = _Nombre;
}

void clsOperarios::setNIF(QString _NIF)
{
    this->strNIF = _NIF;
}

void clsOperarios::setApellidos(QString _Apellidos)
{
    strApellidos = _Apellidos;
}

void clsOperarios::setDireccion(QString _Direccion)
{
    strDireccion = _Direccion;
}

void clsOperarios::setEmail(QString _Email)
{
    strEmail = _Email;
}

void clsOperarios::setCCC(QString _CCC)
{
    strNCCC = _CCC;
}

void clsOperarios::setImagen(QString _Imagen)
{
    this->strRutaImagen = _Imagen;
}

QDate clsOperarios::FechaNacimiento() const
{
    return this->datFechaNacimiento;
}

void clsOperarios::setFechaNacimiento(QDate _FechaNaci)
{
    datFechaNacimiento = _FechaNaci;
}

QVector<QStringList> clsOperarios::ListaOperarios(QString filtro)
{
    bool permiso = config->Permisos().value(Confi);

    QString strConsulta = "SELECT nif,operarios.nombre,apellidos,fecha_nacimiento,operarios.direccion,email,ccc,concat_ws(' - ',centros.nombre,centros.ciudad) as centro "
                          "FROM operarios INNER JOIN centros ON centros.idcentro=operarios.idcentro";

    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(!permiso)
    {
        strConsulta += " WHERE operarios.idCentro = '" + QString::number(config->IdCentroActual()) + "'";

        if(filtro != "")
            strConsulta += " AND " + filtro;
    }
    else
    {
        if(filtro != "")
            strConsulta += " WHERE " + filtro;
    }

    strConsulta += " ORDER BY NIF;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toString()
                           << sqlConsulta.value(2).toString()
                           << sqlConsulta.value(3).toDate().toString(QObject::tr("dd/MM/yyyy"))
                           << sqlConsulta.value(4).toString()
                           << sqlConsulta.value(5).toString()
                           << sqlConsulta.value(6).toString()
                           << sqlConsulta.value(7).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsPartes_Trabajo::clsPartes_Trabajo(int _Parte,QString _Responsable,QString _Descripcion,QDate _FechaParte)
{
    this->intParte = _Parte;
    this->strResponsable = _Responsable;
    this->strDescripcion = _Descripcion;
    this->datFechaParte = _FechaParte;
    this->intCentro = config->IdCentroActual();
}

clsPartes_Trabajo::~clsPartes_Trabajo(){}

bool clsPartes_Trabajo::Insertar()
{
    if (!Existe())
    {
        QString strConsulta;

        if (strResponsable == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un responsable para el parte."),0,0);
            return false;
        }
        else if (datFechaParte.toString("yyyy/MM/dd") == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una fecha para el parte."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "INSERT INTO partes_trabajo (fecha_parte,responsable,descripcion,idcentro) VALUES ('" + datFechaParte.toString("yyyy/MM/dd") + "', '" + strResponsable + "', '" + strDescripcion + "',"+QString::number(this->intCentro)+");";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
            {
                strConsulta = "SELECT LAST_INSERT_ID() FROM partes_trabajo;";
                if (!consulta.exec(strConsulta))
                {
                    QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                    return false;
                }
                else
                {
                    if (consulta.first())
                    {
                        this->intParte = consulta.record().value(0).toInt();
                        return true;
                    }
                    else
                    {
                        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                        return false;
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el parte. Ya existe."),0,0);
        return false;
    }
}

bool clsPartes_Trabajo::Modificar()
{
    if (Existe())
    {
        QString strConsulta;

        if (strResponsable == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un responsable para el parte."),0,0);
            return false;
        }
        else if (datFechaParte.toString("yyyy/MM/dd") == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una fecha para el parte."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "UPDATE partes_trabajo SET Responsable= '" + strResponsable + "', Descripcion= '" + strDescripcion + "', fecha_parte='" + datFechaParte.toString("yyyy/MM/dd") +
                           "' WHERE idparte ="+ QString::number(intParte) +";";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede modificar el parte. No existe."),0,0);
        return false;
    }
}

bool clsPartes_Trabajo::Eliminar()
{
    if (Existe())
    {
        QString strConsulta;

        QSqlQuery consulta;

        strConsulta = "DELETE FROM partes_trabajo WHERE idparte=" + QString::number(intParte) + ";";

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
            return true;
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el parte. No existe."),0,0);
        return false;
    }
}

void clsPartes_Trabajo::Cargar(int NoParte)
{
    QString strConsulta;

    strConsulta = "SELECT fecha_parte,responsable,descripcion,idcentro "
                  "FROM partes_trabajo WHERE idparte = " + QString::number(NoParte) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        if (consulta.first())
        {
            this->intParte = NoParte;
            this->datFechaParte = consulta.value(0).toDate();
            this->strResponsable = consulta.value(1).toString();
            this->strDescripcion = consulta.value(2).toString();
            this->intCentro = consulta.value(3).toInt();

            strConsulta = "SELECT idOperario,tipohora,proyecto,tarea FROM linea_partes WHERE idparte = " + QString::number(intParte) + ";";

            QSqlQuery consultaDetalles;

            if (consultaDetalles.exec(strConsulta))
            {
                ListDetalles.clear();
                while(consultaDetalles.next())
                {
                    clsPartes_Trabajo_Det Det;
                    tHora tipoHora;
                    if (consultaDetalles.value(1).toString() == QObject::tr("Normal"))
                        tipoHora= Normal;
                    else if (consultaDetalles.value(1).toString() == QObject::tr("Extra"))
                        tipoHora = Extra;
                    else
                        tipoHora = Normal;
                    Det.Cargar(NoParte,consultaDetalles.value(0).toString(),tipoHora,consultaDetalles.value(2).toString(),consultaDetalles.value(3).toString());
                    ListDetalles << Det;
                }
            }
        }
}

const QList<clsPartes_Trabajo_Det>& clsPartes_Trabajo::Detalles() const
{
    return this->ListDetalles;
}

bool clsPartes_Trabajo::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM partes_trabajo WHERE idparte = " + QString::number(intParte) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QString clsPartes_Trabajo::Responsable() const
{
    return this->strResponsable;
}

QString clsPartes_Trabajo::Descripcion() const
{
    return this->strDescripcion;
}

QDate clsPartes_Trabajo::FechaParte() const
{
    return this->datFechaParte;
}

int clsPartes_Trabajo::NoParte() const
{
    return this->intParte;
}

int clsPartes_Trabajo::Centro() const
{
    return this->intCentro;
}

void clsPartes_Trabajo::setResponsable(QString _Responsable)
{
    this->strResponsable = _Responsable;
}

void clsPartes_Trabajo::setDescripcion(QString _Desc)
{
    this->strDescripcion = _Desc;
}

void clsPartes_Trabajo::setFechaParte(QDate _FechaParte)
{
    this->datFechaParte = _FechaParte;
}

QVector<QStringList> clsPartes_Trabajo::ListaPartes_Trabajo(QString filtro)
{
    bool permiso = config->Permisos().value(Confi);

    QString strConsulta = "SELECT idparte,fecha_parte,CONCAT(operarios.nombre,CONCAT(' ',operarios.apellidos)) as respon,descripcion,CONCAT_WS(' - ',centros.nombre,centros.ciudad) as centro "
            "FROM partes_trabajo INNER JOIN operarios ON operarios.nif = partes_trabajo.responsable INNER JOIN centros ON centros.idCentro = partes_trabajo.idCentro";

    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(!permiso)
    {
        strConsulta += " WHERE operarios.idCentro = '" + QString::number(config->IdCentroActual()) + "'";

        if(filtro != "")
            strConsulta += " AND " + filtro;
    }
    else
    {
        if(filtro != "")
            strConsulta += " WHERE " + filtro;
    }

    strConsulta += " ORDER BY idparte;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toDate().toString(QObject::tr("dd/MM/yyyy"))
                           << sqlConsulta.value(2).toString()
                           << sqlConsulta.value(3).toString()
                           << sqlConsulta.value(4).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsPartes_Trabajo_Det::clsPartes_Trabajo_Det(int _NoParte,QString _NIF,tHora _TipoHora,double _Horas,QString _Proyecto,QString _Tarea)
{
    this->intNoParte = _NoParte;
    this->strNIF = _NIF;
    this->eTipoHora = _TipoHora;
    this->decHoras = _Horas;
    this->strProyecto = _Proyecto;
    this->strTarea = _Tarea;
}

clsPartes_Trabajo_Det::~clsPartes_Trabajo_Det(){}

bool clsPartes_Trabajo_Det::Insertar()
{
    if (!Existe())
    {
        QString strConsulta;

        if (strNIF == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un operario."),0,0);
            return false;
        }
        else if (decHoras == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un número de horas mayor que cero."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "INSERT INTO linea_partes (idparte,idoperario,tipohora,numhoras,proyecto,tarea) "
                           "VALUES ("+QString::number(this->intNoParte) + ",'" + strNIF + "', '" + qltHora[this->eTipoHora] + "', '" +
                                     QString::number(decHoras) + "','"+ strProyecto +"','"+ strTarea +"');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar la línea del parte. Ya existe."),0,0);
        return false;
    }
}

bool clsPartes_Trabajo_Det::Modificar()
{
    if (Existe())
    {
        QString strConsulta;

        if (strNIF == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un operario."),0,0);
            return false;
        }
        else if (decHoras == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un número de horas mayor que cero."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "UPDATE linea_partes SET numhoras=" + QString::number(decHoras) + " "
                           "WHERE idparte = " + QString::number(intNoParte) + " AND idoperario= '" + strNIF + "' AND tipohora= '" + qltHora[this->eTipoHora] + "' "
                                 "AND proyecto='" + strProyecto + "' AND tarea='"+ strTarea +"';";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede modificar la línea del parte. No existe."),0,0);
        return false;
    }
}

bool clsPartes_Trabajo_Det::Eliminar()
{
    if (Existe())
    {
        QString strConsulta;

        QSqlQuery consulta;

        strConsulta = "DELETE FROM linea_partes "
                      "WHERE idparte = " + QString::number(intNoParte) + " AND idoperario= '" + strNIF + "' AND tipohora= '" + qltHora[this->eTipoHora] + "' "
                        "AND proyecto='" + strProyecto + "' AND tarea='"+ strTarea +"';";

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
            return true;
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar la línea del parte. No existe."),0,0);
        return false;
    }
}

void clsPartes_Trabajo_Det::Cargar(int _NoParte,QString _NIF,tHora _TipoHora,QString _Proyecto,QString _Tarea)
{
    QString strConsulta;

    strConsulta = "SELECT numhoras "
                  "FROM linea_partes "
                  "WHERE idparte = " + QString::number(_NoParte) + " AND idoperario= '" + _NIF + "' AND tipohora= '" + qltHora[_TipoHora] + "' "
                     "AND proyecto='" + _Proyecto + "' AND tarea='"+ _Tarea +"';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        if (consulta.first())
        {
            this->intNoParte = _NoParte;
            this->strNIF = _NIF;
            this->eTipoHora = _TipoHora;
            this->decHoras = consulta.value(0).toDouble();
            this->strProyecto = _Proyecto;
            this->strTarea = _Tarea;
        }
}

bool clsPartes_Trabajo_Det::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM linea_partes WHERE idparte = " + QString::number(intNoParte) + " AND idoperario= '" + strNIF + "' AND tipohora= '" + qltHora[this->eTipoHora] + "' AND proyecto='" + strProyecto + "' AND tarea='"+ strTarea +"';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QString clsPartes_Trabajo_Det::Operario() const
{
    return this->strNIF;
}

double clsPartes_Trabajo_Det::Horas() const
{
    return this->decHoras;
}

QString clsPartes_Trabajo_Det::Proyecto() const
{
    return this->strProyecto;
}

QString clsPartes_Trabajo_Det::Tarea() const
{
    return this->strTarea;
}

int clsPartes_Trabajo_Det::NoParte() const
{
    return this->intNoParte;
}

tHora clsPartes_Trabajo_Det::TipoHora() const
{
    return this->eTipoHora;
}

void clsPartes_Trabajo_Det::setTarea(QString _Tarea)
{
    this->strTarea = _Tarea;
}

void clsPartes_Trabajo_Det::setProyecto(QString _Proyecto)
{
    this->strProyecto = _Proyecto;
}

void clsPartes_Trabajo_Det::setOperario(QString _NIF)
{
    this->strNIF = _NIF;
}

void clsPartes_Trabajo_Det::setTipoHora(tHora _TipoHora)
{
    this->eTipoHora = _TipoHora;
}

void clsPartes_Trabajo_Det::setHoras(double _Horas)
{
    this->decHoras = _Horas;
}

void clsPartes_Trabajo_Det::setNoParte(int _NoParte)
{
    this->intNoParte = _NoParte;
}
