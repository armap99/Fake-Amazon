#include "product.h"

int Product::getSold() const
{
    return sold;
}

void Product::setSold(int value)
{
    sold = value;
}

Product::Product(){

}

Product::Product(const QString &i, const QString &nam, float value)
{
    id = i;
   name = nam;
   price = value;
}
QString Product::getId() const
{
    return id;
}

void Product::setId(const QString &value)
{
    id = value;
}

QString Product::getName() const
{
    return name;
}

void Product::setName(const QString &value)
{
    name = value;
}

float Product::getPrice()
{
    return price;
}

void Product::setPrice(float value)
{
    price = value;
}

bool Product::operator ==(const Product&p) const
{
    return  (price == p.price);
}

bool Product::operator !=(const Product&p) const
{
    return  (price != p.price);
}

bool Product::operator <(const Product&p) const
{
    return  (price < p.price);
}

bool Product::operator <=(const Product&p) const
{
    return  (price <= p.price);
}

bool Product::operator >(const Product&p) const
{
    return  price > p.price;
}

bool Product::operator >=(const Product&p) const
{
    return  (price >= p.price);
}

