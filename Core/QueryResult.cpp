#include "QueryResult.h"

#include <QSqlError>

QueryResult::QueryResult(SqlQuery query)
    : query_(std::move(query))
{

}

bool QueryResult::next() const
{
    return query_->next();
}

QVariant QueryResult::value(const QString &name) const
{
    return query_->value(name);
}

QVariant QueryResult::value(const int index) const
{
    return query_->value(index);
}

bool QueryResult::hasError() const
{
    return query_->lastError().type() != QSqlError::NoError;
}

QString QueryResult::error() const
{
    return query_->lastError().text();
}
