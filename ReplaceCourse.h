#pragma once
#include "Actions\\Action.h"
class ReplaceCourse :
	public Action
{
public:
	ReplaceCourse(Registrar*);
	bool virtual Execute();
	virtual ~ReplaceCourse();
};