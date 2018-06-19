#include "StGen.h"

#include <QDebug>
#include <QSqlError>

SqliteBuilder StGen::createSqlBuilder(SqliteInterface *interface)
{
    return SqliteBuilder::create(interface);
}

SqliteQueryBuilder::SqliteQueryBuilder(SqliteInterface *interface)
    : base_(interface)
{
    qInfo() << "create SqliteQueryBuilder";
}

SelectQuery SqliteQueryBuilder::selectQuery(ColumnsQuery columns)
{
    return SelectQuery(columns);
}
