#pragma once
#include "Actions\\Action.h"
class ActionCalculateGPA : public Action
{
public:
	ActionCalculateGPA(Registrar*);
	bool virtual Execute();
	virtual ~ActionCalculateGPA();
};
