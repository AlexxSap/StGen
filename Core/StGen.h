#ifndef STGEN_H
#define STGEN_H

#include <QSharedPointer>

#include "Common.h"
#include "DataBaseInterface.h"
#include "QueryResult.h"

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

class PreparedQuery : public AbstractQuery
{
public:
    PreparedQuery(QSqlQuery query);

     virtual QString toQueryString() const override;

    QueryResult exec();

private:
    QSqlQuery query_;
};

class SelectQuery : public AbstractQuery
{
public:
    SelectQuery(ColumnsQuery columns);
    SelectQuery& from(FromQuery table);

    PreparedQuery prepare();
    QueryResult exec();

    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
};

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
