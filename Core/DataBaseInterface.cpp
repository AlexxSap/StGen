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

bool DataBaseInterface::open(const DataBaseName &dataBaseName)
{
    db_ = QSqlDatabase::addDatabase(driverID());
    db_.setHostName(settings_.host);
    db_.setDatabaseName(dataBaseName);

    return db_.open();
}

void DataBaseInterface::close()
{
    db_.close();
}

QSqlQuery DataBaseInterface::query() const
{
    QSqlQuery query(db_);
    query.setForwardOnly(true);

    return query;
}

SqliteInterface::SqliteInterface(const DataBaseSettings &settings,
                                 const bool autoClose)
    : DataBaseInterface(settings, autoClose)
{

}

bool SqliteInterface::create(const DataBaseName &name)
{
    QFile file(name);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.close();

    return true;
}

bool SqliteInterface::remove(const DataBaseName &name)
{
    close();
    return QFile::remove(name);
}

QString SqliteInterface::driverID() const
{
    return "QSQLITE";
}

