#ifndef VENTANALOGIN_H
#define VENTANALOGIN_H

/*!
  \class ventanaLogin
  \brief En esta clase se incluye los métodos y atributos necesarios para iniciar una sesión en la aplicación.
*/

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRecord>

namespace Ui {
    class ventanaLogin;
}

class ventanaLogin : public QDialog {
    Q_OBJECT
public:
    /*!
      \brief Constructor de la clase.
      \param parent Objeto de tipo QWidget que contiene un puntero al objeto padre de la clase. Por defecto es 0.
    */
    ventanaLogin(QWidget *parent = 0);
    /*!
      \brief Destructor de la clase.
    */
    ~ventanaLogin();

    /*!
      \brief Comprueba que el usuario existe en la base de datos.
      \return Devuelve verdadero si el usuario existe en la base de datos, y falso en caso contrario.
    */
    bool ComprobarUsuario();

    /*!
      \brief Comprueba que la contraseña se corresponde con la del usuario actual.
      \return Devuelve verdadero si el usuario y la contrasela se corresponden, y falso en caso contrario.
    */
    bool ComprobarPass();

    /*!
      \brief Método que accede al atributo que almacena el código del usuario.
      \return Devuelve el código del usuario actual.
    */
    QString Id_Usuario();
    Ui::ventanaLogin *ui;
protected:
    void changeEvent(QEvent *e);
private:
    /*!
      \brief Atributo que almacena el código del usuario que ejecuta la aplicación.
    */
    QString id_Usuario;
};

#endif // VENTANALOGIN_H
