#include "Utils.h"


TestBase::TestBase(DataBaseInterface *base, const QString &name)
    : base_(base),
      name_(name)
{
    base_->remove(name_);
    base_->create(name_);
    base_->open(name_);
}

TestBase::~TestBase()
{
    base_->remove(name_);
}

DataBaseInterface *TestBase::operator ->()
{
    return base_;
}
