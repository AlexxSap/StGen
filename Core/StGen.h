#ifndef STGEN_H
#define STGEN_H

#include "stable.h"
#include "DataBaseInterface.h"
#include "Query.h"

class AbstractSqlBuilder
{
public:
    AbstractSqlBuilder(AbstractDataBaseInterface* base);

    virtual SelectQuery selectQuery(ColumnsQuery columns) = 0;

    template <typename... Args>
    SelectQuery select(Args&& ... args)
    {
        return selectQuery(ColumnsQuery(conv(args...)));
    }

protected:
    AbstractDataBaseInterface* base_;
};

class SqliteQueryBuilder : public AbstractSqlBuilder
{
public:
    SqliteQueryBuilder(SqliteInterface *base);
    virtual SelectQuery selectQuery(ColumnsQuery columns = ColumnsQuery()) override;

};

using SqliteBuilder = QSharedPointer<SqliteQueryBuilder>;

class StGen
{
public:
    static SqliteBuilder createSqlBuilder(SqliteInterface *interface);
};

#endif // STGEN_H
