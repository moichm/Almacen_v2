#ifndef CONEXIONBD_H
#define CONEXIONBD_H

/*!
  \class conexionBD
  \brief En esta clase se incluye los métodos y atributos que tienen relación con el establecimiento de la conexión de la aplicación con la base de datos.
*/

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>

class conexionBD
{
    public:
        /*!
          \brief Crea una instancia de la clase inicializando el objeto de tipo QSqlDatabase \e db.
          \see db
        */
        conexionBD();

        /*!
          \brief Elimina una instancia de la clase y libera el espacio ocupado por ella.
        */
        ~conexionBD();

        /*!
          \brief Desconecta el control \e db de la base de datos.
          \see db
        */
        void DesconectarBD();

        /*!
          \brief Conecta el control \e db a la base de datos de la aplicación.
          \return Devuelve verdadero si se logra conectar a la base de datos o falso en caso contrario.
        */
        bool conectarBD();

        /*!
          \brief Objeto de tipo \e QSqlDatabase que conecta con la base de datos de la aplicación.
        */
        QSqlDatabase *db;
};

#endif // CONEXIONBD_H
