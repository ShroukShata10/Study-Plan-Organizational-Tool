#pragma once
#include "Actions\\Action.h"
//Class responsible for Dropping course action
class ActionDropCourse : public Action
{
public:
	ActionDropCourse(Registrar*);
	bool virtual Execute();
	virtual ~ActionDropCourse();
};
