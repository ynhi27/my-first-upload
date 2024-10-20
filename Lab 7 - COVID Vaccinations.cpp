/*Y Nhi Tran
Email: tranynhi27@gmail.com
Lab7 - COVID Vaccinations*/
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

#define MAX 1000

int getData(ifstream& inFile, string county[], string demographic[], double pct1[], double pct2[], double pct3[], double pct4[]);
void output (ofstream& outFile, string county[], string demographic[], double pct1[], double pct2[], double pct3[], double pct4[], int num);
int search (string county[], string name, int n);
void search2 (ofstream& outFile, string county[], string demographic[], double pct1[], int n);
void sortSelect (double pct[], int n, string county[], string demographic[]);

int main(void)
{
	//Declare variables
    ifstream inFile;
    string county[MAX];
    string demographic[MAX];
    string name;
	double pct1[MAX], pct2[MAX], pct3[MAX], pct4[MAX];
    
    int number, loc, location;
    double key;
	ofstream outFile;
	
	//Open files
	outFile.open ("Tran_lab7.txt");
	
	inFile.open("Lab 6&7 Data - COVID Vaccination.txt");
	if (!inFile.is_open()) {
		cout << "Failed to open this file!" << endl;
		system ("pause");
		exit (100);
	}
	
	//Input the entire data of the input file
	number = getData(inFile, county, demographic, pct1, pct2, pct3, pct4);
	
	//Output the data file with 4 percentages
	output (outFile, county, demographic, pct1, pct2, pct3, pct4, number);
	
	//Looking for a county with % fully vaccinated with respect to the 12+ population (pct3[])
	cout << "Enter a name of county: ";
	getline(cin, name);
	
	loc = search (county, name, number);
	if (loc == -1)
		cout << "No statistics for " << name << " county." << endl;
	else
	{
		cout << setw (30)<< "Demographic" << setw (35) << "% Fully Vaccinated\n"
			 << setw (65) << "__________________\n" << setw (64) << "12+ population\n";
		while (loc != -1 && county[loc] == name)
		{
			cout << left << setw (52) << demographic[loc];
			cout << setprecision (1) << fixed;
			cout << setw (5) << right << pct3[loc] << endl;
			loc++;
		}
	}
		
	//Prompt user for percent from 1 - 100 and output to the file
	search2 (outFile, county, demographic, pct1, number);

	//Sort pct3[i] in descending order
	sortSelect (pct3, number, county, demographic);
	
	//Output after sorting again the data file
	output (outFile, county, demographic, pct1, pct2, pct3, pct4, number);

	//Close both files
	inFile.close();
	outFile.close();
	
	system("pause");
	return 0;
}//main
////////////////////////////////////////////////////
int getData(ifstream& inFile, string county[], string demographic[], double pct1[], double pct2[], double pct3[], double pct4[])
{
	/*Pre: inFile - reference to the data file
		   county[] - array of name of county
		   demographic[] - array of demographic in this county
		   pct1[] - array of % people fully vaccinated with respect to the est_population
		   pct2[] - array of % people partially vaccinated with respect to the est_population
		   pct3[] - array of % people fully vaccinated with respect to the est_age_12plus_pop
		   pct4[] - array of % people partially vaccinated with respect to the est_age_12plus_pop
	  Post: how many counties
	  Purpose: Input the name of county, the demographic of this county, and 4 percentages (that
	  		   mention above).
	*/
	int i = 0;
	int num1, num2, num3, num4;
	string temp;
	while (i < MAX && !inFile.eof())
	{
        //Input the county name
		getline (inFile, county[i]);
        //Input the demographic of this county
		getline (inFile, demographic[i]);
        //Input 4 values and calculate the percentages
    	cout << setprecision (1) << fixed;
		inFile >> num1 >> num2 >> num3 >> num4;
		if (num1 > 0 || num2 > 0)			//Calculate when the population greater than 0
		{
			pct1[i] = (double)num3/num1*100;
			pct2[i] = (double)num4/num1*100;
			pct3[i] = (double)num3/num2*100;
			pct4[i] = (double)num4/num2*100;
		}
		getline (inFile, temp);
		i++;
   	}
	return i;
}//getData
////////////////////////////////////////////////////
int search (string county[], string name, int n)
{
	/*Pre: county[] - array of name of county
		   name - the values that user prompt
		   n - number of defined values
	  Post: location of what we are looking for or -1 if not found
	  Purpose: Find the location of county
	*/
	int loc = -1;
	for (int j = 0; j < n; j++)
	{
		if (county[j] == name)
		{
			loc = j;
			break;
		}
	}
	return loc;
}//search
///////////////////////////////////////////////////
void search2 (ofstream& outFile, string county[], string demographic[], double pct1[], int n)
{
	/*Pre: county[] - array of name of county
		   demographic[] - array of demographic of this county
		   pct1[] - array of % people fully vaccinated with respect to the est_population
		   n - number of defined values
	  Post: nothing
	  Purpose: Find the values that smaller than the value gets from user and export to the file
	*/
	int flag = 0, key;
	cout << "Enter the percent between 1 and 100: ";
	cin >> key;
	
	outFile << "Enter the percent between 1 and 100: " << key << endl;
	outFile << setw (8) << "\tCounty" << setw (30)<< "Demographic"
			<< setw (41) << "% Fully Vaccinated\n"
			<< setw (85) << "__________________\n" << setw (84) << "Total population\n";
		
	for (int i = 0; i < n; i++)
	{
	    if (pct1[i] < key)
		{
		    outFile << left << setw (18) << county[i]
					<< left << setw (52) << demographic[i];
		    outFile << setprecision (1) << fixed;
		    outFile << setw (8) << right << pct1[i] << endl;
		    flag = 1;
			}
	}
	if (flag == 0)
	{
	    outFile << "No group with fully vaccinated rate below " << key << ".\n";
	}
	outFile << endl;
}//search2
///////////////////////////////////////////////////
void sortSelect (double pct[], int n, string county[], string demographic[])
{
	/* Pre: pct[] - array of values
			n - number of percentage
			county[] - array of name of county
			demographic[] - array of demographic of this county
	   Post: nothing
	   Purpose: sort percentages in descending order
	*/
	int step; int current;
	int smallest; double tempDouble; string tempStr;
	
	for (current = 0; current <= n - 1; current++)
	{
		for (step = current + 1; step <= n - 1; step++)
		{
			if (pct[current] < pct[step])
			{
			    //Swap
		        tempDouble = pct[current];
		        pct[current] = pct[step];
		        pct[step] = tempDouble;
		
	        	tempStr = county[current];
		        county[current] = county[step];
		        county[step] = tempStr;
		
		        tempStr = demographic[current];
		        demographic[current] = demographic[step];
		        demographic[step] = tempStr;
			}
		}//for step
	}//for current
}//sortSelect
///////////////////////////////////////////////////
void output (ofstream& outFile, string county[], string demographic[], double pct1[], double pct2[], double pct3[], double pct4[], int num)
{
	/*Pre: outFile - reference to the output data file
		   county[] - array of name of county
		   demographic[] - array of demographic in this county
		   pct1[] - array of % people fully vaccinated with respect to the est_population
		   pct2[] - array of % people partially vaccinated with respect to the est_population
		   pct3[] - array of % people fully vaccinated with respect to the est_age_12plus_pop
		   pct4[] - array of % people partially vaccinated with respect to the est_age_12plus_pop
		   num - the number of values
	  Post: nothing
	  Purpose: Output the name of county, the demographic of this county, and 4 percentages (that
	  		   mention above).
	*/
	int num1, num2, num3, num4;
	outFile << setw (8) << "\tCounty" << setw (30)<< "Demographic"
			<< setw (35) << "% Fully"  << setw (14) << "% Partially"
			<< setw (10) << "% Fully"  << setw (16) << "% Partially\n"
			<< setw (80) << "Vaccinated" << setw (13) << "Vaccinated"
			<< setw (12) << "Vaccinated" << setw (14) << "Vaccinated\n"
			<< setw (80) << "__________" << setw (13) << "__________"
			<< setw (12) << "__________" << setw (14) << "__________\n"
			<< setw (80) << "Total pop." << setw (13) << "Total pop."
			<< setw (12) << "12+ pop." << setw (14) << "12+ pop.\n";
	for (int i = 0; i < num; i++)
	{
		outFile << left << setw (18) << county[i]
				<< left << setw (52) << demographic[i];
		outFile << setprecision (1) << fixed;
		
		outFile << setw (8) << right << pct1[i]
				<< setw (13) << right << pct2[i]
				<< setw (12) << right << pct3[i]
				<< setw (12) << right << pct4[i] << endl;
	}
	outFile << "\nNotes: Total pop. is the total population for the demographic in the county.\n"
			<< "       12+ pop. is the population 12+ years of age for the demographic in the county.\n";
	
	//Signature
	outFile << endl;
	outFile << "Programmed by: Y Nhi Tran" << endl;
	outFile << "Email: tranynhi27@gmail.com" << endl;
	outFile << "Lab 7 - COVID Vaccinations\n" << endl;
	
	return;
}//output