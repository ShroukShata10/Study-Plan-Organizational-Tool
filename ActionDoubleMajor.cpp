#include"ActionDoubleMajor.h"
#include "Registrar.h"
#include "Courses\\Course.h"
ActionDoubleMajor::ActionDoubleMajor(Registrar* p) :Action(p)
{
}
bool ActionDoubleMajor::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->PrintMsg("If you want double Majors enter 1");
	string s = pGUI->GetSrting();
	if (s == "1")
	{
		pGUI->PrintMsg("You choose two Majors, please enter Major Name");
		string name = pGUI->GetSrting();
		if (name == "CIE")
		{
			pReg->set2Majors();
			pReg->Programreq2("CIE-Requirements.txt");
		}
		else if (name == "NANENG")
		{
			pReg->set2Majors();
			pReg->Programreq2("NANENG-Requirements.txt");
		}
			
	}
	return true;
}
ActionDoubleMajor::~ActionDoubleMajor()
{
}