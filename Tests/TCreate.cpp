#include "TCreate.h"

//#include "Utils.h"
#include "StGenGlobal.h"

TCreate::TCreate(QObject *parent) : QObject(parent)
{

}

void TCreate::TestSimpleCreateTable()
{
    using namespace StGenGlobal;

//    {
//        const QString query = createTable("table1")
//                .addColumn("id", ColumnType::Int())
//                .addColumn("value", ColumnType::String(50))
//                .toQueryString();

//        const QString expected("create table table1(id integer, value varchar(50));");
//        QCOMPARE(query, expected);
//    }

//    {
//        const QString query = createTable("table1")
//                .addColumn(TableColumn("id", ColumnType::Int())
//                           .addPrimaryKey())
//                .addColumn(TableColumn("value", ColumnType::String(50))
//                           .addNotNull())
//                .toQueryString();

//        const QString expected("create table table1(id integer primary key, value varchar(50) not null);");
//        QCOMPARE(query, expected);
//    }

}
