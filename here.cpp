Hello
#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <string>
#include "Student.cpp"
#include "Department.cpp"
#include "Fine.cpp"

using namespace std;

vector<Student>  students;
vector<Department> departments;
vector<Fine> fines;
vector<int> printed;

bool isStudentValid(int studentID)
{
	for (int i = 0; i < students.size(); i++)
	{
		if (students[i].id() == studentID)
			return true;
	}
	return false;
}

bool isDepartmentValid(int departmentID)
{
	for (int i = 0; i < departments.size(); i++)
	{
		if (departments[i].id() == departmentID)
			return true;
	}
	return false;
}

bool isMajorValid(string major)
{
	for (int i = 0; i < students.size(); i++)
	{
		if (students[i].major() == major)
			return true;
	}
	return false;
}

bool doesStudentFineExist(int studentID)
{
	for (int i = 0; i < fines.size(); i++)
	{
		if ((fines[i].studentID() == studentID) && (fines[i].amount() != 0))
			return true;
	}
	return false;
}

/* Check which students have been printed so no duplicates in department reports */
bool isAlreadyPrinted(int studentID)
{
	for (int i = 0; i < printed.size(); i++)
	{
		if (printed[i] == studentID)
			return true;
	}
	return false;
}

void printStudentName(int studentID)
{
	for (int i = 0; i < students.size(); i++)
	{
		if (students[i].id() == studentID)
			cout << students[i].firstName() << " " << students[i].lastName() << endl; 
	}
}

void studentsWithDepartmentFines(int departmentID)
{
	for (int i = 0; i < fines.size(); i++)
	{
		if (fines[i].departmentID() == departmentID && !isAlreadyPrinted(fines[i].studentID()))
		{
			printStudentName(fines[i].studentID());
			printed.push_back(fines[i].studentID());
		}
	}
	printed.clear();
}

/* Makes a payment on a fine and returns the amount that is still due */
float payFine(int studentID, float payment, Date payday)
{
	float zero = 0.0;
	for (int i = 0; i < fines.size(); i++)
	{
		if ((fines[i].studentID() == studentID) && (fines[i].amount() != zero))
		{
			int monthsPassed = payday.numMonthsPassed(fines[i].date());
			fines[i].applyInterest(monthsPassed);
			float fineAmount = fines[i].amount();
			if (fineAmount >= payment)
			{
				fines[i].processNewPayment(payment, payday);
				return zero;
			}
			else
			{
				fines[i].processNewPayment(fineAmount, payday);
				return payment - fineAmount;
			}		
		}
	}
}

/* Updates the total amount of payments a student has made and pays fines */
void processPayment(int studentID, float payment, Date payday)
{	
	for (int i = 0; i < students.size(); i++)
	{
		if (students[i].id() == studentID)
			students[i].addToPayments(payment);
	}
	float amountUnpaid;
	amountUnpaid = payFine(studentID, payment, payday);
	while (amountUnpaid != 0)
	{
		amountUnpaid = payFine(studentID, amountUnpaid, payday);
	}
}

/* Keeps track of the total amount of fines a student collects */
float totalFineAmount(int studentID)
{
	float amount = 0.0;
	for (int i = 0; i < fines.size(); i++)
	{
		if (fines[i].studentID() == studentID)
			amount += fines[i].amount();
	}
	return amount;
}

int main()
{
	bool isDone = false;
	char operation;
	Student newStudent;
	Department newDepartment;
	Fine newFine;
	
	while (isDone == false)
	{
		cout << "Please enter what you would like to do: ";
		string input;
		getline(cin, input);
		stringstream ss;
		ss << input; 
		ss >> operation;
		switch(operation)
		{
			case 'S':
			case 's':
			   {
				ss >> newStudent;
				students.push_back(newStudent);
				cout << "\n" << newStudent.firstName() << " " << newStudent.lastName() << " has been added as a new student.\n\n";	
			   }
			   break;
			case 'D':
			case 'd':
			   {
				ss >> newDepartment;
				departments.push_back(newDepartment);
				cout << "\n" << newDepartment.departmentName() << " has been added as a new department.\n\n";
			   }
			   break;
			case 'F':
			case 'f':
			   {	
				ss >> newFine;
				fines.push_back(newFine);
				cout << "\nA fine of $" << newFine.amount() << " has been added to the student's record.\n";
				cout << "Student " << newFine.studentID() << " now owes: $" << totalFineAmount(newFine.studentID()) << "\n\n";
			   }
			   break;
			case 'P':
			case 'p':
			   {
				/* Adds 'P' to the front of the stringstream and determines which operation is being called */
				const string &temp = ss.str();
				ss.seekp(0);
				ss << operation;
				ss << temp;
				string pOperation;
				ss >> pOperation;
				if (pOperation == "P" || pOperation == "p")
				{
					int studentID;
					float payment;
					Date payDay;
					ss >> studentID >> payment >> payDay;
					if (!doesStudentFineExist(studentID))
					{
						cout << "Sorry, student " << studentID << " does not have any fines to pay for.\n\n";
					}
					else
					{
						processPayment(studentID, payment, payDay);
						cout << "\nStudent " << studentID << " paid $" << payment << ". ";
						cout << "They now owe: $" << totalFineAmount(studentID) << "\n\n";
					}
				}	
				else if (pOperation == "PS" || pOperation == "ps" || pOperation == "Ps" || pOperation == "pS")
				{
					int studentID;
					ss >> studentID;
					if (!isStudentValid(studentID))
					{
						cout << "Sorry, there is no student with that ID.\n\n";
					}
					else
					{
						cout << "\nReport for ";
						printStudentName(studentID);
						cout << "Fines:\n";
						for (int i = 0; i < fines.size(); i++)
						{
							if (fines[i].studentID() == studentID)
							{
								cout << "-Fine for: " << fines[i].type() << "\t\tAmount due: $" << fines[i].amount() << "\t\tDate: " << fines[i].date() << ".\n";
							}
						}
						cout << "Total amount paid: $";
						for (int i = 0; i < students.size(); i++)
						{
							if (students[i].id() == studentID)
								cout << students[i].paid();
						}
						cout << "\nTotal amount due: $" << totalFineAmount(studentID) << "\n\n";
					}
				}
				else if (pOperation == "PD" || pOperation == "pd" || pOperation == "Pd" || pOperation == "pD")
				{
					int departmentID;
					ss >> departmentID;
					if (!isDepartmentValid(departmentID))
					{
						cout << "Sorry, there is no department with that ID.\n\n";
					}
					else
					{
						for (int i = 0; i < departments.size(); i++)
						{
							if (departments[i].id() == departmentID)
							{
								cout << "\nReport for " << departments[i].departmentName() << " department:\n";
								cout << "Students who have collected fines:\n";
								studentsWithDepartmentFines(departmentID);	
							}
						}
						cout << "\n";
					}
				}
				else if (pOperation == "PM" || pOperation == "pm" || pOperation == "Pm" || pOperation == "pM")
				{
					string major;
					ss >> major;
					if (!isMajorValid(major))
					{
						cout << "Sorry, there are no students in that major.\n";
					}
					else
					{
						cout << "\nReport for " << major << ":\n";
						cout << "Students who have collected fines:\n";
						for (int i = 0; i < students.size(); i++)
						{
							if ((students[i].major() == major) && (doesStudentFineExist(students[i].id())))
							{
								printStudentName(students[i].id());
							}
						}
						cout << "\n";
					}
				}
				else
				{
					break;
				}
			   }
			   break;
			case 'Q':
			case 'q':
			   {
				isDone = true;
			   }
			   break;
			default:
				cout << "Please use one of the following operations or enter 'q' to exit:\n"
				     << "\tS - add a student\n\tD - add a department\n\tF - issue a fine\n"
				     << "\tP - process a payment\n\tPS - print a report for a student\n"
				     << "\tPD - print a report for a department\n\tPM - print a report for a major\n\n";	
		}
	}
}
