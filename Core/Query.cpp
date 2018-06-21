#include "Query.h"


AbstractQuery::~AbstractQuery()
{

}

ColumnsQuery::ColumnsQuery()
    : AbstractQuery()
{

}

ColumnsQuery::ColumnsQuery(const QStringList &columns)
    : AbstractQuery(),
      columns_(columns)
{
}

QString ColumnsQuery::toQueryString() const
{
    return columns_.isEmpty() ? "*" : columns_.join(", ");
}

FromQuery::FromQuery()
    : AbstractQuery()
{

}

FromQuery::FromQuery(const QString &tableName)
    : AbstractQuery(),
      table_(tableName)
{

}

FromQuery::FromQuery(const char *tableName)
    : AbstractQuery(),
      table_(tableName)
{

}

QString FromQuery::toQueryString() const
{
    return QString("from %1").arg(table_);
}

SelectQuery::SelectQuery(AbstractDataBaseInterface *base,
                         ColumnsQuery columns)
    : AbstractExecuteQuery(base),
      columns_(std::move(columns))
{

}

SelectQuery &SelectQuery::from(FromQuery table)
{
    from_ = std::move(table);
    return *this;
}

SelectQuery &SelectQuery::where(WhereCase whereCond)
{
    whereExpr_ = std::move(whereCond);
    return *this;
}

QueryResult SelectQuery::exec()
{
    SqlQuery query = this->query();
    query->prepare(toQueryString());
    query->exec();
    return QueryResult(query);
}

QString SelectQuery::toQueryString() const
{
    QString result = "select "
            + columns_.toQueryString()
            + " "
            + from_.toQueryString();

    if(!whereExpr_.isEmpty())
    {
        result += " where "
                + whereExpr_.toQueryString();
    }

    return result+ ";";
}

AbstractExecuteQuery::AbstractExecuteQuery(AbstractDataBaseInterface *base)
    : base_(base)
{

}

SqlQuery AbstractExecuteQuery::query() const
{
    return base_->query();
}

WhereCase::WhereCase()
{

}

WhereCase::WhereCase(Expr expr)
    : expr_(expr)
{

}

QString WhereCase::toQueryString() const
{
    if(expr_.isNull())
    {
        return QString();
    }
    return expr_->toQueryString();
}

bool WhereCase::isEmpty() const
{
    return expr_.isNull() || expr_->isEmpty();
}

ValueExpression::ValueExpression(QVariant value)
    : value_(std::move(value))
{

}

QString ValueExpression::toQueryString() const
{
    if(value_.type() == QVariant::Date)
    {
        return "'" + value_.toDate().toString("yyyy-MM-dd") + "'";
    }
    return value_.toString();
}

bool ValueExpression::isEmpty() const
{
    return value_.isNull();
}

EqualExpression::EqualExpression(QVariant a, QVariant b)
    : a_(ValueExpr::create(a)),
      b_(ValueExpr::create(b))
{

}

QString EqualExpression::toQueryString() const
{
    return a_->toQueryString() + " = " + b_->toQueryString();
}

bool EqualExpression::isEmpty() const
{
    return a_.isNull() || b_.isNull();
}
