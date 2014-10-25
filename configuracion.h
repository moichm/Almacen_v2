#ifndef CONFIGURACION_H
#define CONFIGURACION_H

/*!
  \class Configuracion
  \brief En esta clase se incluye los métodos y atributos que tienen relación con la ventana de configuración de la aplicación.
*/

#include <QDialog>
#include <QMap>
#include <QVector>

namespace Ui {
    class Configuracion;
}

class Configuracion : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    Configuracion(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~Configuracion();

protected:
    /*!
      \brief Controlador de eventos que maneja los cambios de estado.
    */
    void changeEvent(QEvent *e);

private slots:
    /*!
      \brief Carga la lista de centros.
    */
    void CargarCentros();
    /*!
      \brief Guarda los datos de configuración y de los usuarios y cierra la ventana.
    */
    void Guardar();
    /*!
      \brief Guarda los datos de configuración y de los usuarios y no cierra la ventana.
    */
    void Aplicar();
    /*!
      \brief Carga la lista con los permisos de los usuarios.
    */
    void CargarLineasUsuarios();
    /*!
      \brief Marca los registros de los usuarios que han sido modificados.
    */
    void Modificacion();
    /*!
      \brief Añade un usuario a la aplicación.
    */
    void AniadirUsuario();
    /*!
      \brief Elimina un usuario de la aplicación.
    */
    void EliminarUsuario();

    void Copia_Seguridad();

    void SeleccionarArchivo();

    void RestablecerPass();
signals:
    void Guardado();
private:
    QString asignarPass(QString idUsuario);
    void Almacenar();
    Ui::Configuracion *ui;
    /*!
      \brief Atributo que almacena un par <Nombre, codigo> por cada centro dado de alta en la aplicación.
    */
    QMap<QString,int> mapCentros;
    /*!
      \brief Atributo que almacena un par <usuario, contraseña> por cada usuario dado de alta en la aplicación.
    */
    QMap<QString,QString> mapPassUser;
    /*!
      \brief Atributo que almacena un valor booleano por cada linea de usuario.
    */
    QVector<bool> *Mods;
    /*!
      \brief Atributo que almacena los números de las lineas insertadas desde la última vez que se guardaron los datos.
    */
    QVector<int> *Inserts;
    /*!
      \brief Atributo que almacena los números de las lineas eliminadas desde la última vez que se guardaron los datos.
    */
    QVector<int> *Eliminadas;
    QWidget *padre;
};

#endif // CONFIGURACION_H
