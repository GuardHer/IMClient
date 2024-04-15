#pragma once
#include <QString>
#include "manager/User.h"

class Self
{
public:
	Self() = default;
	~Self() = default;

public:
	void setSelf(User* self) { self_.reset(self); }
	User* getSelf() const { return self_.get(); }
	QString getSelfId() const { return self_->getUserId(); }

	
private:
	std::shared_ptr<User> self_;

};

