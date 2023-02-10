#pragma once

//This header file contains some defenitions to be used all over the application
#include <string>
#include <vector>
using namespace std;
typedef string Course_Code;


//Semesters
enum SEMESTER
{
	FALL,
	SPRING,
	SUMMER,
	SEM_CNT	//number of semesters 
};
struct YearSem
{
	SEMESTER sem;
	int year;

};
enum CourseStatus
{
	Done,
	InProgress,
	Pending,
	Replaced
};
//All possible actions
enum ActionType
{
	ADD_CRS,	//Add a course to study plan
	DEL_CRS,	//Delete a course from study plan
	REPLACE_CRS, //replace course 

	SAVE,		//Save a study plan to file
	LOAD,		//Load a study plan from a file

	UNDO,		//Undo the last Action preformed
	REDO,		//Redo the last Action canceled
	Add_Note, //Add Note about the current plan
	Reorder_Course,
	CRS_Info,
	CRS_Status,
	Filter,
	Report,
	Major,
	minor,
	Concentration,
	GPA,
	Grade,
	Level,
	EXIT,		//Exit the application

	STATUS_BAR,	//A click on the status bar
	MENU_BAR,	//A click on an empty place in the menu bar
	DRAW_AREA,	//A click in the drawing area

	CANCEL,		//ESC key is pressed

	//TODO: Add more action types

};

//to sotre data related to the last action
struct ActionData
{
	ActionType actType;
	int x, y;
	
};
enum Course_Type
{
	UnivCompulsory,
	UnivElective,
	MajorElective,
	MajorCompulsory,
	TrackCompulsory,
	Concentration1Compulsory,
	Concentration1Elective,
	Concentration2Compulsory,
	Concentration2Elective,
	Concentration3Compulsory,
	Concentration3Elective,
	Minor,
	NoType
};
enum GPA_letter
{
	A,
	Aminus,
	Bplus,
	B,
	Bminus,
	Cplus,
	C,
	Cminus,
	F,

};
struct checkprogramreq
{
	bool totalcredits = 0;
	bool ReqUnivCredits=0;
	bool ReqUniElectiv=0;
	bool ReqTrackCredits=0;
	bool ReqMajorCredits=0; 
	bool ReqMajorElectiv=0;
	bool MajorCon1compulsorycredits = 0;
	bool MajorCon1electivescredits = 0;
	bool MajorCon2compulsorycredits = 0;
	bool MajorCon2electivescredits = 0;
	bool MajorCon3compulsorycredits = 0;
	bool MajorCon3electivescredits = 0;
	bool MinorCredits = 1;
	bool DoubleMajor = 0;
	//integers
	int SemMinCredit = 12;		//min no. of credit hours per semester
	int SemMaxCredit = 18;		//max no. of credit hours per semester
	int SummerMaxXredit = 6;

	int inttotalcredits = 0;
	int intReqUnivCredits = 0; //total no. of credits req by Univ courses
	int intReqUniElectiv = 0;
	int intReqTrackCredits = 0; //total no. of credits req by Track courses
	int intReqMajorCredits = 0; //total no. of credits req by Major courses
	int intReqMajorElectiv = 0;
	int noconcentration = 0;    //no.of concentrations
	int intMajorCon1compulsorycredits = 0;
	int intMajorCon1electivescredits = 0;
	int intMajorCon2compulsorycredits = 0;
	int intMajorCon2electivescredits = 0;
	int intMajorCon3compulsorycredits = 0;
	int intMajorCon3electivescredits = 0;
	int intMinorCredits = 0;
	int inttakenConcentrations = 0;
	vector<Course_Code> MinorCourses;
	vector<Course_Code> UnivCompulsory; //Univ Compulsory courses
	vector<Course_Code> UnivElective; //Univ Elective courses
	vector<Course_Code> TrackCompulsory;//Track Compulsory courses
	vector<Course_Code> TrackElective; //Track Elective courses (added for future)
	vector<Course_Code> MajorCompulsory;//Major Compulsory courses
	vector<Course_Code> MajorElective; //Major Elective courses
	vector<Course_Code> con1Compulsory;//con1 Compulsory courses
	vector<Course_Code> con1Elective; //con1 Elective courses
	vector<Course_Code> con2Compulsory;//con1 Compulsory courses
	vector<Course_Code> con2Elective; //con1 Elective courses
	vector<Course_Code> con3Compulsory;//con1 Compulsory courses
	vector<Course_Code> con3Elective; //con1 Elective courses
};



