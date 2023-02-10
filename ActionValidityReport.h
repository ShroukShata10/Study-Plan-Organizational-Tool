#pragma once
#include "Actions\\Action.h"
class ActionValidityReport : public Action
{
public:
	ActionValidityReport(Registrar*);
	bool virtual Execute();
	virtual ~ActionValidityReport();
};
