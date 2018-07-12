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

#define DEFAULT_SQLITE_BASE(name) const QString dataBaseName(name); \
    SqliteInterface b(DataBaseSettings(dataBaseName), false); \
    TestBase base(&b); \
    StGenGlobal::setBuilder(StGen::createSqlBuilder(&b)); \
    using namespace StGenGlobal;

#define CHECK_DB_ERRORS() QVERIFY(b.hasNoError());

#define DEFAULT_NULL_CONNECTION() StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr)); \
    using namespace StGenGlobal;

#endif // UTILS_H
