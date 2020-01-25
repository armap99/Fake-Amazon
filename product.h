#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <QString>

class Product
{
private:
    QString id;
    QString name;
    float price;
    int sold;

public:
    Product();
    Product(const QString &i, const QString &nam, float value);
    QString getId() const;
    void setId(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    float getPrice();
    void setPrice(float value);
    int getSold() const;
    void setSold(int value);


    bool operator == (const Product&) const;
    bool operator != (const Product&) const;
    bool operator < (const Product&) const;
    bool operator <= (const Product&) const;
    bool operator > (const Product&) const;
    bool operator >= (const Product&) const;


    //Para la prioridad
    struct comparador{
        bool operator()(Product&p1,Product&p2){
            return p1.getSold() < p2.getSold();
        }
    };
};

#endif // PRODUCT_H
