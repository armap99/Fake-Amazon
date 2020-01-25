#include "user.h"

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    name = value;
}

QString User::getEmail() const
{
    return email;
}

void User::setEmail(const QString &value)
{
    email = value;
}

QString User::getPassword() const
{
    return password;
}

void User::setPassword(const QString &value)
{
    password = value;
}

User::User()
{

}
