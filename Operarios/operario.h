
#ifndef OPERARIO_H
#define OPERARIO_H

/*!
  \class clsOperarios
  \brief En esta clase se incluye los métodos y atributos que tienen relación con los operarios, su creación, modificación y
         eliminación.
*/

/*!
  \class clsOperarios_Partes
  \brief En esta clase se incluye los métodos y atributos que tienen relación con los partes de trabajo, su creación, modificación y
         eliminación.
*/

/*!
  \class clsOperarios_PartesDet
  \brief En esta clase se incluye los métodos y atributos que tienen relación con las lineas de los partes de trabajo, su creación, modificación y
         eliminación.
*/

#include <QString>
#include <QStringList>
#include <QVector>
#include <QDate>

/*!
  \brief Variable global de tipo QVector<QString> que almacena los tipos de horas permitidos.
*/

enum tHora {Normal,Extra};

static QStringList qltHora = (QStringList() << QObject::tr("Normal") << QObject::tr("Extra"));

class clsPartes_Trabajo_Det;

class clsOperarios
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _NIF Parámetro de tipo QString que contiene el NIF del operario.
      \param _Nom Parámetro de tipo QString que contiene el nombre del operario. Por defecto es "".
      \param _Ape Parámetro de tipo QString que contiene los apellidos del operario. Por defecto es "".
      \param _FechaNacimiento Parámetro de tipo QDate que contiene la fecha de nacimiento del operario. Por defecto es 01/01/2001.
      \param _Direcc Parámetro de tipo QString que contiene la direccion del operario. Por defecto es "".
      \param _Email Parámetro de tipo QString que contiene el correo electrónico del operario. Por defecto es "".
      \param _CCC Parámetro de tipo QString que contiene el número de cuenta corriente del operario. Por defecto es "".
      \param _Centro Parámetro de tipo int que contiene el código del centro donde se encuentra el operario. Por defecto es 0.
      \param _rutaImage Parámetro de tipo QString que contiene la ruta de la fotografía del operario. Por defecto es "".
    */
    clsOperarios(QString _NIF="",QString _Nom="",QString _Ape="",QDate _FechaNacimiento=QDate(2000,1,1),QString _Direcc="",QString _Email="",QString _CCC="",int _Centro=0,QString _rutaImage="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsOperarios();

    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El operario debe tener un NIF, nombre, apellidos, nº de cuenta corriente y dirección distintos de la cadena vacía y debe tener algún centro,
           no deben existir dos operarios con el mismo NIF.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El operario debe tener un NIF, nombre, apellidos, nº de cuenta corriente y dirección distintos de la cadena vacía y debe tener algún centro,
           El operario debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El operario debe existir.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param NIF Contiene el NIF del operario a cargar en la instancia.
    */
    void Cargar(QString NIF);
    /*!
      \brief Comprueba si existe un operario en la base de datos con el mismo NIF que el atributo strNIF de la instancia actual.
      \return Devuelve True si se encuentra una instancia en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Devuelve el NIF del operario.
      \return Devuelve un objeto de tipo QString con el NIF del operario contenido en la instancia de la clase.
    */
    QString NIF() const;
    /*!
      \brief Devuelve el nombre del operario.
      \return Devuelve un objeto de tipo QString con el nombre del operario contenido en la instancia de la clase.
    */
    QString Nombre() const;
    /*!
      \brief Devuelve los apellidos del operario.
      \return Devuelve un objeto de tipo QString con los apellidos del operario contenido en la instancia de la clase.
    */
    QString Apellidos() const;
    /*!
      \brief Devuelve la direccion del operario.
      \return Devuelve un objeto de tipo QString con la direccion del operario contenido en la instancia de la clase.
    */
    QString Direccion() const;
    /*!
      \brief Devuelve el correo electrónico del operario.
      \return Devuelve un objeto de tipo QString con el correo electrónico del operario contenido en la instancia de la clase.
    */
    QString Email() const;
    /*!
      \brief Devuelve el nº de cuenta corriente del operario.
      \return Devuelve un objeto de tipo QString con el nº de cuenta corriente del operario contenido en la instancia de la clase.
    */
    QString CCC() const;
    /*!
      \brief Devuelve la ruta de la fotografía del operario.
      \return Devuelve un objeto de tipo QString con la ruta de la fotografía del operario contenido en la instancia de la clase.
    */
    QString Imagen() const;
    /*!
      \brief Devuelve el NIF del operario.
      \return Devuelve un objeto de tipo QString con el NIF del operario contenido en la instancia de la clase.
    */
    QDate FechaNacimiento() const;
    /*!
      \brief Devuelve el centro donde se encuentra el operario.
      \return Devuelve un objeto de tipo int con el centro donde se encuentra el operario contenido en la instancia de la clase.
    */
    int Centro() const;
    /*!
      \brief Modifica el código del centro donde se encuentra el operario.
      \param _Centro Contiene el valor a asignar al atributo intCentro.
    */
    void setCentro(int _Centro);
    /*!
      \brief Modifica el nombre del operario.
      \param _Nombre Contiene el valor a asignar al atributo strNombre.
    */
    void setNombre(QString _Nombre);
    /*!
      \brief Modifica los apellidos del operario.
      \param _Apellidos Contiene el valor a asignar al atributo strApellidos.
    */
    void setApellidos(QString _Apellidos);
    /*!
      \brief Modifica el dirección del operario.
      \param _Direccion Contiene el valor a asignar al atributo strDireccion.
    */
    void setDireccion(QString _Direccion);
    /*!
      \brief Modifica el correo electrónico del operario.
      \param _Email Contiene el valor a asignar al atributo strEmail.
    */
    void setEmail(QString _Email);
    /*!
      \brief Modifica el nº de cuenta corriente del operario.
      \param _NCCC Contiene el valor a asignar al atributo strNCCC.
    */
    void setCCC(QString _CCC);
    /*!
      \brief Modifica la ruta de la fotografía del operario.
      \param _rutaImagen Contiene el valor a asignar al atributo strRutaImagen.
    */
    void setImagen(QString _rutaImagen);
    /*!
      \brief Modifica la fecha de nacimiento del operario.
      \param _FechaNaci Contiene el valor a asignar al atributo datFechaNacimiento.
    */
    void setFechaNacimiento(QDate _FechaNaci);
    void setNIF(QString _NIF);
    QVector<QStringList> ListaOperarios(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el NIF del operario.
    */
    QString strNIF;
    /*!
      \brief Atributo que almacena el nombre del operario.
    */
    QString strNombre;
    /*!
      \brief Atributo que almacena los apellidos del operario.
    */
    QString strApellidos;
    /*!
      \brief Atributo que almacena la dirección del operario.
    */
    QString strDireccion;
    /*!
      \brief Atributo que almacena el correo electrónico del operario.
    */
    QString strEmail;
    /*!
      \brief Atributo que almacena el nº de cuenta corriente del operario.
    */
    QString strNCCC;
    /*!
      \brief Atributo que almacena centro del vehículo.
    */
    int intCentro;
    /*!
      \brief Atributo que almacena la fecha de nacimiento del operario.
    */
    QDate datFechaNacimiento;
    /*!
      \brief Atributo que almacena la ruta de la fotografía del operario.
    */
    QString strRutaImagen;
};

class clsPartes_Trabajo
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _Parte Parámetro de tipo int que contiene el número del parte de trabajo. Por defecto es 0.
      \param _Responsable Parámetro de tipo QString que contiene el NIF del responsable que crea el parte de trabajo. Por defecto es "".
      \param _Descripcion Parámetro de tipo QString que contiene la descripción del parte de trabajo. Por defecto es "".
      \param _FechaParte Parámetro de tipo QDate que contiene la fecha del parte de trabajo. Por defecto es 01/01/2001.
    */
    clsPartes_Trabajo(int _Parte=0,QString _Responsable="",QString _Descripcion="",QDate _FechaParte=QDate(2000,1,1));
    /*!
      \brief Destructor de la clase.
    */
    ~clsPartes_Trabajo();

    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre El parte de trabajo debe tener un responsable.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre El parte de trabajo debe tener un responsable.
           El parte de trabajo debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre El parte de trabajo debe existir.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param NoParte Contiene el nº del parte de trabajo a cargar en la instancia.
    */
    void Cargar(int NoParte);
    /*!
      \brief Comprueba si existe un parte de trabajo en la base de datos con el mismo nº que el atributo intNoParte de la instancia actual.
      \return Devuelve True si se encuentra una instancia en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Devuelve el responsable del parte de trabajo.
      \return Devuelve un objeto de tipo QString con el responsable del parte de trabajo contenido en la instancia de la clase.
    */
    QString Responsable() const;
    /*!
      \brief Devuelve la descripción del parte de trabajo.
      \return Devuelve un objeto de tipo QString con la descripcion del parte de trabajo contenido en la instancia de la clase.
    */
    QString Descripcion() const;
    /*!
      \brief Devuelve la fecha del parte de trabajo.
      \return Devuelve un objeto de tipo QString con la fecha del parte de trabajo contenido en la instancia de la clase.
    */
    QDate FechaParte() const;
    int Centro() const;
    /*!
      \brief Devuelve el número del parte de trabajo.
      \return Devuelve un objeto de tipo int con el número del parte de trabajo contenido en la instancia de la clase.
    */
    int NoParte() const;
    /*!
      \brief Modifica el responsable del parte de trabajo.
      \param _Responsable Contiene el valor a asignar al atributo strResponsable.
    */
    void setResponsable(QString _Responsable);
    /*!
      \brief Modifica la descripción del parte de trabajo.
      \param _Desc Contiene el valor a asignar al atributo strDescripcion.
    */
    void setDescripcion(QString _Desc);
    /*!
      \brief Modifica la fecha del parte de trabajo.
      \param _FechaParte Contiene el valor a asignar al atributo datFechaParte.
    */
    void setFechaParte(QDate _FechaParte);
    /*!
      \brief DEvuelve el detalle del parte.
      \return Devuelve un objeto tipo QList<Partes_Trabajo_Det> con el detalle del parte.
    */
    const QList<clsPartes_Trabajo_Det>& Detalles() const;
    QVector<QStringList> ListaPartes_Trabajo(QString filtro="");
private:
    /*!
      \brief Atributo que almacena el responsable del parte de trabajo.
    */
    QString strResponsable;
    /*!
      \brief Atributo que almacena la descripción del parte de trabajo.
    */
    QString strDescripcion;
    /*!
      \brief Atributo que almacena el número del parte de trabajo.
    */
    int intParte;
    int intCentro;
    /*!
      \brief Atributo que almacena la fecha del parte de trabajo.
    */
    QDate datFechaParte;
    /*!
      \brief Atributo que almacena una lista con las líneas del parte de trabajo.
    */
    QList<clsPartes_Trabajo_Det> ListDetalles;
};

class clsPartes_Trabajo_Det
{
public:
    /*!
      \brief Constructor de la clase. Se asignan a los atributos el valor de los parámetros de entrada.
      \param _NoParte Parámetro de tipo int que contiene el número del parte de trabajo. Por defecto es 0.
      \param _NIF Parámetro de tipo QString que contiene el NIF del operario de la linea del parte de trabajo. Por defecto es "".
      \param _TipoHora Parámetro de tipo QString que contiene el tipo de hora de la linea del parte de trabajo. Por defecto es "normal".
      \param _Horas Parámetro de tipo double que contiene las horas de la linea del parte de trabajo. Por defecto es 0.
      \param _Proyecto Parámetro de tipo QString que contiene el proyecto de la linea del parte de trabajo. Por defecto es "".
      \param _Tarea Parámetro de tipo QString que contiene la tarea de la linea del parte de trabajo. Por defecto es "".
    */
    clsPartes_Trabajo_Det(int _NoParte=0,QString _NIF="",tHora _TipoHora=Normal,double _Horas=0.0,QString _Proyecto="",QString _Tarea="");
    /*!
      \brief Destructor de la clase.
    */
    ~clsPartes_Trabajo_Det();

    /*!
      \brief Inserta los datos de la instancia en la base de datos.
      \pre La linea del parte de trabajo debe tener un operario y un nº de horas mayor que cero.
    */
    bool Insertar();
    /*!
      \brief Modifica los datos de la instancia en la base de datos.
      \pre La linea del parte de trabajo debe tener un operario y un nº de horas mayor que cero.
           La linea del parte de trabajo debe existir.
    */
    bool Modificar();
    /*!
      \brief Elimina los datos de la instancia en la base de datos.
      \pre La linea del parte de trabajo debe existir.
    */
    bool Eliminar();
    /*!
      \brief Carga los datos almacenados en la base de datos en la instancia de la clase.
      \param _NoParte Contiene el nº del parte de trabajo a cargar en la instancia.
      \param _NoLineaParte Contiene el nº de linea del parte de trabajo a cargar en la instancia.
    */
    void Cargar(int _NoParte,QString _NIF,tHora _TipoHora,QString _Proyecto,QString _Tarea);
    /*!
      \brief Comprueba si existe un parte de trabajo en la base de datos con el mismo nº que el atributo intNoParte de la instancia actual.
      \return Devuelve True si se encuentra una instancia en la base de datos, y False en caso contrario.
    */
    bool Existe() const;
    /*!
      \brief Devuelve el operario de la linea del parte de trabajo.
      \return Devuelve un objeto de tipo int con el operario de la linea del parte de trabajo contenido en la instancia de la clase.
    */
    QString Operario() const;
    /*!
      \brief Devuelve el nº de horas de la linea del parte de trabajo.
      \return Devuelve un objeto de tipo double con el nº de horas de la linea del parte de trabajo contenido en la instancia de la clase.
    */
    double Horas() const;
    /*!
      \brief Devuelve el proyecto de la linea del parte de trabajo.
      \return Devuelve un objeto de tipo QString con el proyecto de la linea del parte de trabajo contenido en la instancia de la clase.
    */
    QString Proyecto() const;
    /*!
      \brief Devuelve la tarea de la linea del parte de trabajo.
      \return Devuelve un objeto de tipo QString con la tarea de la linea del parte de trabajo contenido en la instancia de la clase.
    */
    QString Tarea() const;
    /*!
      \brief Devuelve el número del parte de trabajo.
      \return Devuelve un objeto de tipo int con el número del parte de trabajo contenido en la instancia de la clase.
    */
    int NoParte() const;
    /*!
      \brief Devuelve el tipo de hora de la linea del parte de trabajo.
      \return Devuelve un objeto de tipo QString con el tipo de hora de la linea del parte de trabajo contenido en la instancia de la clase.
    */
    tHora TipoHora() const;
    /*!
      \brief Modifica la tarea de la linea del parte de trabajo.
      \param _Tarea Contiene el valor a asignar al atributo strTarea.
    */
    void setTarea(QString _Tarea);
    /*!
      \brief Modifica el proyecto de la linea del parte de trabajo.
      \param _Proyecto Contiene el valor a asignar al atributo strProyecto.
    */
    void setProyecto(QString _Proyecto);
    /*!
      \brief Modifica el operario de la linea del parte de trabajo.
      \param _NIF Contiene el valor a asignar al atributo strNIF.
    */
    void setOperario(QString _NIF);
    /*!
      \brief Modifica el tipo de hora de la linea del parte de trabajo.
      \param _TipoHora Contiene el valor a asignar al atributo strTipoHora.
    */
    void setTipoHora(tHora _TipoHora);
    /*!
      \brief Modifica el número de horas de la linea del parte de trabajo.
      \param _Horas Contiene el valor a asignar al atributo decHoras.
    */
    void setHoras(double _Horas);
    void setNoParte(int _NoParte);
private:
    /*!
      \brief Atributo que almacena el número de horas de la línea del parte de trabajo.
    */
    double decHoras;
    /*!
      \brief Atributo que almacena el número del parte de trabajo.
    */
    int intNoParte;
    /*!
      \brief Atributo que almacena el proyecto de la línea del parte de trabajo.
    */
    QString strProyecto;
    /*!
      \brief Atributo que almacena la tarea de la línea del parte de trabajo.
    */
    QString strTarea;
    /*!
      \brief Atributo que almacena el operario de la línea del parte de trabajo.
    */
    QString strNIF;
    /*!
      \brief Atributo que almacena el tipo de hora de la línea del parte de trabajo.
    */
    enum tHora eTipoHora;
};

#endif // OPERARIO_H
