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

UpdateQuery SqliteQueryBuilder::update(QString tableName)
{
    return UpdateQuery(base_, std::move(tableName));
}

DeleteFromTableQuery SqliteQueryBuilder::deleteFrom(QString tableName)
{
    return DeleteFromTableQuery(base_, std::move(tableName));
}

AbstractSqlBuilder::AbstractSqlBuilder(AbstractDataBaseInterface *base)
    : base_(base)
{

}
