#ifndef QUERY_H
#define QUERY_H

#include "stable.h"
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

class AbstractExecuteQuery : public AbstractQuery
{
public:
    AbstractExecuteQuery(AbstractDataBaseInterface* base);
    virtual QueryResult exec() = 0;

    SqlQuery query() const;

private:
    AbstractDataBaseInterface* base_;
};

class SelectQuery : public AbstractExecuteQuery
{
public:
    SelectQuery(AbstractDataBaseInterface* base,
                ColumnsQuery columns);
    SelectQuery& from(FromQuery table);

    virtual QueryResult exec() override;
    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
};


#endif // QUERY_H
