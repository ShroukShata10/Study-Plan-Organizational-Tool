#include "Course_Info.h"
#include "Registrar.h"
#include "Courses/UnivCourse.h"
#include <string>
Course_Info::Course_Info(Registrar* p) :Action(p)
{
}
bool Course_Info::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("click on the course you want to get its Information");
	int x, y, x2, y2;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		x = actData.x;
		y = actData.y;
		YearSem yearsem = pGUI->GetYearSem(x, y);
		graphicsInfo GI = pReg->getnewpostion(x, y);
		StudyPlan* pS = pReg->getStudyPlan();
		Course* pC = pS->getpCRS(GI.x, GI.y, yearsem.year, yearsem.sem);
		if (pC != NULL)
		{
			pC->setSelected(1);
			pC->DrawMe(pGUI);
			string Corequistes = "";
			list<Course_Code>* CoReq = pC->getCoReq();
			for (auto i = CoReq->begin(); i != CoReq->end(); i++)
			{
				if (i != CoReq->begin())
					Corequistes += " and ";
				Corequistes += *i;
			}
				
			string Prerequistes = "";
			list<Course_Code>* PreReq = pC->getPreReq();
			for (auto i = PreReq->begin(); i != PreReq->end(); i++)
			{
				if (i != PreReq->begin())
					Prerequistes += " and ";
				Prerequistes += *i;
			}
				
			string Status;
			switch (pC->getStatus())
			{
			case 0: Status = "Done";
				break;
			case 1: Status = "InProgress";
				break;
			case 2: Status = "Pending";
				break;
			case 3: Status = "Replaced";
				break;
			}
			string Info = "Course Title: " + pC->getTitle() + ", Course Code: " + pC->getCode() +
				", Course Credit Hours: " + to_string(pC->getCredits()) + ", Corequisites: " + Corequistes +
				", Prerequistes: " + Prerequistes + ", Course Status: " + Status;
			pGUI->GetUserAction(Info);
			pC->setSelected(0);
		}
	}
	return true;
};

Course_Info::~Course_Info()
{
}