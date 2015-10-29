#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <sstream>
using namespace std;
// Any other includes needed

void writeToFile(ofstream &fileout, string x)
{
    fileout << x << endl;
}
struct StudentGrades
{
StudentGrades() { }
double midterm =0;
double final;
int no_of_hw;//Added for iterating number of homeworks.
std::vector<double> homework;
   
friend std::istream& operator>>(std::istream& input, StudentGrades& student_grades)
{
	double a=0,b=0,d=0,e=0,f=0; int c;
	input>>a>>b>>c;
	student_grades.midterm=a;
	student_grades.final=b;
	student_grades.no_of_hw=c;
	student_grades.homework.resize(c,0);
	if(student_grades.no_of_hw!=0)
	for(int i=0; i<c ; i++)
	{
		input >> d;
		student_grades.homework[i]=d;
	} 
	return input;
}
};

class Grader
{
public:
double midterm_weight, homework_weight, final_weight, min_grade, max_grade;//Added by anusha
size_t no_of_students, student_of_interest;//Added by anusha
//string err;
ofstream fileout;

vector<StudentGrades> s;

Grader() { }
// the weights are self-explanatory
// min_grade and max_grade are the minimum/maximum possible grades
// any grades outside of the range [min_grade, max_grade] are invalid

Grader(double midterm_weight, double homework_weight, double final_weight,
double min_grade, double max_grade) { 
	this->midterm_weight=midterm_weight;
	this->homework_weight=homework_weight;
	this->final_weight=final_weight;
	this->min_grade=min_grade;
	this->max_grade=max_grade;
}

void add_student_grades(const StudentGrades& student_grades) 
{ 
	s.push_back(student_grades);
}

double get_overall_grade_for_student(size_t student_of_interest)
{
	bool flag=true;
// throw an exception if student_of_interest is out of bounds
	std::stringstream ss;
	ss<<student_of_interest;
	try
	{	//Trying to access out of bound element
		if(student_of_interest>=no_of_students)
		throw "Error encountered when processing student ";
	}
	catch(const char * c)
	{
		writeToFile(fileout,"Error encountered when processing student "+ss.str());
		writeToFile(fileout,"Student number is out of range");
		flag=false;
	}
	if(flag==true)
	{// call check_grades()
	 	flag =check_grades(s[student_of_interest]);
	}
	if(flag!=false)//Only if all the above checks are possitive then grade the student
	{// call the private get_overall_grade_for_student()
		return get_overall_grade_for_student(s[student_of_interest]);
	}	
	else
		return -1;
}

private:

double get_overall_grade_for_student(StudentGrades& student_grades)
{
// compute the overall grade using the following algorithm
// 1. first, find the median of all the homework grades
// (refer to error_handling.cpp from Class 10)
//********Assuming that the homeworks have been given in the sorted order.	
double median;
size_t one=student_grades.no_of_hw/2;
size_t two=(student_grades.no_of_hw/2)-1;
//Calculating the Median
if(student_grades.no_of_hw%2==0)
{
	median=(student_grades.homework[one]+student_grades.homework[two])/2;
}
else
{
	median=student_grades.homework[one];
}

// 2. Use the following formula:
// overall = midterm_weight * midterm +
// final_weight * final +
// homework_weight * median
double overall= (midterm_weight * student_grades.midterm) 
				+ (final_weight * student_grades.final) 
				+ (homework_weight * median);
return overall;
}

bool check_grades(const StudentGrades& student_grades)		//DONE
{
// check the following conditions in the listed order,
// and throw an exception
// 1. if the list of homework is empty, or
// 2. if the midterm grade is out of range or
// 3. if the final grade is out of range or
// 4. if any of the homework grades is out of range
	std::stringstream ss;
	ss<<student_of_interest;
	try{
		//Trying to access out of bound element
		 
		if(student_grades.midterm>max_grade || student_grades.midterm<min_grade)
			{
				//err= student_of_interest + "\Midterm grade is out of range\n";
				throw "Midterm grade is out of range";}
		else 
			if(student_grades.final > max_grade || student_grades.final < min_grade)
				throw "Final grade is out of range";
			else
				if(student_grades.no_of_hw==0)
				{
					throw "There are no homework grades";}
				else
				{
					int i=0;
					while(i<student_grades.no_of_hw)
					{
						if(student_grades.homework[i] > max_grade || student_grades.homework[i] < min_grade)
								throw "Homework grade is out of range";	
							i++;
						}
				}
		

	}
	catch(const char * c)
	{
		

		writeToFile(fileout,"Error encountered when processing student "+ss.str());
		writeToFile(fileout,c);
		//cout<<"Error encountered when processing student "<<student_of_interest<<":\n"<< c;
		return false;
	}
}
};

int main() {

	ifstream file;
	file.open("grading_system_in.txt");//Reading file
	
	
	int i=1;
	int num_cases;
	double overall;
	Grader g;
	g.fileout.open("grading_system_out.txt",ios::out);
	StudentGrades s;
	while(!file.eof() && i!=2)//First Line of the file
	{
		file>>num_cases;
		i++;
	}
	g.s.reserve(num_cases);//Reserving some space for vector.
	for (int i = 0; i < num_cases; ++i) {
		g.fileout << "Case " << i << ":\n";
		//Taking the grades in the line
		file>>g.midterm_weight >> g.final_weight >> g.homework_weight >>  g.min_grade >> g.max_grade >> g.no_of_students >> g.student_of_interest;
		int j=0;
		g.s.resize(0);
		//Resizing the vector Struct to 0. So that it starts from 0 every time 
		//New grader comes in
		while(!file.eof() && j<g.no_of_students)
		{
			file>>s;//Read student info
			g.add_student_grades(s);//Adding it to grader list
			j++;
		}
		overall=g.get_overall_grade_for_student(g.student_of_interest);
		if(overall!=-1)//When the grading has been done
		g.fileout<<"Overall grade of student:"<<g.student_of_interest<<"\n"<< overall<<endl;
	}
		
	file.close();//Closing the file
	g.fileout.close();
	return 0;
}


