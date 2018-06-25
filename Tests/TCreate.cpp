#include "TCreate.h"

#include "Utils.h"
#include "StGenGlobal.h"

TCreate::TCreate(QObject *parent) : QObject(parent)
{

}

void TCreate::TestSimpleCreateTable()
{
    StGenGlobal::setBuilder(StGen::createSqlBuilder(nullptr));
    using namespace StGenGlobal;

    {
        const QString query = createTable("table1")
                .addColumn("id", ColumnType::Integer())
                .addColumn("value", ColumnType::String(50))
                .toQueryString();

        const QString expected("create if exists table table1 (id integer, value varchar(50));");
        QCOMPARE(query, expected);
    }

    {
        const QString query = createTable("table1")
                .addColumn(TableColumn("id", ColumnType::Integer()))
                .addColumn(TableColumn("value", ColumnType::String(50)))
                .toQueryString();

        const QString expected("create if exists table table1 (id integer, value varchar(50));");
        QCOMPARE(query, expected);
    }

}
