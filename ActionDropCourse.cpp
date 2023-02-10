#include "ActionDropCourse.h"
#include "Registrar.h"
#include "Courses\\Course.h"
ActionDropCourse::ActionDropCourse(Registrar* p) :Action(p)
{
}
bool ActionDropCourse::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("Select the course you want to drop");
	int x, y;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{ 
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
	YearSem year_sem = pGUI->GetYearSem(x, y);
	graphicsInfo GI = pReg->getnewpostion(x, y);
	StudyPlan* pS = pReg->getStudyPlan();
	pS->DropCourse(GI.x,GI.y,year_sem.year,year_sem.sem);
	}
	return true;
}
ActionDropCourse::~ActionDropCourse()
{
}