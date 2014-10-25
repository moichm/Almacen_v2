#include "centrosficha.h"
#include "ui_centrosficha.h"
#include "centro.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include "clsconfiguracion.h"

extern clsConfiguracion *config;

CentrosFicha::CentrosFicha(int _Codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CentrosFicha)
{
    ui->setupUi(this);

    Centro = new clsCentros;
    Centro->Cargar(_Codigo);

    CargarDatos();

    QPushButton *btnGuardar = ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *btnCancelar = ui->buttonBox->button(QDialogButtonBox::Cancel);
    connect(btnGuardar,SIGNAL(clicked()),this,SLOT(Guardar()));
    connect(btnCancelar,SIGNAL(clicked()),this,SLOT(accept()));

    DeshabilitarControles();
}

CentrosFicha::~CentrosFicha()
{
    delete Centro;
    delete ui;
}

void CentrosFicha::changeEvent(QEvent *e)
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

void CentrosFicha::CargarDatos()
{
    if (Centro->Codigo() != 0)
    {
        Centro->Cargar(ui->edtCodigo->text().toInt());

        ui->edtCodigo->setText(QString::number(Centro->Codigo()));
        ui->edtNombre->setText(Centro->Nombre());
        ui->edtCiudad->setText(Centro->Ciudad());
        ui->edtDireccion->setText(Centro->Direccion());
    }
}

void CentrosFicha::Guardar()
{
    Centro->setNombre(ui->edtNombre->text());
    Centro->setCiudad(ui->edtCiudad->text());
    Centro->setDireccion(ui->edtDireccion->text());

    if (!Centro->Existe())
    {
        if (Centro->Insertar())
            this->accept();
        else
            this->reject();
    }
    else
    {
        if(Centro->Modificar())
            this->accept();
        else
            this->reject();
    }
}

void CentrosFicha::DeshabilitarControles()
{
    bool permiso = config->Permisos().value(CentMod);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(permiso);
}
