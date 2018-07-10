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
