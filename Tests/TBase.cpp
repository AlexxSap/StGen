#include "TBase.h"

#include <QDebug>
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

    QSqlQuery query = base->query();
    query.exec("create table table1(id integer, value varchar(50));");
    query.exec("insert into table1 values(1, 'value1');");
    query.exec("insert into table1 values(2, 'value2');");

    query.exec("select * from table1");
    while(query.next())
    {
        qInfo() << query.value(0).toInt() << query.value(1).toString();
    }
}
