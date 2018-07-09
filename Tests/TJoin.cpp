#include "TJoin.h"

#include "Utils.h"
#include "StGenGlobal.h"

TJoin::TJoin(QObject *parent) : QObject(parent)
{

}

void TJoin::TestSimpleInnerJoin()
{
    DEFAULT_NULL_CONNECTION();

    const QString actual = select("col1", "col2")
            .from("table1")
            .innerJoin("table2")
            .on(equal(column("table1", "id"), column("table2", "uid"))).toQueryString();

    const QString expected("select col1, col2 from table1 "
                           "inner join table2 on table1.id = table2.uid;");

    QCOMPARE(actual, expected);
}

void TJoin::TestSimpleInnerJoinFromBase()
{
    DEFAULT_SQLITE_BASE("TestSimpleInnerJoinFromBase.db");

    createTable("table1")
            .addColumn("id", ColumnType::Integer())
            .addColumn("value", ColumnType::String(5))
            .exec();
    insert("id", "value").into("table1")
            .values(1, "aaaaa")
            .values(2, "bbbbb")
            .values(3, "aaaaa")
            .values(4, "ccccc").exec();

    createTable("table2")
            .addColumn("uid", ColumnType::Integer())
            .addColumn("value", ColumnType::String(5))
            .exec();
    insert("uid","value").into("table2")
            .values(1, "11111")
            .values(3, "11111").exec();

    const QueryResult result = select(column("table1", "value", "v1"),
                                      column("table2", "value", "v2"))
            .from("table1")
            .innerJoin("table2").on(equal("id", "uid")).exec();

    while(result.next())
    {
        QCOMPARE(result.value("v1").toString(), "aaaaa");
        QCOMPARE(result.value("v2").toString(), "11111");
    }
}
