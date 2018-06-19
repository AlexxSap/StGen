#ifndef QUERY_H
#define QUERY_H

#include <QString>
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
    QueryResult exec() const;

    virtual QString toQueryString() const override;

private:
    QSqlQuery query_;
};

class SelectQuery : public AbstractQuery
{
public:
    SelectQuery(ColumnsQuery columns);
    SelectQuery& from(FromQuery table);
    PreparedQuery prepare() const;
    QueryResult exec() const;

    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
};


#endif // QUERY_H
