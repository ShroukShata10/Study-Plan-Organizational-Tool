#include "ActionFilter.h"
#include "Registrar.h"
#include "Courses\\Course.h"
#include <iostream>
ActionFilter::ActionFilter(Registrar* p) :Action(p)
{
}
void loopsem(int year,StudyPlan* pS,bool Hide)
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
		list<Course*> list = *(pS->getyear(year)->getlist(sem));
		for (auto i = list.begin(); i != list.end(); i++)
		{
			Course* course = *i;
			course->setHide(Hide);

		}
	}
}
void looptype( StudyPlan* pS, Course_Type type,bool Hide)
{
	for(int year=1; year<6; year++)
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
			list<Course*> list = *(pS->getyear(year)->getlist(sem));
			for (auto i = list.begin(); i != list.end(); i++)
			{
				Course* course = *i;
				if (course->gettype() == type)
					course->setHide(Hide);
			}
		}
	}
}
bool ActionFilter::Execute()
{
	GUI* pGUI = pReg->getGUI();
	pGUI->DrawFilterOptions();
	ActionData actData = pGUI->GetUserAction("Select Filter Option");
	int x, y, x2, y2;
	if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
	{
		//get coord where user clicked
		x = actData.x;
		y = actData.y;
		StudyPlan* pS = pReg->getStudyPlan();
		if (x >= 1260 && x <= 1310 && y >= 560 && y <= 600)
		{
			for (int i = 1; i < 6; i++)
				loopsem(i, pS, 0);
		}
		else if (y >= 610 && y <= 640)
		{
			if (x >= 1020 && x <= 1070) //year
			{
				ActionData actData = pGUI->GetUserAction("click on the year you want");
				if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
				{
					x2 = actData.x;
					y2 = actData.y;
					if (y2 > 80 && y2 < 130)
					{
						int year = x2 / 270 + 1;
						for (int i = 1; i < 6; i++)
							loopsem(i, pS, 1);
						loopsem(year, pS, 0);
					}
				
				}
			}
			else if (x >= 1080 && x <= 1130) //sem
			{

				ActionData actData = pGUI->GetUserAction("click on the sem you want");
				if (actData.actType == DRAW_AREA)	//user clicked inside drawing area
				{
					x2 = actData.x;
					y2 = actData.y;
					if (y2 > 130 && y2 < 200)
					{
						int s = x2 / 90;
						SEMESTER semester;
						if (s == 0 || s == 3 || s == 6 || s == 9 || s == 12)
							semester = FALL;
						else if (s == 1 || s == 4 || s == 7 || s == 10 || s == 13)
							semester = SPRING;
						else if (s == 2 || s == 5 || s == 8 || s == 11 || s == 14)
							semester = SUMMER;
						for (int i = 1; i < 6; i++)
							loopsem(i, pS, 1);
						list<Course*> list = *(pS->getyear(x2/270+1)->getlist(semester));
						for (auto i = list.begin(); i != list.end(); i++)
						{
							Course* course = *i;
							course->setHide(0);
						}
					}

				}
			}	
			else if (x >= 1140 && x <= 1190) //Univ
			{
				for (int i = 1; i < 6; i++)
					loopsem(i, pS, 1);
				looptype(pS, UnivCompulsory, 0);
				looptype(pS, UnivElective, 0);
			}
			else if (x >= 1200 && x <= 1250) //Major
			{
				for (int i = 1; i < 6; i++)
					loopsem(i, pS, 1);
				looptype(pS, MajorCompulsory, 0);
				looptype(pS, MajorElective, 0);
			}
			else if (x >= 1260 && x <= 1310) //Track
			{
				for (int i = 1; i < 6; i++)
					loopsem(i, pS, 1);
				looptype(pS, TrackCompulsory, 0);
			}
		}
		
		
		
	}
	return true;
}
ActionFilter::~ActionFilter()
{
}