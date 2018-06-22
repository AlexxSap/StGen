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

class AbstractExpression : public AbstractQuery
{
public:
    virtual bool isEmpty() const = 0;

    enum class Operation
    {
        Equal,
        Not_Equal,
        Less,
        Less_Equal,
        More,
        More_Equal
    };

protected:
    QString operationToString(const Operation& type) const;
};

using AbsExpr = QSharedPointer<AbstractExpression>;

class ValueExpression : public AbstractExpression
{
public:
    ValueExpression(QVariant value);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    QVariant value_;
};

using ValueExpr = QSharedPointer<ValueExpression>;

class Expression : public AbstractExpression
{
public:
    Expression(AbstractExpression::Operation type, QVariant a, QVariant b);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    AbstractExpression::Operation type_;
    AbsExpr a_;
    AbsExpr b_;
};

using Expr = QSharedPointer<Expression>;

class WhereCase : public AbstractQuery
{
public:
    WhereCase();
    WhereCase(AbsExpr expr);
    virtual QString toQueryString() const override;
    bool isEmpty() const;

private:
    AbsExpr expr_;

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
