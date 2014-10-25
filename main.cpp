#include <QtGui/QApplication>
#include <QTextCodec>
#include <QLocale>
#include <QTranslator>
#include "clsconfiguracion.h"
#include "principal.h"

/*!
 \mainpage Almacén
 \author Moisés Chaves Mota
 \date Noviembre 2010
*/

clsConfiguracion *config;

int main(int argc, char *argv[])
{    
    config = new clsConfiguracion();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QApplication app(argc, argv);

    QString local = QLocale::system().country() + "_" + QLocale::system().language();

    QTranslator traductor;
    traductor.load(QString("Almacen_") + local);
    app.installTranslator(&traductor);

    Principal *ventanaPrincipal = new Principal();

    app.connect(&app,SIGNAL(lastWindowClosed()),config,SLOT(GuardarArchivoConf()));

    if(ventanaPrincipal->boEntra)
    {
        ventanaPrincipal->showMaximized();
        return app.exec();
    }
}
