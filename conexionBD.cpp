#include "conexionBD.h"

conexionBD::conexionBD()
{
    db = new QSqlDatabase();
}

conexionBD::~conexionBD()
{
    DesconectarBD();
    delete db;
}

bool conexionBD::conectarBD()
{
    *db = QSqlDatabase::addDatabase("QMYSQL");
    db->setDatabaseName("almacen");
    db->setUserName("uAlmacen");
    db->setPassword("u4lm4c3n");
    db->setHostName("localhost");

    if(db->open())
        return true;
    else
    {
        QMessageBox::critical(0,QObject::tr("Error"),db->lastError().text());
        return false;
    }
}

void conexionBD::DesconectarBD()
{
    db->removeDatabase(db->connectionName());
}
