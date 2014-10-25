#include "ventanaLogin.h"
#include "ui_ventanaLogin.h"
#include "ventanaLogin.h"
#include <QDesktopWidget>
#include <string>

using namespace std;

ventanaLogin::ventanaLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ventanaLogin)
{
    ui->setupUi(this);
}

ventanaLogin::~ventanaLogin()
{
    delete ui;
}

void ventanaLogin::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool ventanaLogin::ComprobarUsuario()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("perm_user");
    model->setFilter("idUsuario = '" + ui->leUsuario->text()+"'");
    model->select();

    if (model->rowCount() == 1)
    {
        id_Usuario = ui->leUsuario->text();
        return true;
    }
    else
        return false;
}

bool ventanaLogin::ComprobarPass()
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("perm_user");
    model->setFilter("idUsuario = '" + ui->leUsuario->text()+"'");
    model->select();

    QSqlRecord registro = model->record(0);

    if (registro.value(tr("pass")).toString() == ui->lePass->text())
        return true;
    else
        return false;
}

QString ventanaLogin::Id_Usuario()
{
    return id_Usuario;
}
