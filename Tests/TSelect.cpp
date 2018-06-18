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

    const QString query = builder->selectQuery().from("tableName").toQueryString();
    const QString expected("select * from tableName;");
    QCOMPARE(query, expected);
}

void TSelect::TestSimpleSelectColumns()
{
    SqlBuilder builder = StGen::createSqlBuilder();

    {
        const QString query = builder->select("col1", "col2").from("tableName").toQueryString();
        const QString expected("select col1, col2 from tableName;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = builder->select("col1", "col2", "col3", "col4").from("tableName").toQueryString();
        const QString expected("select col1, col2, col3, col4 from tableName;");
        QCOMPARE(query, expected);
    }
}


