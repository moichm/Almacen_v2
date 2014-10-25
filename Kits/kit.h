#ifndef KIT_H
#define KIT_H

/*!
  \class clsKits
  \brief En esta clase se incluyen los métodos y atributos que tienen relación con el manejo de los kits, su creación, modificación y eliminación.
*/

#include <QString>
#include <QDate>
#include <QList>
#include <QStringList>
#include <QVector>

class clsKitDetalle;

class clsKits
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Desc Parámetro de tipo QString que almacena la descripción del kit. Por defecto es "".
    */
    clsKits(QString Desc="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsKits();

    /*!
      \brief Accede al atributo que almacena el código de la instancia.
      \return Devuelve el código del kit contenido en la instancia.
    */
    int Codigo() const;
    /*!
      \brief Accede al atributo que almacena el nombre del kit contenido de la instancia.
      \return Devuelve el nombre del kit contenido en la instancia.
    */
    QString Descripcion() const;
    /*!
      \brief Accede a la lista de productos que componen el kit.
      \return Devuelve una lista de instancias de tipo clsKitDetalle que componen el kit.
    */
    const QList<clsKitDetalle>& Detalles() const;
    /*!
      \brief Modifica el atributo strDescripcion de la instancia actual.
      \param _Desc Contiene el nuevo valor para el atributo strDescripcion.
    */
    void setDescripcion(QString _Desc);
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El kit debe tener una descripción.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El kit debe tener una descripción.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El kit debe existir.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Codigo Contiene el codigo del kit a cargar en la instancia.
    */
    void Cargar(int Codigo);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    QVector<QStringList> ListaKits(QString filtro="");
private:
    /*!
      \brief Atributo de tipo QList<clsKitDetalle> que almacena la lista de productos contenidas en el kit.
    */
    QList<clsKitDetalle> ListDetalles;
    /*!
      \brief Atributo que almacena el código del kit.
    */
    int intCodigo;
    /*!
      \brief Atributo que almacena la descripción del kit.
    */
    QString strDescripcion;
};

class clsKitDetalle
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param Cod Parámetro de tipo int que almacena el código del kit. Por defecto es 0.
      \param CodProd Parámetro de tipo QString que almacena el código del producto. Por defecto es "".
      \param Cant Parámetro de tipo int que almacena la cantidad del producto en el kit. Por defecto es 0.
    */
    clsKitDetalle(int Cod,QString CodProd,int Cant);
    /*!
      \brief Constructor por copia de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param KitDet Parámetro de tipo clsKitDetalle& que almacena la clase base del kit.
    */
    clsKitDetalle(const clsKitDetalle& KitDet);
    /*!
      \brief Destructor de la clase.
    */
    ~clsKitDetalle();

    /*!
      \brief Accede al atributo que almacena la cantidad del producto contenido en la instancia.
      \return Devuelve la cantidad del producto.
    */
    int Cantidad() const;
    /*!
      \brief Accede al atributo que almacena el código del producto contenido en la instancia.
      \return Devuelve el código del producto.
    */
    QString CodProd() const;
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre La cantidad del producto debe ser mayor que cero.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Codigo Contiene el codigo del kit.
      \param CodProd Contiene el codigo del producto.
    */
    void Cargar(int Codigo,QString CodProd);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    void setCantidad(int _Cant);
private:
    /*!
      \brief Atributo que almacena la cantidad del producto.
    */
    int intCantidad;
    /*!
      \brief Atributo que almacena el código del kit.
    */
    int intCod;
    /*!
      \brief Atributo que almacena el código del producto.
    */
    QString strCodProd;
};

#endif // KIT_H
