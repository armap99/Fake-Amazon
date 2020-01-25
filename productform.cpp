#include "productform.h"
#include "ui_productform.h"

ProductForm::ProductForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductForm)
{
    ui->setupUi(this);
}

ProductForm::~ProductForm()
{
    delete ui;
}

void ProductForm::SetProduct(Product value)
{
    setLabels(value);
}

void ProductForm::setLabels(Product v)
{
    QImage Imagen("C:/Users/Armando Prado/Documents/seminario de algoritmia/Practica 1/build-Practica_1-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/" + v.getId()+ ".jpg");
    ui->ImageLabel->setPixmap(QPixmap::fromImage(Imagen).scaled(ui->ImageLabel->width(),ui->ImageLabel->height(),Qt::KeepAspectRatio));
    ui->TitleLabel->setText(v.getName());
    ui->PriceLabel->setText(QString::number(v.getPrice()));
    des = v.getId();
}

void ProductForm::on_Buy_clicked()
{
    if(ui->spinBoxCant->value() != 0){
        emit valueChanged(ui->spinBoxCant->value(),des);
    }
}
