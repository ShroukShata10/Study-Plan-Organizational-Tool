#pragma once
#include "Actions\\Action.h"
//Class responsible for Dropping course action
class ActionFilter : public Action
{
public:
	ActionFilter(Registrar*);
	bool virtual Execute();
	virtual ~ActionFilter();
};

