#ifndef UTILS_H
#define UTILS_H

#include "../Core/DataBaseInterface.h"
#include <QtTest>
#include <QDebug>

class TestBase
{
private:
    DataBaseInterface *base_ = nullptr;

public:
    TestBase(DataBaseInterface* base);
    ~TestBase();
    DataBaseInterface* operator ->();
};


#endif // UTILS_H
