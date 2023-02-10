#pragma once
#include "Actions\\Action.h"
class ReorderCourse :
	public Action
{
public:
	ReorderCourse(Registrar*);
	bool virtual Execute();
	virtual ~ReorderCourse();
};