#include "categoriasficha.h"
#include "ui_categoriasficha.h"
#include "clsconfiguracion.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QDialogButtonBox>
#include <QPushButton>

extern clsConfiguracion *config;

CategoriasFicha::CategoriasFicha(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoriasFicha)
{
    ui->setupUi(this);

    Cat = new clsCategorias();

    Cat->Cargar(codigo);
    CargarDatos();

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

    DeshabilitarControles();
}

CategoriasFicha::~CategoriasFicha()
{
    delete Cat;
    delete ui;
}

void CategoriasFicha::changeEvent(QEvent *e)
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

void CategoriasFicha::CargarDatos()
{
    ui->edtCodigo->setText(QString::number(Cat->Codigo()));
    ui->edtDescripcion->setText(Cat->Descripcion());
}

void CategoriasFicha::InsertarCategoria()
{
    Cat->setDescripcion(ui->edtDescripcion->text());
    Cat->Insertar();
}

void CategoriasFicha::ModificarCategoria()
{
    Cat->setDescripcion(ui->edtDescripcion->text());
    Cat->Modificar();
}

void CategoriasFicha::Guardar()
{
    if (!Cat->Existe())
        InsertarCategoria();
    else
        ModificarCategoria();
}

void CategoriasFicha::DeshabilitarControles()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(config->Permisos().value(ProdMod));
}
