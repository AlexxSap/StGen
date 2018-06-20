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

QueryResult SelectQuery::exec()
{
    QSqlQuery query = this->query();
    query.prepare(toQueryString());
    query.exec();
    return QueryResult(query);
}

QString SelectQuery::toQueryString() const
{
    return QString("select ")
            + columns_.toQueryString()
            + " "
            + from_.toQueryString()
            + ";";
}

AbstractExecuteQuery::AbstractExecuteQuery(AbstractDataBaseInterface *base)
    : base_(base)
{

}

QSqlQuery AbstractExecuteQuery::query() const
{
    return base_->query();
}
