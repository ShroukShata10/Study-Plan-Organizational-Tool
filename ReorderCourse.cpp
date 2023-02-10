#include "ReorderCourse.h"
#include "Registrar.h"
#include "Courses\\Course.h"
#include "Actions\\Action.h"
#include <iostream>
ReorderCourse ::ReorderCourse(Registrar* p) :Action(p)
{
}
bool ReorderCourse::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("click on the course you want to drag");
	int x, y,x2,y2;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		x = actData.x;
		y = actData.y;
		YearSem yearsem = pGUI->GetYearSem(x,y);
		graphicsInfo GI = pReg->getnewpostion(x, y);
		StudyPlan* pS = pReg->getStudyPlan();
		Course* pC = pS->getpCRS(GI.x, GI.y, yearsem.year, yearsem.sem);
		if(pC!=NULL)
		{ 
			Course_Code code = pC->getCode();
		    pC->setSelected(1);
		    pC->DrawMe(pGUI);
		    graphicsInfo a = pS->DropCourse(GI.x, GI.y,yearsem.year,yearsem.sem);
		    ActionData actData = pGUI->GetUserAction("click on the position you want to drop course in");
		    if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
		    {
			   x2 = actData.x;
			   y2 = actData.y;
			   graphicsInfo gInfo = pReg->getnewpostion(x2, y2);
			   YearSem yearsem2 = pGUI->GetYearSem(x2,y2);
			   CourseInfo CRS_Info = pReg->getCourseInfo(code);
			   string Title = CRS_Info.Title;
			   int crd = CRS_Info.Credits;
			   Course* pC = new Course(code, Title, crd);
			   pC->setGfxInfo(gInfo);
			   pC->setyearsem(yearsem2.year, yearsem2.sem);
			   pC->settype(pReg->coursetype(code));
			   for (auto j = CRS_Info.CoReqList.begin(); j < CRS_Info.CoReqList.end(); j++)
				   pC->addCoReq(*j);
			   for (auto i = CRS_Info.PreReqList.begin(); i < CRS_Info.PreReqList.end(); i++)
				   pC->addPreReq(*i);
			   pS->AddCourse(pC, yearsem2.year, yearsem2.sem);
		    }
		}
	}
	return true;
}
ReorderCourse :: ~ReorderCourse()
{

}