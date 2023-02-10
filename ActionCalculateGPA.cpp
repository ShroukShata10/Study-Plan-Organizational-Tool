#include "Registrar.h"
#include "Courses\\Course.h"
#include"ActionCalculateGPA.h"
#include <string>
ActionCalculateGPA::ActionCalculateGPA(Registrar* p) :Action(p)
{
}
bool ActionCalculateGPA:: Execute()
{
	    GUI* pGUI = pReg->getGUI();
	    StudyPlan* pS = pReg->getStudyPlan();
		GPA_letter e;
		double count_gpa=0;
		int total_count=0;
		int cr = 0;
		int total_cr = 0;
		double gpa=0;
		for (int year = 1; year < 6; year++)
		{
			for (int s = 0; s < 3; s++)
			{
				SEMESTER sem;
				switch (s)
				{
				case 0: sem = FALL;
					break;
				case 1: sem = SPRING;
					break;
				case 2: sem = SUMMER;
					break;
				}
				list<Course*> mainlist = *(pS->getyear(year)->getlist(sem));
				for (auto i = mainlist.begin(); i != mainlist.end(); i++)
				{
					Course* pC = *i;
					e = pC->get_gpa_letter();
					cr = pC->getCredits();
					if (pC->isDone()) 
					{
						switch (e)
						{
						case A:
							total_cr = total_cr + cr;
							count_gpa = 4 * cr;
							total_count = total_count + count_gpa;
							break;
						case Aminus:
							total_cr = total_cr + cr;
							count_gpa = 3.7 * cr;
							total_count = total_count + count_gpa;
							break;
						case Bplus:
							total_cr = total_cr + cr;
							count_gpa = 3.3 * cr;
							total_count = total_count + count_gpa;
							break;
						case B:
							total_cr = total_cr + cr;
							count_gpa = 3 * cr;
							total_count = total_count + count_gpa;
							break;
						case Bminus:
							total_cr = total_cr + cr;
							count_gpa = 2.7 * cr;
							total_count = total_count + count_gpa;
							break;
						case Cplus:
							total_cr = total_cr + cr;
							count_gpa = 2.3 * cr;
							total_count = total_count + count_gpa;
							break;
						case C:
							total_cr = total_cr + cr;
							count_gpa = 2 * cr;
							total_count = total_count + count_gpa;
							break;
						case Cminus:
							total_cr = total_cr + cr;
							count_gpa = 1.7 * cr;
							total_count = total_count + count_gpa;
							break;
						case F:
							total_cr = total_cr + cr;
							count_gpa = 0 * cr;
							total_count = total_count + count_gpa;
							break;
						}
					}
					
				}
			}
		}
		gpa = total_count / total_cr;
		string msg = "You GPA = " + to_string(gpa);
		pGUI->GetUserAction(msg);
		return true;
}
ActionCalculateGPA::~ActionCalculateGPA()
{
}
