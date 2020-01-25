#ifndef PRODUCTFORM_H
#define PRODUCTFORM_H

#include <QWidget>
#include <iostream>
#include "product.h"

namespace Ui {
class ProductForm;
}

class ProductForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProductForm(QWidget *parent = nullptr);
    ~ProductForm();
    void SetProduct(Product value);

signals:
    void valueChanged(int newValue,QString idComprado);

private slots:
    void on_Buy_clicked();

private:
    Ui::ProductForm *ui;
    void setLabels(Product v);
    QString des;
};

#endif // PRODUCTFORM_H
