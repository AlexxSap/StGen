#include "TUpdate.h"

#include "Utils.h"
#include "StGenGlobal.h"

TUpdate::TUpdate(QObject *parent) : QObject(parent)
{

}

void TUpdate::TestSimpleUpdate()
{
    DEFAULT_NULL_CONNECTION();

    {
        const QString actual = update("table1").set("col1", "val").toQueryString();
        const QString expected = "update table1 set col1 = 'val';";
        QCOMPARE(actual, expected);
    }

    {
        const QString actual = update("table1").set("col1", 33).toQueryString();
        const QString expected = "update table1 set col1 = 33;";
        QCOMPARE(actual, expected);
    }

    {
        const QString actual = update("table1")
                .set("col1", 33)
                .set("col2", "val")
                .set("col3", QDate(2018, 8, 10)).toQueryString();
        const QString expected = "update table1 set col1 = 33, col2 = 'val', col3 = '2018-08-10';";
        QCOMPARE(actual, expected);
    }
}

void TUpdate::TestUpdateWhere()
{
    DEFAULT_NULL_CONNECTION();

    {
        const QString actual = update("table1").set("col1", "val").where(more("id", 13)).toQueryString();
        const QString expected = "update table1 set col1 = 'val' where id > 13;";
        QCOMPARE(actual, expected);
    }
}

void TUpdate::TestUpdateFromBase()
{
    DEFAULT_SQLITE_BASE("TestUpdateFromBase.db");

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
    }

    {
        update("table1").set("value", "fffff").where(equal("id", 2)).exec();
    }

    {
        const QueryResult result = select("value").from("table1").where(equal("id", 2)).exec();
        QVERIFY(result.next());
        QCOMPARE(result.value(0).toString(), "fffff");
    }
}
