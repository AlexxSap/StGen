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

class Expression : public AbstractQuery
{
public:
    virtual bool isEmpty() const = 0;
};

using Expr = QSharedPointer<Expression>;

class ValueExpression : public Expression
{
public:
    ValueExpression(QVariant value);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    QVariant value_;
};

using ValueExpr = QSharedPointer<ValueExpression>;

class EqualExpression : public Expression
{
public:
    EqualExpression(QVariant a, QVariant b);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;
private:
    Expr a_;
    Expr b_;
};

using EqualExpr = QSharedPointer<EqualExpression>;

class WhereCase : public AbstractQuery
{
public:
    WhereCase();
    WhereCase(Expr expr);
    virtual QString toQueryString() const override;
    bool isEmpty() const;

private:
    Expr expr_;

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
    SelectQuery& where(WhereCase whereCond);

    virtual QueryResult exec() override;
    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
    WhereCase whereExpr_;
};


#endif // QUERY_H
