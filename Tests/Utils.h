#ifndef UTILS_H
#define UTILS_H

#include "../Core/DataBaseInterface.h"

class TestBase
{
private:
    DataBaseInterface *base_ = nullptr;
    QString name_;

public:
    TestBase(DataBaseInterface* base,
             const QString &name);
    ~TestBase();
    DataBaseInterface* operator ->();
};


#endif // UTILS_H
