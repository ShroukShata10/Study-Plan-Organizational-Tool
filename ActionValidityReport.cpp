#include "ActionValidityReport.h"
#include "Registrar.h"
#include "Courses\\Course.h"
#include <fstream>
#include <string>
ActionValidityReport::ActionValidityReport(Registrar* p) :Action(p)
{
}
bool ActionValidityReport::Execute()
{
	ofstream Report("ValidityReport.txt");
	Report.clear();
	GUI* pGUI = pReg->getGUI();
	StudyPlan* pS = pReg->getStudyPlan();
	string creditsissue = "";
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
			list<Course*> list = *(pS->getyear(year)->getlist(sem));
			for (auto i = list.begin(); i != list.end(); i++)
			{
				Course* course = *i;
				if (course->getsuitabletime() == 0)
				{
					Course_Code code = *(course->getCoReq()->begin());
					Report << "Moderate Issue (Offering Time): " << course->getCode()
						<< "," << endl;
				}
				if (course->get_checkcore() == 0)
				{
					Course_Code code = *(course->getCoReq()->begin());
					Report << "Critical Issue: " << course->getCode()
						<< ", to solve the issue add its Corequistes: " << code << "," << endl;
				}
				if (course->get_checkpre() == 0)
				{
					string pre = "";
					for (auto k = course->getPreReq()->begin(); k != course->getPreReq()->end(); k++)
						pre += *k + ", ";
					Report << "Critical Issue: " << course->getCode()
						<< ", to solve the issue add its Prerequistes: " << pre << endl;
				}
			}
		}
	}
	if (pReg->checkcredits() == 0)
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
				if (pReg->checkcredits() == 0)
				{
					int credits = pS->getsemcredits(year, sem);
					if ((sem == SUMMER && credits > 6))
						Report << "Moderate Issue: Year " + to_string(year) + " Sem SUMMER, no of credits= "
						<< to_string(credits) << ", you need Overload Petition;" << endl;
					if ((sem == FALL && credits > 18))
						Report << "Moderate Issue: Year " + to_string(year) + " Sem FALL, no of credits= "
						<< to_string(credits) << ", you need Overload Petition;" << endl;
					if ((sem == SPRING && credits > 18))
						Report << "Moderate Issue: Year " + to_string(year) + " Sem SPRING, no of credits= "
						<< to_string(credits) << ", you need Overload Petition;" << endl;
					if ((sem == FALL && credits < 12))
						Report << "Moderate Issue: Year " + to_string(year) + " Sem FALL, no of credits= "
						<< to_string(credits) << ", you need Underload Petition;" << endl;
					if ((sem == SPRING && credits < 12))
						Report << "Moderate Issue: Year " + to_string(year) + " Sem SPRING, no of credits= "
						<< to_string(credits) << ", you need Underload Petition;" << endl;
				}
			}

		}
	}
	checkprogramreq progreq;
	bool check = pReg->check_req(&progreq);
	if (check == 0)
	{
		if (progreq.totalcredits == 0)
			Report << "Critical Issue: total number of credits less than total number of credits for program requirments;"
			<<endl;
		if (progreq.ReqUnivCredits == 0)
		{
			Report << "Critical Issue: number of credits for Univ Compulsory isn't achived, the following courses are missed: "
				<< endl;
			for (auto it = progreq.UnivCompulsory.begin(); it != progreq.UnivCompulsory.end(); it++)
				Report << *it << ",";
			Report << endl;
		}
		if (progreq.ReqUniElectiv == 0 && progreq.intReqUniElectiv < 0)
		{
			Report << "Critical Issue: number of credits for Univ Electives isn't achived";
			Report << ", you can add courses from the following courses: " << endl;
			for (auto it = progreq.UnivElective.begin(); it != progreq.UnivElective.end(); it++)
				Report << *it << ",";
			Report << endl;
		}
		else
			Report << "Critical Issue: number of credits for Univ Electives isn't achived!! too much Univ Electives,"
			<< "Decrease the Univ Electives Credits by " << to_string(progreq.intReqUniElectiv)<<endl;
		if (progreq.ReqTrackCredits == 0)
		{
			Report << "Critical Issue: number of credits for Track Compulsory isn't achived, the following courses are missed"
				<< endl;
			for (auto it = progreq.TrackCompulsory.begin(); it != progreq.TrackCompulsory.end(); it++)
				Report << *it << ",";
			Report << endl;
		}
		if (progreq.ReqMajorCredits == 0)
		{
			Report << "Critical Issue: number of credits for Major compulsory isn't achived, the following courses are missed"
				<< endl;
			for (auto it = progreq.TrackCompulsory.begin(); it != progreq.TrackCompulsory.end(); it++)
				Report << *it << ",";
			Report << endl;
		}

		if (progreq.ReqMajorElectiv == 0 && progreq.intReqMajorElectiv < 0)
		{
			Report << "Critical Issue: number of credits for Major Electives isn't achived";
			Report << ", you can add courses from the following courses: " << endl;
			for (auto it = progreq.MajorElective.begin(); it != progreq.MajorElective.end(); it++)
				Report << *it << ",";
			Report << endl;
		}
		else
			Report << "Critical Issue: number of credits for Major Electives isn't achived!! too much Major Electives!!"
			<< "Decrease the Major Electives Credits by "<<to_string(progreq.intReqMajorElectiv)<<endl;
		if (progreq.noconcentration != 0)
		{
			if (progreq.MajorCon1compulsorycredits == 0 && progreq.MajorCon2compulsorycredits == 0
				&& progreq.MajorCon3compulsorycredits == 0)
			{
				Report << "Critical Issue: number of credits for Concentration Compulsory isn't achived;" << endl;
				if (progreq.MajorCon1electivescredits == 1&&progreq.intMajorCon1electivescredits!=0)
				{
					Report << "The following courses are missed: ";
					for (auto it = progreq.con1Compulsory.begin(); it != progreq.con1Compulsory.end(); it++)
						Report << *it << ",";
					Report << endl;
				}
				if (progreq.MajorCon2electivescredits == 1 && progreq.intMajorCon2electivescredits != 0)
				{
					Report << "The following courses are missed: ";
					for (auto it = progreq.con2Compulsory.begin(); it != progreq.con2Compulsory.end(); it++)
						Report << *it << ",";
					Report << endl;
				}
				if (progreq.MajorCon3electivescredits == 1 && progreq.intMajorCon3electivescredits != 0)
				{
					Report << "The following courses are missed: ";
					for (auto it = progreq.con3Compulsory.begin(); it != progreq.con3Compulsory.end(); it++)
						Report << *it << ",";
					Report << endl;
				}
				if ((progreq.MajorCon1electivescredits == 0 && progreq.MajorCon2electivescredits == 0
					&& progreq.MajorCon3electivescredits == 0)|| (progreq.intMajorCon1electivescredits == 0
						&& progreq.intMajorCon2electivescredits == 0&& progreq.intMajorCon3electivescredits == 0))
				{
					
					Report << "You should complete concentration requirements by adding courses ";
					if(progreq.inttakenConcentrations==2)
						Report << "of two of the following concentrations Compulsory courses: " << endl;
					else 
						Report << "of one of the following concentrations Compulsory courses: " << endl;
					Report << "First Concentration Compulsory: ";
					for (auto it = progreq.con1Compulsory.begin(); it != progreq.con1Compulsory.end(); it++)
						Report << *it << ",";
					Report << endl;
					if (progreq.noconcentration > 1)
					{
						Report << "Second Concentration Compulsory: ";
						for (auto it = progreq.con2Compulsory.begin(); it != progreq.con2Compulsory.end(); it++)
							Report << *it << ",";
						Report << endl;
						if (progreq.noconcentration == 3)
						{
							Report << "Third Concentration compulsory: ";
							for (auto it = progreq.con3Compulsory.begin(); it != progreq.con3Compulsory.end(); it++)
								Report << *it << ",";
							Report << endl;
						}

					}
				}

				if (progreq.MajorCon1electivescredits == 0 && progreq.MajorCon2electivescredits == 0
					&& progreq.MajorCon3electivescredits == 0)
				{
					Report << "Critical Issue: number of credits for Concentration Electives isn't achived;" << endl;
					if (progreq.MajorCon1compulsorycredits == 1)
					{
						Report << "You should add courses from the following courses: ";
						for (auto it = progreq.con1Elective.begin(); it != progreq.con1Elective.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if (progreq.MajorCon2compulsorycredits == 1)
					{
						Report << "You should add courses from the following courses: ";
						for (auto it = progreq.con2Elective.begin(); it != progreq.con2Elective.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if (progreq.MajorCon3compulsorycredits == 1)
					{
						Report << "You should add course from the following courses: ";
						for (auto it = progreq.con3Elective.begin(); it != progreq.con3Elective.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if (progreq.MajorCon1compulsorycredits == 0 && progreq.MajorCon2compulsorycredits == 0
						&& progreq.MajorCon3compulsorycredits == 0)
					{
						if (progreq.inttakenConcentrations == 2)
							Report << "You should complete concentration requirements by adding courses from two";
						else
							Report << "You should complete concentration requirements by adding courses from one";
						Report << "of the following concentrations Electives courses: " << endl;
						Report << "First Concentration electives: ";
						for (auto it = progreq.con1Elective.begin(); it != progreq.con1Elective.end(); it++)
							Report << *it << ",";
						Report << endl;
						if (progreq.noconcentration > 1)
						{
							Report << "Second Concentration electives: ";
							for (auto it = progreq.con2Elective.begin(); it != progreq.con2Elective.end(); it++)
								Report << *it << ",";
							Report << endl;
							if (progreq.noconcentration == 3)
							{
								Report << "Third Concentration electives: ";
								for (auto it = progreq.con3Elective.begin(); it != progreq.con3Elective.end(); it++)
									Report << *it << ",";
								Report << endl;
							}

						}



					}
				}

			}

		}
		
		
	}
	pReg->checkMinor(&progreq);
	if (progreq.MinorCredits == 0)
	{
		Report << "Critical Issue: number of credits for Minor isn't achived";
		Report << ", the following courses are missed: " << endl;
		for (auto it = progreq.MinorCourses.begin(); it != progreq.MinorCourses.end(); it++)
			Report << *it << ",";
		Report << endl;
	}
	if (progreq.DoubleMajor == 1)
	{
		checkprogramreq progreq2;
		bool check = pReg->check_req2(&progreq2);
		if (check == 0)
		{
			Report << "Second Major:" << endl;
			if (progreq2.totalcredits == 0)
				Report << "Critical Issue: total number of credits less than total number of credits for program requirments;"
				<< endl;
			if (progreq2.ReqUnivCredits == 0)
			{
				Report << "Critical Issue: number of credits for Univ Compulsory isn't achived, the following courses are missed: "
					<< endl;
				for (auto it = progreq2.UnivCompulsory.begin(); it != progreq2.UnivCompulsory.end(); it++)
					Report << *it << ",";
				Report << endl;
			}
			if (progreq2.ReqUniElectiv == 0 && progreq2.intReqUniElectiv < 0)
			{
				Report << "Critical Issue: number of credits for Univ Electives isn't achived";
				Report << ", you can add courses from the following courses: " << endl;
				for (auto it = progreq2.UnivElective.begin(); it != progreq2.UnivElective.end(); it++)
					Report << *it << ",";
				Report << endl;
			}
			else
				Report << "Critical Issue: number of credits for Univ Electives isn't achived!! too much Univ Electives,"
				<< "Decrease the Univ Electives Credits by " << to_string(progreq2.intReqUniElectiv) << endl;
			if (progreq2.ReqTrackCredits == 0)
			{
				Report << "Critical Issue: number of credits for Track Compulsory isn't achived, the following courses are missed"
					<< endl;
				for (auto it = progreq2.TrackCompulsory.begin(); it != progreq2.TrackCompulsory.end(); it++)
					Report << *it << ",";
				Report << endl;
			}
			if (progreq2.ReqMajorCredits == 0)
			{
				Report << "Critical Issue: number of credits for Major compulsory isn't achived, the following courses are missed"
					<< endl;
				for (auto it = progreq2.TrackCompulsory.begin(); it != progreq2.TrackCompulsory.end(); it++)
					Report << *it << ",";
				Report << endl;
			}

			if (progreq2.ReqMajorElectiv == 0 && progreq2.intReqMajorElectiv < 0)
			{
				Report << "Critical Issue: number of credits for Major Electives isn't achived";
				Report << ", you can add courses from the following courses: " << endl;
				for (auto it = progreq2.MajorElective.begin(); it != progreq2.MajorElective.end(); it++)
					Report << *it << ",";
				Report << endl;
			}
			else
				Report << "Critical Issue: number of credits for Major Electives isn't achived!! too much Major Electives!!"
				<< "Decrease the Major Electives Credits by " << to_string(progreq2.intReqMajorElectiv) << endl;
			if (progreq2.noconcentration != 0)
			{
				if (progreq2.MajorCon1compulsorycredits == 0 && progreq2.MajorCon2compulsorycredits == 0
					&& progreq2.MajorCon3compulsorycredits == 0)
				{
					Report << "Critical Issue: number of credits for Concentration Compulsory isn't achived;" << endl;
					if (progreq2.MajorCon1electivescredits == 1 && progreq2.intMajorCon1electivescredits != 0)
					{
						Report << "The following courses are missed: ";
						for (auto it = progreq2.con1Compulsory.begin(); it != progreq2.con1Compulsory.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if (progreq2.MajorCon2electivescredits == 1 && progreq2.intMajorCon2electivescredits != 0)
					{
						Report << "The following courses are missed: ";
						for (auto it = progreq2.con2Compulsory.begin(); it != progreq2.con2Compulsory.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if (progreq2.MajorCon3electivescredits == 1 && progreq2.intMajorCon3electivescredits != 0)
					{
						Report << "The following courses are missed: ";
						for (auto it = progreq2.con3Compulsory.begin(); it != progreq2.con3Compulsory.end(); it++)
							Report << *it << ",";
						Report << endl;
					}
					if ((progreq2.MajorCon1electivescredits == 0 && progreq2.MajorCon2electivescredits == 0
						&& progreq2.MajorCon3electivescredits == 0) || (progreq2.intMajorCon1electivescredits == 0
							&& progreq2.intMajorCon2electivescredits == 0 && progreq2.intMajorCon3electivescredits == 0))
					{
						Report << "You should complete concentration requirements by adding courses ";
						if (progreq2.inttakenConcentrations == 2)
							Report << "of two of the following concentrations Compulsory courses: " << endl;
						else 
							Report << "of one of the following concentrations Compulsory courses: " << endl;
						Report << "First Concentration Compulsory: ";
						for (auto it = progreq2.con1Compulsory.begin(); it != progreq2.con1Compulsory.end(); it++)
							Report << *it << ",";
						Report << endl;
						if (progreq2.noconcentration > 1)
						{
							Report << "Second Concentration Compulsory: ";
							for (auto it = progreq2.con2Compulsory.begin(); it != progreq2.con2Compulsory.end(); it++)
								Report << *it << ",";
							Report << endl;
							if (progreq2.noconcentration == 3)
							{
								Report << "Third Concentration compulsory: ";
								for (auto it = progreq2.con3Compulsory.begin(); it != progreq2.con3Compulsory.end(); it++)
									Report << *it << ",";
								Report << endl;
							}

						}
					}

					if (progreq2.MajorCon1electivescredits == 0 && progreq2.MajorCon2electivescredits == 0
						&& progreq2.MajorCon3electivescredits == 0)
					{
						Report << "Critical Issue: number of credits for Concentration Electives isn't achived;" << endl;
						if (progreq2.MajorCon1compulsorycredits == 1)
						{
							Report << "You should add courses from the following courses: ";
							for (auto it = progreq2.con1Elective.begin(); it != progreq2.con1Elective.end(); it++)
								Report << *it << ",";
							Report << endl;
						}
						if (progreq2.MajorCon2compulsorycredits == 1)
						{
							Report << "You should add courses from the following courses: ";
							for (auto it = progreq2.con2Elective.begin(); it != progreq2.con2Elective.end(); it++)
								Report << *it << ",";
							Report << endl;
						}
						if (progreq2.MajorCon3compulsorycredits == 1)
						{
							Report << "You should add course from the following courses: ";
							for (auto it = progreq2.con3Elective.begin(); it != progreq2.con3Elective.end(); it++)
								Report << *it << ",";
							Report << endl;
						}
						if (progreq2.MajorCon1compulsorycredits == 0 && progreq2.MajorCon2compulsorycredits == 0
							&& progreq2.MajorCon3compulsorycredits == 0)
						{
							if (progreq2.inttakenConcentrations == 2)
								Report << "You should complete concentration requirements by adding courses from two";
							else
								Report << "You should complete concentration requirements by adding courses from one";
							Report << "of the following concentrations Electives courses: " << endl;
							Report << "First Concentration electives: ";
							for (auto it = progreq2.con1Elective.begin(); it != progreq2.con1Elective.end(); it++)
								Report << *it << ",";
							Report << endl;
							if (progreq2.noconcentration > 1)
							{
								Report << "Second Concentration electives: ";
								for (auto it = progreq2.con2Elective.begin(); it != progreq2.con2Elective.end(); it++)
									Report << *it << ",";
								Report << endl;
								if (progreq2.noconcentration == 3)
								{
									Report << "Third Concentration electives: ";
									for (auto it = progreq2.con3Elective.begin(); it != progreq2.con3Elective.end(); it++)
										Report << *it << ",";
									Report << endl;
								}

							}



						}
					}

				}

			}


		}

	
	}
	Report.close();
	return true;
}

ActionValidityReport:: ~ActionValidityReport()
{
}
