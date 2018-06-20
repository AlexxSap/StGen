#include "DataBaseInterface.h"

#include <QFile>

DataBaseInterface::DataBaseInterface(const DataBaseSettings &settings,
                                     const bool autoClose)
    : settings_(settings),
      autoClose_(autoClose)
{

}

DataBaseInterface::~DataBaseInterface()
{
    if(autoClose_)
    {
        close();
    }
}

bool DataBaseInterface::open()
{
    if(QSqlDatabase::contains(QSqlDatabase::defaultConnection))
    {
        db_ = QSqlDatabase::database();
    }
    else
    {
        db_ = QSqlDatabase::addDatabase(driverID());
    }

    db_.setHostName(settings_.host);
    db_.setDatabaseName(dataBaseName());

    return db_.open();
}

void DataBaseInterface::close()
{
    db_.close();
}

SqlQuery DataBaseInterface::query() const
{
    SqlQuery query = SqlQuery::create(db_);
    query->setForwardOnly(true);

    return query;
}

QString DataBaseInterface::dataBaseName() const
{
    return settings_.base;
}

Bases DataBaseInterface::type() const
{
    return type_;
}

SqliteInterface::SqliteInterface(const DataBaseSettings &settings,
                                 const bool autoClose)
    : DataBaseInterface(settings, autoClose)
{
    type_ = Bases::SQLITE;
}

bool SqliteInterface::create()
{
    QFile file(dataBaseName());
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.close();

    return true;
}

bool SqliteInterface::remove()
{
    close();
    return QFile::remove(dataBaseName());
}

bool SqliteInterface::exists()
{
    return QFile::exists(dataBaseName());
}

QString SqliteInterface::driverID() const
{
    return "QSQLITE";
}

