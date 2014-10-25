#include "kit.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

clsKits::clsKits(QString Desc)
{
    this->intCodigo = 0;
    this->strDescripcion = Desc;
}

clsKits::~clsKits(){}

int clsKits::Codigo() const
{
    return this->intCodigo;
}

QString clsKits::Descripcion() const
{
    return this->strDescripcion;
}

const QList<clsKitDetalle>& clsKits::Detalles() const
{
    return this->ListDetalles;
}

void clsKits::setDescripcion(QString _Desc)
{
    this->strDescripcion = _Desc;
}

bool clsKits::Insertar()
{
    QString txtDescripcion = this->strDescripcion;

    QString strConsulta;

    if (txtDescripcion == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción para el kit"),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO kits (descripcion) VALUES ('" + txtDescripcion + "');";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {
            strConsulta = "SELECT LAST_INSERT_ID() FROM kits;";
            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
            {
                if (consulta.first())
                {
                    this->intCodigo = consulta.record().value(0).toInt();
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

bool clsKits::Modificar()
{
    QString txtDescripcion = this->strDescripcion;

    QString strConsulta;

    if (txtDescripcion == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción para el kit"),0,0);
        return false;
    }
    else
    {
        strConsulta = "UPDATE kits SET descripcion='" + txtDescripcion + "' WHERE idKit="+QString::number(intCodigo)+";";

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

bool clsKits::Eliminar()
{
    QString strConsulta;

    strConsulta = "DELETE FROM kits WHERE idKit=" + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

void clsKits::Cargar(int Codigo)
{
    QString strConsulta;

    strConsulta = "SELECT descripcion FROM kits WHERE idKit = " + QString::number(Codigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            intCodigo = Codigo;
            strDescripcion = consulta.value(0).toString();

            strConsulta = "SELECT idProducto,cantidad FROM linea_kit WHERE idKit = " + QString::number(Codigo) + ";";

            QSqlQuery consultaKitDetalle;

            if (consultaKitDetalle.exec(strConsulta))
            {
                ListDetalles.clear();
                while(consultaKitDetalle.next())
                {
                    clsKitDetalle KitDet(Codigo,consultaKitDetalle.value(0).toString(),consultaKitDetalle.value(1).toInt());
                    ListDetalles << KitDet;
                }
            }
        }
    }
}

bool clsKits::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM kits WHERE idKit = " + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QVector<QStringList> clsKits::ListaKits(QString filtro)
{
    QString strConsulta = "SELECT idKit,descripcion FROM kits";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY idKit;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString() << sqlConsulta.value(1).toString();
            vectLista << listResultados;
        }

    return vectLista;
}

clsKitDetalle::clsKitDetalle(int Cod,QString CodProd,int Cant)
{
    intCod = Cod;
    strCodProd = CodProd;
    intCantidad = Cant;
}

clsKitDetalle::clsKitDetalle(const clsKitDetalle& KitDet)
{
    intCod = KitDet.intCod;
    strCodProd = KitDet.strCodProd;
    intCantidad = KitDet.intCantidad;
}

clsKitDetalle::~clsKitDetalle(){}

int clsKitDetalle::Cantidad() const
{
    return this->intCantidad;
}

void clsKitDetalle::setCantidad(int _Cant)
{
    this->intCantidad = _Cant;
}

QString clsKitDetalle::CodProd() const
{
    return this->strCodProd;
}

bool clsKitDetalle::Insertar()
{
    QString strConsulta;

    if (intCantidad == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero para la línea."),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO linea_kit (idKit,idProducto,cantidad) VALUES (" + QString::number(intCod) + ",'" + strCodProd + "'," + QString::number(intCantidad) + ");";

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

bool clsKitDetalle::Modificar()
{
    QString strConsulta;

    if (intCantidad == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero para la línea."),0,0);
        return false;
    }
    else
    {
        strConsulta = "UPDATE linea_kit SET Cantidad=" + QString::number(intCantidad) + " WHERE idKit = " + QString::number(intCod) + " AND idProducto = '" + strCodProd + "';";

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

bool clsKitDetalle::Eliminar()
{
    QString strConsulta;

    strConsulta = "DELETE FROM linea_kit WHERE idKit=" + QString::number(intCod) + " AND idProducto = '" + strCodProd + "';";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

void clsKitDetalle::Cargar(int Codigo,QString CodProd)
{
    QString strConsulta;

    strConsulta = "SELECT cantidad "
                  "FROM linea_kit "
                  "WHERE idKit = " + QString::number(Codigo) + " AND idProducto = '" + CodProd + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            intCod = Codigo;
            strCodProd = CodProd;
            intCantidad = consulta.value(0).toInt();
        }
    }
}

bool clsKitDetalle::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM linea_kit WHERE idKit=" + QString::number(intCod) + " AND idProducto='" + strCodProd + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}
