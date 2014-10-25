#include "producto.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QMessageBox>
#include <QSqlError>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

clsProductos::clsProductos(QString Cod,QString Desc,int categoria,int familia,bool Disp,QString _rutaImagen)
{
    intCategoria = categoria;
    intFamilia = familia;
    boDisponible = Disp;
    strCodigo = Cod;
    strDescripcion = Desc;
    strRutaImagen = _rutaImagen;
}

clsProductos::~clsProductos(){}

int clsProductos::StockCentro() const
{
    int intEntradas=0,intSalidas=0,intCantidad=0,intDevueltas=0,intEnvios=0,intRecepciones=0;

    QString strConsultaEnt = "SELECT SUM(Cantidad) "
                             "FROM stock "
                             "WHERE idproducto = '" + this->strCodigo + "' AND Origen='Entrada' AND idcentro = " + QString::number(config->IdCentroActual()) + ";";

    QSqlQuery consultaEnt;

    QString strConsultaSal = "SELECT SUM(Cantidad) "
                             "FROM stock "
                             "WHERE idproducto = '" + this->strCodigo + "' AND Origen='Salida' AND idcentro = " + QString::number(config->IdCentroActual()) + ";";

    QSqlQuery consultaSal;

    QString strConsultaCant = "SELECT SUM(Cantidad) "
                              "FROM linea_salidas INNER JOIN salidas ON salidas.idsalida=linea_salidas.idsalida "
                              "WHERE linea_salidas.idproducto = '" + this->strCodigo + "' AND salidas.idcentro = " + QString::number(config->IdCentroActual()) + ";";

    QSqlQuery consultaCant;

    QString strConsultaDevPerd = "SELECT SUM(Devueltas) as Devueltas "
                                 "FROM linea_entradas INNER JOIN salidas ON salidas.idsalida=linea_entradas.idsalida "
                                 "WHERE linea_entradas.idproducto = '" + this->strCodigo + "' AND salidas.idcentro = " + QString::number(config->IdCentroActual()) + ";";
    QSqlQuery consultaDevPerd;


    QString strConsultaEnvios = "SELECT SUM(cantidad) "
                                "FROM envios INNER JOIN linea_envios ON envios.idenvio=linea_envios.idenvio "
                                "WHERE linea_envios.idproducto = '" + this->strCodigo + "' AND envios.idcentroOrigen = " + QString::number(config->IdCentroActual()) + ";";

    QSqlQuery consultaEnv;

    QString strConsultaRecepcion = "SELECT SUM(Cantidad) "
                                   "FROM envios INNER JOIN linea_envios ON envios.idenvio=linea_envios.idenvio "
                                   "WHERE linea_envios.idproducto = '" + this->strCodigo + "' AND envios.idcentroDestino = " + QString::number(config->IdCentroActual()) + " AND linea_envios.recibido=1;";

    QSqlQuery consultaRecep;

    if (consultaEnt.exec(strConsultaEnt))
        if (consultaEnt.first())
            intEntradas =  consultaEnt.value(0).toInt();
    if (consultaSal.exec(strConsultaSal))
        if (consultaSal.first())
            intSalidas =  consultaSal.value(0).toInt();
    if (consultaCant.exec(strConsultaCant))
        if (consultaCant.first())
            intCantidad =  consultaCant.value(0).toInt();
    if (consultaDevPerd.exec(strConsultaDevPerd))
        if (consultaDevPerd.first())
            intDevueltas =  consultaDevPerd.value(0).toInt();
    if (consultaEnv.exec(strConsultaEnvios))
        if (consultaEnv.first())
            intEnvios =  consultaEnv.value(0).toInt();
    if (consultaRecep.exec(strConsultaRecepcion))
        if (consultaRecep.first())
            intRecepciones =  consultaRecep.value(0).toInt();
    return (intEntradas + intDevueltas + intRecepciones) - (intCantidad + intSalidas + intEnvios);
}

int clsProductos::StockTotal() const
{
    int intEntradas=0,intSalidas=0;

    QString strConsultaEnt = "SELECT SUM(Cantidad) "
                             "FROM stock "
                             "WHERE idproducto = '" + this->strCodigo + "' AND Origen='entrada';";

    QSqlQuery consultaEnt;

    QString strConsultaSal = "SELECT SUM(Cantidad) "
                             "FROM stock "
                             "WHERE idproducto = '" + this->strCodigo + "' AND Origen='salida';";

    QSqlQuery consultaSal;

    if (consultaEnt.exec(strConsultaEnt))
        if (consultaEnt.first())
            intEntradas =  consultaEnt.value(0).toInt();
    if (consultaSal.exec(strConsultaSal))
        if (consultaSal.first())
            intSalidas =  consultaSal.value(0).toInt();
    return intEntradas - intSalidas;
}

tEstado clsProductos::Estado() const
{
    clsFamilias Fam;
    Fam.Cargar(this->intFamilia);

    if (Fam.Certificable())
    {
        if (this->FechaProxCertificacion() <= QDate::currentDate())
            return ePendienteCertificacion;
        else
        {
            if (boDisponible)
                return eDisponible;
            else
                return eNoDisponible;
        }
    }
    else
    {
        if (boDisponible)
            return eDisponible;
        else
            return eNoDisponible;
    }
}

QDate clsProductos::FechaUltimaCertificacion() const
{
    QString strConsulta = "SELECT MAX(fecha_certificacion) "
                          "FROM certificaciones "
                          "WHERE idproducto = '" + this->strCodigo + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        if (consulta.first())
            return consulta.value(0).toDate();
        else
            return QDate();
    else
        return QDate();
}

QDate clsProductos::FechaProxCertificacion() const
{
    clsFamilias Fam;
    Fam.Cargar(this->Familia());
    return FechaUltimaCertificacion().addMonths(Fam.FrecuenciaCertificacion());
}

void clsProductos::Certificar(QDate fecha_certificacion,QString emisor)
{
    if (!Existe())
        QMessageBox::critical(0,QObject::tr("Error al certificar"),QObject::tr("No se ha podido certificar el producto. No existe."),0,0);
    else
    {
        clsHistoricoCertificaciones HistCert;
        HistCert.setEmisior(emisor);
        HistCert.setFechaEmision(fecha_certificacion);
        HistCert.setProducto(this->strCodigo);
        if(!HistCert.Existe())
        {
            if(HistCert.Insertar())
                this->setDisponible(true);
        }
    }
}

void clsProductos::Insertar()
{
    if (!Existe())
    {        
        clsFamilias Fam;
        Fam.Cargar(this->intFamilia);

        QString strCertificable;
        QString strDisponible;
        if (Fam.Certificable() == true)
            strCertificable = "true";
        else
            strCertificable = "false";
        if (boDisponible == true)
            strDisponible = "true";
        else
            strDisponible = "false";

        QString strConsulta;

        if (strCodigo == "")
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un código para el producto."),0,0);
        else if (strDescripcion == "")
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción para el producto."),0,0);
        else if (this->intCategoria == 0)
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una categoría para el producto."),0,0);
        else if (this->intFamilia == 0)
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una familia para el producto."),0,0);
        else
        {
            strConsulta =  "INSERT INTO productos VALUES ('" + strCodigo + "', '" + strDescripcion + "', " + QString::number(intCategoria) + ", " +
                          QString::number(intFamilia) + ", " + strDisponible + ", '" + strRutaImagen + "');";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha insertado el producto."),0,0);
        }
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede insertar el producto. Ya existe."),0,0);
}

void clsProductos::Modificar()
{
    if (Existe())
    {
        clsFamilias Fam;
        Fam.Cargar(this->intFamilia);

        QString strCertificable;
        QString strDisponible;
        if (Fam.Certificable() == true)
            strCertificable = "true";
        else
            strCertificable = "false";
        if (boDisponible == true)
            strDisponible = "true";
        else
            strDisponible = "false";

        QString strConsulta;

        if (strDescripcion == "")
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción para el producto."),0,0);
        else if (this->intCategoria == 0)
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una categoría para el producto."),0,0);
        else if (this->intFamilia == 0)
            QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una familia para el producto."),0,0);
        else
        {
            strConsulta = "UPDATE productos SET Descripcion='" + strDescripcion + "', idcategoria = '" + QString::number(intCategoria) +"', idfamilia = '" + QString::number(intFamilia) +
                          "', Disponible = " + strDisponible + ", imagen = '" + strRutaImagen + "' WHERE idproducto='" + strCodigo + "';";

            QSqlQuery consulta;

            if (!consulta.exec(strConsulta))
                QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha modificado el producto."),0,0);
        }
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede modificar el producto. No existe."),0,0);
}

void clsProductos::Eliminar()
{
    if (Existe())
    {
        QSqlQuery consulta;

        QString strConsulta = "DELETE FROM productos WHERE idproducto='" + strCodigo + "';";
        if (!consulta.exec(strConsulta))
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nNo se ha eliminado el producto."),0,0);
    }
    else
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),QObject::tr("No se puede eliminar el producto. No existe."),0,0);
}

void clsProductos::Cargar(QString CodProd)
{
    QString strConsulta;

    strConsulta = "SELECT Descripcion,idcategoria,idfamilia,disponible,Imagen FROM productos WHERE idproducto = '" + CodProd + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            strCodigo = CodProd;
            strDescripcion = consulta.value(0).toString();
            intCategoria = consulta.value(1).toInt();
            intFamilia = consulta.value(2).toInt();
            boDisponible = consulta.value(3).toBool();
            strRutaImagen = consulta.value(4).toString();
        }
    }
}

bool clsProductos::Existe() const
{
    QString strConsulta;

    strConsulta = "SELECT * FROM productos WHERE idproducto = '" + strCodigo + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

int clsProductos::Categoria() const
{
    return this->intCategoria;
}

int clsProductos::Familia() const
{
    return this->intFamilia;
}

void clsProductos::setCodigo(QString _codigo)
{
    this->strCodigo = _codigo;
}

void clsProductos::setDescripcion(QString _Desc)
{
    this->strDescripcion = _Desc;
}

void clsProductos::setDisponible(bool _Dispo)
{
    this->boDisponible = _Dispo;
}

void clsProductos::setCategoria(int codCategoria)
{
    intCategoria = codCategoria;
}

void clsProductos::setFamilia(int codFamilia)
{
    intFamilia = codFamilia;
}

bool clsProductos::Disponible() const
{
    return boDisponible;
}

QString clsProductos::Codigo() const
{
    return strCodigo;
}

QString clsProductos::Descripcion() const
{
    return strDescripcion;
}

void clsProductos::setImagen(QString _rutaImagen)
{
    this->strRutaImagen = _rutaImagen;
}

QString clsProductos::Imagen() const
{
    return this->strRutaImagen;
}

QVector<QStringList> clsProductos::ListaProductos(QString filtro)
{
    QString strConsulta = "SELECT productos.idproducto,productos.Descripcion,IF(productos.disponible=1,'"+QObject::tr("Sí")+"','No'),categorias.descripcion,familias.descripcion,familias.frec_certif,IF(familias.certificable=1,'"+QObject::tr("Sí")+"','No'),familias.limite_entrega,IF(familias.limitado=1,'"+QObject::tr("Sí")+"','No') "
            "FROM productos INNER JOIN categorias ON categorias.idcategoria=productos.idcategoria INNER JOIN familias ON familias.idfamilia=productos.idfamilia ORDER BY productos.idproducto;";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY productos.idproducto;";

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
                           << sqlConsulta.value(5).toString()
                           << sqlConsulta.value(6).toString()
                           << sqlConsulta.value(7).toString()
                           << sqlConsulta.value(8).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsCategorias::clsCategorias(QString Desc)
{
    this->intCodigo = 0;
    this->strDescripcion = Desc;
}

clsCategorias::~clsCategorias(){}

int clsCategorias::Codigo() const
{
    return this->intCodigo;
}

QString clsCategorias::Descripcion() const
{
    return this->strDescripcion;
}

bool clsCategorias::Insertar()
{
    QString strConsulta;

    if (strDescripcion == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción."),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO categorias (descripcion) VALUES ('" + strDescripcion + "');";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {
            strConsulta = "SELECT LAST_INSERT_ID() FROM categorias;";
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

void clsCategorias::Modificar()
{
    QString strConsulta;

    if (strDescripcion == "")
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción."),0,0);
    else
    {
        strConsulta = "UPDATE categorias SET Descripcion='" + strDescripcion + "' WHERE idcategoria = " + QString::number(intCodigo) + ";";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
    }
}

void clsCategorias::Eliminar()
{
    QString strConsulta = "DELETE FROM categorias WHERE idcategoria=" + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsCategorias::Cargar(int Codigo)
{
    QString strConsulta = "SELECT descripcion "
                          "FROM categorias "
                          "WHERE idcategoria = " + QString::number(Codigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intCodigo = Codigo;
            this->strDescripcion = consulta.value(0).toString();
        }
    }
}

bool clsCategorias::Existe() const
{
    QString strConsulta = "SELECT * FROM categorias WHERE idcategoria=" + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

void clsCategorias::setDescripcion(QString _Desc)
{
    this->strDescripcion = _Desc;
}

QVector<QStringList> clsCategorias::ListaCategorias(QString filtro)
{
    QString strConsulta = "SELECT categorias.idCategoria,categorias.Descripcion "
                          "FROM categorias";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY categorias.idCategoria;";

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


clsFamilias::clsFamilias(QString Desc,int Frec,bool Certif,int Limite,bool Limitada)
{
    this->intCodigo = 0;
    this->strDescripcion = Desc;
    this->intLimite = Limite;
    this->boLimitada = Limitada;
    this->intFrec_Certif = Frec;
    this->boCertificable = Certif;
}

clsFamilias::~clsFamilias(){}

int clsFamilias::Codigo() const
{
    return this->intCodigo;
}

QString clsFamilias::Descripcion() const
{
    return this->strDescripcion;
}

int clsFamilias::Limite() const
{
    return this->intLimite;
}

bool clsFamilias::Limitada() const
{
    return this->boLimitada;
}

int clsFamilias::FrecuenciaCertificacion() const
{
    return this->intFrec_Certif;
}

bool clsFamilias::Certificable() const
{
    return this->boCertificable;
}

bool clsFamilias::Insertar()
{
    QString strConsulta,strLimitada,strCertificable;

    if (boCertificable)
        strCertificable = "true";
    else
        strCertificable = "false";

    if (boLimitada)
        strLimitada = "true";
    else
        strLimitada = "false";

    if (strDescripcion == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción."),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO familias (descripcion,limite_entrega,limitado,frec_certif,certificable) VALUES ('" + strDescripcion + "'," + QString::number(intLimite) + "," + strLimitada + "," + QString::number(intFrec_Certif) + "," + strCertificable + ");";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
        else
        {
            strConsulta = "SELECT LAST_INSERT_ID() FROM familias;";
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

void clsFamilias::Modificar()
{
    QString strConsulta,strLimitada,strCertificable;

    if (boCertificable)
        strCertificable = "true";
    else
        strCertificable = "false";

    if (boLimitada)
        strLimitada = "true";
    else
        strLimitada = "false";

    if (strDescripcion == "")
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir una descripción."),0,0);
    else
    {
        strConsulta = "UPDATE familias SET Descripcion='" + strDescripcion + "', limite_entrega = '" + QString::number(intLimite) + "' , limitado = " + strLimitada + ", frec_certif = '" + QString::number(intFrec_Certif) + "' , certificable = " + strCertificable + " WHERE idfamilia = " + QString::number(intCodigo) + ";";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
    }
}

void clsFamilias::Eliminar()
{
    QString strConsulta = "DELETE FROM familias WHERE idfamilia=" + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (!consulta.exec(strConsulta))
        QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
}

void clsFamilias::Cargar(int Codigo)
{
    QString strConsulta = "SELECT descripcion,limite_entrega,limitado,frec_certif,certificable "
                          "FROM familias "
                          "WHERE idfamilia = " + QString::number(Codigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->intCodigo = Codigo;
            this->strDescripcion = consulta.value(0).toString();
            this->intLimite = consulta.value(1).toInt();
            this->boLimitada = consulta.value(2).toBool();
            this->intFrec_Certif = consulta.value(3).toInt();
            this->boCertificable = consulta.value(4).toBool();
        }
    }
}

bool clsFamilias::Existe() const
{
    QString strConsulta = "SELECT * FROM familias WHERE idfamilia=" + QString::number(intCodigo) + ";";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

void clsFamilias::setCertificable(bool _Certificable)
{
    this->boCertificable = _Certificable;
}

void clsFamilias::setFrecCertificacion(int _FrecCertif)
{
    this->intFrec_Certif = _FrecCertif;
}

void clsFamilias::setLimitada(bool _Limitada)
{
    this->boLimitada = _Limitada;
}

void clsFamilias::setLimite(int _LimEnt)
{
    this->intLimite = _LimEnt;
}

void clsFamilias::setDescripcion(QString _Desc)
{
    this->strDescripcion = _Desc;
}

int clsFamilias::N_Ent_Prod_Op(clsOperarios Op) const
{
    if (this->boLimitada)
    {
        int intCantidad=0,intDevueltas=0,intPerdidas=0;

        QString strConsultaCant = "SELECT sum(linea_salidas.cantidad) AS Cant "
                                  "FROM salidas "
                                  "INNER JOIN linea_salidas ON salidas.idsalida = linea_salidas.idsalida "
                                  "INNER JOIN productos ON linea_salidas.idproducto = productos.idproducto "
                                  "WHERE salidas.responsable = '" + Op.NIF() + "' AND productos.idfamilia = '"+ QString::number(this->intCodigo) +"';";

        QString strConsultaDevPer = "SELECT sum(linea_entradas.devueltas) AS Dev, sum(linea_entradas.perdidas) AS Per "
                                    "FROM salidas "
                                    "INNER JOIN linea_entradas ON salidas.idsalida = linea_entradas.idsalida "
                                    "INNER JOIN productos ON linea_entradas.idproducto = productos.idproducto "
                                    "WHERE salidas.responsable = '" + Op.NIF() + "' AND productos.idfamilia = '"+ QString::number(this->intCodigo) +"';";

        QSqlQuery consulta;

        if (consulta.exec(strConsultaCant))
            if (consulta.first())
            {
                intCantidad =  consulta.value(0).toInt();
                if (consulta.exec(strConsultaDevPer))
                    if (consulta.first())
                    {
                       intDevueltas =  consulta.value(0).toInt();
                       intPerdidas =  consulta.value(1).toInt();
                    }
            }

        return intCantidad - intDevueltas - intPerdidas;
    }
    else
        return 0;
}

QVector<QStringList> clsFamilias::ListaFamilias(QString filtro)
{
    QString strConsulta = "SELECT familias.idFamilia,familias.Descripcion,IF(familias.limitado=1,'"+QObject::tr("Sí")+"','No'),familias.limite_entrega,IF(familias.certificable=1,'"+QObject::tr("Sí")+"','No'),familias.frec_certif "
                          "FROM familias";
    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY familias.idFamilia;";

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
                           << sqlConsulta.value(5).toString();
            vectLista << listResultados;
        }

    return vectLista;
}


clsHistoricoCertificaciones::clsHistoricoCertificaciones(QString _Prod,QString _Emisior,QDate _Fecha_Emision)
{
    this->strProducto = _Prod;
    this->strEmisior = _Emisior;
    this->datFechaEmision = _Fecha_Emision;
}

clsHistoricoCertificaciones::~clsHistoricoCertificaciones(){}

QString clsHistoricoCertificaciones::Producto() const
{
    return this->strProducto;
}

QString clsHistoricoCertificaciones::Emisior() const
{
    return this->strEmisior;
}

QDate clsHistoricoCertificaciones::Fecha_Emision() const
{
    return this->datFechaEmision;
}

void clsHistoricoCertificaciones::setProducto(QString _Prod)
{
    this->strProducto = _Prod;
}

void clsHistoricoCertificaciones::setEmisior(QString _Emisior)
{
    this->strEmisior = _Emisior;
}

void clsHistoricoCertificaciones::setFechaEmision(QDate _FechaEmision)
{
    this->datFechaEmision = _FechaEmision;
}

bool clsHistoricoCertificaciones::Insertar()
{
    QString strConsulta;

    if (strProducto == "")
    {
        QMessageBox::critical(0,QObject::tr("Error de introducción de datos"),QObject::tr("Debe introducir un producto."),0,0);
        return false;
    }
    else
    {
        strConsulta =  "INSERT INTO certificaciones (idProducto,fecha_certificacion,emisior) VALUES ('" + strProducto + "','" + datFechaEmision.toString("yyyy/MM/dd") + "','" + strEmisior + "');";

        QSqlQuery consulta;

        if (!consulta.exec(strConsulta))
        {
            QMessageBox::critical(0,QObject::tr("Error de base de datos"),consulta.lastError().text()+QObject::tr("\n\nLa ventana se cerrará"),0,0);
            return false;
        }
    }
}

void clsHistoricoCertificaciones::Cargar(QString _Prod,QDate _Fecha_Emision)
{
    QString strConsulta = "SELECT emisior "
            "FROM certificaciones "
            "WHERE idProducto = '" + _Prod + "' AND fecha_certificacion ='" + _Fecha_Emision.toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
    {
        if (consulta.first())
        {
            this->strProducto = _Prod;
            this->datFechaEmision = _Fecha_Emision;
            this->strEmisior = consulta.value(0).toString();
        }
    }
}

bool clsHistoricoCertificaciones::Existe() const
{
    QString strConsulta = "SELECT * FROM certificaciones "
            "WHERE idProducto = '" + this->Producto() + "' AND fecha_certificacion ='" + this->Fecha_Emision().toString("yyyy/MM/dd") + "';";

    QSqlQuery consulta;

    if (consulta.exec(strConsulta))
        return consulta.first();
    else
        return false;
}

QVector<QStringList> clsHistoricoCertificaciones::ListasHistCert(QString filtro)
{
    QString strConsulta = "SELECT certificaciones.idproducto,productos.Descripcion,certificaciones.fecha_certificacion,certificaciones.emisor "
            "FROM certificaciones INNER JOIN productos ON productos.idproducto=certificaciones.idproducto";

    QStringList listResultados;
    QVector<QStringList> vectLista;

    if(filtro != "")
        strConsulta += " WHERE " + filtro;

    strConsulta += " ORDER BY certificaciones.idProducto,fecha_certificacion;";

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
