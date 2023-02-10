#pragma once
#include "Actions/Action.h"
//Class responsible for adding course action
class Course_Info :
	public Action
{
public:
	Course_Info(Registrar* p);
	bool virtual Execute();
	virtual ~Course_Info();
};

