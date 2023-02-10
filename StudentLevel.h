#pragma once
#include "Actions\Action.h"
class StudentLevel : public Action
{
public:
	StudentLevel(Registrar*);
	bool virtual Execute();
	virtual ~StudentLevel();
};