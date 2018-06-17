#ifndef STGEN_H
#define STGEN_H

#include <QSharedPointer>

#include "Common.h"

class AbstractQuery
{
public:
    virtual ~AbstractQuery();
    virtual QString toQueryString() const = 0;
};

class ColumnsQuery : public AbstractQuery
{
public:
    ColumnsQuery();
    ColumnsQuery(const QStringList &columns);

    virtual QString toQueryString() const override;

private:
    QStringList columns_;
};

class FromQuery : public AbstractQuery
{
public:
    FromQuery();
    FromQuery(const QString &tableName);
    FromQuery(const char* tableName);

    virtual QString toQueryString() const override;

private:
    QString table_;
};

class SelectQuery : public AbstractQuery
{
public:
    SelectQuery(ColumnsQuery columns);
    SelectQuery& from(FromQuery table);

    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
};

class AbstractSqlBuilder
{
public:
    virtual SelectQuery select(ColumnsQuery columns = ColumnsQuery()) = 0;

    template <typename... Args>
    SelectQuery selectColumns(Args&& ... args)
    {
        return select(ColumnsQuery(conv(args...)));
    }
};

class SqlQueryBuilder : public AbstractSqlBuilder
{
public:
    SqlQueryBuilder();
    virtual SelectQuery select(ColumnsQuery columns = ColumnsQuery()) override;
};

using SqlBuilder = QSharedPointer<SqlQueryBuilder>;

class StGen
{
public:
    static SqlBuilder createSqlBuilder();
};

#endif // STGEN_H
