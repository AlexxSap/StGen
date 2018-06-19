#include "StGen.h"

#include <QDebug>
#include <QSqlError>



SqliteBuilder StGen::createSqlBuilder(SqliteInterface *interface)
{
    return SqliteBuilder::create(interface);
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

PreparedQuery SelectQuery::prepare()
{
    QSqlQuery query;
    query.prepare(toQueryString());
    return PreparedQuery(query);
}

QueryResult SelectQuery::exec()
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

SqliteQueryBuilder::SqliteQueryBuilder(SqliteInterface *interface)
    : base_(interface)
{
    qInfo() << "create SqliteQueryBuilder";
}

SelectQuery SqliteQueryBuilder::selectQuery(ColumnsQuery columns)
{
    return SelectQuery(columns);
}

AbstractQuery::~AbstractQuery()
{

}


PreparedQuery::PreparedQuery(QSqlQuery query)
    : query_(std::move(query))
{

}

QString PreparedQuery::toQueryString() const
{
    return query_.lastQuery();
}

QueryResult PreparedQuery::exec()
{
    query_.exec();
    return QueryResult(query_);
}

