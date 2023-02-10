#include "AddNote.h"
using namespace std;
AddNote::AddNote(Registrar* p) :Action(p)
{
}

bool AddNote::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->PrintMsg("Write the note you want to add: ");
	string Note= pGUI->GetSrting();
	StudyPlan* pS = pReg->getStudyPlan();
	pS->AddNote(Note);
	return 1;
}
AddNote::~AddNote()
{

}