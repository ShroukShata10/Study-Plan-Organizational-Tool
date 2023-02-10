#include<iostream>
#include"Actions/ActionAddCourse.h"
#include"Courses/Course.h"
#include"StudentLevel.h"
#include"Registrar.h"
using namespace std;
StudentLevel::StudentLevel(Registrar* p) : Action(p)
{
}

bool StudentLevel::Execute()
{
	
	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlan();
	int credits =pS->getstudentlevel();
	if (credits <= 32)
		pGUI->GetUserAction("Foundation Student");
	else if (credits > 32 && credits <= 67) //67:Number of credits of foundation + year 2
		pGUI->GetUserAction("Sophomore Student");
	else if (credits > 67 && credits <= 103) //103:Number of credits of foundation + year 2 + year 3
		pGUI->GetUserAction("Junior Student");
	else if (credits > 103 && credits <= 139) //139:Number of credits of foundation + year 2 + year 3 + year 4
		pGUI->GetUserAction("Pre-Senior Student");
	else
		pGUI->GetUserAction("Senior Student");
	return true;
}

StudentLevel :: ~StudentLevel()
{

}