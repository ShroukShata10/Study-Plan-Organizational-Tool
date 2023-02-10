#pragma once
#include "GUI\GUI.h"
#include "Actions\Action.h"
#include "Rules.h"
#include "StudyPlan/StudyPlan.h"


//The maestro class for the application
class Registrar
{
	GUI *pGUI;	//pointer to GUI 
	Rules RegRules;	//Registration rules
	Rules RegRules2;
	StudyPlan *pSPlan;
	string filename1;
	string filename2;

public:
	Registrar();
	GUI* getGUI() const;
	Action* CreateRequiredAction();
	bool ExecuteAction(Action*);
	void UpdateInterface();
	StudyPlan* getStudyPlan() const;
	void ConcReq(string name);
	void load_courseInfo();
	void Run();
	void Programreq(string filename);
	void Programreq2(string filename);
	graphicsInfo getnewpostion(int x, int y);
	CourseInfo getCourseInfo(Course_Code code) const;
	bool check_pre_cor();
	Course_Type coursetype(Course_Code code) const;
	bool checkcredits();
	bool check_req(checkprogramreq *progreq);
	bool check_req2(checkprogramreq* progreq);
	bool checkMinor(checkprogramreq* progreq);
	void ImportCourseOfferingTime();
	bool checktimeoffering();
	void AddMinorCourse(Course_Code code,int c);
	void set2Concentration();
	void set2Majors();
	void totalchecker();
	~Registrar();
};

