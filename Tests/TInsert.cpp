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

void TInsert::TestInsertFromSelect()
{

}
