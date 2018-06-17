#include "TSelect.h"

#include <QtTest>

#include "../Core/StGen.h"

TSelect::TSelect(QObject *parent)
    : QObject(parent)
{

}

void TSelect::TestSimpleSelect()
{
    SqlBuilder builder = StGen::createSqlBuilder();
    {
        const QString query = builder->select().from("tableName").toQueryString();
        const QString expected("select * from tableName;");
        QCOMPARE(query, expected);
    }

}
