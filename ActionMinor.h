#pragma once
#include "Actions\\Action.h"
class ActionMinor : public Action
{
public:
	ActionMinor(Registrar*);
	bool virtual Execute();
	virtual ~ActionMinor();
};
