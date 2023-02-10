#include "ReplaceCourse.h"
#include "Registrar.h"
#include "Courses\\Course.h"
#include <iostream>
ReplaceCourse::ReplaceCourse(Registrar* p) :Action(p)
{
}

bool ReplaceCourse::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("Select the course you want to replace");
	
	int x, y;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		graphicsInfo gInfo = pReg->getnewpostion(x, y);
		YearSem yearsem = pGUI->GetYearSem(x,y);
		StudyPlan* pS = pReg->getStudyPlan();
		Course* oldpC = pS->getpCRS(gInfo.x, gInfo.y, yearsem.year, yearsem.sem);
		oldpC->setSelected(1);
		oldpC->DrawMe(pGUI);
		pGUI->PrintMsg("Replace Course: Enter new course Code(e.g. CIE202):");
		Course_Code code = pGUI->GetSrting();
		CourseInfo CRS_Info = pReg->getCourseInfo(code);
		if (CRS_Info.Credits != 0)
		{
			graphicsInfo a = pS->DropCourse(gInfo.x, gInfo.y, yearsem.year, yearsem.sem);
			string Title = CRS_Info.Title;
			int crd = CRS_Info.Credits;
			Course* pC = new Course(code, Title, crd);
			pC->setGfxInfo(gInfo);
			pC->setyearsem(yearsem.year, yearsem.sem);
			pC->settype(pReg->coursetype(code));
			for (auto j = CRS_Info.CoReqList.begin(); j < CRS_Info.CoReqList.end(); j++)
				pC->addCoReq(*j);
			for (auto i = CRS_Info.PreReqList.begin(); i < CRS_Info.PreReqList.end(); i++)
				pC->addPreReq(*i);
			pS->AddCourse(pC, yearsem.year, yearsem.sem);
		}
		else
			pGUI->PrintMsg("Wrong Course Code!!");
		

	}
	
	return true;
}
ReplaceCourse::~ReplaceCourse()
{
}
