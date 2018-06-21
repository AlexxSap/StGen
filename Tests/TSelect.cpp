#include "TSelect.h"

#include <QtTest>
#include "StGen.h"
#include "Utils.h"
#include "QueryResult.h"
#include <StGenGlobal.h>

TSelect::TSelect(QObject *parent)
    : QObject(parent)
{
    SqliteBuilder builder = StGen::createSqlBuilder(nullptr);
    StGenGlobal::sqlBuilder = builder;
}

void TSelect::TestSimpleSelect()
{
    SqliteBuilder builder = StGen::createSqlBuilder(nullptr);

    const QString query = builder->select().from("tableName").toQueryString();
    const QString expected("select * from tableName;");
    QCOMPARE(query, expected);
}

void TSelect::TestSimpleSelectColumns()
{
    using namespace StGenGlobal;

    {
        const QString query = select("col1", "col2").from("tableName").toQueryString();
        const QString expected("select col1, col2 from tableName;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select("col1", "col2", "col3", "col4").from("tableName").toQueryString();
        const QString expected("select col1, col2, col3, col4 from tableName;");
        QCOMPARE(query, expected);
    }
}

void TSelect::TestSimpleSelectFromBase()
{
    const QString dataBaseName("TestSimpleSelectFromBase.db");
    SqliteInterface b(DataBaseSettings(dataBaseName), false);
    TestBase base(&b);

    QHash<int, QString> expected;
    {
        expected.insert(1, "value1");
        expected.insert(2, "value2");
        SqlQuery query = base->query();
        query->exec("create table table1(id integer, value varchar(50));");
        query->exec("insert into table1 values(1, 'value1');");
        query->exec("insert into table1 values(2, 'value2');");
    }

    SqliteBuilder builder = StGen::createSqlBuilder(&b);

    {
        const QueryResult result = builder->select("id", "value").from("table1").exec();
        QHash<int, QString> actual;
        while(result.next())
        {
            actual.insert(result.value("id").toInt(), result.value(1).toString());
        }
        QCOMPARE(actual, expected);
    }

}


