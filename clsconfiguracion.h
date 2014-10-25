#ifndef CLSCONFIGURACION_H
#define CLSCONFIGURACION_H

/*!
  \class clsConfiguracion
  \brief En esta clase se incluye los métodos y atributos que tienen relación con la configuración de la aplicación.
*/

#include <QString>
#include <QFile>
#include <QApplication>
#include <QVector>
#include <QMap>
#include <QPair>

/*!
  \brief Tipo de datos que enumera los posibles permisos.
*/
typedef enum{ProdVer,ProdMod,MovVer,MovMod,KitVer,KitMod,VehVer,VehMod,OpVer,OpMod,CentVer,CentMod,Confi} tPermisos;

class clsConfiguracion: public QObject
{
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _id_usuario Parámetro de tipo QString que contiene el usuario que está ejecutando la aplicación.
      \param app Parámetro de tipo puntero a QApplication que contiene la referencia de la aplicación.
    */
    clsConfiguracion(QString _id_usuario,QApplication *app);    
    /*!
      \brief Constructor por defecto de la clase.
    */
    clsConfiguracion();
    /*!
      \brief Destructor de la clase.
    */
    ~clsConfiguracion();

    /*!
      \brief Accede al atributo que contiene el código del usuario que ejecuta actualmente la aplicación.
      \return Devuelve un objeto de tipo QString con el usuario que ejecuta actualmente la aplicación.
    */
    QString IdUsuarioActual() const;
    /*!
      \brief Carga los datos de configuración de la aplicación para el usuario que recibe como parámetro.
      \param _id_usuario Parámetro de tipo QString que contiene el usuario que está ejecutando la aplicación.
      \param app Parámetro de tipo puntero a QApplication que contiene la referencia de la aplicación.
    */
    void Cargar(QString _id_usuario,QApplication *app);
    /*!
      \brief Accede al atributo que contiene el código del centro con el que está configurado la aplicación.
      \return Devuelve un objeto de tipo int que contiene el código del centro donde se ejecuta la aplicación.
    */
    int IdCentroActual() const;
    /*!
      \brief Modifica el usuario que ejecuta actualmente la aplicación.
      \param idUsuario Parámetro de tipo QString que contiene el usuario que está ejecutando la aplicación.
    */
    void setUsuarioActual(QString idUsuario);
    /*!
      \brief Modifica el centro donde se ejecuta la aplicación.
      \param _Centro Contiene el valor a asignar al atributo intCentro.
    */
    void setCentroActual(int _Centro);
    /*!
      \brief Carga el archivo de configuración de la aplicación.
    */
    void CargarArchivoConf();
    /*!
      \brief Comprueba que existe el archivo de configuración de la aplicación en la ruta de instalación de la aplicación.
    */
    bool ExisteArchivoConf() const;
    /*!
      \brief Crea el archivo de configuración de la aplicación.
    */
    void CrearArchivoConf();
    /*!
      \brief Accede al atributo que contiene los datos y los permisos del usuario que ejecuta actualmente la aplicación.
      \return Devuelve un objeto de tipo QMap<QPair<QString,QString>,QMap<tPermisos,bool>> que contiene los datos y los permisos asignados del usuario actual.
    */
    QMap<QPair<QString,QString>,QMap<tPermisos,bool> > Usuarios() const;
    /*!
      \brief Modifica los datos y/o permisos del usuario.
      \param user Contiene el nuevo valor para el par <usuario,contraseña> del usuario actual.
      \param _Permisos Contiene el nuevo valor para los datos y permisos del usuario.
    */
    void ModUsuarios(QPair<QString,QString> user,QMap<tPermisos,bool> _Permisos);
    /*!
      \brief Carga los datos y los permisos de los usuarios.
    */
    void CargarUsuarios();
    /*!
      \brief Agrega un nuevo usuario a la aplicación.
      \param user Parámetro de tipo QPair<QString,QString> que contiene el par usuario contraseña del usuario.
      \param perm Parámetro de tipo QMap<tPermisos,bool> que contiene los datos de los permisos asignado al usuario.
    */
    void AnyadirUsuario(QPair<QString,QString> user,QMap<tPermisos,bool> perm);
    /*!
      \brief Elimina un usuario de la base de datos.
      \param user Id del usuario a eliminar.
    */
    void EliminarUsuario(QString user);
    /*!
      \brief Accede al atributo que contiene los permisos asignados al usuario que ejecuta actualmente la aplicación.
      \return Devuelve un objeto de tipo QMap<tPermisos,bool> con los permisos del usuario que ejecuta actualmente la aplicación.
    */
    QMap<tPermisos,bool> Permisos();
    /*!
      \brief Modifica el idioma de la aplicación.
      \param _Idioma Contiene el idioma al que se va a traducir la aplicación.
    */
    void setIdioma(QString _Idioma);
    /*!
      \brief Accede al atributo que contiene el idioma actual de la aplicación.
      \return Devuelve un objeto de tipo QString con el idioma actual de la aplicación.
    */
    QString Idioma() const;
    /*!
      \brief Inicializa el parámetro que contiene los idiomas en los que está disponible la apliación.
    */
    void IdiomasDisponibles();
    /*!
      \brief Accede al atributo que contiene los idiomas en los que está disponible la aplicación y su código correspondiente.
      \return Devuelve un objeto de tipo QMap<QString,QTranslator *> con los idiomas en los que esta disponible la aplicación y su correspondiente código.
    */
    QMap<QString,QTranslator *> Traducciones() const;
    /*!
      \brief Accede al atributo que contiene los idiomas en los que está disponible la aplicación.
      \return Devuelve un objeto de tipo QStringList con la lista de idiomas en los que está disponible la aplicación.
    */
    QStringList Idiomas();

public slots:
    /*!
      \brief Almacena la configuración actual de la aplicación en un archivo de configuración.
    */
    void GuardarArchivoConf();

private:
    /*!
      \brief Atributo que almacena un par <<usuario, contraseña>, permisos> por cada usuario dado de alta en la aplicación.
    */
    QMap<QPair<QString,QString>,QMap<tPermisos,bool> > mUsuarios;
    /*!
      \brief Atributo que almacena el id del usuario que ejecuta actualmente la aplicación.
    */
    QString strIdUsuario;
    /*!
      \brief Atributo que almacena el idioma actual de la aplicación.
    */
    QString strIdioma;
    /*!
      \brief Atributo que almacena el par <código, traducción> por cada idioma en el que está disponible la apliacación.
    */
    QMap<QString,QTranslator *> *ListIdIdiomasDispo;
    /*!
      \brief Atributo que almacena el centro donde está configurado la aplicación.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena el archivo de configuración de la aplicación.
    */
    QFile *archConf;
    /*!
      \brief Atributo que almacena la clase padre de la aplicación.
    */
    QApplication *qapp;
    /*!
      \brief Atributo que almacena los permisos del usuario actual de la aplicación.
    */
    QMap<tPermisos,bool> *mPermisos;
};

#endif // CLSCONFIGURACION_H
