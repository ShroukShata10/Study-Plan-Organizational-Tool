#include "Registrar.h"
#include "Rules.h"
#include <string>
#include <vector>
#include <fstream>
#include "Actions/ActionAddCourse.h"
#include <iostream>     // std::cout
#include <sstream> 
#include "StudyPlan\AcademicYear.h"
#include"Actions\Action.h"
#include"ImportStudyPlan.h"
using namespace std;
ImportStudyPlan::ImportStudyPlan(Registrar* p) :Action(p)
{
}

bool ImportStudyPlan::Execute()
{
	GUI* pGUI = pReg->getGUI();
	ActionData actData = pGUI->GetUserAction("Write the name of the file you want to import the study plan from:");
	string filename = pGUI->GetSrting();
	vector <string> gettheyear;
	vector <string> courses;
	vector <string> semesters;
	fstream file;
	if (filename == "CIE-StudyPlan.txt")
		pReg->Programreq("CIE-Requirements.txt");
	else if (filename == "NAN-StudyPlan.txt")
		pReg->Programreq("NANENG-Requirements.txt");
	file.open(filename, ios::in);
	if (file.fail())
		return false;
	else
	{
		//go insidse the file and import the years, semesters and courses
		char* u; //it's a char to read everything 
		char* context = nullptr;
		const int size = 300;
		char line[size];
		//for the gui
		int x, y;
		int a = 0, b = 0,c=0,j=0;
		//inialize the variables to store the file
		SEMESTER semester;
		int sem;
		int year;
		while (file.getline(line, size))
		{
			int t = 1;
			int i = 0;
			u = strtok_s(line, ",", &context);
			i++;
			int yy = 0;
			while(u!=NULL)
			{ 
			if (t == 1)
			{
				//know which year i am in
				gettheyear.push_back(u);
				if (gettheyear[b] == "Year 1")
				{
					year = 1;
				}
				if (gettheyear[b] == "Year 2")
				{
					year = 2;
				}
				if (gettheyear[b] == "Year 3")
				{
					year = 3;
				}
				if (gettheyear[b] == "Year 4")
				{
					year = 4;
				}
				if (gettheyear[b] == "Year 5")
				{
					year = 5;
				}

				b++;

			}
			if (t == 2)
			{
				semesters.push_back(u);
				if (semesters[c] == "Fall")
				{
					semester = FALL;
					sem = 0;
					

				}
				if (semesters[c] == "Spring")
				{
					semester = SPRING;
					sem = 1;
					

				}
				if (semesters[c] == "Summer")
				{
					semester = SUMMER;
					sem = 2;

				}
				c++;
			}
			//for gui
			
			int PLAN_SEMESTER_HEIGHT = 70;
			if (t>2)
			{
					    courses.push_back(u);
						switch (year)
						{
						case 1:
							x = 2;
							break;
						case 2:
							x = 272;
							break;
						case 3:
							x = 542;
							break;
						case 4:
							x = 812;
							break;
						case 5:
							x = 1082;
							break;
						}
						x = x + 90 * sem;
						y = 202+42*yy;
						graphicsInfo gInfo{ x, y };
						CourseInfo CRS_Info = pReg->getCourseInfo(courses[j]);
						string Title = CRS_Info.Title;
						int crd = CRS_Info.Credits;
						Course* pC = new Course(courses[j], Title, crd);
						pC->setGfxInfo(gInfo);
						pC->setyearsem(year,semester);
						pC->settype(pReg->coursetype(courses[j]));
						for (auto j = CRS_Info.CoReqList.begin(); j < CRS_Info.CoReqList.end(); j++)
							pC->addCoReq(*j);
						for (auto i = CRS_Info.PreReqList.begin(); i < CRS_Info.PreReqList.end(); i++)
							pC->addPreReq(*i);
						StudyPlan* pS = pReg->getStudyPlan();
						pS->AddCourse(pC,year,semester);
						j++;
						yy++;
				      
			}
			u = strtok_s(context, ",", &context);
			t++;
			}
		}

		return true;

	}
}
ImportStudyPlan::~ImportStudyPlan()
{

}