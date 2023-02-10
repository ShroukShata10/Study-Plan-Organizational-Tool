#pragma once
#include "Actions\\Action.h"
class ActionDoubleMajor : public Action
{
public:
	ActionDoubleMajor(Registrar*);
	bool virtual Execute();
	virtual ~ActionDoubleMajor();
};
