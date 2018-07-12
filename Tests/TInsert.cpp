#include "TInsert.h"

#include "Utils.h"
#include "StGenGlobal.h"

TInsert::TInsert(QObject *parent) : QObject(parent)
{

}

void TInsert::TestSimpleInsert()
{
    DEFAULT_NULL_CONNECTION();

    {
        const QString query = insert("col1", "col2")
                .into("table1")
                .values("a", "b")
                .toQueryString();

        const QString expected("insert into table1(col1, col2) values('a', 'b');");
        QCOMPARE(query, expected);
    }

    {
        const QString query = insert("col1", "col2")
                .into("table1")
                .values("a1", "b1")
                .values("a2", "b2")
                .values("a3", "b3")
                .toQueryString();

        const QString expected("insert into table1(col1, col2) values('a1', 'b1'),('a2', 'b2'),('a3', 'b3');");
        QCOMPARE(query, expected);
    }

    {
        const QString query = insert("col1", "col2")
                .into("table1")
                .values(Values()
                        << QVariantList{"a1", "b1"}
                        << QVariantList{"a2", "b2"}
                        << QVariantList{"a3", "b3"})
                .toQueryString();

        const QString expected("insert into table1(col1, col2) values('a1', 'b1'),('a2', 'b2'),('a3', 'b3');");
        QCOMPARE(query, expected);
    }
}

void TInsert::TestSimpleInsertToBase()
{   
    DEFAULT_SQLITE_BASE("TestSimpleInsertToBase.db");

    createTable("table1")
            .addColumn("id", ColumnType::Integer())
            .addColumn("value", ColumnType::String(5))
            .exec();

    insert("id", "value")
            .into("table1")
            .values(1, "aaaaa")
            .values(2, "bbbbb")
            .values(3, "ccccc")
            .exec();

    QHash<int, QString> expected;
    expected.insert(1, "aaaaa");
    expected.insert(2, "bbbbb");
    expected.insert(3, "ccccc");

    const QueryResult result = select("id", "value").from("table1").exec();
    QVERIFY(result.error().isEmpty());

    QHash<int, QString> actual;
    while(result.next())
    {
        actual.insert(result.value("id").toInt(), result.value("value").toString());
    }
    QCOMPARE(actual, expected);
    CHECK_DB_ERRORS();
}

void TInsert::TestInsertFromSelect()
{
    DEFAULT_NULL_CONNECTION();

    {
        const QString query = insert("col1", "col2")
                .into("table1")
                .from(select("col11", "col22").from("table2"))
                .toQueryString();

        const QString expected("insert into table1(col1, col2) select col11, col22 from table2;");
        QCOMPARE(query, expected);
    }
}

void TInsert::TestInsertFromSelectOnBase()
{
    DEFAULT_SQLITE_BASE("TestInsertFromSelectOnBase.db");

    QHash<int, QString> expected;
    {
        createTable("table1")
                .addColumn("id", ColumnType::Integer())
                .addColumn("value", ColumnType::String(5))
                .exec();

        insert("id", "value")
                .into("table1")
                .values(1, "aaaaa")
                .values(2, "bbbbb")
                .values(3, "ccccc")
                .exec();

        expected.insert(1, "aaaaa");
        expected.insert(2, "bbbbb");
        expected.insert(3, "ccccc");
    }

    {
        createTable("table2")
                .addColumn("uid", ColumnType::Integer())
                .addColumn("val", ColumnType::String(5))
                .exec();

        insert("uid", "val")
                .into("table2")
                .from(select("id", "value").from("table1"))
                .exec();
    }

    {
        const QueryResult result = select("uid", "val").from("table2").exec();
        QVERIFY(result.error().isEmpty());

        QHash<int, QString> actual;
        while(result.next())
        {
            actual.insert(result.value("uid").toInt(), result.value("val").toString());
        }
        QCOMPARE(actual, expected);
    }
    CHECK_DB_ERRORS();
}

void TInsert::TestInsertWithBind()
{
    DEFAULT_SQLITE_BASE("TestInsertWithBind.db");

    QHash<int, QString> expected;
    expected.insert(1, "aaaaa");
    expected.insert(2, "bbbbb");
    expected.insert(3, "ccccc");
    {
        createTable("table1")
                .addColumn("id", ColumnType::Integer())
                .addColumn("value", ColumnType::String(5))
                .exec();

        InsertQuery query = insert("id", "value")
                .into("table1")
                .values(bind("id"), bind("value"))
                .prepare();

        for(auto iter = expected.constBegin(); iter != expected.constEnd(); ++iter)
        {
            query.bind("id", iter.key()).bind("value", iter.value()).exec();
        }
    }

    {
        const QueryResult result =  select("id", "value").from("table1").exec();
        QVERIFY(result.error().isEmpty());
        QHash<int, QString> actual;
        while(result.next())
        {
            actual.insert(result.value("id").toInt(), result.value("value").toString());
        }
        QCOMPARE(actual, expected);
    }
    CHECK_DB_ERRORS();
}

void TInsert::TestInsertWithHalfBind()
{
    DEFAULT_SQLITE_BASE("TestInsertWithHalfBind.db");

    const QSet<int> expected{1,2,3,4};
    {
        createTable("table1")
                .addColumn("id", ColumnType::Integer())
                .addColumn("value", ColumnType::String(5))
                .exec();

        InsertQuery query = insert("id", "value")
                .into("table1")
                .values(bind("id"), "aaaaa")
                .prepare();

        foreach (const int &val, expected)
        {
            query.bind("id", val).exec();
        }
    }

    {
        const QueryResult result =  select("id", "value").from("table1").exec();
        QVERIFY(result.error().isEmpty());
        QHash<int, QString> actual;
        while(result.next())
        {
            actual.insert(result.value("id").toInt(), result.value("value").toString());
        }
        QCOMPARE(actual.keys().toSet(), expected);
        QCOMPARE(actual.values().toSet(), QSet<QString>{"aaaaa"});
    }
    CHECK_DB_ERRORS();
}
