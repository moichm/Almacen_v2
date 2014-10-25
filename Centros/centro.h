#ifndef CENTRO_H
#define CENTRO_H

/*!
  \class Centro
  \brief En esta clase se incluye los métodos y atributos que tienen relación con los centros, su creación, modificación y eliminación.
*/

#include <QString>
#include <QStringList>
#include <QVector>

class clsCentros
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Codigo Parámetro de tipo int que almacena el codigo del centro. Por defecto es 0.
      \param _Nombre Parámetro de tipo QString que contiene el nombre del centro. Por defecto es "".
      \param _Ciudad Parámetro de tipo QString que contiene el nombre de la ciudad del centro. Por defecto es "".
    */
    clsCentros(int _Codigo=0,QString _Nombre="",QString _Ciudad="", QString _Direccion="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsCentros();

    /*!
      \brief Inserta la instancia de la clase en la base de datos.
      \pre El centro debe tener un nombre y una ciudad distintos de la cadena vacía.
    */
    bool Insertar();
    /*!
      \brief Modifica la instancia de la clase almacenada en la base de datos.
      \pre El centro debe existir y tener un nombre y una ciudad distintos de la cadena vacía.
    */
    bool Modificar();
    /*!
      \brief Elimina la instancia de la clase almacenada en la bases de datos.
      \pre La instancia a eliminar debe haber sido añadida en la base de datos previamente.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param Codigo Contiene el codigo del centro a cargar en la instancia.
    */
    void Cargar(int Codigo);
    /*!
      \brief Comprueba si la instancia de la clase ha sido almacenada en la base de datos.
      \return Devuelve True si la instancia ha sido almacenda en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Accede al atributo Codigo de la instancia.
      \return Devuelve el Codigo de la instancia de la clase.
    */
    int Codigo() const;
    /*!
      \brief Accede al atributo Nombre de la instancia.
      \return Devuelve el Nombre de la instancia de la clase.
    */
    QString Nombre() const;
    /*!
      \brief Accede al atributo Ciudad de la instancia.
      \return Devuelve la Ciudad de la instancia de la clase.
    */
    QString Ciudad() const;
    /*!
      \brief Accede al atributo Ciudad de la instancia.
      \return Devuelve la Ciudad de la instancia de la clase.
    */
    QString Direccion() const;
    /*!
      \brief Modifica el Nombre de la instancia.
      \param _Nombre Nuevo nombre para la instancia de la clase.
    */
    void setNombre(QString _Nombre);
    /*!
      \brief Modifica la Ciudad de la instancia.
      \param _Ciudad Nueva ciudad para la instancia de la clase.
    */
    void setCiudad(QString _Ciudad);
    /*!
      \brief Modifica la Ciudad de la instancia.
      \param _Ciudad Nueva ciudad para la instancia de la clase.
    */
    void setDireccion(QString _Direccion);
    QVector<QStringList> ListaCentros(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el codigo del centro.
    */
    int intCodigo;
    /*!
      \brief Atributo que almacena el nombre del centro.
    */
    QString strNombre;
    /*!
      \brief Atributo que almacena la ciudad del centro.
    */
    QString strCiudad;
    /*!
      \brief Atributo que almacena la ciudad del centro.
    */
    QString strDireccion;
};

#endif // CENTRO_H
