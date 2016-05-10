#include "newcity.h"
#include "ui_newcity.h"
#include "editor.h"

NewCity::NewCity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCity)
{
    ui->setupUi(this);
}

NewCity::~NewCity()
{
    delete ui;
}

void NewCity::on_okButton_clicked()
{
    _cityName = ui->lineEdit->text ();
    close ();
}

QString NewCity::cityName() const
{
    return _cityName;
}
