#include "Utils.h"


TestBase::TestBase(DataBaseInterface *base)
    : base_(base)
{
    base_->close();
    base_->remove();
    base_->create();
    base_->open();
}

TestBase::~TestBase()
{
    base_->close();
    base_->remove();
}

DataBaseInterface *TestBase::operator ->()
{
    return base_;
}
