#include "ActionDropCourse.h"
#include "Registrar.h"
#include "Courses\\Course.h"
#include <iostream>
class AddNote :
	public Action
{
public:
	AddNote(Registrar*);
	bool virtual Execute();
	virtual ~AddNote();
};


