#include "centro.h"
#include <QSqlRecord>
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>

clsCentros::clsCentros(int _Codigo,QString _Nombre,QString _Ciudad,QString _Direccion)
{
    this->intCodigo = _Codigo;
    this->strCiudad = _Nombre;
    this->strNombre = _Ciudad;
    this->strDireccion = _Direccion;
}

clsCentros::~clsCentros(){}

bool clsCentros::Insertar()
{
    if (!Existe())
    {
        QString strConsulta;

        if (strNombre == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nombre para el centro."),0,0);
            return false;
        }
        else if (strCiudad == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una ciudad para el centro."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "INSERT INTO centros (nombre,ciudad,Direccion) VALUES ('" + strNombre + "', '" + strCiudad + "','" + strDireccion + "');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha podido insertar el centro."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el centro. Ya existe."),0,0);
        return false;
    }
}

bool clsCentros::Modificar()
{
    if (Existe())
    {
        QString strConsulta;

        if (strNombre == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nombre para el centro."),0,0);
            return false;
        }
        else if (strCiudad == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una ciudad para el centro."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "UPDATE centros SET nombre= '" + strNombre + "', ciudad= '" + strCiudad + "', Direccion='" + strDireccion + "' WHERE idCentro="+ QString::number(intCodigo) + ";";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha modificado el centro."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el centro. Ya existe."),0,0);
        return false;
    }
}

bool clsCentros::Eliminar()
{
    if (Existe())
    {
        QString strConsulta;

        QSqlQuery consulta;

        strConsulta = "DELETE FROM centros WHERE idCentro="+ QString::number(intCodigo) + ";";

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha eliminado el centro."),0,0);
            return false;
        }
        else
            return true;
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el centro. No existe."),0,0);
        return false;
    }
}

void clsCentros::Cargar(int Codigo)
{
    QString strConsulta;

    strConsulta = "SELECT nombre,ciudad,Direccion FROM centros WHERE idCentro="+ QString::number(Codigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intCodigo = Codigo;
            this->strCiudad = consulta.value(1).toString();
            this->strNombre = consulta.value(0).toString();
            this->strDireccion = consulta.value(2).toString();
        }
    }
}

bool clsCentros::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM centros WHERE idCentro= " + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

int clsCentros::Codigo() const
{
    return this->intCodigo;
}

QString clsCentros::Nombre() const
{
    return this->strNombre;
}

QString clsCentros::Ciudad() const
{
    return this->strCiudad;
}

QString clsCentros::Direccion() const
{
    return this->strDireccion;
}

void clsCentros::setNombre(QString _Nombre)
{
    this->strNombre = _Nombre;
}

void clsCentros::setCiudad(QString _Ciudad)
{
    this->strCiudad = _Ciudad;
}

void clsCentros::setDireccion(QString _Direccion)
{
    this->strDireccion = _Direccion;
}

QVector<QStringList> clsCentros::ListaCentros(QString filtro)
{
    QString strConsulta = "SELECT idCentro,nombre,ciudad,Direccion FROM centros";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY idCentro;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString() << sqlConsulta.value(1).toString() << sqlConsulta.value(2).toString() << sqlConsulta.value(3).toString();
            vectLista << listResultados;
        }

    return vectLista;
}
