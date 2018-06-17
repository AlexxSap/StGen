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

SqliteInterface::SqliteInterface(const DataBaseSettings &settings)
    : DataBaseInterface(settings)
{

}

bool SqliteInterface::create(const DataBaseName &name)
{

}

bool SqliteInterface::remove(const DataBaseName &name)
{

}

QString SqliteInterface::driverID() const
{
    return "QSQLITE";
}


