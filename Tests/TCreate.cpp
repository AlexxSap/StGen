#include "TCreate.h"

#include "Utils.h"
#include "StGenGlobal.h"

TCreate::TCreate(QObject *parent) : QObject(parent)
{

}

void TCreate::TestSimpleCreateTable()
{
    StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr));
    using namespace StGenGlobal;

    {
        const QString query = createTable("table1")
                .addColumn("id", ColumnType::Integer())
                .addColumn("value", ColumnType::String(50))
                .toQueryString();

        const QString expected("create table if not exists table1 (id integer, value varchar(50));");
        QCOMPARE(query, expected);
    }

    {
        const QString query = createTable("table1")
                .addColumn(TableColumn("id", ColumnType::Integer()))
                .addColumn(TableColumn("value", ColumnType::String(50)))
                .toQueryString();

        const QString expected("create table if not exists table1 (id integer, value varchar(50));");
        QCOMPARE(query, expected);
    }
}

void TCreate::TestSimpleCreateTableOnBase()
{
    const QString dataBaseName("TestSimpleCreateTableOnBase.db");
    SqliteInterface b(DataBaseSettings(dataBaseName), false);
    TestBase base(&b);

    StGenGlobal::setBuilder(StGen::createSqlBuilder(&b));
    using namespace StGenGlobal;

    createTable("table1")
            .addColumn("id", ColumnType::Integer())
            .addColumn("value", ColumnType::String(50))
            .exec();

    QHash<int, QString> expected;
    {
        expected.insert(1, "value1");
        expected.insert(2, "value2");
        SqlQuery query = base->query();
        query->exec("insert into table1 values(1, 'value1');");
        query->exec("insert into table1 values(2, 'value2');");
    }

    {
        SqlQuery query = base->query();
        query->exec("select id, value from table1;");

        QHash<int, QString> actual;
        while(query->next())
        {
            actual.insert(query->value("id").toInt(), query->value("value").toString());
        }
        QCOMPARE(actual, expected);
    }
}

void TCreate::TestPrimaryKey()
{
    StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr));
    using namespace StGenGlobal;

    {
        const QString query = createTable("table1")
                .addColumn("user", ColumnType::Integer())
                .addColumn("current", ColumnType::Date())
                .addColumn("value", ColumnType::String(50))
                .setPrimaryKey("user", "date")
                .toQueryString();

        const QString expected("create table if not exists table1 (user integer, current date, value varchar(50), primary key(user, date));");
        QCOMPARE(query, expected);
    }

}
