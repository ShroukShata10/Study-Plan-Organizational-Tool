#pragma once
#include "Actions\\Action.h"
class ActionDoubleConcentration : public Action
{
public:
	ActionDoubleConcentration(Registrar*);
	bool virtual Execute();
	virtual ~ActionDoubleConcentration();
};

