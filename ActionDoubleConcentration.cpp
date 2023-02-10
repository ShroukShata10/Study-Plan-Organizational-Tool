#include "ActionDoubleConcentration.h"
#include "Registrar.h"
#include "Courses\\Course.h"
ActionDoubleConcentration::ActionDoubleConcentration(Registrar* p) :Action(p)
{
}
bool ActionDoubleConcentration::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->PrintMsg("If you want double concentrations enter 1");
	string s = pGUI->GetSrting();
	if (s == "1")
	{
		pReg->set2Concentration();
		pGUI->GetUserAction("You choose two concentrations");
	}
	return true;
}
ActionDoubleConcentration::~ActionDoubleConcentration()
{
}