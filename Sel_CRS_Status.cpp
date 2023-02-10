#include"Sel_CRS_Status.h"
#include "Registrar.h"
#include "Courses\\Course.h"
Sel_CRS_Status::Sel_CRS_Status(Registrar* p) :Action(p)
{
}

bool Sel_CRS_Status::Execute()
{
	GUI* pGUI = pReg->getGUI();
    ActionData actData = pGUI->GetUserAction("Select Course you want to change its status: ");
	int x, y,x2,y2;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		CourseStatus status=Done;
		YearSem yearsem = pGUI->GetYearSem(x, y);
        graphicsInfo gInfo = pReg->getnewpostion(x, y);
		StudyPlan* pS = pReg->getStudyPlan();
		Course* pC = pS->getpCRS(gInfo.x, gInfo.y, yearsem.year, yearsem.sem);
		if (pC != NULL)
		{
			pC->setSelected(1);
			pC->DrawMe(pGUI);
			pGUI->DrawStatus();
			ActionData actData = pGUI->GetUserAction("Select one of the available Status by clicking on it");
			if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
			{
				x2 = actData.x;
				y2 = actData.y;
				if (y2 >= 610 && y2 <= 640)
				{
					if (x2 >= 1020 && x2 <= 1090)
						status = Done;
					if (x2 >= 1100 && x2 <= 1170)
						status = InProgress;
					if (x2 >= 1180 && x2 <= 1250)
						status = Pending;
					if (x2 >= 1260 && x2 <= 1330)
						status = Replaced;
				}
				pC->setStatus(status);
			}
			pC->setSelected(0);
		}
		
	}


	return true;
}


Sel_CRS_Status::~Sel_CRS_Status()
{
}