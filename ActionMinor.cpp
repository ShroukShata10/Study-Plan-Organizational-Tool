using namespace std;
#include"ActionMinor.h"
#include"Registrar.h"

ActionMinor::ActionMinor(Registrar* p) :Action(p)
{
}
bool ActionMinor::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->PrintMsg("Enter Minor Course code: ");
	Course_Code code = pGUI->GetSrting();
	CourseInfo CRS_Info = pReg->getCourseInfo(code);
	if (CRS_Info.Credits != 0 && pReg->coursetype(code)==NoType)
		pReg->AddMinorCourse(code, CRS_Info.Credits);
	return true;

}
ActionMinor::~ActionMinor()
{
}