#include "TDelete.h"

#include "Utils.h"
#include "StGenGlobal.h"

TDelete::TDelete(QObject *parent) : QObject(parent)
{

}

void TDelete::TestSimpleDelete()
{
    DEFAULT_NULL_CONNECTION();
    {
        const QString query = deleteFrom("table1")
                .toQueryString();

        const QString expected("delete from table1;");
        QCOMPARE(query, expected);
    }

}

void TDelete::TestSimpleDeleteWithWhere()
{
    DEFAULT_NULL_CONNECTION();
    {
        const QString query = deleteFrom("table1").where(in("id", {1,2,3}))
                .toQueryString();

        const QString expected("delete from table1 where id in (1, 2, 3);");
        QCOMPARE(query, expected);
    }
}

void TDelete::TestSimpleDeleteFromBase()
{
    DEFAULT_SQLITE_BASE("TestSimpleDeleteFromBase.db");

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
        deleteFrom("table1").where(in("id", {1,3})).exec();
    }

    {
        const QueryResult result = select("value").from("table1").exec();
        QVERIFY(result.error().isEmpty());
        QVERIFY(result.next());
        QCOMPARE(result.value(0).toString(), "bbbbb");
        QVERIFY(!result.next());
    }
}
