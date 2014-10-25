#include "movimientos.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

clsMovimiento_Salida::clsMovimiento_Salida(int _Sal,QString _proy,QString _tarea,QString _op)
{
    this->intNoSalida = _Sal;
    this->strProyecto = _proy;
    this->strTarea = _tarea;
    this->strOperario = _op;
    this->datFecha_creacion = QDate::currentDate();
}

clsMovimiento_Salida::~clsMovimiento_Salida(){}

int clsMovimiento_Salida::NoSalida() const
{
   return this->intNoSalida;
}

QString clsMovimiento_Salida::Proyecto() const
{
    return this->strProyecto;
}

QString clsMovimiento_Salida::Tarea() const
{
    return this->strTarea;
}

QString clsMovimiento_Salida::Operario() const
{
    return this->strOperario;
}

int clsMovimiento_Salida::Centro() const
{
    return this->intCentro;
}

QDate clsMovimiento_Salida::Fecha_Creacion() const
{
    return this->datFecha_creacion;
}

void clsMovimiento_Salida::setProyecto(QString _proy)
{
    this->strProyecto = _proy;
}

void clsMovimiento_Salida::setTarea(QString _tarea)
{
    this->strTarea = _tarea;
}

void clsMovimiento_Salida::setOperario(QString _op)
{
    this->strOperario = _op;
}

const QList<clsMovimiento_DetSal>& clsMovimiento_Salida::DetallesSalida() const
{
    return this->SalDetalles;
}

QString clsMovimiento_Salida::Comentarios() const
{
    return this->strComentarios;
}

void clsMovimiento_Salida::setComentarios(QString _Coment)
{
    this->strComentarios = _Coment;
}

bool clsMovimiento_Salida::Insertar()
{
    if (this->strOperario == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un operario."),0,0);
        return false;
    }
    else if (this->strProyecto == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un proyecto."),0,0);
        return false;
    }
    else if (this->strTarea == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una tarea."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "INSERT INTO salidas (idCentro,proyecto,tarea,responsable,fecha_creacion,comentario) "
                "VALUES (" + QString::number(config->IdCentroActual()) + ",'" + this->strProyecto + "','" + this->strTarea + "', '" + this->strOperario + "', '" + this->datFecha_creacion.toString("yyyy/MM/dd") + "','" + this->strComentarios + "');";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {
            strConsulta = "SELECT LAST_INSERT_ID() FROM salidas;";
            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
            {
                if (consulta.first())
                {
                    this->intNoSalida = consulta.record().value(0).toInt();
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

bool clsMovimiento_Salida::Modificar()
{
    if (this->strOperario == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un operario."),0,0);
        return false;
    }
    else if (this->strProyecto == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un proyecto."),0,0);
        return false;
    }
    else if (this->strTarea == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una tarea."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "UPDATE salidas SET proyecto='"+ this->strProyecto + "',"
                "tarea='" + this->strTarea + "',responsable='" + this->strOperario + "', comentario = '" + this->strComentarios + "' "
                               "WHERE idSalida=" +QString::number(this->intNoSalida) +";";

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

void clsMovimiento_Salida::Eliminar()
{
    QString strConsulta ="";

    strConsulta = "DELETE FROM salidas WHERE idSalida=" +QString::number(this->intNoSalida) +";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsMovimiento_Salida::Cargar(int _Salida)
{
    QString strConsulta = "SELECT proyecto,tarea,responsable,fecha_creacion,comentario,idCentro "
                          "FROM salidas "
                          "WHERE idSalida = " + QString::number(_Salida) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intNoSalida = _Salida;
            this->intCentro = consulta.value(5).toInt();
            this->strProyecto = consulta.value(0).toString();
            this->strTarea = consulta.value(1).toString();
            this->strOperario = consulta.value(2).toString();
            this->datFecha_creacion = consulta.value(3).toDate();
            this->strComentarios = consulta.value(4).toString();

            strConsulta = "SELECT fecha_salida,idProducto,cantidad FROM linea_salidas WHERE idSalida = '" + QString::number(_Salida) + "';";

            if (consulta.exec(strConsulta))
            {
                SalDetalles.clear();
                while(consulta.next())
                {
                    clsMovimiento_DetSal DetSal;
                    DetSal.Cargar(_Salida,consulta.value(1).toString(),consulta.value(0).toDate());
                    SalDetalles << DetSal;
                }
            }
        }
    }
}

bool clsMovimiento_Salida::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM salidas "
                          "WHERE idSalida = " + QString::number(this->intNoSalida) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QVector<QStringList> clsMovimiento_Salida::ListaSalida(QString filtro)
{
    bool permiso = config->Permisos().value(Confi);

    QString strConsulta = "SELECT salidas.idsalida,concat_ws(' - ',centros.nombre,centros.ciudad) as centro,salidas.proyecto,salidas.tarea,concat_ws(' ',operarios.nombre,operarios.apellidos) as NomApeOp, salidas.fecha_creacion "
            "FROM salidas INNER JOIN centros ON centros.idcentro = salidas.idcentro INNER JOIN operarios ON salidas.responsable=operarios.nif ";

    if(!permiso)
    {
        strConsulta += "WHERE salidas.idCentro = '" + QString::number(config->IdCentroActual()) + "'";

        if(filtro != "")
            strConsulta += " AND " + filtro;
    }
    else
    {
        if(filtro != "")
            strConsulta += " WHERE " + filtro;
    }

    QStringList listResultados;
    QVector<QStringList> vectLista;

    strConsulta += " ORDER BY salidas.idsalida;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toString()
                           << sqlConsulta.value(2).toString()
                           << sqlConsulta.value(3).toString()
                           << sqlConsulta.value(4).toString()
                           << sqlConsulta.value(5).toDate().toString(QObject::tr("dd/MM/yyyy"));
            vectLista << listResultados;
        }

    return vectLista;
}


clsMovimiento_DetSal::clsMovimiento_DetSal(int Sal,QString CodProd,int Cant,QDate _Fecha_Salida)
{
    this->intNoSalida = Sal;
    this->strCodProd = CodProd;
    this->intCantidad = Cant;
    this->datFecha_salida = _Fecha_Salida;
}

clsMovimiento_DetSal::clsMovimiento_DetSal(const clsMovimiento_DetSal& SalDetalle)
{
    this->intNoSalida = SalDetalle.intNoSalida;
    this->strCodProd = SalDetalle.strCodProd;
    this->intCantidad = SalDetalle.intCantidad;
    this->datFecha_salida = SalDetalle.datFecha_salida;
}

clsMovimiento_DetSal::~clsMovimiento_DetSal(){}

int clsMovimiento_DetSal::Salida() const
{
    return this->intNoSalida;
}

int clsMovimiento_DetSal::Cantidad() const
{
    return this->intCantidad;
}

QString clsMovimiento_DetSal::CodProd() const
{
    return this->strCodProd;
}

QDate clsMovimiento_DetSal::Fecha_Salida() const
{
    return this->datFecha_salida;
}

const QList<clsMovimiento_DetEnt>& clsMovimiento_DetSal::DetallesEntrada() const
{
    return this->EntDetalles;
}

bool clsMovimiento_DetSal::Insertar()
{
    if (this->intNoSalida == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de salida."),0,0);
        return false;
    }
    else if (this->strCodProd == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
        return false;
    }
    else if (this->intCantidad < 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "INSERT INTO linea_salidas (idSalida,idProducto,cantidad,fecha_salida) "
                "VALUES (" + QString::number(this->intNoSalida) + ",'" + this->strCodProd + "'," + QString::number(this->intCantidad) + ", '" + this->datFecha_salida.toString("yyyy/MM/dd") + "');";

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

bool clsMovimiento_DetSal::Modificar()
{

    if (this->intNoSalida == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de salida."),0,0);
        return false;
    }
    else if (this->strCodProd == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
        return false;
    }
    else if (this->intCantidad < 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "UPDATE linea_salidas SET cantidad=" + QString::number(this->intCantidad) +
                               " WHERE idSalida=" + QString::number(this->intNoSalida) + " AND idProducto='" + strCodProd + "' AND fecha_salida='" + this->datFecha_salida.toString("yyyy/MM/dd") + "';";

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

bool clsMovimiento_DetSal::Eliminar()
{
    QString strConsulta ="";

    strConsulta = "DELETE FROM linea_salidas WHERE idSalida=" +QString::number(this->intNoSalida) + " AND idProducto='" + strCodProd + "' AND fecha_salida='" + this->datFecha_salida.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

void clsMovimiento_DetSal::Cargar(int Sal,QString prod,QDate fecha)
{
    QString strConsulta = "SELECT cantidad "
                          "FROM linea_salidas "
                          "WHERE idSalida = " + QString::number(Sal) + " AND idProducto='" + prod + "' AND fecha_salida='" + fecha.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        this->intNoSalida = Sal;
        this->strCodProd = prod;
        this->datFecha_salida = fecha;
        if (consulta.first())
        {
            this->intCantidad = consulta.value(0).toInt();

            strConsulta = "SELECT fecha_entrada,devueltas,perdidas FROM linea_entradas WHERE idSalida = " + QString::number(Sal) + " AND idProducto='" + prod + "' AND fecha_salida='" + fecha.toString("yyyy/MM/dd") + "';";

            if (consulta.exec(strConsulta))
            {
                EntDetalles.clear();
                while(consulta.next())
                {
                    clsMovimiento_DetEnt DetEnt;
                    DetEnt.Cargar(Sal,prod,fecha,consulta.value(0).toDate());
                    EntDetalles << DetEnt;
                }
            }
        }
    }
}

bool clsMovimiento_DetSal::Existe() const
{
    QString strfecha= this->datFecha_salida.toString("yyyy/MM/dd");
    QString strConsulta = "SELECT * FROM linea_salidas WHERE idSalida = " + QString::number(this->intNoSalida) + " AND idProducto='" + this->strCodProd + "' AND fecha_salida='" + strfecha + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if(consulta.first())
        {
            if (consulta.size() > 0)
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}

void clsMovimiento_DetSal::setCantidad(int _Cant)
{
    this->intCantidad = _Cant;
}


clsMovimiento_DetEnt::clsMovimiento_DetEnt(int Sal,int _Devueltas,int _Perdidas,QString CodProd,QDate _Fecha_Entrada,QDate _Fecha_Salida)
{
    this->intNoSalida = Sal;
    this->intDevueltas = _Devueltas;
    this->intPerdidas = _Perdidas;
    this->datFecha_entrada = _Fecha_Entrada;
    this->datFecha_salida = _Fecha_Salida;
    this->strProd = CodProd;
}

clsMovimiento_DetEnt::clsMovimiento_DetEnt(const clsMovimiento_DetEnt& EntDetalle)
{
    this->intNoSalida = EntDetalle.intNoSalida;
    this->intDevueltas = EntDetalle.intDevueltas;
    this->intPerdidas = EntDetalle.intPerdidas;
    this->datFecha_entrada = EntDetalle.datFecha_entrada;
    this->datFecha_salida = EntDetalle.datFecha_salida;
    this->strProd = EntDetalle.strProd;
}

clsMovimiento_DetEnt::~clsMovimiento_DetEnt(){}

int clsMovimiento_DetEnt::Salida() const
{
    return this->intNoSalida;
}

int clsMovimiento_DetEnt::Devueltas() const
{
    return this->intDevueltas;
}

int clsMovimiento_DetEnt::Perdidas() const
{
    return this->intPerdidas;
}

QDate clsMovimiento_DetEnt::Fecha_Entrada() const
{
    return this->datFecha_entrada;
}

QDate clsMovimiento_DetEnt::Fecha_Salida() const
{
    return this->datFecha_salida;
}

QString clsMovimiento_DetEnt::CodProd() const
{
    return this->strProd;
}

bool clsMovimiento_DetEnt::Insertar()
{
    if (this->intNoSalida == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de salida."),0,0);
        return false;
    }
    else if (this->strProd == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "INSERT INTO linea_entradas (idSalida,devueltas,perdidas,fecha_entrada,fecha_salida,idProducto) "
                "VALUES (" + QString::number(this->intNoSalida) + "," + QString::number(this->intDevueltas) + "," + QString::number(this->intPerdidas) + ", '" + this->datFecha_entrada.toString("yyyy/MM/dd") + "', '" + this->datFecha_salida.toString("yyyy/MM/dd") + "','" +strProd+"');";

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

bool clsMovimiento_DetEnt::Modificar()
{
    if (this->intNoSalida == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de salida."),0,0);
        return false;
    }
    else if (this->strProd == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "UPDATE linea_entradas SET devueltas=" + QString::number(this->intDevueltas) + ",perdidas=" + QString::number(this->intPerdidas) +
                               " WHERE idSalida= " + QString::number(this->intNoSalida) + " AND fecha_entrada='" + this->datFecha_entrada.toString("yyyy/MM/dd") + "' AND fecha_salida='" + this->datFecha_salida.toString("yyyy/MM/dd") + "' AND idProducto = '" + this->strProd + "';";

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

bool clsMovimiento_DetEnt::Eliminar()
{
    QString strConsulta ="";

    strConsulta = "DELETE FROM linea_entradas WHERE idSalida=" +QString::number(this->intNoSalida) +" AND fecha_entrada='" + this->datFecha_entrada.toString("yyyy/MM/dd") + "' AND fecha_salida='" + this->datFecha_salida.toString("yyyy/MM/dd") + "' AND idProducto = '" + this->strProd + "';";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

void clsMovimiento_DetEnt::Cargar(int Sal,QString prod,QDate fecha_sal,QDate fecha_ent)
{
    QString strConsulta = "SELECT devueltas,perdidas "
                          "FROM linea_entradas "
                          "WHERE idSalida = " + QString::number(Sal) + " AND idProducto='" + prod + "' AND fecha_entrada='" + fecha_ent.toString("yyyy/MM/dd") + "' AND fecha_salida='" + fecha_sal.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        this->intNoSalida = Sal;
        this->datFecha_entrada = fecha_ent;
        this->datFecha_salida = fecha_sal;
        this->strProd = prod;
        if (consulta.first())
        {
            this->intDevueltas = consulta.value(0).toInt();
            this->intPerdidas = consulta.value(1).toInt();
        }
    }
}

bool clsMovimiento_DetEnt::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM linea_entradas "
                          "WHERE idSalida = " + QString::number(this->intNoSalida) + " AND idProducto='" + this->strProd + "' AND fecha_entrada='" + this->datFecha_entrada.toString("yyyy/MM/dd") + "' AND fecha_salida='" + this->datFecha_salida.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

void clsMovimiento_DetEnt::setDevueltas(int _Dev)
{
    this->intDevueltas = _Dev;
}

void clsMovimiento_DetEnt::setPerdidas(int _Perd)
{
    this->intPerdidas = _Perd;
}


clsMovimiento_Pedido::clsMovimiento_Pedido(int _Ped)
{
    this->intNoPedido = _Ped;
    this->datFecha = QDate::currentDate();
    this->intCentro = config->IdCentroActual();
}

clsMovimiento_Pedido::~clsMovimiento_Pedido(){}

int clsMovimiento_Pedido::NoPedido() const
{
    return this->intNoPedido;
}

int clsMovimiento_Pedido::Centro() const
{
    return this->intCentro;
}

const QList<clsMovimiento_DetPed>& clsMovimiento_Pedido::Detalles() const
{
    return this->PedDetalles;
}

QDate clsMovimiento_Pedido::Fecha() const
{
    return this->datFecha;
}

QString clsMovimiento_Pedido::Comentarios() const
{
    return this->strComentarios;
}

void clsMovimiento_Pedido::setComentarios(QString _Coment)
{
    this->strComentarios = _Coment;
}

bool clsMovimiento_Pedido::Insertar()
{
    QString strConsulta =  "INSERT INTO pedidos_internos (idCentro,Fecha_Creacion,Comentario) VALUES (" + QString::number(this->intCentro) + ",'"+ this->datFecha.toString("yyyy/MM/dd") + "','" + this->strComentarios + "');";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
    {
        strConsulta = "SELECT LAST_INSERT_ID() FROM pedidos_internos;";
        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {
            if (consulta.first())
            {
                this->intNoPedido = consulta.record().value(0).toInt();
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

bool clsMovimiento_Pedido::Modificar()
{
    QString strConsulta =  "UPDATE pedidos_internos SET idCentro=" + QString::number(intCentro) + ", comentario = '" +this->strComentarios +"' WHERE idPedido=" +QString::number(this->intNoPedido) + ";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

bool clsMovimiento_Pedido::Eliminar()
{
    QString strConsulta ="";

    if(!SeHaServido())
    {
        strConsulta = "DELETE FROM pedidos_internos WHERE idPedido=" +QString::number(this->intNoPedido) +";";

        QSqlQuery consulta;

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
        QMessageBox::critical(0,QObject::tr("Error al eliminar el pedido"),QObject::tr("No se puede eliminar el pedido. El pedido o parte de él ya se ha servido."));
        return false;
    }
}

void clsMovimiento_Pedido::Cargar(int _Pedido)
{
    QString strConsulta = "SELECT idCentro,fecha_creacion,comentario "
                          "FROM pedidos_internos "
                          "WHERE idPedido = " + QString::number(_Pedido) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intNoPedido = _Pedido;
            this->intCentro = consulta.value(0).toInt();
            this->datFecha = consulta.value(1).toDate();
            this->strComentarios = consulta.value(2).toString();

            strConsulta = "SELECT fecha_pedido,idproducto FROM linea_pedido WHERE idPedido = " + QString::number(_Pedido) + ";";

            if (consulta.exec(strConsulta))
            {
                PedDetalles.clear();
                while(consulta.next())
                {
                    clsMovimiento_DetPed DetPed;
                    DetPed.Cargar(_Pedido,consulta.value(1).toString(),consulta.value(0).toDate());
                    PedDetalles << DetPed;
                }
            }
        }
    }
}

bool clsMovimiento_Pedido::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM pedidos_internos "
                          "WHERE idPedido = " + QString::number(this->intNoPedido) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

bool clsMovimiento_Pedido::SeHaServido() const
{
    for(int i =0;i<PedDetalles.count();i++)
    {
        if(!PedDetalles.at(0).Servido())
            return false;
    }
    return true;
}

QVector<QStringList> clsMovimiento_Pedido::ListaPedido(QString filtro)
{
    QString strConsulta = "SELECT pedidos_internos.idPedido,concat_ws(' - ',centros.nombre,centros.ciudad) as centro "
            "FROM pedidos_internos INNER JOIN centros ON centros.idcentro=pedidos_internos.idcentro";

    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY pedidos_internos.idPedido";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsMovimiento_DetPed::clsMovimiento_DetPed(int Ped,QString CodProd,int Cant)
{
    this->intNoPedido = Ped;
    this->strCodProd = CodProd;
    this->intCantidad = Cant;
    this->boServido = false;
    this->datFecha = QDate::currentDate();
}

clsMovimiento_DetPed::clsMovimiento_DetPed(const clsMovimiento_DetPed& Detalle)
{
    this->intCantidad = Detalle.intCantidad;
    this->intNoPedido = Detalle.intNoPedido;
    this->strCodProd = Detalle.strCodProd;
    this->boServido = Detalle.boServido;
    this->datFecha = Detalle.datFecha;
}

clsMovimiento_DetPed::~clsMovimiento_DetPed(){}

int clsMovimiento_DetPed::Pedido() const
{
    return this->intNoPedido;
}

int clsMovimiento_DetPed::Cantidad() const
{
    return this->intCantidad;
}

QString clsMovimiento_DetPed::CodProd() const
{
    return this->strCodProd;
}

bool clsMovimiento_DetPed::Servido() const
{
    return this->boServido;
}

QDate clsMovimiento_DetPed::Fecha() const
{
    return this->datFecha;
}

bool clsMovimiento_DetPed::Insertar()
{
    if(!Existe())
    {
        if (this->intNoPedido == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de pedido."),0,0);
            return false;
        }
        else if (this->strCodProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "INSERT INTO linea_pedido (idPedido,idProducto,cantidad,fecha_pedido,servido) "
                    "VALUES (" + QString::number(this->intNoPedido) + ",'" + this->strCodProd + "'," +
                    QString::number(this->intCantidad) + ",'" + this->datFecha.toString("yyyy/MM/dd") + "'," + QString::number(this->boServido) + ");";

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
        QMessageBox::critical(0,QObject::tr("Error al insertar la línea del pedido"),QObject::tr("No se puede insertar la línea del pedido. Ya existe una línea para ese producto en el pedido."),0,0);
        return false;
    }
}

bool clsMovimiento_DetPed::Modificar()
{
    if(Existe())
    {
        if(CantidadServida() == 0)
        {
            if (this->intNoPedido == 0)
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un nº de pedido."),0,0);
                return false;
            }
            else if (this->strCodProd == "")
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
                return false;
            }
            else
            {
                QString strConsulta =  "UPDATE linea_pedido SET cantidad=" + QString::number(this->intCantidad) + " servido= " + QString::number(this->boServido) + " "
                                       "WHERE idPedido= " + QString::number(this->intNoPedido) + " AND idProducto='" + this->strCodProd + "' AND fecha_pedido='" + this->datFecha.toString("yyyy/MM/dd") + "';";

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
            QMessageBox::critical(0,QObject::tr("Error al modificar la línea del pedido"),QObject::tr("No se puede modificar la línea del pedido. Ya se ha servido parte de él."),0,0);
            return false;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error al modificar la línea del pedido"),QObject::tr("No se puede modificar la línea del pedido. No existe una línea para este producto en el pedido."),0,0);
        return false;
    }
}

bool clsMovimiento_DetPed::Eliminar()
{
    if (Existe())
    {
        if(CantidadServida() == 0)
        {
            QString strConsulta  = "DELETE FROM linea_pedido WHERE idPedido= " + QString::number(this->intNoPedido) + " AND idProducto='" + this->strCodProd + "' AND fecha_pedido='" + this->datFecha.toString("yyyy/MM/dd") + "';";

            QSqlQuery consulta;

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
            QMessageBox::critical(0,QObject::tr("Error al eliminar la línea del pedido"),QObject::tr("No se puede eliminar la línea del pedido. Ya se ha servido parte de él."),0,0);
            return false;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error al eliminar la línea del pedido"),QObject::tr("No se puede eliminar la línea del pedido. No existe una línea para este producto en el pedido."),0,0);
        return false;
    }
}

void clsMovimiento_DetPed::Cargar(int Ped,QString prod,QDate fecha)
{
    QString strConsulta = "SELECT cantidad,servido "
                          "FROM linea_pedido "
                          "WHERE idPedido = " + QString::number(Ped) + " AND idProducto='" + prod + "' AND fecha_pedido='" + fecha.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        this->intNoPedido = Ped;
        this->strCodProd = prod;
        this->datFecha = fecha;
        if (consulta.first())
        {
            this->intCantidad = consulta.value(0).toInt();
            this->boServido = consulta.value(1).toBool();
        }
    }
}

bool clsMovimiento_DetPed::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM linea_pedido "
                          "WHERE idPedido = " + QString::number(this->intNoPedido) + " AND idProducto='" + this->strCodProd + "' AND fecha_pedido='" + this->datFecha.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

void clsMovimiento_DetPed::setCantidad(int _Cant)
{
    this->intCantidad = _Cant;
}

int clsMovimiento_DetPed::CantidadServida() const
{
    QString strConsulta = "SELECT SUM(linea_envios.cantidad) "
                          "FROM linea_pedido INNER JOIN linea_envios INNER JOIN ped_env_prod ON linea_envios.idEnvio = ped_env_prod.idEnvio "
                          "AND linea_envios.fecha_envio = ped_env_prod.Fecha_Envio ON linea_pedido.idPedido = ped_env_prod.idPedido AND "
                          "linea_pedido.fecha_pedido = ped_env_prod.Fecha_Pedido "
                          "WHERE ped_env_prod.idPedido = " + QString::number(this->intNoPedido) + " AND ped_env_prod.Fecha_Pedido = '" +
                          this->datFecha.toString("yyyy/MM/dd") + "' AND ped_env_prod.idProducto='" + strCodProd + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if(consulta.first())
            return consulta.record().value(0).toInt();
        else
            return 0;
    }
    else
        return 0;
}

bool clsMovimiento_DetPed::Servir(int NoEnvio,QDate datFechaEnv)
{
    if(Existe())
    {
        if(this->strCodProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "INSERT INTO ped_env_prod (idPedido,idEnvio,idProducto,fecha_pedido,fecha_envio) "
                    "VALUES (" + QString::number(intNoPedido) + "," + QString::number(NoEnvio) + ", '" + strCodProd + "', '" +
                             datFecha.toString("yyyy/MM/dd") + "', '" + datFechaEnv.toString("yyyy/MM/dd") + "');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
            {

                strConsulta =  "UPDATE linea_pedido SET servido=1 WHERE idPedido=" + QString::number(intNoPedido) +
                                                   " AND idProducto='" + strCodProd + "' AND fecha_pedido='" + datFecha.toString("yyyy/MM/dd") + "';";

                consulta.clear();

                if (!consulta.exec(strConsulta))
                {
                    QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                    return false;
                }
                else
                    return true;
            }
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error al recibir la línea del envío"),QObject::tr("No se puede recibir la línea del envío. No existe una línea para este producto en el envío."),0,0);
        return false;
    }
}


clsMovimiento_Envio::clsMovimiento_Envio(int _Env,int _CentroOrigen,int _CentroDestino,QDate _FechaCrea)
{
    this->intCentroDestino = _CentroDestino;
    this->intCentroOrigen = _CentroOrigen;
    this->intNoEnvio = _Env;
    this->datFechaCreacion = _FechaCrea;
}

clsMovimiento_Envio::~clsMovimiento_Envio(){}

int clsMovimiento_Envio::NoEnvio() const
{
    return this->intNoEnvio;
}

int clsMovimiento_Envio::CentroOrigen() const
{
    return this->intCentroOrigen;
}

int clsMovimiento_Envio::CentroDestino() const
{
    return this->intCentroDestino;
}

QDate clsMovimiento_Envio::FechaCreacion() const
{
    return this->datFechaCreacion;
}

const QList<clsMovimiento_DetEnv>& clsMovimiento_Envio::Detalles() const
{
    return this->EnvDetalles;
}

void clsMovimiento_Envio::setCentroDestino(int _CentroDestino)
{
    this->intCentroDestino = _CentroDestino;
}

void clsMovimiento_Envio::setFechaCreacion(QDate _FechaCrea)
{
    this->datFechaCreacion = _FechaCrea;
}

QString clsMovimiento_Envio::Comentarios() const
{
    return this->strComentarios;
}

void clsMovimiento_Envio::setComentarios(QString _Coment)
{
    this->strComentarios = _Coment;
}

bool clsMovimiento_Envio::Insertar()
{
    if(this->intCentroDestino == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro de destino."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "INSERT INTO envios (idcentroOrigen,idcentroDestino,fecha_creacion,comentario) VALUES (" + QString::number(config->IdCentroActual()) + "," + QString::number(intCentroDestino) + ",'" + this->datFechaCreacion.toString("yyyy/MM/dd") + "','" + this->strComentarios+"');";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {            
            strConsulta = "SELECT LAST_INSERT_ID() FROM envios;";
            if (!consulta.exec(strConsulta))
            {
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
                return false;
            }
            else
            {
                if (consulta.first())
                {
                    this->intNoEnvio = consulta.record().value(0).toInt();
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

bool clsMovimiento_Envio::Modificar()
{
    if(this->intCentroDestino == 0)
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un centro de destino."),0,0);
        return false;
    }
    else
    {
        QString strConsulta =  "UPDATE envios SET idcentroDestino=" + QString::number(intCentroDestino) + ", comentario = '" + this->strComentarios +
                "', fecha_creacion='" + this->datFechaCreacion.toString("yyyy/MM/dd") + "' WHERE idEnvio=" +QString::number(this->NoEnvio()) +";";

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

bool clsMovimiento_Envio::Eliminar()
{
    QString strConsulta = "DELETE FROM envios WHERE idEnvio=" +QString::number(this->intNoEnvio) +";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
    {
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
        return false;
    }
    else
        return true;
}

void clsMovimiento_Envio::Cargar(int _Envio)
{
    QString strConsulta = "SELECT idcentroDestino,fecha_creacion,comentario "
                          "FROM envios "
                          "WHERE idEnvio = " + QString::number(_Envio) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intNoEnvio = _Envio;
            this->intCentroDestino = consulta.value(0).toInt();
            this->intCentroOrigen = config->IdCentroActual();
            this->datFechaCreacion = consulta.value(1).toDate();
            this->strComentarios = consulta.value(2).toString();

            consulta.clear();

            strConsulta = "SELECT idProducto,fecha_envio FROM linea_envios WHERE idEnvio = '" + QString::number(_Envio) + "';";

            if (consulta.exec(strConsulta))
            {
                EnvDetalles.clear();
                while(consulta.next())
                {
                    clsMovimiento_DetEnv DetEnv;
                    DetEnv.Cargar(_Envio,consulta.value(0).toString(),consulta.value(1).toDate());
                    EnvDetalles << DetEnv;
                }
            }
        }
    }
}

bool clsMovimiento_Envio::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM envios "
                          "WHERE idEnvio = " + QString::number(this->intNoEnvio) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QVector<QStringList> clsMovimiento_Envio::ListaEnvio(QString filtro)
{
    QString strConsulta = "SELECT envios.idenvio,concat_ws(' - ',centD.nombre,centD.ciudad) as centroDest,envios.fecha_creacion,concat_ws(' - ',centO.nombre,centO.ciudad) as centroOrig "
            "FROM envios INNER JOIN centros as centD ON centD.idCentro=envios.idcentroDestino INNER JOIN centros as centO ON centO.idCentro=envios.idcentroOrigen ";

    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY envios.idenvio;";

    QSqlQuery sqlConsulta;

    if (sqlConsulta.exec(strConsulta))
        while(sqlConsulta.next())
        {
            listResultados.clear();
            listResultados << sqlConsulta.value(0).toString()
                           << sqlConsulta.value(1).toString()
                           << sqlConsulta.value(2).toDate().toString(QObject::tr("dd/MM/yyyy"))
                           << sqlConsulta.value(3).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsMovimiento_DetEnv::clsMovimiento_DetEnv(int Env,QString CodProd,int Cant)
{
    this->intNoEnvio = Env;
    this->strCodProd = CodProd;
    this->intCantidad = Cant;
    this->boRecibida = false;
    this->datFecha = QDate::currentDate();
}

clsMovimiento_DetEnv::clsMovimiento_DetEnv(const clsMovimiento_DetEnv& Detalle)
{
    this->intNoEnvio = Detalle.intNoEnvio;
    this->strCodProd = Detalle.strCodProd;
    this->intCantidad = Detalle.intCantidad;
    this->boRecibida = Detalle.boRecibida;
    this->datFecha = Detalle.datFecha;
}

clsMovimiento_DetEnv::~clsMovimiento_DetEnv(){}

int clsMovimiento_DetEnv::Envio() const
{
    return this->intNoEnvio;
}

QDate clsMovimiento_DetEnv::FechaEnvio() const
{
    return this->datFecha;
}

int clsMovimiento_DetEnv::Cantidad() const
{
    return this->intCantidad;
}

QString clsMovimiento_DetEnv::CodProd() const
{
    return this->strCodProd;
}

bool clsMovimiento_DetEnv::Recibida() const
{
    return this->boRecibida;
}

bool clsMovimiento_DetEnv::Insertar()
{
    if(!Existe())
    {
        if(this->intCantidad == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
            return false;
        }
        else if(this->strCodProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "INSERT INTO linea_envios VALUES (" + QString::number(this->intNoEnvio) + ",'" + this->strCodProd + "','" + this->datFecha.toString("yyyy/MM/dd") + "'," + QString::number(intCantidad)+",0);";

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
        QMessageBox::critical(0,QObject::tr("Error al insertar la línea del envío"),QObject::tr("No se puede insertar la línea del envío. Ya existe una línea para este producto en el envío."),0,0);
        return false;
    }
}

bool clsMovimiento_DetEnv::Modificar()
{
    if(Existe())
    {
        if (!Recibida())
        {
            if(this->intCantidad == 0)
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
                return false;
            }
            else if(this->strCodProd == "")
            {
                QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
                return false;
            }
            else
            {
                QString strConsulta =  "UPDATE linea_envios SET cantidad=" + QString::number(intCantidad) + " WHERE idEnvio=" +QString::number(this->intNoEnvio) +" AND idProducto='"+strCodProd+"';";

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
            QMessageBox::critical(0,QObject::tr("Error al modificar la línea del envío"),QObject::tr("No se puede modificar la línea del envío. El producto está recibido."),0,0);
            return false;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error al modificar la línea del envío"),QObject::tr("No se puede modificar la línea del envío. No existe una línea para este producto en el envío."),0,0);
        return false;
    }
}

bool clsMovimiento_DetEnv::Eliminar()
{
    if(Existe())
    {
        if(!Recibida())
        {
            QString strConsulta ="";

            strConsulta = "DELETE FROM linea_envios WHERE idEnvio=" +QString::number(this->intNoEnvio) +" AND idProducto='" + strCodProd + "';";

            QSqlQuery consulta;

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
            QMessageBox::critical(0,QObject::tr("Error al eliminar la línea del envío"),QObject::tr("No se puede eliminar la línea del envío. El producto está recibido."),0,0);
            return false;
        }
    }
    else
    {
        QMessageBox::critical(0,QObject::tr("Error al eliminar la línea del envío"),QObject::tr("No se puede eliminar la línea del envío. No existe una línea para este producto en el envío."),0,0);
        return false;
    }
}

void clsMovimiento_DetEnv::Cargar(int Env,QString prod,QDate FechaEnv)
{
    QString strConsulta = "SELECT cantidad,recibido "
                          "FROM linea_envios "
                          "WHERE idEnvio = " + QString::number(Env) + " AND idProducto='" + prod + "' AND fecha_envio='" + FechaEnv.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        this->intNoEnvio = Env;
        this->strCodProd = prod;
        this->datFecha = FechaEnv;
        if (consulta.first())
        {
            this->intCantidad = consulta.value(0).toInt();
            this->boRecibida = consulta.value(1).toBool();
        }
    }
}

bool clsMovimiento_DetEnv::Existe() const
{
    QString strConsulta = "SELECT * "
                          "FROM linea_envios "
                          "WHERE idEnvio = " + QString::number(this->intNoEnvio) + " AND idProducto='" + this->strCodProd + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

void clsMovimiento_DetEnv::setCantidad(int _Cant)
{
    this->intCantidad = _Cant;
}

bool clsMovimiento_DetEnv::Recibir()
{
    if(Existe())
    {
        if(this->strCodProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "UPDATE linea_envios SET Recibido=1 WHERE idEnvio=" + QString::number(intNoEnvio) +
                                   " AND idProducto='" + strCodProd + "' AND fecha_envio='" + datFecha.toString("yyyy/MM/dd") + "';";

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
        QMessageBox::critical(0,QObject::tr("Error al recibir la línea del envío"),QObject::tr("No se puede recibir la línea del envío. No existe una línea para este producto en el envío."),0,0);
        return false;
    }
}


clsMovimiento_Stock::clsMovimiento_Stock(QString _Prod,QDate _datStock,int _Centro,tOrigen _Origen)
{
    this->strProd = _Prod;
    this->datFecha = _datStock;
    this->intCentro = _Centro;
    this->eOrigen = _Origen;
    this->strComentario = "";
    this->intCantidad = 0;
}

clsMovimiento_Stock::~clsMovimiento_Stock() {}

QString clsMovimiento_Stock::CodProd() const
{
    return this->strProd;
}

int clsMovimiento_Stock::Centro() const
{
    return this->intCentro;
}

QString clsMovimiento_Stock::Comentario() const
{
    return this->strComentario;
}

int clsMovimiento_Stock::Cantidad() const
{
    return this->intCantidad;
}

tOrigen clsMovimiento_Stock::Origen() const
{
    return this->eOrigen;
}

bool clsMovimiento_Stock::Insertar()
{
    if(!Existe())
    {
        if(this->intCantidad == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
            return false;
        }
        else if(this->strProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "INSERT INTO stock (fecha_stock,idCentro,idProducto,Origen,comentario,Cantidad) "
                    "VALUES ('" + this->datFecha.toString("yyyy/MM/dd")  + "'," + QString::number(intCentro)  + ",'"+ this->strProd +"','"+
                    qlOrigen[this->eOrigen]+"','"+ this->strComentario +"','" + QString::number(intCantidad) + "');";

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
        QMessageBox::critical(0,QObject::tr("Error al insertar la línea del movimiento de stock"),QObject::tr("No se puede insertar la línea del movimiento de stock. Ya existe una línea para este producto en el movimiento de stock."),0,0);
        return false;
    }
}

bool clsMovimiento_Stock::Modificar()
{
    if(Existe())
    {
        if(this->intCantidad == 0)
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una cantidad mayor que cero."),0,0);
            return false;
        }
        else if(this->strProd == "")
        {
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
            return false;
        }
        else
        {
            QString strConsulta =  "UPDATE stock SET cantidad=" + QString::number(intCantidad) +
                    " WHERE idCentro=" +QString::number(this->intCentro) +" AND idProducto='"+strProd+"' AND fecha_stock ='" +
                    this->datFecha.toString("yyyy/MM/dd") + "' AND Origen='"+ qlOrigen[this->eOrigen]+"';";

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
        QMessageBox::critical(0,QObject::tr("Error al modificar la línea del movimiento de stock"),QObject::tr("No se puede modificar la línea del movimiento de stock. No existe una línea para este producto en el movimiento de stock."),0,0);
        return false;
    }
}

bool clsMovimiento_Stock::Eliminar()
{
    if(Existe())
    {
        QString strConsulta ="";

            strConsulta = "DELETE FROM stock WHERE idCentro=" +QString::number(this->intCentro) +" AND idProducto='"+strProd+"' AND fecha_stock ='" +
                    this->datFecha.toString("yyyy/MM/dd") + "' AND Origen='"+ qlOrigen[this->eOrigen]+"';";

            QSqlQuery consulta;

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
        QMessageBox::critical(0,QObject::tr("Error al eliminar la línea del movimiento de stock"),QObject::tr("No se puede eliminar la línea del movimiento de stock. No existe una línea para este producto en el movimiento de stock."),0,0);
        return false;
    }
}

void clsMovimiento_Stock::Cargar(QString _Prod,QDate _datStock,int _Centro,tOrigen _Origen)
{
    QString strConsulta = "SELECT cantidad,comentario "
            "FROM stock "
            "WHERE idCentro=" +QString::number(_Centro) +" AND idProducto='"+_Prod+"' AND fecha_stock ='" + _datStock.toString("yyyy/MM/dd") + "' AND Origen='"+ qlOrigen[this->eOrigen]+"';";
    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        this->datFecha = _datStock;
        this->strProd = _Prod;
        this->eOrigen = _Origen;
        this->intCentro = _Centro;
        if (consulta.first())
        {
            this->intCantidad = consulta.value(0).toInt();
            this->strComentario = consulta.value(1).toBool();
        }
    }
}

bool clsMovimiento_Stock::Existe() const
{
    QString strConsulta = "SELECT * "
            "FROM stock "
            "WHERE idCentro=" +QString::number(intCentro) +" AND idProducto='"+strProd+"' AND fecha_stock ='" + datFecha.toString("yyyy/MM/dd") + "' AND Origen='"+ qlOrigen[this->eOrigen]+"';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QDate clsMovimiento_Stock::Fecha() const
{
    return this->datFecha;
}

void clsMovimiento_Stock::setComentario(QString _Comentario)
{
    this->strComentario = _Comentario;
}

void clsMovimiento_Stock::setCantidad(int _Cant)
{
    this->intCantidad = _Cant;
}

QVector<QStringList> clsMovimiento_Stock::ListaStock(QString filtro)
{
    bool permiso = config->Permisos().value(Confi);

    QString strConsulta = "SELECT stock.idproducto,coalesce(productos.Descripcion,' '),stock.origen,stock.fecha_stock,coalesce(stock.comentario,' '),coalesce(stock.cantidad,0),concat_ws(' - ',centros.nombre,centros.ciudad) as centro "
            "FROM stock INNER JOIN productos ON productos.idProducto = stock.idProducto INNER JOIN centros ON centros.idCentro=stock.idCentro ";

    if(!permiso)
    {
        strConsulta += "WHERE stock.idCentro = '" + QString::number(config->IdCentroActual()) + "'";

        if(filtro != "")
            strConsulta += " AND " + filtro;
    }
    else
    {
        if(filtro != "")
            strConsulta += " WHERE " + filtro;
    }

    QStringList listResultados;
    QVector<QStringList> vectLista;

    strConsulta += " ORDER BY stock.fecha_stock;";

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
                           << sqlConsulta.value(6).toString();
            vectLista << listResultados;
        }

    return vectLista;
}
