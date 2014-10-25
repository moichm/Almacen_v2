#ifndef PRODUCTO_H
#define PRODUCTO_H

/*!
  \class clsProductos
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de los productos, su creación,
         modificación, eliminación, salidas, envios, pedidos, etc.
*/

/*!
  \class clsCategorias
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de las categorías, su creación,
         modificación y eliminación.
*/

/*!
  \class clsFamilias
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de las familias, su creación,
         modificación y eliminación.
*/

#include <QStringList>
#include <QString>
#include <QDate>
#include <QList>
#include "Operarios/operario.h"

enum tEstado {eDisponible,ePendienteCertificacion,eNoDisponible};

static QStringList qlEstado = (QStringList() << QObject::tr("Disponible") << QObject::tr("Pendiente de Certificación") << QObject::tr("No Disponible"));

class clsProductos
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Cod Parámetro de tipo QString que almacena el codigo del producto. Por defecto es "".
      \param Desc Parámetro de tipo QString que contiene el nombre del producto. Por defecto es "".
      \param categoria Parámetro de tipo int que contiene el codigo de la categoria del producto. Por defecto es 0.
      \param familia Parámetro de tipo int que contiene el codigo de la familia del producto. Por defecto es 0.
      \param Disp Parámetro de tipo bool que almacena el estado de disponibilidad del producto. Por defecto es true.
      \param _rutaImage Parámetro de tipo QString que contiene la ruta completa de la imagen del producto. Por defecto es "".
    */
    clsProductos(QString Cod="",QString Desc="",int categoria=0,int familia=0,bool Disp=true,QString _rutaImage="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsProductos();

    /*!
      \brief Calcula el inventario del producto en el centro en el que se está ejecutando la aplicación.
      \return Devuelve un objeto de tipo int con el inventario del producto en el centro en el que se está ejecutando la aplicación.
    */
    int StockCentro() const;
    /*!
      \brief Calcula el inventario total del producto.
      \return Devuelve un objeto de tipo int con el inventario total del producto.
    */
    int StockTotal() const;
    /*!
      \brief Calcula el estado actual del producto.
      \return Devuelve el estado actual del producto.
    */
    tEstado Estado() const;
    /*!
      \brief Devuelve el código del producto.
      \return Devuelve el código del producto.
    */
    QString Codigo() const;
    /*!
      \brief Devuelve la descripción del producto.
      \return Devuelve la descripción del producto.
    */
    QString Descripcion() const;
    /*!
      \brief Devuelve la categoría del producto.
      \return Devuelve la categoría del producto.
    */
    int Categoria() const;
    /*!
      \brief Devuelve la familia del producto.
      \return Devuelve la familia del producto.
    */
    int Familia() const;
    /*!
      \brief Devuelve la disponibilidad del producto.
      \return Devuelve True si el producto esta disponible, y False en caso contrario.
    */
    bool Disponible() const;
    /*!
      \brief Crea una nueva certificación para el producto.
      \param fecha_certificacion Fecha en la que se ha certificado el producto.
      \param emisor Emisor del certificado donde se ha certificado el producto.
      \pre El producto debe existir.
    */
    void Certificar(QDate fecha_certificacion,QString emisor);
    /*!
      \brief Calucula y devuelve la fecha de la última certificación del producto.
    */
    QDate FechaUltimaCertificacion() const;
    /*!
      \brief Calcula y devuelve la fecha de la próxima certificación del producto. La fecha se calcula a sumándole la
             frecuencia de certificación del producto a la fecha de la última certificación.
    */
    QDate FechaProxCertificacion() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El producto debe tener un código y una descripción distintos de la cadena vacía, no debe existir ning'un
           producto con el mismo código, y debe tener alguna familia y categoría asociadas.
    */
    void Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El producto debe tener un código distinto de la cadena vacía, debe existir, y debe tener alguna familia y categoría asociadas.
    */
    void Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El producto debe existir.
    */
    void Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Codigo Contiene el codigo del producto a cargar en la instancia.
    */
    void Cargar(QString Codigo);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Modifica el código del producto.
      \param _codigo Contiene el nuevo valor para el atributo strCodigo.
    */
    void setCodigo(QString _codigo);
    /*!
      \brief Modifica la disponibilidad del producto.
      \param _Dispo Contiene el nuevo valor para el atributo boDisponible.
    */
    void setDisponible(bool _Dispo);
    /*!
      \brief Modifica la descripción del producto.
      \param _Desc Contiene el nuevo valor para el atributo strDescripcion.
    */
    void setDescripcion(QString _Desc);
    /*!
      \brief Modifica la categoría del producto.
      \param intCategoria Contiene el valor del atributo intCategoria.
    */
    void setCategoria(int intCategoria);
    /*!
      \brief Modifica la familia del producto.
      \param intFamilia Contiene el valor del atributo intFamilia.
    */
    void setFamilia(int intFamilia);
    /*!
      \brief Modifica la ruta de la imagen del producto.
      \param _rutaImagen Contiene el valor del atributo strRutaImagen.
    */
    void setImagen(QString _rutaImagen);
    /*!
      \brief Devuelve la ruta de la imagen del producto.
    */
    QString Imagen() const;
    QVector<QStringList> ListaProductos(QString filtro="");
private:
    /*!
      \brief Atributo que almacena la categoría del producto.
    */
    int intCategoria;
    /*!
      \brief Atributo que almacena la familia del producto.
    */
    int intFamilia;
    /*!
      \brief Atributo que almacena la disponibilidad del producto.
    */
    bool boDisponible;
    /*!
      \brief Atributo que almacena el código del producto.
    */
    QString strCodigo;
    /*!
      \brief Atributo que almacena la descripción del producto.
    */
    QString strDescripcion;
    /*!
      \brief Atributo que almacena la ruta de la imagen del producto.
    */
    QString strRutaImagen;
};

class clsCategorias
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Desc Parámetro de tipo QString que almacena el nombre de la categoría. Por defecto es "".
    */
    clsCategorias(QString Desc ="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsCategorias();

    /*!
      \brief Accede al atributo que almacena el código de la categoría contenida en la instancia.
      \return Devuelve el coódigo de la categoría.
    */
    int Codigo() const;
    /*!
      \brief Accede al atributo que almacena la descripción de la categoría.
      \return Devuelve la descripción de la categoría.
    */
    QString Descripcion() const;
    /*!
      \brief Modifica la descripción de la categoría.
      \param Desc Contiene el nuevo valor a asignar al atributo que indica la descripción de la categoría.
    */
    void setDescripcion(QString Desc);
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    void Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
    */
    void Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Código Contiene el codigo de la categoría del producto.
    */
    void Cargar(int Codigo);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    QVector<QStringList> ListaCategorias(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el código de la categoría.
    */
    int intCodigo;
    /*!
      \brief Atributo que almacena la descripción de la categoría.
    */
    QString strDescripcion;
};

class clsFamilias
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Desc Parámetro de tipo QString que almacena el nombre de la familia. Por defecto es "".
      \param Frec Parámetro de tipo int que almacena la frecuencia de certificación de la familia. Por defecto es 0.
      \param Certif Parámetro de tipo bool que indica si el tipo es certificable. Por defecto es false.
      \param Limite Parámetro de tipo int que almacena el limite de productos de la familia que se pueden entregar a un mismo operario. Por defecto es 0.
      \param limitado Parámetro de tipo bool que indica si la familia tiene límite de entrega. Por defecto es false.
    */
    clsFamilias(QString Desc ="",int Frec =0,bool Certif =false,int Limite=0,bool limitado =false);
    /*!
      \brief Destructor de la clase.
    */
    ~clsFamilias();

    /*!
      \brief Accede al atributo que almacena el código de la familia contenida en la instancia.
      \return Devuelve el coódigo de la familia.
    */
    int Codigo() const;
    /*!
      \brief Accede al atributo que almacena la descripción de la familia.
      \return Devuelve la descripción de la familia.
    */
    QString Descripcion() const;
    /*!
      \brief Accede al atributo que indica la frecuencia de certificación de la familia.
      \return Devuelve la frecuencia de certificación de la familia.
    */
    int FrecuenciaCertificacion() const;
    /*!
      \brief Devuelve true si el tipo es certificable y false en caso contrario.
    */
    bool Certificable() const;
    /*!
      \brief Accede al atributo que indica el límite de productos de la familia que se pueden entregar a un mismo operario.
      \return Devuelve la cantidad de productos de la familia que se pueden entregar a un mismo operario.
    */
    int Limite() const;
    /*!
      \brief Devuelve true si la familia tiene la entrega limitada y false en caso contrario.
    */
    bool Limitada() const;
    /*!
      \brief Modifica el atributo de la familia que indica si está limitado en la entrega.
      \param Limitada Contiene el nuevo valor a asignar al atributo que indica si la familia está limitada.
    */
    void setLimitada(bool Limitada);
    /*!
      \brief Modifica el limite de entrega de la familia.
      \param _Limite Contiene el nuevo valor a asignar al atributo que indica el límite de productos a entregar a un mismo operario.
    */
    void setLimite(int _Limite);
    /*!
      \brief Modifica el atributo del tipo que indica si es certificable.
      \param Certificable Contiene el nuevo valor a asignar al atributo que indica si la familia está limitada.
    */
    void setCertificable(bool Certificable);
    /*!
      \brief Modifica la frecuencia de certificación de la familia.
      \param _FrecCertif Contiene el nuevo valor a asignar al atributo que indica la frecuencia de certificación de la familia.
    */
    void setFrecCertificacion(int _FrecCertif);
    /*!
      \brief Modifica la descripción de la familia.
      \param Desc Contiene el nuevo valor a asignar al atributo que indica la descripción de la familia.
    */
    void setDescripcion(QString Desc);
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    void Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
    */
    void Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Codigo Contiene el codigo de la familia del producto.
    */
    void Cargar(int Codigo);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Calcula el número de veces que se le ha entregado a un operario productos de una determinada familia.
      \param clsFamilia Clase de tipo Familias que contiene la información sobre la familia del que se desea calcular el nº de productos entregados al operario.
      \param Op Clase de tipo Operario que contiene el operario.
      \return Devuelve el nº de productos que se le han entregado al operario de la familia indicada por el parametro clsFamilias.
    */
    int N_Ent_Prod_Op(clsOperarios Op) const;
    QVector<QStringList> ListaFamilias(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el código de la familia.
    */
    int intCodigo;
    /*!
      \brief Atributo que almacena la frecuencia de certificación de la familia.
    */
    int intFrec_Certif;
    /*!
      \brief Atributo que almacena la descripción de la familia.
    */
    QString strDescripcion;
    /*!
      \brief Atributo que almacena si la familia es certificable.
    */
    bool boCertificable;
    /*!
      \brief Atributo que almacena el límite de entregas de productos de esta familia que se pueden entregar a un mismo operario.
    */
    int intLimite;
    /*!
      \brief Atributo que almacena si la familia está limitado en las entregas.
    */
    bool boLimitada;
};

class clsHistoricoCertificaciones
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Desc Parámetro de tipo QString que almacena el nombre de la categoría. Por defecto es "".
    */
    clsHistoricoCertificaciones(QString Prod ="",QString _Emisior ="",QDate _Fecha_Emision = QDate(2000,1,1));
    /*!
      \brief Destructor de la clase.
    */
    ~clsHistoricoCertificaciones();

    /*!
      \brief Accede al atributo que almacena el código de la categoría contenida en la instancia.
      \return Devuelve el coódigo de la categoría.
    */
    QString Producto() const;
    /*!
      \brief Accede al atributo que almacena la descripción de la categoría.
      \return Devuelve la descripción de la categoría.
    */
    QDate Fecha_Emision() const;
    /*!
      \brief Accede al atributo que almacena la descripción de la categoría.
      \return Devuelve la descripción de la categoría.
    */
    QString Emisior() const;
    /*!
      \brief Modifica la descripción de la categoría.
      \param Desc Contiene el nuevo valor a asignar al atributo que indica la descripción de la categoría.
    */
    void setProducto(QString _Prod);
    /*!
      \brief Modifica la descripción de la categoría.
      \param Desc Contiene el nuevo valor a asignar al atributo que indica la descripción de la categoría.
    */
    void setFechaEmision(QDate _Fecha_Emision);
    /*!
      \brief Modifica la descripción de la categoría.
      \param Desc Contiene el nuevo valor a asignar al atributo que indica la descripción de la categoría.
    */
    void setEmisior(QString _Emisior);
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Código Contiene el codigo de la categoría del producto.
    */
    void Cargar(QString _Producto,QDate _Fecha_Emision);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    QVector<QStringList> ListasHistCert(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el código de la categoría.
    */
    QString strProducto;
    /*!
      \brief Atributo que almacena la descripción de la categoría.
    */
    QString strEmisior;
    /*!
      \brief Atributo que almacena la descripción de la categoría.
    */
    QDate datFechaEmision;
};

#endif // PRODUCTO_H
