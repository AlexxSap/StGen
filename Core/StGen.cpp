#include "StGen.h"

#include <QSqlError>

SqliteBuilder StGen::createSqlBuilder(SqliteInterface *interface)
{
    return SqliteBuilder::create(interface);
}

SqliteQueryBuilder::SqliteQueryBuilder(SqliteInterface *base)
    : AbstractSqlBuilder(base)
{
}

SelectQuery SqliteQueryBuilder::select(const ColumnsQuery &columns)
{
    return SelectQuery(base_, columns);
}

CreateTableQuery SqliteQueryBuilder::createTable(QString name)
{
    return CreateTableQuery(base_, std::move(name));
}

InsertQuery SqliteQueryBuilder::insert(const ColumnsQuery &columns)
{
    return InsertQuery(base_, columns);
}

AbstractSqlBuilder::AbstractSqlBuilder(AbstractDataBaseInterface *base)
    : base_(base)
{

}
