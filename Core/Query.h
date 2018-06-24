#ifndef QUERY_H
#define QUERY_H

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
        equal,
        notEqual,
        less,
        lessEqual,
        more,
        moreEqual,
        andE,
        orE
    };

protected:
    QString operationToString(const Operation& type) const;
};

using AbsExprPointer = QSharedPointer<AbstractExpression>;

class ValueExpression : public AbstractExpression
{
public:
    ValueExpression(QVariant value);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    QVariant value_;
};

using ValueExprPointer = QSharedPointer<ValueExpression>;

class Expression : public AbstractExpression
{
public:
    Expression(AbstractExpression::Operation type, QVariant a, QVariant b);
    Expression(AbstractExpression::Operation type,
               AbsExprPointer a,
               AbsExprPointer b);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    AbstractExpression::Operation type_;
    AbsExprPointer a_;
    AbsExprPointer b_;
};

using ExprPointer = QSharedPointer<Expression>;

class WhereCase : public AbstractQuery
{
public:
    WhereCase();
    WhereCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
    bool isEmpty() const;

private:
    AbsExprPointer expr_;

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
    SelectQuery& prepare();

    void bind(const QString &id, const QVariant& value);

    virtual QueryResult exec() override;
    virtual QString toQueryString() const override;

private:
    ColumnsQuery columns_;
    FromQuery from_;
    WhereCase whereExpr_;

    SqlQuery query_;
};

class ColumnType
{
public:
    static QString Integer();
    static QString String(const int value);
};

class TableColumn : public AbstractQuery
{
public:
    TableColumn(QString name, QString type);

    virtual QString toQueryString() const override;

private:
    QString name_;
    QString type_;
};

class CreateTableQuery : public AbstractExecuteQuery
{
public:
    CreateTableQuery(AbstractDataBaseInterface* base,
                     QString name);
    CreateTableQuery& addColumn(QString name, QString type);
    CreateTableQuery& addColumn(TableColumn column);

    virtual QueryResult exec() override;
    virtual QString toQueryString() const override;

private:
    QString tableName_;
    QList<TableColumn> columns_;
};


#endif // QUERY_H
