#include "Centros/centro.h"
#include "vehiculo.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>

clsVehiculos::clsVehiculos(QString _Matricula,QString _Marca,QString _Modelo,QDate _FechaMatriculacion,int _Centro)
{
    strMatricula = _Matricula;
    strMarca = _Marca;
    strModelo = _Modelo;
    datFechaMatriculacion = _FechaMatriculacion;
    intCentro = _Centro;
}

clsVehiculos::~clsVehiculos(){}

QString clsVehiculos::Matricula() const
{
    return this->strMatricula;
}

QString clsVehiculos::Marca() const
{
    return this->strMarca;
}

QString clsVehiculos::Modelo() const
{
    return this->strModelo;
}

int clsVehiculos::Centro() const
{
    return this->intCentro;
}

QDate clsVehiculos::FechaMatriculacion() const
{
    return this->datFechaMatriculacion;
}

void clsVehiculos::setImagen(QString _rutaImagen)
{
    this->strRutaImagen = _rutaImagen;
}

QString clsVehiculos::Imagen() const
{
    return this->strRutaImagen;
}

QDate clsVehiculos::FechaITV() const
{
    QDate NuevaFechaITV;
    QDate fechaMatr = this->datFechaMatriculacion;
    if (fechaMatr.addYears(4) < QDate::currentDate())
        NuevaFechaITV.setDate(this->datFechaMatriculacion.year() + 4,this->datFechaMatriculacion.month(),this->datFechaMatriculacion.day());
    else if((fechaMatr.addYears(4) >= QDate::currentDate()) && (fechaMatr.addYears(10) < QDate::currentDate()))
    {
        int i = 4;
        while(QDate::currentDate() < this->datFechaMatriculacion.addYears(i))
            i = i + 2;
        NuevaFechaITV.setDate(this->datFechaMatriculacion.year() + i,this->datFechaMatriculacion.month(),this->datFechaMatriculacion.day());
    }
    else
    {
        int i = 4;
        while(QDate::currentDate() < this->datFechaMatriculacion.addYears(i))
            i++;
        NuevaFechaITV.setDate(this->datFechaMatriculacion.year() + i,this->datFechaMatriculacion.month(),this->datFechaMatriculacion.day());
    }
    return NuevaFechaITV;
}

const QList<clsVehiculo_Movs>& clsVehiculos::Movimientos() const
{
    return this->ListMovimientos;
}

void clsVehiculos::setMatricula(QString _Matricula)
{
    this->strMatricula = _Matricula;
}

void clsVehiculos::setMarca(QString _Marca)
{
    this->strMarca = _Marca;
}

void clsVehiculos::setModelo(QString _Modelo)
{
    this->strModelo = _Modelo;
}

void clsVehiculos::setFechaMatriculacion(QDate _FechaMatriculacion)
{
    this->datFechaMatriculacion = _FechaMatriculacion;
}

void clsVehiculos::setCentro(int _Centro)
{
    this->intCentro = _Centro;
}

bool clsVehiculos::Insertar()
{
    if (!Existe())
    {
        QString strConsulta;

        if (strMatricula == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un matrícula para el vehículo."),0,0);
            return false;
        }
        else if (strMarca == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una marca para el vehículo."),0,0);
            return false;
        }
        else if (strModelo == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un modelo para el vehículo."),0,0);
            return false;
        }
        else if (intCentro == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro para el vehículo."),0,0);
            return false;
        }
        else
        {
            strConsulta =  "INSERT INTO vehiculos VALUES ('" + strMatricula + "', '" + strMarca + "', '" + strModelo +
                           + "', '" + datFechaMatriculacion.toString("yyyy/MM/dd") + "', '" + strRutaImagen + "');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha insertado el vehículo."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el vehículo. Ya existe."),0,0);
        return false;
    }
}

bool clsVehiculos::Modificar()
{
    if (Existe())
    {
        QString strConsulta;

        if (strMatricula == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un matrícula para el vehículo."),0,0);
            return false;
        }
        else if (strMarca == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una marca para el vehículo."),0,0);
            return false;
        }
        else if (strModelo == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un modelo para el vehículo."),0,0);
            return false;
        }
        else if (intCentro == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro para el vehículo."),0,0);
            return false;
        }
        else
        {
            strConsulta = "UPDATE vehiculos SET Marca='" + strMarca + "', modelo = '" + strModelo +
                          "', fecha_matriculacion = '" + datFechaMatriculacion.toString("yyyy/MM/dd") +
                          "', imagen = '" + strRutaImagen + "' WHERE matricula='" + strMatricula + "';";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha modificado el vehículo."),0,0);
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede modificar el vehículo. No existe."),0,0);
        return false;
    }
}

bool clsVehiculos::Eliminar()
{
    if (Existe())
    {
        QString strConsulta;

        strConsulta = "DELETE FROM vehiculos WHERE matricula='" + strMatricula + "';";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha eliminado el vehículo.\nError al eliminar."),0,0);
            return false;
        }
        else
            return true;
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el vehículo. No existe."),0,0);
        return false;
    }
}

void clsVehiculos::Cargar(QString _Matricula)
{
    QString strConsulta;

    strConsulta = "SELECT marca,modelo,fecha_matriculacion,Imagen FROM vehiculos WHERE matricula = '" + _Matricula + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            strMatricula = _Matricula;
            strMarca = consulta.value(0).toString();
            strModelo = consulta.value(1).toString();
            datFechaMatriculacion = consulta.value(2).toDate();
            strRutaImagen = consulta.value(3).toString();

            strConsulta = "SELECT idcentro FROM mov_vehiculos WHERE matricula = '" + _Matricula + "' ORDER BY fecha_mov DESC;";

            consulta.clear();

            if (consulta.exec(strConsulta))
            {
                ListMovimientos.clear();
                if(consulta.first())
                {
                    intCentro = consulta.value(0).toInt();
                    clsVehiculo_Movs Vehi_Mov(consulta.value(0).toInt(),_Matricula);
                    Vehi_Mov.Cargar(_Matricula,consulta.value(0).toInt());
                    ListMovimientos << Vehi_Mov;
                }
                while(consulta.next())
                {
                    clsVehiculo_Movs Vehi_Mov(consulta.value(0).toInt(),_Matricula);
                    Vehi_Mov.Cargar(_Matricula,consulta.value(0).toInt());
                    ListMovimientos << Vehi_Mov;
                }
            }
        }
    }
}

bool clsVehiculos::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM vehiculos WHERE matricula = '" + strMatricula + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QVector<QStringList> clsVehiculos::ListaVehiculos(QString filtro)
{
    QString strConsulta = "SELECT matricula,marca,modelo,fecha_matriculacion "
                          "FROM vehiculos";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY matricula;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toString()
                           << sqlConsulta.value(2).toString()
                           << sqlConsulta.value(3).toDate().toString(QObject::tr("dd/MM/yyyy"));
            vectLista << listResultados;
        }

    return vectLista;
}


clsVehiculo_Movs::clsVehiculo_Movs(int _Centro,QString _Matricula, double _Kms)
{
    this->intCentro = _Centro;
    this->strMatricula = _Matricula;
    this->datFechaMov = QDate::currentDate();
    this->douKms = _Kms;
}

clsVehiculo_Movs::clsVehiculo_Movs(const clsVehiculo_Movs& VehiMovs)
{
    this->intCentro = VehiMovs.intCentro;
    this->strMatricula = VehiMovs.strMatricula;
    this->datFechaMov = VehiMovs.datFechaMov;
    this->douKms = VehiMovs.douKms;
}

clsVehiculo_Movs::~clsVehiculo_Movs(){}

QDate clsVehiculo_Movs::FechaMovimiento()
{
    return this->datFechaMov;
}

double clsVehiculo_Movs::KmsFinales()
{
    return this->douKms;
}

int clsVehiculo_Movs::Centro()
{
    return this->intCentro;
}

bool clsVehiculo_Movs::Insertar()
{
    QString strConsulta;

    clsCentros Centro;
    Centro.Cargar(this->intCentro);

    clsVehiculos Vehiculo;
    Vehiculo.Cargar(this->strMatricula);

    if(!Vehiculo.Existe())
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("El vehiculo debe existir."),0,0);
        return false;
    }
    else if(!Centro.Existe())
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("El centro debe existir."),0,0);
        return false;
    }
    else if(this->douKms == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Los kilómetros finales no pueden ser cero."),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO mov_vehiculos (matricula,idcentro,fecha_mov,km_finales) VALUES ('" + strMatricula + "', " + QString::number(intCentro) + ",'" + datFechaMov.toString("yyyy/MM/dd") + "', " + QString::number(douKms) + ");";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha insertado el movimiento."),0,0);
            return false;
        }
        else
            return true;
    }
}

void clsVehiculo_Movs::Cargar(QString _Matricula,int _Centro)
{
    QString strConsulta;

    strConsulta = "SELECT fecha_mov,km_finales FROM mov_vehiculos WHERE matricula = '" + _Matricula + "' AND idCentro = '" + QString::number(_Centro) + "' ORDER BY fecha_mov DESC;";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            strMatricula = _Matricula;
            intCentro = _Centro;
            datFechaMov = consulta.value(0).toDate();
            douKms = consulta.value(1).toDouble();
        }
    }
}
