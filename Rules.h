#pragma once

//This file contains data sturctures required to store all registration RULES
#include <vector>
using namespace std;
#include "DEFs.h"


//Information for each course
//Title, code, credit hours, and pre-req list, type
struct CourseInfo
{
	string Title;
	Course_Code	Code;
	vector<Course_Code> PreReqList;	//Pre-requesite list of courses codes
	vector<Course_Code> CoReqList;	//Co-requesite list of courses codes
	int Credits;
	string type;	//Univ, track, major course
};

struct AcademicYearOfferings
{
	string Year; //academic year of the offerings e.g. 2019-2020


	//Each year has an array of of 3 vectors of courses CODE. 
	//Each vector cossrsponds to a semester
	//So Offerings[FALL] is a vector of courses offered in FALL in THAT year
	//So Offerings[SPRING] is a vector of courses offered in SPRING in THAT year
	//So Offerings[SUMMER] is a vector of courses offered in SUMMER in THAT year
	vector<Course_Code> Offerings[SEM_CNT];


};

struct Rules	//contains all objects for registrations rules
{
	// TODO: add more fields if needed

	vector<CourseInfo> CourseCatalog;	//List of ALL courses with full info
	vector<AcademicYearOfferings> OffringsList;	//all offerings for all acedmic years

	int SemMinCredit=12;		//min no. of credit hours per semester
	int SemMaxCredit=18;		//max no. of credit hours per semester
	int summerMaxCredit = 6;
/*1*/
	int totalcredits=0;
	/*2*/
	int ReqUnivCredits=0; //total no. of credits req by Univ courses
	int ReqUniElectiv=0;
	/*3*/
	int ReqTrackCredits=0; //total no. of credits req by Track courses
	/*4*/
	int ReqMajorCredits=0; //total no. of credits req by Major courses
	int ReqMajorElectiv=0;
	//Errorr Handling in diff majors
	/*5*/
	int noconcentration=0;    //no.of concentrations
	/*6*/
	int MajorCon1compulsorycredits=0;
	int MajorCon1electivescredits=0;
	int MajorCon2compulsorycredits=0;
	int MajorCon2electivescredits=0;
	int MajorCon3compulsorycredits=0;
	int MajorCon3electivescredits=0;
	/*7*/
	vector<Course_Code> UnivCompulsory; //Univ Compulsory courses
	/*8*/
	vector<Course_Code> UnivElective; //Univ Elective courses
	/*9*/
	vector<Course_Code> TrackCompulsory;//Track Compulsory courses

	vector<Course_Code> TrackElective; //Track Elective courses (added for future)
	/*10*/
	vector<Course_Code> MajorCompulsory;//Major Compulsory courses
	/*11*/
	vector<Course_Code> MajorElective; //Major Elective courses
	/*12*/
	vector<Course_Code> con1Compulsory;//con1 Compulsory courses
	/*13*/
	vector<Course_Code> con1Elective; //con1 Elective courses
	/*14*/
	vector<Course_Code> con2Compulsory;//con1 Compulsory courses
	/*15*/
	vector<Course_Code> con2Elective; //con1 Elective courses
	/*16*/
	vector<Course_Code> con3Compulsory;//con1 Compulsory courses
	/*17*/
	vector<Course_Code> con3Elective; //con1 Elective courses
	int MinorCredits = 0;
	vector<Course_Code> MinorCourses;
	int takenConcentrations = 0;
	bool DoubleMajor = 0;

};

