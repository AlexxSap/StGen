#include "TBase.h"

#include "Utils.h"

TBase::TBase(QObject *parent)
    : QObject(parent)
{

}

void TBase::TestSimpleSelect()
{
    const QString dataBaseName("TestSimpleSelect.db");
    SqliteInterface b(DataBaseSettings(dataBaseName), false);
    TestBase base(&b);

    QHash<int, QString> expected;
    expected.insert(1, "value1");
    expected.insert(2, "value2");

    SqlQuery query = base->query();
    query->exec("create table table1(id integer, value varchar(50));");
    query->exec("insert into table1 values(1, 'value1');");
    query->exec("insert into table1 values(2, 'value2');");

    query->exec("select * from table1");
    QHash<int, QString> actual;
    while(query->next())
    {
        actual.insert(query->value("id").toInt(), query->value("value").toString());
    }
    QCOMPARE(actual, expected);
}
