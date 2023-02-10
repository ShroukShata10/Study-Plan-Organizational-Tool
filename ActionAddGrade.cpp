#include"ActionAddGrade.h"
#include "Registrar.h"
#include "Courses\\Course.h"
ActionAddGrade::ActionAddGrade(Registrar* p) :Action(p)
{
}
bool ActionAddGrade::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("Select Course you want to add its grade: ");
	int x, y;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		CourseStatus status = Done;
		YearSem yearsem = pGUI->GetYearSem(x, y);
		graphicsInfo gInfo = pReg->getnewpostion(x, y);
		StudyPlan* pS = pReg->getStudyPlan();
		Course* pC = pS->getpCRS(gInfo.x, gInfo.y, yearsem.year, yearsem.sem);
		if (pC != NULL)
		{
			pC->setSelected(1);
			pC->DrawMe(pGUI);
			pGUI->PrintMsg("Enter Grade (A,A-,B+,B,B-,C+,C,C-,F):");
			string Grade= pGUI->GetSrting();
			if (Grade == "A")
				pC->set_gpa_letter(A);
			else if (Grade == "A-")
				pC->set_gpa_letter(Aminus);
			else if (Grade == "B+")
				pC->set_gpa_letter(Bplus);
			else if (Grade == "B")
				pC->set_gpa_letter(B);
			else if (Grade == "B-")
				pC->set_gpa_letter(Bminus);
			else if (Grade == "C+")
				pC->set_gpa_letter(Cplus);
			else if (Grade == "C")
				pC->set_gpa_letter(C);
			else if (Grade == "C-")
				pC->set_gpa_letter(Cminus);
			else if (Grade == "F")
				pC->set_gpa_letter(F);
			
			pC->setSelected(0);
		}

	}


	return true;
}
ActionAddGrade::~ActionAddGrade()
{
}