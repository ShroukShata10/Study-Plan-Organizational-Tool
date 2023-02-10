#pragma once
//#include "Action.h"
#include"Registrar.h"
class ImportStudyPlan :
	public Action
{
public:
	ImportStudyPlan(Registrar*);
	bool virtual Execute();
	virtual ~ImportStudyPlan();
};