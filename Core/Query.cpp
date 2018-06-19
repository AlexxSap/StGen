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

PreparedQuery::PreparedQuery(QSqlQuery query)
    : query_(std::move(query))
{

}

QString PreparedQuery::toQueryString() const
{
    return query_.lastQuery();
}

QueryResult PreparedQuery::exec() const
{
    query_.exec();
    return QueryResult(query_);
}

SelectQuery::SelectQuery(ColumnsQuery columns)
    : AbstractQuery(),
      columns_(std::move(columns))
{

}

SelectQuery &SelectQuery::from(FromQuery table)
{
    from_ = std::move(table);
    return *this;
}

PreparedQuery SelectQuery::prepare() const
{
    QSqlQuery query;
    query.prepare(toQueryString());
    return PreparedQuery(query);
}

QueryResult SelectQuery::exec() const
{
    QSqlQuery query;
    query.exec(toQueryString());
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
