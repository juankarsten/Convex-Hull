/*	Andy Callaghans Convex Hull Program
	Written in C++ for Essential Algorithms Coursework - Nov 2007
 */

#include <iostream>
#include <vector>
#include "convexhull.h"
using namespace std;

int main () {
	
	time_t start, end;
	double diff;
	cout << "Andy Callaghan's Convex Hull Program!\n";
	
	// Make a new objct from the convex hull class
	convexHull hull;
	
	// Initialise coordinate vectors
	coordinateList rawCoordList;
	coordinateList orderedCoordList;
	coordinateList outputCoordList;
	
	int number;
	cout << "How many coordinates do you want?\n";
	cin >> number;
	// number = 9;
	cout << "If you want to use random coordinates\t\t type 1\n";
	cout << "If you want to specify you own\t\t\t\t type 2\n";
	
	int mode;
	cin >> mode;
	//mode = 1;
	switch (mode)
	{
		case 1:
			hull.randCoords(rawCoordList,number);
			break;
		case 2: 
			hull.readCoords(rawCoordList,number);
			break;
		default:
			cout << "Don't make me angry... you wouldn't like me when I'm angry.\nI'm being random now.\n";
			hull.randCoords(rawCoordList,number);
			break;
			
	}

	cout << "Now doing hull...\n";
	coordinateList hullList, hullList2;
	start = time (NULL);

	// Divide and Conquer Hull!
	// hullList = hull.dcHull(rawCoordList);
	
	// Graham Scan Hull!
	// hullList = hull.doGrahamHull(rawCoordList);

	// Non Divide and Conquer Hull!
	hullList = hull.doHull(rawCoordList);
	
	end = time (NULL);
	diff = end - start;
	cout << "It has taken " << diff << " seconds to compute a " << hullList.size() + 1 << " point hull from " << rawCoordList.size() + 1 << " points.\n";
//	int a;
//	cin >> a;
	// hullList2 = hull.dcHull(hullList);
	
    return 0;
}
