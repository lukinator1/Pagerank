//Jared Dawson
//Assignment 2 COP3530
//8-1-19
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iterator>
#include <vector>
#include <iomanip> 
using namespace std;
int main(int argc, char ** argv)
{
	ifstream myfile;
	string buffer = "";
	map<int, string> pagemap;
	map<string, float> alphabeticalmap;
	int linecount = 0;
	int poweriterations = 0;
	int totalpages = 0;

	myfile.open(argv[1]);
	getline(myfile, buffer, ' ');
	stringstream streambuffer(buffer);
	streambuffer >> linecount;
	getline(myfile, buffer);
	stringstream streambuffertwo(buffer);
	streambuffertwo >> poweriterations; 
	for (int i = 0; i < linecount; i++) {  //build map
		getline(myfile, buffer, ' '); //1st page
		bool toemplace = true;
		for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++) {  //check if it's already in the map
			if (iterator->second == buffer) {
				toemplace = false;
			}
		}
		if (toemplace == true)
		{
			pagemap.emplace(totalpages, buffer);
			totalpages++;
		}
		getline(myfile, buffer); //2nd page
		toemplace = true;
		for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++) {  //check if it's already in the map
			if (iterator->second == buffer) {
				toemplace = false;
			}
		}
		if (toemplace == true)
		{
			pagemap.emplace(totalpages, buffer);
			totalpages++;
		}
	}
	myfile.clear();
	myfile.seekg(0, ios::beg);
	getline(myfile, buffer);

	vector <vector<float>> matrix(totalpages);  //regular adjacency matrix
	vector <float> column(totalpages);
	for (unsigned int i = 0; i < matrix.size(); i++) {
		matrix[i] = column;
	}

	string from = "";
	string to = "";
	int rowindex = 0;
	int columnindex = 0;

	for (int i = 0; i < linecount; i++) {  //get degrees
		getline(myfile, buffer, ' ');
		from = buffer;
		getline(myfile, buffer);
		to = buffer;
		rowindex = 0;
		columnindex = 0;
		for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++) {    //find what it is in the map
			if (iterator->second == to) {
				rowindex = iterator->first;
			}
		}
		for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++) {
			if (iterator->second == from) {
				columnindex = iterator->first;
			}
		}
		matrix[rowindex][columnindex]++;
	}
	myfile.clear();
	myfile.seekg(0, ios::beg);
	getline(myfile, buffer);

	vector <vector<float>> pagerankmatrix(totalpages);  //make pagerank matrix
	vector <float> pagerankcolumn(totalpages);
	for (unsigned int i = 0; i < pagerankmatrix.size(); i++) {
			pagerankmatrix[i] = pagerankcolumn;
	}

	float outdegree = 0.0;
	for (unsigned int i = 0; i < pagerankmatrix.size(); i++) { //add in values using previous matrix
		for (unsigned int u = 0; u < pagerankmatrix.size(); u++) {
			if (matrix[i][u] != float(0.0)) {
				for (unsigned int w = 0; w < matrix.size(); w++) {   //calculate out-degree
					outdegree += matrix[w][u];
				}
				pagerankmatrix[i][u] = (float(1.0)/outdegree);
				outdegree = 0.0;
			}
		}
	}

	vector <float> tempvector(pagerankmatrix.size());
	for (unsigned int i = 0; i < tempvector.size(); i++) {
		tempvector[i] = (float(1.0) / totalpages);
	}
	vector <float> rankvector = tempvector;

	float pagerank = 0.0;				//calculate pagerank
	for (int i = 1; i < poweriterations; i++) {			//iterations
		for (unsigned int u = 0; u < pagerankmatrix.size(); u++) {     ///rank vector
			for (unsigned int w = 0; w < pagerankmatrix.size(); w++) {   //columns
				pagerank += (pagerankmatrix[u][w] * tempvector[w]);
			}
			rankvector[u] = pagerank;
			pagerank = 0;
		} 
		tempvector = rankvector;
	}
	int counter = 0;
	/*for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++)
	{
		cout << iterator->second << " " << fixed << setprecision(2) << rankvector[counter] << endl;
	}*/
	for (auto iterator = pagemap.begin(); iterator != pagemap.end(); iterator++) {
		alphabeticalmap.emplace(iterator->second, rankvector[counter]);
		counter++;
	}
	for (auto iterator = alphabeticalmap.begin(); iterator != alphabeticalmap.end(); iterator++)
	{
		cout << iterator->first << " " << fixed << setprecision(2) << iterator->second << endl;
	}
	myfile.close();
}