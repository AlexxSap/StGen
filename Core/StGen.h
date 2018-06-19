#ifndef STGEN_H
#define STGEN_H

#include <QSharedPointer>

#include "Common.h"
#include "DataBaseInterface.h"
#include "QueryResult.h"
#include "Query.h"

class AbstractSqlBuilder
{
public:
    virtual SelectQuery selectQuery(ColumnsQuery columns = ColumnsQuery()) = 0;

    template <typename... Args>
    SelectQuery select(Args&& ... args)
    {
        return selectQuery(ColumnsQuery(conv(args...)));
    }
};

class SqliteQueryBuilder : public AbstractSqlBuilder
{
public:
    SqliteQueryBuilder(SqliteInterface *interface);
    virtual SelectQuery selectQuery(ColumnsQuery columns = ColumnsQuery()) override;

private:
    SqliteInterface *base_;
};

using SqliteBuilder = QSharedPointer<SqliteQueryBuilder>;

class StGen
{
public:
    static SqliteBuilder createSqlBuilder(SqliteInterface *interface);
};

#endif // STGEN_H
