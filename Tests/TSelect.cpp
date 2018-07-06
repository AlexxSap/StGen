#include "TSelect.h"

#include "Utils.h"
#include "QueryResult.h"
#include "StGenGlobal.h"

TSelect::TSelect(QObject *parent)
    : QObject(parent)
{
    init();
}

void TSelect::init()
{
    StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr));
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

void TSelect::TestSimpleWhere()
{
    using namespace StGenGlobal;

    {
        const QString query = select().from("tableName").where(equal("id", 35)).toQueryString();
        const QString expected("select * from tableName where id = 35;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(equal("date", QDate(2018, 6, 21))).toQueryString();
        const QString expected("select * from tableName where date = '2018-06-21';");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(notEqual("id", 35)).toQueryString();
        const QString expected("select * from tableName where id <> 35;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(less("id", 35)).toQueryString();
        const QString expected("select * from tableName where id < 35;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(lessEqual("id", 35)).toQueryString();
        const QString expected("select * from tableName where id <= 35;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(more("id", 35)).toQueryString();
        const QString expected("select * from tableName where id > 35;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(moreEqual("id", 35)).toQueryString();
        const QString expected("select * from tableName where id >= 35;");
        QCOMPARE(query, expected);
    }

}

void TSelect::TestSimpleWhereWithBind()
{
    using namespace StGenGlobal;

    {
        const QString query = select().from("tableName").where(equal("id", bind("id"))).toQueryString();
        const QString expected("select * from tableName where id = :id;");
        QCOMPARE(query, expected);
    }
}

void TSelect::TestSimpleWhereWithBindFromBase()
{
    DEFAULT_SQLITE_BASE("TestSimpleWhereWithBindFromBase.db");

    {
        SqlQuery query = base->query();
        query->exec("create table table1(id integer, value varchar(50));");
        query->exec("insert into table1 values(1, 'value1');");
        query->exec("insert into table1 values(2, 'value2');");
        query->exec("insert into table1 values(3, 'value3');");
        query->exec("insert into table1 values(4, 'value4');");
    }

    {
        SelectQuery query = select("value").from("table1").where(equal("id", bind("id"))).prepare();
        query.bind("id", 2);
        const QueryResult result = query.exec();

        QVERIFY(result.next());
        QCOMPARE(result.value("value").toString(), "value2");
    }

    {
        QHash<int, QString> expected;
        expected.insert(1, "value1");
        expected.insert(2, "value2");
        expected.insert(3, "value3");
        expected.insert(4, "value4");

        QSet<int> ids;
        SelectQuery query = select("value").from("table1").where(equal("id", bind("id"))).prepare();

        foreach (auto id, ids)
        {
            query.bind("id", id);
            const QueryResult result = query.exec();
            QVERIFY(result.next());
            QCOMPARE(result.value("value").toString(), expected[id]);
        }

    }
}

void TSelect::TestComplexWhere()
{
    using namespace StGenGlobal;

    {
        const QString query = select().from("tableName").where(andE(less("id", 35), notEqual("id", 7))).toQueryString();
        const QString expected("select * from tableName where id < 35 and id <> 7;");
        QCOMPARE(query, expected);
    }

    {
        const QString query = select().from("tableName").where(orE(less("id", 35), notEqual("id", 7))).toQueryString();
        const QString expected("select * from tableName where id < 35 or id <> 7;");
        QCOMPARE(query, expected);
    }

}

void TSelect::TestDistinct()
{
    using namespace StGenGlobal;

    {
        const QString query = select(distinct("col1"), "col2").from("tableName").toQueryString();
        const QString expected("select distinct col1, col2 from tableName;");
        QCOMPARE(query, expected);
    }
}


