#pragma once
#include "Actions\\Action.h"
//Class responsible for adding course action
class Sel_CRS_Status:
	public Action
{
public:
	Sel_CRS_Status(Registrar*);
	bool virtual Execute();
	virtual ~Sel_CRS_Status();
};