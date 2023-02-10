#pragma once
#include "Actions\\Action.h"
class ActionAddGrade : public Action
{
public:
	ActionAddGrade(Registrar*);
	bool virtual Execute();
	virtual ~ActionAddGrade();
};