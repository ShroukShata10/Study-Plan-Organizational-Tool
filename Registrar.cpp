#include "Registrar.h"
#include "Actions/ActionAddCourse.h"
#include "ActionDropCourse.h"
#include "ReplaceCourse.h"
#include "AddNote.h"
#include "ReorderCourse.h"
#include "Course_Info.h"
#include<fstream>
#include "Rules.h"
#include <string>
#include <vector>
#include <sstream>
#include "Course_Info.h"
#include "ImportStudyPlan.h"
#include "Sel_CRS_Status.h"
#include "ActionFilter.h"
#include "ActionValidityReport.h"
#include <list>
#include"ActionAddGrade.h"
#include"ActionCalculateGPA.h"
#include "StudentLevel.h"
#include "ActionDoubleConcentration.h"
#include "ActionMinor.h"
#include "ActionDoubleMajor.h"
Registrar::Registrar()
{
	pGUI = new GUI;	//create interface object
	pSPlan = new StudyPlan;	//create a study plan.
	ImportCourseOfferingTime();
	load_courseInfo();
	ofstream myfile;
	myfile.open("SaveCourses.txt");
	myfile << "CIE202.\n";
	myfile.close();
}

//returns a pointer to GUI
GUI* Registrar::getGUI() const
{
	return pGUI;
}

//returns the study plan
StudyPlan* Registrar::getStudyPlan() const
{
	return pSPlan;
}

Action* Registrar::CreateRequiredAction() 
{	
	ActionData actData = pGUI->GetUserAction("Pick and action...");
	Action* RequiredAction = nullptr;

	switch (actData.actType)
	{
	case ADD_CRS:	//add_course action
		RequiredAction = new ActionAddCourse(this);
		break;
	case DEL_CRS:
		RequiredAction = new ActionDropCourse(this);
			break;
	case REPLACE_CRS:
		RequiredAction = new ReplaceCourse(this);
		break;
	case SAVE:
		break;
	case LOAD:
		RequiredAction = new ImportStudyPlan(this);
		break;
	case Report:
		RequiredAction = new ActionValidityReport(this);
		break;
	case Reorder_Course:
		RequiredAction = new ReorderCourse(this);
		break;
	case Add_Note:
		RequiredAction = new AddNote(this);
		break;
	case CRS_Info:
		RequiredAction = new Course_Info(this);
		break;
	case CRS_Status:
		RequiredAction = new Sel_CRS_Status(this);
		break;
	case Filter:
		RequiredAction = new ActionFilter(this);
		break;
	case Grade: 
		RequiredAction = new ActionAddGrade(this);
		break;
	case GPA: 
		RequiredAction = new ActionCalculateGPA(this);
		break;
	case Level:
		RequiredAction = new StudentLevel(this);
		break;
	case Concentration:
		RequiredAction = new ActionDoubleConcentration(this);
		break;
	case minor:
		RequiredAction = new ActionMinor(this);
		break;
	case Major:
		RequiredAction = new ActionDoubleMajor(this);
		break;
	//TODO: Add case for each action
	/*case EXIT:
		break;*/
		
	}
	return RequiredAction;
}

//Executes the action, Releases its memory, and return true if done, false if cancelled
bool Registrar::ExecuteAction(Action* pAct)
{
	bool done = pAct->Execute();
	delete pAct;	//free memory of that action object (either action is exec or cancelled)
	return done;
}

void Registrar::Run()
{
	while (true)
	{
		//update interface here as CMU Lib doesn't refresh itself
		//when window is minimized then restored
		UpdateInterface();

		Action* pAct = CreateRequiredAction();
		if (pAct)	//if user doesn't cancel
		{
			if (ExecuteAction(pAct))	//if action is not cancelled
				UpdateInterface();
		}
	}
}


void Registrar::UpdateInterface()
{
	pGUI->UpdateInterface();	//update interface items
	pSPlan->DrawMe(pGUI);		//make study plan draw itself
	check_pre_cor();
	checkcredits();
	checktimeoffering();
	totalchecker();
}

Registrar::~Registrar()
{
	delete pGUI;
}
void Registrar::load_courseInfo()
{
	ifstream ofile;
	ofile.open("Course Catalog.txt");
	string scredits;
	string alldata;
	string scorreq;
	string sprereq;
	string core;
	string pre;

	while (getline(ofile, alldata))
	{
		CourseInfo info;
		stringstream uu(alldata);
		getline(uu, info.Code, ',');
		//cout << info.Code;
		getline(uu, info.Title, ',');
		//cout << info.Title;
		getline(uu, scredits, ',');
		info.Credits = stoi(scredits); //to store an integer as string
		//cout << info.Credits;
		getline(uu, core, ':');
		if (core == "Coreq")
		{
			string courses;
			getline(uu, courses, ',');
			while (true)
			{
				//Substring starting at position 0 and length end-1 is:
				//auto change the type in he run time error
				auto end = courses.find("And");
				string coursescode = courses.substr(0, end - 1);//to remove the space
				info.CoReqList.push_back(coursescode);
				courses = courses.substr(end + 4, courses.length());
				if (end == -1)
				{//there is no more
					break;
				}
			}
			//for (int i = 0; i < info.CoReqList.size(); i++) {
				//cout << info.CoReqList[i] << " ";
			//}
		
        
		getline(uu, core, ':');
		if (core == "Prereq")
		{
			string pre;
			getline(uu, pre, ',');
			while (true)
			{
				//Substring starting at position 0 and length end-1 is:
				//auto change the type in he run time error
				auto end = pre.find("And");
				string coursescode = pre.substr(0, end - 1);//to remove the space
				info.PreReqList.push_back(coursescode);
				pre = pre.substr(end + 4, pre.length());
				if (end == -1)
				{//there is no more
					break;
				}
			}
			//for (int i = 0; i < info.PreReqList.size(); i++) {
				//cout << info.PreReqList[i] << " ";
			//}
		}
		//cout << endl;
		}
		else if(core=="Prereq")
		{ 
			string pre;
		getline(uu, pre, ',');
		while (true)
		{
			//Substring starting at position 0 and length end-1 is:
			//auto change the type in he run time error
			auto end = pre.find("And");
			string coursescode = pre.substr(0, end - 1);//to remove the space
			info.PreReqList.push_back(coursescode);
			pre = pre.substr(end + 4, pre.length());
			if (end == -1)
			{//there is no more
				break;
			}
		}
		}

		RegRules.CourseCatalog.push_back(info);
	}
}
CourseInfo Registrar::getCourseInfo(Course_Code code) const
{
	CourseInfo CRS_Info;
	CRS_Info.Credits = 0;
	for (auto i = RegRules.CourseCatalog.begin(); i < RegRules.CourseCatalog.end(); i++)
	{
		if (i->Code == code)
		{
			CRS_Info = *i;
			return CRS_Info;
		}
	}
	return CRS_Info;
}


void Registrar::ConcReq(string name)
{
	if (name == "robotics")
		name = "CIE-robotics.txt";
	else if (name == "AI")
		name = "CIE-AI.txt";


	ifstream finput(name);

	char* u;
	char* context = nullptr;
	const int size = 300;
	char line[300];
	u = strtok_s(line, ",", &context);


	while (finput.getline(line, size))
	{
		u = strtok_s(line, ",", &context);
		while (u != NULL)
		{
			cout << u << " ";;
			u = strtok_s(NULL, ",", &context);
		}
		cout << endl;
	}
}
void Registrar::Programreq(string filename)
{
	fstream file;
	filename1 = filename;
	file.open(filename, ios::in);
	char* u;
	char* context = NULL;
	const int size = 1000;
	char line[size];
	int i = 0;
	while (file.getline(line, size))
	{
		u = strtok_s(line, ",", &context);
		i++;
		while (u != NULL)
		{
			switch (i)
			{
			case 1: RegRules.totalcredits = stoi(u);
				break;
			case 2:
			{
				RegRules.ReqUnivCredits = stoi(u);
				u = strtok_s(context, ",", &context);
				RegRules.ReqUniElectiv = stoi(u);
				break;
			}
			case 3:
			{
				RegRules.ReqTrackCredits = stoi(u);
				break;
			}
			case 4:
			{
				RegRules.ReqMajorCredits = stoi(u);
				u = strtok_s(context, ",", &context);
				RegRules.ReqMajorElectiv = stoi(u);
				break;
			}
			case 5:
			{
				RegRules.noconcentration = stoi(u);
				break;
			}
			case 6:
			{
				int j = 0;
				while (u != NULL)
				{
					j++;
					switch (j)
					{
					case 1: RegRules.MajorCon1compulsorycredits = stoi(u);
						break;
					case 2: RegRules.MajorCon1electivescredits = stoi(u);
						break;
					case 3: RegRules.MajorCon2compulsorycredits = stoi(u);
						break;
					case 4: RegRules.MajorCon2electivescredits = stoi(u);
						break;
					case 5: RegRules.MajorCon3compulsorycredits = stoi(u);
						break;
					case 6: RegRules.MajorCon3electivescredits = stoi(u);
						break;
					}
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 7:
			{
				while (u != NULL)
				{
					RegRules.UnivCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 8:
			{
				while (u != NULL)
				{
					RegRules.UnivElective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 9:
			{
				while (u != NULL)
				{
					RegRules.TrackCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 10:
			{
				while (u != NULL)
				{
					RegRules.MajorCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 11:
			{
				while (u != NULL)
				{
					RegRules.MajorElective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 12:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con1Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 13:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con1Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 14:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con2Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 15:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con2Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 16:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con3Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 17:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules.con3Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			}
			u = strtok_s(context, ",", &context);
		}
	}
	if (RegRules.noconcentration > 0)
		RegRules.takenConcentrations = 1;
}
void Registrar::set2Concentration()
{
	RegRules.takenConcentrations = 2;
}
void Registrar::Programreq2(string filename)
{
	filename2=filename;
	fstream file;
	file.open(filename, ios::in);
	char* u;
	char* context = NULL;
	const int size = 1000;
	char line[size];
	int i = 0;
	while (file.getline(line, size))
	{
		u = strtok_s(line, ",", &context);
		i++;
		while (u != NULL)
		{
			switch (i)
			{
			case 1: RegRules2.totalcredits = stoi(u);
				break;
			case 2:
			{
				RegRules2.ReqUnivCredits = stoi(u);
				u = strtok_s(context, ",", &context);
				RegRules2.ReqUniElectiv = stoi(u);
				break;
			}
			case 3:
			{
				RegRules2.ReqTrackCredits = stoi(u);
				break;
			}
			case 4:
			{
				RegRules2.ReqMajorCredits = stoi(u);
				u = strtok_s(context, ",", &context);
				RegRules2.ReqMajorElectiv = stoi(u);
				break;
			}
			case 5:
			{
				RegRules2.noconcentration = stoi(u);
				break;
			}
			case 6:
			{
				int j = 0;
				while (u != NULL)
				{
					j++;
					switch (j)
					{
					case 1: RegRules2.MajorCon1compulsorycredits = stoi(u);
						break;
					case 2: RegRules2.MajorCon1electivescredits = stoi(u);
						break;
					case 3: RegRules2.MajorCon2compulsorycredits = stoi(u);
						break;
					case 4: RegRules2.MajorCon2electivescredits = stoi(u);
						break;
					case 5: RegRules2.MajorCon3compulsorycredits = stoi(u);
						break;
					case 6: RegRules2.MajorCon3electivescredits = stoi(u);
						break;
					}
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 7:
			{
				while (u != NULL)
				{
					RegRules2.UnivCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 8:
			{
				while (u != NULL)
				{
					RegRules2.UnivElective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 9:
			{
				while (u != NULL)
				{
					RegRules2.TrackCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 10:
			{
				while (u != NULL)
				{
					RegRules2.MajorCompulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 11:
			{
				while (u != NULL)
				{
					RegRules2.MajorElective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 12:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con1Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 13:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con1Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 14:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con2Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 15:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con2Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 16:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con3Compulsory.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			case 17:
			{
				if (u == "0")
					break;
				while (u != NULL)
				{
					RegRules2.con3Elective.push_back(u);
					u = strtok_s(context, ",", &context);
				}
				break;
			}
			}
			u = strtok_s(context, ",", &context);
		}
	}

}
Course_Type Registrar :: coursetype(Course_Code code) const
{
	for (auto i = RegRules.UnivCompulsory.begin(); i != RegRules.UnivCompulsory.end(); i++)
	{
		if (*i == code)
			return UnivCompulsory;
	}
	for (auto i = RegRules.UnivElective.begin(); i != RegRules.UnivElective.end(); i++)
	{
		if (*i == code)
			return UnivElective;
	}
	for (auto i = RegRules.MajorElective.begin(); i != RegRules.MajorElective.end(); i++)
	{
		if (*i == code)
			return MajorElective;
	}
	for (auto i = RegRules.MajorCompulsory.begin(); i != RegRules.MajorCompulsory.end(); i++)
	{
		if (*i == code)
			return MajorCompulsory;
	}
	for (auto i = RegRules.TrackCompulsory.begin(); i != RegRules.TrackCompulsory.end(); i++)
	{
		if (*i == code)
			return TrackCompulsory;
	}
	for (auto i = RegRules.con1Compulsory.begin(); i != RegRules.con1Compulsory.end(); i++)
	{
		if (*i == code)
			return Concentration1Compulsory;
	}
	for (auto i = RegRules.con1Elective.begin(); i != RegRules.con1Elective.end(); i++)
	{
		if (*i == code)
			return Concentration1Elective;
	}
	for (auto i = RegRules.con2Compulsory.begin(); i != RegRules.con2Compulsory.end(); i++)
	{
		if (*i == code)
			return Concentration2Compulsory;
	}
	for (auto i = RegRules.con2Elective.begin(); i != RegRules.con2Elective.end(); i++)
	{
		if (*i == code)
			return Concentration2Elective;
	}
	for (auto i = RegRules.con3Compulsory.begin(); i != RegRules.con3Compulsory.end(); i++)
	{
		if (*i == code)
			return Concentration3Compulsory;
	}
	for (auto i = RegRules.con3Elective.begin(); i != RegRules.con3Elective.end(); i++)
	{
		if (*i == code)
			return Concentration3Elective;
	}
	for (auto i = RegRules.MinorCourses.begin(); i != RegRules.MinorCourses.end(); i++)
	{
		if (*i == code)
			return Minor;
	}
	return NoType;
}
graphicsInfo Registrar:: getnewpostion(int x, int y)
{
		graphicsInfo GI;
		int yearwidth = 270;
		int semwidth = 90;
		int ClickedYOrder = ((y-200) / 40);
		int ClickedYearOrder = (x / yearwidth);
		int ClickedSemOrder = (x / semwidth);
		int  sem;

		switch (ClickedYearOrder)
		{
		case 0: GI.x = 2;
			break;
		case 1: GI.x = 272;
			break;
		case 2: GI.x = 542;
			break;
		case 3: GI.x = 812;
			break;
		case 4: GI.x = 1082;
			break;
		}
		switch (ClickedSemOrder)
		{
		case 0: sem = 0;
			break;
		case 1: sem = 1;
			break;
		case 2: sem = 2;
			break;
		case 3: sem = 0;
			break;
		case 4: sem = 1;
			break;
		case 5: sem = 2;
			break;
		case 6: sem = 0;
			break;
		case 7: sem = 1;
			break;
		case 8: sem = 2;
			break;
		case 9: sem = 0;
			break;
		case 10: sem = 1;
			break;
		case 11: sem = 2;
			break;
		case 12: sem = 0;
			break;
		case 13: sem = 1;
			break;
		case 14: sem = 2;
			break;
		}
		GI.x = GI.x + 90 * sem;
		GI.y = 202 + 42 * ClickedYOrder;
		return GI;

}
bool Registrar::check_pre_cor()
{
	bool valid = 1;
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
			list<Course*> mainlist = *(pSPlan->getyear(year)->getlist(sem));
			for (auto i = mainlist.begin(); i != mainlist.end(); i++)
			{
				Course* course = *i;
				int c = 0;
				for (auto w = course->getCoReq()->begin(); w != course->getCoReq()->end(); w++)
				{
					Course_Code corcourse = *w;
					for (auto t = mainlist.begin(); t != mainlist.end(); t++)
					{
						Course* checkcourse = *t;
						if (corcourse == checkcourse->getCode())
						{
							course->set_checkcore(1);
							c++;
							break;
						}
					}
				}
				if (c == 0)
				{
                  course->set_checkcore(0);
				  valid = 0;
				}
					
				if (course->getCoReq()->size() == 0)
					course->set_checkcore(1);
				int checkcount = 0;
				for (auto sn = course->getPreReq()->begin(); sn != course->getPreReq()->end(); sn++)
				{
					Course_Code precourse = *sn;
					for (int yearcheck = 1; yearcheck < (year + 1); yearcheck++)
					{
						SEMESTER semcheck;
						for (int ss = 0; ss < 3; ss++)
						{

							switch (ss)
							{
							case 0: semcheck = FALL;
								break;
							case 1: semcheck = SPRING;
								break;
							case 2: semcheck = SUMMER;
								break;
							}
							if (yearcheck == year && semcheck == sem)
							{
								break;
							}
							list<Course*> listcheck = *(pSPlan->getyear(yearcheck)->getlist(semcheck));
							for (auto t = listcheck.begin(); t != listcheck.end(); t++)
							{
								Course* checkcourse = *t;
								if (precourse == checkcourse->getCode())
								{
									checkcount++;
									break;
								}
							}
							if (checkcount == course->getPreReq()->size())
							{
								course->set_checkpre(1);
								break;
							}

						}
						if (checkcount == course->getPreReq()->size())
						{
							course->set_checkpre(1);
							break;
						}
						else
						{
							course->set_checkpre(0);
							valid = 0;

						}
							

					}
				}
				if (checkcount == course->getPreReq()->size())
				{
					course->set_checkpre(1);
				}
			}

		}
	}
	return valid;
}
bool Registrar:: checkcredits()
{
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
			int credits = pSPlan->getsemcredits(year, sem);
			if ((sem == SUMMER && credits > RegRules.summerMaxCredit)||(sem!=SUMMER&&(credits<RegRules.SemMinCredit||credits>RegRules.SemMinCredit)))
				return false;
		}
	}
	return true;
	
}
bool Registrar::check_req(checkprogramreq* progreq)
{
	Programreq(filename1);
	int checker_if = 0;
	Rules check_st;
	Course_Type u;
	Course_Code k;
	int cr;
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
			list<Course*> mainlist = *(pSPlan->getyear(year)->getlist(sem));
			for (auto i = mainlist.begin(); i != mainlist.end(); i++)
			{
				Course* course = *i;
				k = course->getCode();
				cr = course->getCredits();
				u = course->gettype();
				check_st.totalcredits = cr + check_st.totalcredits;
				switch (u)
				{
				case UnivCompulsory:
					check_st.ReqUnivCredits = cr + check_st.ReqUnivCredits;
					break;
				case UnivElective:
					check_st.ReqUniElectiv = cr + check_st.ReqUniElectiv;
					break;
				case MajorElective:
					check_st.ReqMajorElectiv = cr + check_st.ReqMajorElectiv;
					break;
				case MajorCompulsory:
					check_st.ReqMajorCredits = cr + check_st.ReqMajorCredits;
					break;
				case TrackCompulsory:
					check_st.ReqTrackCredits = cr + check_st.ReqTrackCredits;
					break;
				case Concentration1Compulsory:
					check_st.MajorCon1compulsorycredits = cr + check_st.MajorCon1compulsorycredits;
					break;
				case Concentration1Elective:
					check_st.MajorCon1electivescredits = cr + check_st.MajorCon1electivescredits;
					break;
				case Concentration2Compulsory:
					check_st.MajorCon2compulsorycredits = cr + check_st.MajorCon2compulsorycredits;
					break;
				case Concentration2Elective:
					check_st.MajorCon2electivescredits = cr + check_st.MajorCon2electivescredits;
					break;
				case Concentration3Compulsory:
					check_st.MajorCon3compulsorycredits = cr + check_st.MajorCon3compulsorycredits;
					break;
				case Concentration3Elective:
					check_st.MajorCon3electivescredits = cr + check_st.MajorCon3electivescredits;
					break;
				case NoType:
					break;
				}
			}
		}
	}
	if (check_st.totalcredits >= RegRules.totalcredits)
	{
		checker_if++;
		progreq->totalcredits = 1;
	}
	if (check_st.ReqUnivCredits == RegRules.ReqUnivCredits)
	{
		checker_if++;
		progreq->ReqUnivCredits = 1;
	}
	if (check_st.ReqUniElectiv == RegRules.ReqUniElectiv)
	{
		checker_if++;
		progreq->ReqUniElectiv = 1;
	}
	if (check_st.ReqMajorElectiv == RegRules.ReqMajorElectiv)
	{
		checker_if++;
		progreq->ReqMajorElectiv = 1;
	}
	if (check_st.ReqMajorCredits == RegRules.ReqMajorCredits)
	{
		checker_if++;
		progreq->ReqMajorCredits = 1;
	}
	if (check_st.ReqTrackCredits == RegRules.ReqTrackCredits)
	{
		checker_if++;
		progreq->ReqTrackCredits = 1;
	}
	if (RegRules.noconcentration != 0)
	{
		if (check_st.MajorCon1compulsorycredits == RegRules.MajorCon1compulsorycredits)
		{
			checker_if++;
			progreq->MajorCon1compulsorycredits = 1;
		}
		if (check_st.MajorCon1electivescredits == RegRules.MajorCon1electivescredits)
		{
			checker_if++;
			progreq->MajorCon1electivescredits = 1;
		}
		if (RegRules.noconcentration>1)
		{
				if (check_st.MajorCon2compulsorycredits == RegRules.MajorCon2compulsorycredits)
				{
					checker_if++;
					progreq->MajorCon2compulsorycredits = 1;
				}
				if (check_st.MajorCon2electivescredits == RegRules.MajorCon2electivescredits)
				{
					checker_if++;
					progreq->MajorCon2electivescredits = 1;
				}
				if (RegRules.noconcentration>2)
				{
						if (check_st.MajorCon3compulsorycredits == RegRules.MajorCon3compulsorycredits)
						{
							checker_if++;
							progreq->MajorCon3compulsorycredits = 1;
						}
						if (check_st.MajorCon3electivescredits == RegRules.MajorCon3electivescredits)
						{
							checker_if++;
							progreq->MajorCon3electivescredits = 1;
						}
				}	
		}
	}
	Course_Code course_code;
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
			list<Course*> newlist = *(pSPlan->getyear(year)->getlist(sem));
			for (auto pC = newlist.begin(); pC != newlist.end(); pC++)
			{
				Course* pcourse = *pC;
				u = pcourse->gettype();
				course_code = pcourse->getCode();
				switch (u)
				{
				case UnivCompulsory:
				{
					if (progreq->ReqUnivCredits == 1)
						break;
					for (auto q = RegRules.UnivCompulsory.begin(); q != RegRules.UnivCompulsory.end(); q++)
					{
						Course_Code procourse1 = *q;
						if (procourse1 == course_code)
						{
							RegRules.UnivCompulsory.erase(q);
							break;
						}
					}
					break;
				}
					
				case UnivElective:
				{
					if (progreq->ReqUniElectiv == 1)
						break;
					for (auto w = RegRules.UnivElective.begin(); w != RegRules.UnivElective.end(); w++)
					{
						Course_Code procourse2 = *w;
						if (procourse2 == course_code)
						{
							RegRules.UnivElective.erase(w);
							break;
						}
					}
					break;
				}
				case MajorElective:
				{
					if (progreq->ReqMajorElectiv == 1)
						break;
					for (auto e = RegRules.MajorElective.begin(); e != RegRules.MajorElective.end(); e++)
					{
						Course_Code procourse3 = *e;
						if (procourse3 == course_code)
						{
							RegRules.MajorElective.erase(e);
							break;
						}
					}
					break;
				}
				case MajorCompulsory:
				{
					if (progreq->ReqMajorCredits == 1)
						break;
					for (auto i = RegRules.MajorCompulsory.begin(); i != RegRules.MajorCompulsory.end(); i++)
					{
						Course_Code procourse4 = *i;
						if (procourse4 == course_code)
						{
							RegRules.MajorCompulsory.erase(i);
							break;
						}
					}
					break;
				}
				case TrackCompulsory:
				{
					if (progreq->ReqTrackCredits == 1)
						break;
					for (auto r = RegRules.TrackCompulsory.begin(); r != RegRules.TrackCompulsory.end(); r++)
					{
						Course_Code procourse5 = *r;
						if (procourse5 == course_code)
						{
							RegRules.TrackCompulsory.erase(r);
							break;
						}
					}
					break;
				}
				case Concentration1Compulsory:
				{
					if (progreq->MajorCon1compulsorycredits == 1)
						break;
					for (auto a = RegRules.con1Compulsory.begin(); a != RegRules.con1Compulsory.end(); a++)
					{
						Course_Code procourse6 = *a;
						if (procourse6 == course_code)
						{
							RegRules.con1Compulsory.erase(a);
							break;
						}
					}
					break;
				}	
				case Concentration1Elective:
				{
					if (progreq->MajorCon1electivescredits == 1)
						break;
					for (auto t = RegRules.con1Elective.begin(); t != RegRules.con1Elective.end(); t++)
					{
						Course_Code procourse7 = *t;
						if (procourse7 == course_code)
						{
							RegRules.con1Elective.erase(t);
							break;
						}
					}
					break;
				}
				case Concentration2Compulsory:
				{
					if (progreq->MajorCon2compulsorycredits == 1)
						break;
					for (auto aa = RegRules.con2Compulsory.begin(); aa != RegRules.con2Compulsory.end(); aa++)
					{
						Course_Code procourse8 = *aa;
						if (procourse8 == course_code)
						{
							RegRules.con2Compulsory.erase(aa);
							break;
						}
					} break;
				}	
				case Concentration2Elective:
				{
					if (progreq->MajorCon2electivescredits == 1)
						break;
					for (auto tt = RegRules.con3Elective.begin(); tt != RegRules.con3Elective.end(); tt++)
					{
						Course_Code procourse9 = *tt;
						if (procourse9 == course_code)
						{
							RegRules.con3Elective.erase(tt);
							break;
						}
					}
					break;
				}
				case Concentration3Compulsory:
				{
					if (progreq->MajorCon3compulsorycredits == 1)
						break;
					for (auto aaa = RegRules.con3Compulsory.begin(); aaa != RegRules.con3Compulsory.end(); aaa++)
					{
						Course_Code procourse10 = *aaa;
						if (procourse10 == course_code)
						{
							RegRules.con3Compulsory.erase(aaa);
							break;
						}
					} 
					break;
				}
					
				case Concentration3Elective:
				{
					if (progreq->MajorCon3electivescredits == 1)
						break;
					for (auto ttt = RegRules.con3Elective.begin(); ttt != RegRules.con3Elective.end(); ttt++)
					{
						Course_Code procourse11 = *ttt;
						if (procourse11 == course_code)
						{
							RegRules.con3Elective.erase(ttt);
							break;
						}
					} break;
				}
				case NoType:
					break;
				}
			}
		}
	}
	for (auto i = RegRules.UnivCompulsory.begin(); i != RegRules.UnivCompulsory.end(); i++)
		progreq->UnivCompulsory.push_back(*i);
	for (auto i = RegRules.UnivElective.begin(); i != RegRules.UnivElective.end(); i++)
		progreq->UnivElective.push_back(*i);
	for (auto i = RegRules.TrackCompulsory.begin(); i != RegRules.TrackCompulsory.end(); i++)
		progreq->TrackCompulsory.push_back(*i);
	for (auto i = RegRules.MajorCompulsory.begin(); i != RegRules.MajorCompulsory.end(); i++)
		progreq->MajorCompulsory.push_back(*i);
	for (auto i = RegRules.MajorElective.begin(); i != RegRules.MajorElective.end(); i++)
		progreq->MajorElective.push_back(*i);
	for (auto i = RegRules.con1Compulsory.begin(); i != RegRules.con1Compulsory.end(); i++)
		progreq->con1Compulsory.push_back(*i);
	for (auto i = RegRules.con1Elective.begin(); i != RegRules.con1Elective.end(); i++)
		progreq->con1Elective.push_back(*i);
	for (auto i = RegRules.con2Compulsory.begin(); i != RegRules.con2Compulsory.end(); i++)
		progreq->con2Compulsory.push_back(*i);
	for (auto i = RegRules.con2Elective.begin(); i != RegRules.con2Elective.end(); i++)
		progreq->con2Elective.push_back(*i);
	for (auto i = RegRules.con3Compulsory.begin(); i != RegRules.con3Compulsory.end(); i++)
		progreq->con3Compulsory.push_back(*i);
	for (auto i = RegRules.con3Elective.begin(); i != RegRules.con3Elective.end(); i++)
		progreq->con3Elective.push_back(*i);
	progreq->noconcentration = RegRules.noconcentration;
	progreq->intMajorCon1compulsorycredits = RegRules.MajorCon1compulsorycredits;
	progreq->intMajorCon2compulsorycredits = RegRules.MajorCon2compulsorycredits;
	progreq->intMajorCon3compulsorycredits = RegRules.MajorCon3compulsorycredits;
	progreq->intMajorCon1electivescredits = RegRules.MajorCon1electivescredits;
	progreq->intMajorCon2electivescredits = RegRules.MajorCon2electivescredits;
	progreq->intMajorCon3electivescredits = RegRules.MajorCon3electivescredits;
	progreq->inttotalcredits = check_st.totalcredits - RegRules.totalcredits;
	progreq->intReqUnivCredits = check_st.ReqUnivCredits- RegRules.ReqUnivCredits;
	progreq->intReqUniElectiv = check_st.ReqUniElectiv - RegRules.ReqUniElectiv;
	progreq->intReqTrackCredits= check_st.ReqTrackCredits - RegRules.ReqTrackCredits;
	progreq->intReqMajorCredits = check_st.ReqMajorCredits - RegRules.ReqMajorCredits;
	progreq->intReqMajorElectiv = check_st.ReqMajorElectiv - RegRules.ReqMajorElectiv;
	progreq->inttakenConcentrations = RegRules.takenConcentrations;
	progreq->DoubleMajor = RegRules.DoubleMajor;
	Programreq(filename1);
	if ((RegRules.noconcentration==0&& checker_if==6)||(RegRules.noconcentration>0&&checker_if>7&&checker_if<11))
		return true;
	else
		return false;
}
bool Registrar::check_req2(checkprogramreq* progreq)
{
	Programreq2(filename2);
	int checker_if = 0;
	Rules check_st;
	Course_Type u;
	Course_Code k;
	int cr;
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
			list<Course*> mainlist = *(pSPlan->getyear(year)->getlist(sem));
			for (auto i = mainlist.begin(); i != mainlist.end(); i++)
			{
				Course* course = *i;
				k = course->getCode();
				cr = course->getCredits();
				u = course->gettype();
				check_st.totalcredits = cr + check_st.totalcredits;
				switch (u)
				{
				case UnivCompulsory:
					check_st.ReqUnivCredits = cr + check_st.ReqUnivCredits;
					break;
				case UnivElective:
					check_st.ReqUniElectiv = cr + check_st.ReqUniElectiv;
					break;
				case MajorElective:
					check_st.ReqMajorElectiv = cr + check_st.ReqMajorElectiv;
					break;
				case MajorCompulsory:
					check_st.ReqMajorCredits = cr + check_st.ReqMajorCredits;
					break;
				case TrackCompulsory:
					check_st.ReqTrackCredits = cr + check_st.ReqTrackCredits;
					break;
				case Concentration1Compulsory:
					check_st.MajorCon1compulsorycredits = cr + check_st.MajorCon1compulsorycredits;
					break;
				case Concentration1Elective:
					check_st.MajorCon1electivescredits = cr + check_st.MajorCon1electivescredits;
					break;
				case Concentration2Compulsory:
					check_st.MajorCon2compulsorycredits = cr + check_st.MajorCon2compulsorycredits;
					break;
				case Concentration2Elective:
					check_st.MajorCon2electivescredits = cr + check_st.MajorCon2electivescredits;
					break;
				case Concentration3Compulsory:
					check_st.MajorCon3compulsorycredits = cr + check_st.MajorCon3compulsorycredits;
					break;
				case Concentration3Elective:
					check_st.MajorCon3electivescredits = cr + check_st.MajorCon3electivescredits;
					break;
				case NoType:
					break;
				}
			}
		}
	}
	if (check_st.totalcredits >= RegRules2.totalcredits)
	{
		checker_if++;
		progreq->totalcredits = 1;
	}
	if (check_st.ReqUnivCredits == RegRules2.ReqUnivCredits)
	{
		checker_if++;
		progreq->ReqUnivCredits = 1;
	}
	if (check_st.ReqUniElectiv == RegRules2.ReqUniElectiv)
	{
		checker_if++;
		progreq->ReqUniElectiv = 1;
	}
	if (check_st.ReqMajorElectiv == RegRules2.ReqMajorElectiv)
	{
		checker_if++;
		progreq->ReqMajorElectiv = 1;
	}
	if (check_st.ReqMajorCredits == RegRules2.ReqMajorCredits)
	{
		checker_if++;
		progreq->ReqMajorCredits = 1;
	}
	if (check_st.ReqTrackCredits == RegRules2.ReqTrackCredits)
	{
		checker_if++;
		progreq->ReqTrackCredits = 1;
	}
	if (RegRules2.noconcentration != 0)
	{
		if (check_st.MajorCon1compulsorycredits == RegRules2.MajorCon1compulsorycredits)
		{
			checker_if++;
			progreq->MajorCon1compulsorycredits = 1;
		}
		if (check_st.MajorCon1electivescredits == RegRules2.MajorCon1electivescredits)
		{
			checker_if++;
			progreq->MajorCon1electivescredits = 1;
		}
		if (RegRules2.noconcentration > 1)
		{
			if (check_st.MajorCon2compulsorycredits == RegRules2.MajorCon2compulsorycredits)
			{
				checker_if++;
				progreq->MajorCon2compulsorycredits = 1;
			}
			if (check_st.MajorCon2electivescredits == RegRules2.MajorCon2electivescredits)
			{
				checker_if++;
				progreq->MajorCon2electivescredits = 1;
			}
			if (RegRules2.noconcentration > 2)
			{
				if (check_st.MajorCon3compulsorycredits == RegRules2.MajorCon3compulsorycredits)
				{
					checker_if++;
					progreq->MajorCon3compulsorycredits = 1;
				}
				if (check_st.MajorCon3electivescredits == RegRules2.MajorCon3electivescredits)
				{
					checker_if++;
					progreq->MajorCon3electivescredits = 1;
				}
			}
		}
	}
	Course_Code course_code;
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
			list<Course*> newlist = *(pSPlan->getyear(year)->getlist(sem));
			for (auto pC = newlist.begin(); pC != newlist.end(); pC++)
			{
				Course* pcourse = *pC;
				u = pcourse->gettype();
				course_code = pcourse->getCode();
				switch (u)
				{
				case UnivCompulsory:
				{
					if (progreq->ReqUnivCredits == 1)
						break;
					for (auto q = RegRules2.UnivCompulsory.begin(); q != RegRules2.UnivCompulsory.end(); q++)
					{
						Course_Code procourse1 = *q;
						if (procourse1 == course_code)
						{
							RegRules2.UnivCompulsory.erase(q);
							break;
						}
					}
					break;
				}

				case UnivElective:
				{
					if (progreq->ReqUniElectiv == 1)
						break;
					for (auto w = RegRules2.UnivElective.begin(); w != RegRules2.UnivElective.end(); w++)
					{
						Course_Code procourse2 = *w;
						if (procourse2 == course_code)
						{
							RegRules2.UnivElective.erase(w);
							break;
						}
					}
					break;
				}
				case MajorElective:
				{
					if (progreq->ReqMajorElectiv == 1)
						break;
					for (auto e = RegRules2.MajorElective.begin(); e != RegRules2.MajorElective.end(); e++)
					{
						Course_Code procourse3 = *e;
						if (procourse3 == course_code)
						{
							RegRules2.MajorElective.erase(e);
							break;
						}
					}
					break;
				}
				case MajorCompulsory:
				{
					if (progreq->ReqMajorCredits == 1)
						break;
					for (auto i = RegRules2.MajorCompulsory.begin(); i != RegRules2.MajorCompulsory.end(); i++)
					{
						Course_Code procourse4 = *i;
						if (procourse4 == course_code)
						{
							RegRules2.MajorCompulsory.erase(i);
							break;
						}
					}
					break;
				}
				case TrackCompulsory:
				{
					if (progreq->ReqTrackCredits == 1)
						break;
					for (auto r = RegRules2.TrackCompulsory.begin(); r != RegRules2.TrackCompulsory.end(); r++)
					{
						Course_Code procourse5 = *r;
						if (procourse5 == course_code)
						{
							RegRules2.TrackCompulsory.erase(r);
							break;
						}
					}
					break;
				}
				case Concentration1Compulsory:
				{
					if (progreq->MajorCon1compulsorycredits == 1)
						break;
					for (auto a = RegRules2.con1Compulsory.begin(); a != RegRules2.con1Compulsory.end(); a++)
					{
						Course_Code procourse6 = *a;
						if (procourse6 == course_code)
						{
							RegRules2.con1Compulsory.erase(a);
							break;
						}
					}
					break;
				}
				case Concentration1Elective:
				{
					if (progreq->MajorCon1electivescredits == 1)
						break;
					for (auto t = RegRules2.con1Elective.begin(); t != RegRules2.con1Elective.end(); t++)
					{
						Course_Code procourse7 = *t;
						if (procourse7 == course_code)
						{
							RegRules2.con1Elective.erase(t);
							break;
						}
					}
					break;
				}
				case Concentration2Compulsory:
				{
					if (progreq->MajorCon2compulsorycredits == 1)
						break;
					for (auto aa = RegRules2.con2Compulsory.begin(); aa != RegRules2.con2Compulsory.end(); aa++)
					{
						Course_Code procourse8 = *aa;
						if (procourse8 == course_code)
						{
							RegRules2.con2Compulsory.erase(aa);
							break;
						}
					} break;
				}
				case Concentration2Elective:
				{
					if (progreq->MajorCon2electivescredits == 1)
						break;
					for (auto tt = RegRules2.con3Elective.begin(); tt != RegRules2.con3Elective.end(); tt++)
					{
						Course_Code procourse9 = *tt;
						if (procourse9 == course_code)
						{
							RegRules2.con3Elective.erase(tt);
							break;
						}
					}
					break;
				}
				case Concentration3Compulsory:
				{
					if (progreq->MajorCon3compulsorycredits == 1)
						break;
					for (auto aaa = RegRules2.con3Compulsory.begin(); aaa != RegRules2.con3Compulsory.end(); aaa++)
					{
						Course_Code procourse10 = *aaa;
						if (procourse10 == course_code)
						{
							RegRules2.con3Compulsory.erase(aaa);
							break;
						}
					}
					break;
				}

				case Concentration3Elective:
				{
					if (progreq->MajorCon3electivescredits == 1)
						break;
					for (auto ttt = RegRules2.con3Elective.begin(); ttt != RegRules2.con3Elective.end(); ttt++)
					{
						Course_Code procourse11 = *ttt;
						if (procourse11 == course_code)
						{
							RegRules2.con3Elective.erase(ttt);
							break;
						}
					} break;
				}
				case NoType:
					break;
				}
			}
		}
	}
	for (auto i = RegRules2.UnivCompulsory.begin(); i != RegRules2.UnivCompulsory.end(); i++)
		progreq->UnivCompulsory.push_back(*i);
	for (auto i = RegRules2.UnivElective.begin(); i != RegRules2.UnivElective.end(); i++)
		progreq->UnivElective.push_back(*i);
	for (auto i = RegRules2.TrackCompulsory.begin(); i != RegRules2.TrackCompulsory.end(); i++)
		progreq->TrackCompulsory.push_back(*i);
	for (auto i = RegRules2.MajorCompulsory.begin(); i != RegRules2.MajorCompulsory.end(); i++)
		progreq->MajorCompulsory.push_back(*i);	
	for (auto i = RegRules2.MajorElective.begin(); i != RegRules2.MajorElective.end(); i++)
		progreq->MajorElective.push_back(*i);
	for (auto i = RegRules2.con1Compulsory.begin(); i != RegRules2.con1Compulsory.end(); i++)
		progreq->con1Compulsory.push_back(*i);
	for (auto i = RegRules2.con1Elective.begin(); i != RegRules2.con1Elective.end(); i++)
		progreq->con1Elective.push_back(*i);
	for (auto i = RegRules2.con2Compulsory.begin(); i != RegRules2.con2Compulsory.end(); i++)
		progreq->con2Compulsory.push_back(*i);
	for (auto i = RegRules2.con2Elective.begin(); i != RegRules2.con2Elective.end(); i++)
		progreq->con2Elective.push_back(*i);
	for (auto i = RegRules2.con3Compulsory.begin(); i != RegRules2.con3Compulsory.end(); i++)
		progreq->con3Compulsory.push_back(*i);
	for (auto i = RegRules2.con3Elective.begin(); i != RegRules2.con3Elective.end(); i++)
		progreq->con3Elective.push_back(*i);
	progreq->noconcentration = RegRules2.noconcentration;
	progreq->intMajorCon1compulsorycredits = RegRules2.MajorCon1compulsorycredits;
	progreq->intMajorCon2compulsorycredits = RegRules2.MajorCon2compulsorycredits;
	progreq->intMajorCon3compulsorycredits = RegRules2.MajorCon3compulsorycredits;
	progreq->intMajorCon1electivescredits = RegRules2.MajorCon1electivescredits;
	progreq->intMajorCon2electivescredits = RegRules2.MajorCon2electivescredits;
	progreq->intMajorCon3electivescredits = RegRules2.MajorCon3electivescredits;
	progreq->inttotalcredits = check_st.totalcredits - RegRules2.totalcredits;
	progreq->intReqUnivCredits = check_st.ReqUnivCredits - RegRules2.ReqUnivCredits;
	progreq->intReqUniElectiv = check_st.ReqUniElectiv - RegRules2.ReqUniElectiv;
	progreq->intReqTrackCredits = check_st.ReqTrackCredits - RegRules2.ReqTrackCredits;
	progreq->intReqMajorCredits = check_st.ReqMajorCredits - RegRules2.ReqMajorCredits;
	progreq->intReqMajorElectiv = check_st.ReqMajorElectiv - RegRules2.ReqMajorElectiv;
	Programreq2(filename2);
	if ((RegRules2.noconcentration == 0 && checker_if == 6) || (RegRules2.noconcentration > 0 && checker_if > 7 && checker_if < 11))
		return true;
	else
		return false;
}
void Registrar::ImportCourseOfferingTime()
{
	fstream file;
	file.open("offering.txt", ios::in);
	AcademicYearOfferings yearoffering;
	char* u;
	char* context = nullptr;
	const int size = 300;
	char line[size];
	int i = 0;
	while (file.getline(line, size))
	{
		int t = 1;
		u = strtok_s(line, ",", &context);
		i++;
		int yy = 0;
		while (u != NULL)
		{
			if (t == 1)
			{
				yearoffering.Year = u;
			}
			if (t > 2)
			{
				int k;
				if (i%3==0)
					k = 2;
				else
					k = i % 3 - 1;
				yearoffering.Offerings[k].push_back(u);
			}
			u = strtok_s(context, ",", &context);
			t++;
		}
		if (i % 3 == 0)
		{
			RegRules.OffringsList.push_back(yearoffering);
			yearoffering.Offerings[0].clear();
			yearoffering.Offerings[1].clear();
			yearoffering.Offerings[2].clear();
		}
			

	}
}
bool Registrar::checkMinor(checkprogramreq* progreq)
{
	int credits = 0;
	for (auto i = RegRules.MinorCourses.begin(); i != RegRules.MinorCourses.end(); i++)
		progreq->MinorCourses.push_back(*i);
	for (auto i = RegRules.MinorCourses.begin(); i != RegRules.MinorCourses.end(); i++)
	{
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
				list<Course*> mainlist = *(pSPlan->getyear(year)->getlist(sem));
				for (auto c = mainlist.begin(); c != mainlist.end(); c++)
				{
					if ((*c)->getCode() == *i)
					{
						progreq->MinorCourses.erase(i);
						credits += (*c)->getCredits();
					}
						
				}
			}
		}
	}
	if (credits == RegRules.MinorCredits)
		return true;	
	else
	{
		progreq->intMinorCredits = credits - RegRules.MinorCredits;
		progreq->MinorCredits = 0;
		return false;
	}
		
}
bool Registrar::checktimeoffering()
{
	int count = 0, error = 0;
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
			list<Course*> mainlist = *(pSPlan->getyear(year)->getlist(sem));
			vector<Course_Code> offerlist = (RegRules.OffringsList[year-1].Offerings[s]);
			for (auto i = mainlist.begin(); i != mainlist.end(); i++)
			{
				Course_Code coursecode = (*i)->getCode();
				for (auto j = offerlist.begin(); j != offerlist.end(); j++)
				{
					Course_Code offeringcode = *j;
					if (coursecode == offeringcode)
					{
                        count++;
						(*i)->setsuitabletime(1);
					}
						
					if (count != mainlist.size())
						error++;

				}
			}
		}
	}

	if (error == 0)
		return true;
	else
		return false;
}
void Registrar::AddMinorCourse(Course_Code code,int c)
{
	RegRules.MinorCourses.push_back(code);
	RegRules.MinorCredits +=c;
}
void Registrar::set2Majors()
{
	RegRules.DoubleMajor = 1;
}
void Registrar::totalchecker()
{
	checkprogramreq p;
	bool reqchecker = check_req(&p);
	bool creditchecker = checkcredits();
	bool porcorchecker = check_pre_cor();
	bool offeringchecker = checktimeoffering();
	if (reqchecker == false || porcorchecker == false)
	{
		GUI* pGUI = getGUI();
		pGUI->PrintMsg("ERROR, There is a CRITICAL ISSUE!!");
	}
	if (creditchecker == false || offeringchecker == false)
	{
		GUI* pGUI = getGUI();
		pGUI->PrintMsg("ERROR, There is a MODERATE ISSUE!!");
	}
}