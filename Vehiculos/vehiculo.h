#ifndef VEHICULO_H
#define VEHICULO_H

/*!
  \class clsVehiculos
  \brief En esta clase se incluye los métodos y atributos que tienen relación con los vehiculos, su creación, modificación y
         eliminación.
*/

/*!
  \class clsVehiculo_Movs
  \brief En esta clase se incluye los métodos y atributos que tienen relación con la creación de movimientos de vehiculos.
*/

#include <QString>
#include <QDate>

class clsVehiculo_Movs;

class clsVehiculos
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Matricula Parámetro de tipo QString que contiene la matrícula del vehículo. Por defecto es "".
      \param _Marca Parámetro de tipo QString que contiene la marca del vehículo. Por defecto es "".
      \param _Modelo Parámetro de tipo QString que contiene el modelo del vehículo. Por defecto es "".
      \param _FechaMatriculacion Parámetro de tipo QDate que contiene la fecha de compra del vehículo. Por defecto es 01/01/2001.
      \param _Centro Parámetro de tipo int que contiene el código del centro donde se encuentra el vehículo. Por defecto es 0.
    */
    clsVehiculos(QString _Matricula="",QString _Marca="",QString _Modelo="",QDate _FechaMatriculacion=QDate(01,01,2001),int _Centro=0);
    /*!
      \brief Destructor de la clase.
    */
    ~clsVehiculos();

    /*!
      \brief Devuelve la matrícula del vehículo.
      \return Devuelve un objeto de tipo QString con la matricula del vehiculo contenido en la instancia de la clase.
    */
    QString Matricula() const;
    /*!
      \brief Devuelve la marca del vehículo.
      \return Devuelve un objeto de tipo QString con la marca del vehiculo contenido en la instancia de la clase.
    */
    QString Marca() const;
    /*!
      \brief Devuelve el modelo del vehículo.
      \return Devuelve un objeto de tipo QString con el modelo del vehiculo contenido en la instancia de la clase.
    */
    QString Modelo() const;
    /*!
      \brief Devuelve el centro donde se encuentra el vehículo.
      \return Devuelve un objeto de tipo int con el centro donde se encuentra el vehiculo contenido en la instancia de la clase.
    */
    int Centro() const;
    /*!
      \brief Devuelve la fecha de compra del vehículo.
      \return Devuelve un objeto de tipo QDate con la fecha de compra del vehiculo contenido en la instancia de la clase.
    */
    QDate FechaMatriculacion() const;
    QDate FechaITV() const;
    /*!
      \brief Modifica la matrícula del vehículo.
      \param _Matricula Contiene el valor a asignar al atributo strMatricula.
    */
    void setMatricula(QString _Matricula);
    /*!
      \brief Modifica la marca del vehículo.
      \param _Marca Contiene el valor a asignar al atributo strMarca.
    */
    void setMarca(QString _Marca);
    /*!
      \brief Modifica el modelo del vehículo.
      \param _Modelo Contiene el valor a asignar al atributo strModelo.
    */
    void setModelo(QString _Modelo);
    /*!
      \brief Modifica la fecha de compra del vehículo.
      \param _FechaMatriculacion Contiene el valor a asignar al atributo datFechaMatriculacion.
    */
    void setFechaMatriculacion(QDate _FechaMatriculacion);
    /*!
      \brief Modifica el código del centro donde se encuentra el vehículo.
      \param _Centro Contiene el valor a asignar al atributo intCentro.
    */
    void setCentro(int _Centro);
    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El vehículo debe tener una matrícula, marca, modelo y tipo distintos de la cadena vacía y debe tener algún centro,
           no deben existir dos vehículos con la misma matrícula.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El vehículo debe tener una matrícula, marca, modelo y tipo distintos de la cadena vacía y debe tener algún centro.
           El vehículo debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El vehículo debe existir.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _Matricula Contiene matrícula del vehículo a cargar en la instancia.
    */
    void Cargar(QString _Matricula);
    /*!
      \brief Comprueba si existe un vehículo en la base de datos con la misma matrícula que el atributo strMatricula de la instancia actual.
      \return Devuelve True si se encuentra una instancia en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Accede a la lista de productos que componen el kit.
      \return Devuelve una lista de instancias de tipo clsKitDetalle que componen el kit.
    */
    const QList<clsVehiculo_Movs>& Movimientos() const;
    QVector<QStringList> ListaVehiculos(QString filtro="");
    /*!
      \brief Modifica la ruta de la imagen del producto.
      \param _rutaImagen Contiene el valor del atributo strRutaImagen.
    */
    void setImagen(QString _rutaImagen);
    /*!
      \brief Devuelve la ruta de la imagen del producto.
    */
    QString Imagen() const;
private:
    /*!
      \brief Atributo de tipo QList<clsKitDetalle> que almacena la lista de productos contenidas en el kit.
    */
    QList<clsVehiculo_Movs> ListMovimientos;
    /*!
      \brief Atributo que almacena el centro del vehículo.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena la matrícula del vehículo.
    */
    QString strMatricula;
    /*!
      \brief Atributo que almacena la marca del vehículo.
    */
    QString strMarca;
    /*!
      \brief Atributo que almacena el modelo del vehículo.
    */
    QString strModelo;
    /*!
      \brief Atributo que almacena la fecha de compra del vehículo.
    */
    QDate datFechaMatriculacion;
    QString strRutaImagen;
};

class clsVehiculo_Movs
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Centro Parámetro de tipo int que contiene el código del centro donde se encuentra el vehículo.
      \param _Matricula Parámetro de tipo QString que contiene la matrícula del vehículo.
    */
    clsVehiculo_Movs(int _Centro,QString _Matricula,double _Kms=0);
    clsVehiculo_Movs(const clsVehiculo_Movs& VehiMovs);
    /*!
      \brief Destructor de la clase.
    */
    ~clsVehiculo_Movs();

    /*!
      \brief Crea un nuevo movimiento del vehículo en la base de datos.
      \pre El vehiculo y el centro deben existir.
    */
    bool Insertar();
    QDate FechaMovimiento();
    double KmsFinales();
    int Centro();
    void Cargar(QString _Matricula,int _Centro);
private:
    /*!
      \brief Atributo que almacena el centro donde se encuentra vehículo.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena la matrícula del vehículo.
    */
    QString strMatricula;
    QDate datFechaMov;
    double douKms;
};

#endif // VEHICULO_H
