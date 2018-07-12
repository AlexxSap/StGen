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
        orE,
        in
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

class InExpression : public AbstractExpression
{
public:
    InExpression(QString name,
                 QVariantList values,
                 bool isNot = false);

    virtual QString toQueryString() const override;
    virtual bool isEmpty() const override;

private:
    QString name_;
    QStringList values_;
    bool isNot_ = false;
};

using InExprPointer = QSharedPointer<InExpression>;

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

template <class Q>
class WhereableQuery : public AbstractQuery
{
public:
    Q& where(WhereCase whereCond)
    {
        whereExpr_ = std::move(whereCond);
        return *static_cast<Q*>(this);
    }

    QString toQueryString() const override
    {
        return whereExpr_.toQueryString();
    }

private:
    Q* sender_;
    WhereCase whereExpr_;
};

class NamedTableQuery : public AbstractQuery
{
public:
    NamedTableQuery(QString name = QString());
    void setName(QString name);
    bool isEmpty() const;
    QString toQueryString() const override;

private:
    QString name_;
};

class HavingCase : public CommonCase
{
public:
    HavingCase();
    HavingCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
};

class OnCase : public CommonCase
{
public:
    OnCase();
    OnCase(AbsExprPointer expr);
    virtual QString toQueryString() const override;
};

template <class Q>
class ExecutableQuery : public AbstractQuery
{
public:
    ExecutableQuery(AbstractDataBaseInterface* base)
        : base_(base)
    {

    }
    QueryResult exec()
    {
        if(query_.isNull())
        {
            prepare();
        }

        query_->exec();
        base_->checkError(query_);
        return QueryResult(query_);
    }

    SqlQuery query() const
    {
        return base_->query();
    }

    Q& prepare()
    {
        query_ = this->query();
        query_->prepare(toQueryString());
        base_->checkError(query_);

        return *static_cast<Q*>(this);
    }

protected:
    AbstractDataBaseInterface* base_;
    SqlQuery query_;
};

template <class Q>
class BindedExecutableQuery : public ExecutableQuery<Q>
{
public:
    BindedExecutableQuery(AbstractDataBaseInterface* base)
        : ExecutableQuery<Q>(base)
    {

    }

    Q& bind(const QString& id,
            const QVariant& value)
    {
        ExecutableQuery<Q>::query_->bindValue(":" + id, value);
        return *static_cast<Q*>(this);
    }
};

class SelectQuery;
class JoinQuery : public AbstractQuery
{
public:
    enum Type
    {
        Inner,
        Left,
        Right
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

class Group : public AbstractQuery
{
public:
    void set(QStringList columns);
    virtual QString toQueryString() const;

private:
    QStringList columns_;
};

class Order : public AbstractQuery
{
public:
    enum Type
    {
        None,
        Asc,
        Desc
    };

    Order();
    void set(QStringList columns,
             Type t);
    virtual QString toQueryString() const;

private:
    QStringList columns_;
    Type type_;
};

class SelectQuery : public BindedExecutableQuery<SelectQuery>,
        public WhereableQuery<SelectQuery>,
        public NamedTableQuery
{
public:
    SelectQuery();
    SelectQuery(AbstractDataBaseInterface* base,
                ColumnsQuery columns);
    SelectQuery& from(QString table);
    SelectQuery& having(HavingCase havingCond);

    SelectQuery& orderBy(QStringList columns, Order::Type type = Order::None);
    SelectQuery& orderBy(QString column, Order::Type type = Order::None);

    SelectQuery& groupBy(QStringList columns);
    SelectQuery& groupBy(QString column);

    bool isEmpty() const;

    virtual QString toQueryString() const override;

    JoinQuery& innerJoin(QString tableName);

private:
    ColumnsQuery columns_;
    JoinQuery joinQuery_;
    Group group_;
    Order order_;
    HavingCase havingExpr_;
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

class UpdateQuery : public ExecutableQuery<UpdateQuery>,
        public WhereableQuery<UpdateQuery>,
        public NamedTableQuery
{
public:
    UpdateQuery(AbstractDataBaseInterface* base,
                QString tableName);

    UpdateQuery& set(QString colName, QVariant value);
    QString toQueryString() const override;

private:
    QList<QPair<QString, QVariant>> columns_;
};

class DeleteFromTableQuery : public ExecutableQuery<DeleteFromTableQuery>,
        public WhereableQuery<DeleteFromTableQuery>,
        public NamedTableQuery
{
public:
    DeleteFromTableQuery(AbstractDataBaseInterface* base,
                QString table);
    QString toQueryString() const override;
};

class InsertQuery : public BindedExecutableQuery<InsertQuery>,
        public NamedTableQuery
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

private:
    ColumnsQuery columns_;
    Values values_;
    SelectQuery selectQuery_;
};

class CreateTableQuery : public ExecutableQuery<CreateTableQuery>,
        public NamedTableQuery
{
public:
    CreateTableQuery(AbstractDataBaseInterface* base,
                     QString name);
    CreateTableQuery& addColumn(QString name,
                                QString type,
                                Flags flags = Flags());
    CreateTableQuery& addColumn(TableColumn column);

    template <typename... Args>
    CreateTableQuery& setPrimaryKey(Args ... args)
    {
        primary_ = conv(args...);
        return *this;
    }

    virtual QString toQueryString() const override;

private:
    QList<TableColumn> columns_;
    QStringList primary_;
};


#endif // QUERY_H
