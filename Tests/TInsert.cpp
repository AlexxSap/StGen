#include "TInsert.h"

#include "Utils.h"
#include "StGenGlobal.h"

TInsert::TInsert(QObject *parent) : QObject(parent)
{

}

void TInsert::TestSimpleInsert()
{
    StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr));
    using namespace StGenGlobal;

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
}

void TInsert::TestSimpleInsertToBase()
{
    const QString dataBaseName("TestSimpleInsertToBase.db");
    SqliteInterface b(DataBaseSettings(dataBaseName), false);
    TestBase base(&b);

    StGenGlobal::setBuilder(StGen::createSqlBuilder(&b));
    using namespace StGenGlobal;

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
    QHash<int, QString> actual;
    while(result.next())
    {
        actual.insert(result.value("id").toInt(), result.value("value").toString());
    }
    QCOMPARE(actual, expected);
}

void TInsert::TestInsertFromSelect()
{

}
