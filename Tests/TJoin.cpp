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
