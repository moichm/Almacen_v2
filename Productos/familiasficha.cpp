#include "familiasficha.h"
#include "ui_familiasficha.h"
#include "clsconfiguracion.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QDialogButtonBox>
#include <QPushButton>

extern clsConfiguracion *config;

FamiliasFicha::FamiliasFicha(int codigo,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FamiliasFicha)
{
    ui->setupUi(this);

    Fam = new clsFamilias();

    Fam->Cargar(codigo);
    CargarDatos();

    ActivarCampo();

    connect(ui->chkCertificable,SIGNAL(clicked()),this,SLOT(ActivarCampo()));
    connect(ui->chkLimitada,SIGNAL(clicked()),this,SLOT(ActivarCampo()));
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Guardar()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(close()));

    DeshabilitarControles();
}

FamiliasFicha::~FamiliasFicha()
{
    delete Fam;
    delete ui;
}

void FamiliasFicha::changeEvent(QEvent *e)
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

void FamiliasFicha::CargarDatos()
{
    ui->edtCodigo->setText(QString::number(Fam->Codigo()));
    ui->edtDescripcion->setText(Fam->Descripcion());
    ui->sboxLimite->setValue(Fam->Limite());
    ui->chkLimitada->setChecked(Fam->Limitada());
    ui->sboxFrecuenciaCertif->setValue(Fam->FrecuenciaCertificacion());
    ui->chkCertificable->setChecked(Fam->Certificable());
}

void FamiliasFicha::ActivarCampo()
{
    if (ui->chkLimitada->isChecked())
        ui->sboxLimite->setEnabled(true);
    else
        ui->sboxLimite->setEnabled(false);

    if (ui->chkCertificable->isChecked())
        ui->sboxFrecuenciaCertif->setEnabled(true);
    else
        ui->sboxFrecuenciaCertif->setEnabled(false);
}

void FamiliasFicha::InsertarFamilia()
{
        Fam->setDescripcion(ui->edtDescripcion->text());
        Fam->setLimite(ui->sboxLimite->value());
        Fam->setLimitada(ui->chkLimitada->isChecked());
        Fam->setFrecCertificacion(ui->sboxFrecuenciaCertif->value());
        Fam->setCertificable(ui->chkCertificable->isChecked());
        Fam->Insertar();
}

void FamiliasFicha::ModificarFamilia()
{
    Fam->setDescripcion(ui->edtDescripcion->text());
    Fam->setLimite(ui->sboxLimite->value());
    Fam->setLimitada(ui->chkLimitada->isChecked());
    Fam->setFrecCertificacion(ui->sboxFrecuenciaCertif->value());
    Fam->setCertificable(ui->chkCertificable->isChecked());
    Fam->Modificar();
}

void FamiliasFicha::Guardar()
{
    if (!Fam->Existe())
        InsertarFamilia();
    else
        ModificarFamilia();
}

void FamiliasFicha::DeshabilitarControles()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(config->Permisos().value(ProdMod));
}
