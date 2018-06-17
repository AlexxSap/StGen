#include "StGen.h"

#include <QDebug>



SqlBuilder StGen::createSqlBuilder()
{
    return SqlBuilder::create();
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

QString SelectQuery::toQueryString() const
{
    return QString("select ")
            + columns_.toQueryString()
            + " "
            + from_.toQueryString()
            + ";";
}

SqlQueryBuilder::SqlQueryBuilder()
    : AbstractSqlBuilder()
{

}

SelectQuery SqlQueryBuilder::select(ColumnsQuery columns)
{
    return SelectQuery(columns);
}

AbstractQuery::~AbstractQuery()
{

}
