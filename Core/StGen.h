#ifndef STGEN_H
#define STGEN_H

#include "Query.h"

class AbstractSqlBuilder
{
public:
    AbstractSqlBuilder(AbstractDataBaseInterface* base);
    virtual SelectQuery select(const ColumnsQuery &columns = ColumnsQuery()) = 0;
    virtual CreateTableQuery createTable(QString name) = 0;


protected:
    AbstractDataBaseInterface* base_;
};

class SqliteQueryBuilder : public AbstractSqlBuilder
{
public:
    SqliteQueryBuilder(SqliteInterface *base);
    virtual SelectQuery select(const ColumnsQuery &columns = ColumnsQuery()) override;
    virtual CreateTableQuery createTable(QString name) override;

    template <typename... Args>
    SelectQuery select(Args ... args)
    {
        return select(ColumnsQuery(conv(args...)));
    }
};

using AbstractBuilder = QSharedPointer<AbstractSqlBuilder>;
using SqliteBuilder = QSharedPointer<SqliteQueryBuilder>;

class StGen
{
public:
    static SqliteBuilder createSqlBuilder(SqliteInterface *interface);
};

#endif // STGEN_H
