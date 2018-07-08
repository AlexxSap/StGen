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

    int size() const;

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
    bool isEmpty() const;

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

class CommonCase : public AbstractQuery
{
public:
    CommonCase();
    CommonCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
    bool isEmpty() const;

private:
    AbsExprPointer expr_;
};

class WhereCase : public CommonCase
{
public:
    WhereCase();
    WhereCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
};

class OnCase : public CommonCase
{
public:
    OnCase();
    OnCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
};

class AbstractExecuteQuery : public AbstractQuery
{
public:
    AbstractExecuteQuery(AbstractDataBaseInterface* base);
    QueryResult exec();
    SqlQuery query() const;

protected:
    void prepare();

protected:
    AbstractDataBaseInterface* base_;
    SqlQuery query_;
};

class SelectQuery;
class JoinQuery : public AbstractQuery
{
public:
    enum Type
    {
        Inner
    };

    JoinQuery();
    JoinQuery(Type type,
              QString table,
              SelectQuery *selectQuery);
    SelectQuery& on(OnCase expr);

    void set(Type type,
             QString table,
             SelectQuery *selectQuery);

    bool isEmpty() const;

    virtual QString toQueryString() const override;

private:
    QString typeToStr() const;

private:
    Type type_ = Inner;
    QString table_;
    SelectQuery *selectQuery_ = nullptr;
    OnCase case_;
};

class SelectQuery : public AbstractExecuteQuery
{
public:
    SelectQuery();
    SelectQuery(AbstractDataBaseInterface* base,
                ColumnsQuery columns);
    SelectQuery& from(FromQuery table);
    SelectQuery& where(WhereCase whereCond);
    SelectQuery& prepare();

    SelectQuery& bind(const QString &id,
                      const QVariant& value);
    bool isEmpty() const;

    virtual QString toQueryString() const override;

    JoinQuery& innerJoin(QString tableName);

private:
    ColumnsQuery columns_;
    FromQuery from_;
    WhereCase whereExpr_;
    JoinQuery joinQuery_;
};

class ColumnType
{
public:
    static QString Integer();
    static QString String(const int value);
    static QString Date();
};

using Flags = QStringList;

class TableColumn : public AbstractQuery
{
public:
    TableColumn(QString name,
                QString type,
                Flags flags = Flags());

    virtual QString toQueryString() const override;

private:
    QString name_;
    QString type_;
    Flags flags_;
};

class Values : public AbstractQuery
{
public:
    Values& operator <<(QVariantList values);
    virtual QString toQueryString() const override;

    bool isEmpty() const;

private:
    QList<QVariantList> values_;
};

class InsertQuery : public AbstractExecuteQuery
{
public:
    InsertQuery(AbstractDataBaseInterface* base,
                ColumnsQuery columns);
    InsertQuery& into(QString tableName);

    template <typename... Args>
    InsertQuery& values(Args ... args)
    {
        const QVariantList lst(convVar(args...));
        if(lst.size() == columns_.size())
        {
            values_ << std::move(lst);
        }
        else
        {
            qWarning() << "values size != columns size";
        }

        return *this;
    }

    InsertQuery& values(Values values);
    InsertQuery& from(SelectQuery selectQuery);
    virtual QString toQueryString() const override;

    InsertQuery& prepare();

    InsertQuery& bind(const QString &id,
                      const QVariant& value);

private:
    ColumnsQuery columns_;
    QString tableName_;
    Values values_;
    SelectQuery selectQuery_;
};

class CreateTableQuery : public AbstractExecuteQuery
{
public:
    CreateTableQuery(AbstractDataBaseInterface* base,
                     QString name);
    CreateTableQuery& addColumn(QString name,
                                QString type,
                                Flags flags = Flags());
    CreateTableQuery& addColumn(TableColumn column);
    CreateTableQuery& prepare();

    template <typename... Args>
    CreateTableQuery& setPrimaryKey(Args ... args)
    {
        primary_ = conv(args...);
        return *this;
    }


    virtual QString toQueryString() const override;

private:
    QString tableName_;
    QList<TableColumn> columns_;
    QStringList primary_;
};


#endif // QUERY_H
