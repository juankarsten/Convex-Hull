#include <vector>
#include <iostream>
#include <sstream>
#include <time.h>
using namespace std;
#define MAXCOORD 100

struct coordinate {
	int x;
	int y;
	friend bool operator== (const coordinate& lhs, const coordinate& rhs)
	{
		if (lhs.x == rhs.x) {
			if (lhs.y == rhs.y)
				return true;		// The two coords are equal
		}
		return false;
	}	
	friend bool operator> (const coordinate& lhs, const coordinate& rhs)
	{
		if (lhs.x > rhs.x) {
			return true;
		}
		else if (lhs.x < rhs.x) {
			return false;
		}
		else if (lhs.x == rhs.x) {
			if (lhs.y > rhs.y) {
				return true;
			}
			else if(lhs.y < rhs.y) {
				return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	
	friend bool operator< (const coordinate& lhs, const coordinate& rhs)
	{
		if (lhs.x < rhs.x) {
			return true;
		}
		else if (lhs.x > rhs.x) {
			return false;
		}
		else if (lhs.x == rhs.x) {
			if (lhs.y < rhs.y) {
				return true;
			}
			else if(lhs.y > rhs.y) {
				return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	
};

// From : www.thescripts.com/forum/thread62219.html
template<typename T>
T FromStringEx( const std::string& s)
// string conversion from string to typename T with a flag
// the represents success or failure.
//
// e.g: int d = FromString<int>( s );
{
	istringstream is(s);
	T t;
	is >> t;
	return t;
}

typedef vector<coordinate> coordinateList;

class convexHull {
	
public:
	void readCoords (coordinateList &, int);
	void randCoords (coordinateList &, int);
	coordinateList dcHull (coordinateList &);
	coordinateList doHull (coordinateList &);
	coordinate getCoord (coordinateList &, int);
	coordinateList merge(coordinateList, coordinateList );
	coordinateList merge_back(coordinateList, coordinateList );
	int findCoord(coordinateList, coordinate);
	coordinateList dcMerge(coordinateList, coordinateList);
	void printList (coordinateList &);
	// For Graham Scan Hull
	coordinateList doGrahamHull(coordinateList &);
	coordinateList grahamScan(coordinateList &);
private:
	coordinateList doLowerHull (coordinateList );
	coordinateList doUpperHull (coordinateList );
	void split (coordinateList &,coordinateList &,coordinateList &);
	void putCoord (coordinateList &,int,int);
	void putCoord (coordinateList &,coordinate); // overridden for coordinate input
	void delCoord (coordinateList &,int);
	bool isCoord  (coordinateList &,int);
	void delDuplicates(coordinateList &); 
	coordinateList reverse(coordinateList &);
protected:
	int findTurn(coordinate, coordinate, coordinate);
	
};

void convexHull::randCoords (coordinateList &L, int number = 10)
{
	cout << "I'm now being random...\n";
	int x = 0, y = 0;
	
	coordinate c;
	srand ( time(NULL) );
	for (int n = number;n > 0;n--)
	{
		
		x = rand() % MAXCOORD +1;
		y = rand() % MAXCOORD +1;
		
		cout << "x = " << x;
		cout << "\ty = " << y << "\n";
		
		c.x = x;
		c.y = y;
		
		L.push_back(c);
		
		// Move onto the next vector space
		number--;
	}
}

void convexHull::readCoords (coordinateList &L, int number = 10)
{
	cout << "You will now input in your coordinates to be 'hulled!\n";
	cout << "Please enter your coordinates now. \t x then y\n";
	cout << "Between 0 and 200 for both\n\n";
	string inx = "", iny = "";
	int x = 0, y = 0;
	// Where are we in the vector? (position)
	int pos = 0;
	
	while (number > 0)
	{
		cout << "x = ";
		cin >> inx;
		cout << "\t\tGot it!\n";
		x = FromStringEx<int>(inx);
		
		cout << "y = ";
		cin >> iny;
		cout << "\t\tGot it!\n";
		y = FromStringEx<int>(iny);
		
		cout << "L.size() = " << L.size();
		
		coordinate c;
		
		c.x = x;
		c.y = y;
		
		L.push_back(c);
		
		// Reset strings and ints
		inx = "";
		iny = "";
		x = 0;
		y = 0;
		
		// Move onto the next vector space
		pos++;
		number--;
	}
	if (L.size() <= 3) {
		cout << "Sorry, a convex hull needs at least 3 points to work.\n";
		int req = 3 - L.size();
		for (int left = req; left >= 0; left--) {
			if (left > 1) {
				cout << "Please enter another " << left << " more points.\n";
			}
			else {
				cout << "Please enter another point.\n";
			}
			cout << "x = ";
			cin >> inx;
			cout << "\t\tGot it!\n";
			L[pos].x = FromStringEx<int>(inx);
			
			cout << "y = ";
			cin >> iny;
			cout << "\t\tGot it!\n";
			L[pos].y = FromStringEx<int>(iny);
			
			L[L.size()].x = x;
			L[L.size()].y = y;
			
			// Reset strings
			inx = "";
			iny = "";
			y = 0;
			x = 0;
			
			// Move onto the next vector space
			pos++;
		}
	}
	else {
		cout << "That's it. We have them all " << L.size() << " points now.\n\n";
	}
}

void convexHull::delCoord (coordinateList &P,int a)
{
	P.erase (P.begin()+a);
	cout << "Deleting the " << a << "th element." << endl;
}

void convexHull::putCoord(coordinateList &P,int x,int y) {
	coordinate a;
	a.x = x;
	a.y = y;
	P.push_back(a);
	cout << "Pushing x = " << x << ", y = " << y << endl;
}

void convexHull::delDuplicates(coordinateList &P) {
	unsigned int Psize = P.size();
	for (unsigned int a = 0; a < Psize; a++){
		for (unsigned int b = 0; b < Psize; b++){
			if (P[b] == P[a] && a != b) {	// If we find a duplicate
				delCoord(P,b);	// Delete it
				Psize--;		// And decement the size
				cout << "Found a duplicate with position " << a << " and " << b << endl;
			}
			if  (P[b].x > MAXCOORD || P[b].y > MAXCOORD) {
				delCoord(P,b);	// Delete out of scope coordinates
				Psize--;		// And decement the size
				cout << "Found a coordinate which is out of scope at position " << b << endl;
			}
		}
	}
}

void convexHull::putCoord(coordinateList &P,coordinate a) {
	P.push_back(a);
	cout << "Pushing x = " << a.x << ", y = " << a.y << endl;
}

void convexHull::printList(coordinateList &L) {
	cout << "\n";
	for(unsigned int i=0; i < L.size(); i++)
	{
		cout << "x = " << L[i].x << "\ty = " << L[i].y << endl;
	}
	cout << "\n";
}

// Does the given coordinate actually exist
bool convexHull::isCoord(coordinateList &P, int position = 0) {
	coordinate r;
	r.x = P[position].x;
	r.y = P[position].y;
	if (r.x >= 0 && r.y != 0)
		return true;
	else
		return false;
}

coordinate convexHull::getCoord(coordinateList &P, int position = 0) {
	coordinate r;
	r.x = P[position].x;
	r.y = P[position].y;
	cout << "Getting coord x = " << r.x << ", y = " << r.y << " from position " << position << "." << endl;
	return r;
}

int convexHull::findCoord(coordinateList P, coordinate A) {
	for (int i = 0; i <= P.size(); i++ ) {
		if (getCoord(P,i) == A )
			return i;
	}
	return NULL;
}

coordinateList convexHull::reverse(coordinateList &P) {
	// Get elements in reverse order.
	coordinateList L;
    for (int i=P.size()-1; i>=0; i--) {
		putCoord(L,getCoord(P,i));
    }
	return L;
}

int convexHull::findTurn(coordinate P, coordinate Q, coordinate R) {
	// To determine whether a turn is right, compute the cross product:
	// (x2-x1)(y3-x1)-(y2-y1)(x3-x1)
	// If result is 0	- linear
	// If result is +ve - left turn
	// If result is -ve - right turn
	int a = Q.x - P.x;
	int b = R.y - P.x;
	int c = Q.y - P.y;
	int d = R.x - P.x;
	int g = a * b;
	int h = c * d;
	int result = g - h;
	if (result > 0)
	{
		cout << "Left turn!\n";
		return 1;
	}
	else if (result < 0)
	{
		cout << "Right turn!\n";
		return 2;
	}
	else
	{
		cout << "Straight LINE?!?\n";
		return 3;
	}	
}

coordinateList convexHull::merge(coordinateList A, coordinateList B){
	coordinateList P, D;
	cout << "Merging forwards\n";
	cout << "List A\n";
	printList(A);
	cout << "List B\n";
	printList(B);
	// Merges two coordinateLists and returns a hull of both of them
	for (unsigned int i = 0; i < A.size(); i++) {
		// Add all the elements from A into O
		putCoord(P,A[i]);
	}
	for (unsigned int j = 0; j < B.size(); j++) {
		// Add all the elements from B into O
		putCoord(P,B[j]);
	}
	
    // cout << "before hull, merged";
	// printList(P);
	D = doGrahamHull(P);
	cout << "merged";
	printList(P);
	return P;
}

coordinateList convexHull::dcMerge(coordinateList A, coordinateList B) {
	coordinateList G = A, H = B;
	coordinate c, d, j, k;
	int kPosition, dPosition;
	cout << "Performing DC merge - tangental method\n";
	
	/* Left hull work */
	// Sort left vector G
	sort(G.begin(), G.end());
	int gEnd = G.size();
	// Let d be the right most point on hull G
	d = convexHull::getCoord( G , gEnd );
	// Le c be te point before the right most point
	c = convexHull::getCoord( G , gEnd - 1 );
	
	/* Right hull work */
	// Sort right vector H
	sort(H.begin(), H.end());
	int hBegin = 0;
	// Let j be the left most point on hull H
	j = convexHull::getCoord( H , hBegin );
	// Let k be the point after the left most point
	k = convexHull::getCoord( H , hBegin + 1);
	
	// Until angle djk makes a right and angle jdc makes a left turn
	while (findTurn(d,j,k) != 2 && findTurn(j,d,c) != 1 )
	{
		switch (findTurn(d,j,k)) {
	case 1: // If there is a left turn
				cout << "Found a left turn";
		kPosition = findCoord(H,k);			// Record the position of k
		if (kPosition != H.size() )			// Check we're not at the last point
		{
			j = k;							// Move the comparison on one
			k = getCoord(H,kPosition + 1);	// Set k to be the next coordinate in list
			delCoord(H , kPosition);		// Discard the passed coordinate
			cout << "\n\nNew H structure:\n";
			printList(H);
			cout << endl << endl;
		}
		else								// at last point, cycle back around
		{	
			j = k;							// Move the comparison on one
			k = getCoord(H,0);				// Set k to be the first point
			delCoord(H , H.size() );		// Delete the last coordinate
			cout << "\n\nNew H structure:\n";
			printList(H);
			cout << endl << endl;
		}
		break;
		case 2: // If it's a right turn
		
		// We should have just found the top of the right hull which there is the tangent
		dPosition = findCoord(G,d);
		if (dPosition != 0) {					// Check we're not at the first point
			d = c;								// Move the comparison back one
			c = getCoord(G, dPosition - 1);		// Set c to be the previous coordinate in list
			delCoord(G , dPosition);			// Discard the passed coordinate
			cout << "\n\nNew G structure:\n";
			printList(G);
			cout << endl << endl;
		}
		else									// at last point, cycle to the front again
		{
			d = c;								// Move the comparison back one
			c = getCoord(G,G.size());			// Set c to be the last coordinate
			delCoord(G,0);						// Delete the first point
			cout << "\n\nNew G structure:\n";
			printList(G);
			cout << endl << endl;
		}
		break;
				case 3: // If it's a straght line
				
				// We should have just found the top of the right hull which there is the tangent
				dPosition = findCoord(G,d);
				if (dPosition != 0) {					// Check we're not at the first point
					d = c;								// Move the comparison back one
					c = getCoord(G, dPosition - 1);		// Set c to be the previous coordinate in list
					delCoord(G , dPosition);			// Discard the passed coordinate
					cout << "\n\nNew G structure:\n";
					printList(G);
					cout << endl << endl;
				}
				else									// at last point, cycle to the front again
				{
					d = c;								// Move the comparison back one
					c = getCoord(G,G.size());			// Set c to be the last coordinate
					delCoord(G,0);						// Delete the first point
					cout << "\n\nNew G structure:\n";
					printList(G);
					cout << endl << endl;
				}
				break;
		}// end switch/case
	} // End while
	return merge(G,H);
	
}


coordinateList convexHull::merge_back(coordinateList A, coordinateList B){
	coordinateList P, D;
	
	P = merge(A,B);
	copy(P.begin(),P.end(),D.begin());
	cout << "Merging backwards\n";
	
	// reverse(D.begin(),D.end());
	
	cout << "before hull, merged";
	printList(P);
	D = doGrahamHull(P);
	cout << "after hull, merged";
	printList(D);
	return D;
}

/* CONVEX HULL ALGORITHMS*/

// Graham Scan for the Graham Convex Hull
coordinateList convexHull::grahamScan(coordinateList &R) {
	coordinateList G = R;
	int GSize;
	GSize = (int) G.size();
	int element = 2; // We start from the third coordinate in the list
	
	do {
		if ( findTurn(getCoord(G,element-2),getCoord(G,element-1),getCoord(G,element)) == 1 ) {
			cout << "GSize: " << GSize << endl;
			element++;
		}
		else
			delCoord(G,element);
		GSize = (int) G.size();
	}
	while (element != GSize - 1);	// Repeat until we get to the end of the list
	return G;
}

coordinateList convexHull::doGrahamHull(coordinateList &L) {
	coordinateList R;
	cout << "\n-----------------------------\nSTART GRAHAM SCAN HULL\n\n";
	cout << "L list:";
	printList(L);
	
	// Sort the ConvexHull P input vector
	// sort(&P,&L);
	sort(L.begin(), L.end());
	
	cout << "Ordered L list: ";
	printList(L);
	
	cout << "\n\n";
	
	delDuplicates(L);
	
	if (L.size() > 3) {
	cout << "Now Graham Scanning...\n";
	R = grahamScan(L);
	
	cout << "Graham scanned list: ";
	printList(R);
	
	cout << "\n\n";
	return R;
	}
	else {
		cout << "Given hull is too small" << endl;
		return L;
	}
}

// DC-iness not working yet. Try again later when doHull is working!
// Input - set of points, vector S // Output - Convex hull of S, vector H
coordinateList convexHull::dcHull (coordinateList &P) {
	coordinateList O, listOne, listTwo;
	// Divide and conquer optimisations for ConvexHull
	if (P.size() <= 5)
		return doGrahamHull(P);
	else {
		convexHull::split(P,listOne,listTwo); // divide the list into two distinct lists, for Divide
		coordinateList K, R;
		K = convexHull::dcHull(listOne);
		cout << "List One:";
		printList(K);
		R = convexHull::dcHull(listTwo);
		cout << "List Two";
		printList(R);
		return convexHull::dcMerge(K,R); // Now recursively call the split strings -- Conquer, very clever.
	}
}

void convexHull::split (coordinateList &P,coordinateList &A,coordinateList &B) {
	cout << "Now splitting a vector of size : " << P.size() << "." << endl;
	int pEnd = (int) P.size() - 1;
	int pStart = 0;
	// Only split when the input hull has more than 6 elements
//	if (pEnd > 6) {
		do {				
			// Push current element on vector A
			putCoord( A,getCoord(P,pStart) );
			cout << "pStart is " << pStart << endl;
			pStart++;
			putCoord( B,getCoord(P,pEnd) );
			cout << "pEnd is " << pEnd << endl; 
			pEnd--;
			}
		while (pEnd > pStart);
//	} // end if
}

coordinateList convexHull::doHull (coordinateList &L) {
	// Input - Set of points, vector P from 2-D coordinates given as(x,y)
	// Output - Set of points, vector cH, the vertices of inputs convex hull in clockwise order.
	if (L.size() > 3) {
		cout << "\n-----------------------------\nSTART HULL\n\n";
		cout << "L list:";
		printList(L);
		
		// Sort the ConvexHull P input vector
		// sort(&P,&L);
		//	sort(L.begin(), L.end());
		
		cout << "Ordered L list: ";
		printList(L);
		
		cout << "\n\n";
		// Initialise vectors
		coordinateList upperList = L;
		coordinateList lowerList = L;
		
		// return the union of lowerList and upperList as the covex hull
		// split(L,upperList,lowerList);
		
		upperList = doUpperHull(upperList);
		lowerList = doLowerHull(reverse(lowerList));
		
		L = merge(upperList,lowerList);
		// for(int i=0; i < L.size(); i++)
		// {
		// 	cout << "x = " << L[i].x << "\ty = " << L[i].y << "\n" << endl;
		// }
		
		cout << "After merge-back: Returned hull!\n";
		printList(L);
		cout << "END HULL\n-----------------------------\n";
	}
	return L;
} // end algorithm

coordinateList convexHull::doLowerHull (coordinateList lowerList) {
	// Lower hull
	// Put points L[0] and L[1] into vector lowerList
	
	coordinateList lowerOut;
	
	cout << "\n-----------------------------\nSTART LOWER HULL\n\n";
	cout << "Not ordered lower hull list:";
	printList(lowerList);
	
	// Sort the input vector backwards
	sort(lowerList.begin(),lowerList.end());
	
	cout << "Ordered lower hull list: ";
	printList(lowerList);
	
	cout << "\n\n";
	
	lowerOut.push_back(getCoord(lowerList,lowerList.size()));
	lowerOut.push_back(getCoord(lowerList,lowerList.size()-1));
	for (int b = lowerList.size() - 2; b >= 1; b--)
	{
		// push L[b] onto upperList;
		lowerOut.push_back(getCoord(lowerList,b));
		
		while (lowerOut.size() > 2 && findTurn(lowerOut[b-2],lowerOut[b-1],lowerOut[b]) != 1) {
			delCoord(lowerOut,lowerOut.size()-2);
			cout << "Deleted point " << b << endl;
		}
		
		cout << "lowerOut.size() = " << lowerOut.size() << endl;
		cout << "Lower b = " << b << endl;
		int sizeOfList = lowerList.size();
		
		cout << "Size of Hull: " << sizeOfList << endl;
	} // end lower hull
	cout << "\n\nHulled list: \n";
	printList(lowerList);
	
	// Remove the first and last point from lowerList
	// delCoord(lowerList,lowerList.size()-1);
	// delCoord(lowerList,lowerList.size()-2);
	
	cout << "Returned lower list:\n";
	printList(lowerList);
	cout << "END LOWER HULL\n-----------------------------\n";
	return lowerList;
} // end algorithm

coordinateList convexHull::doUpperHull(coordinateList upperList) {
	// Put points L[0] and L[1] into vector upperList
	
	coordinateList upperOut;
	
	cout << "\n-----------------------------\nSTART UPPER HULL\n\n";
	cout << "Not ordered upper hull list:";
	printList(upperList);
	
	// Sort the input vector backwards
	sort(upperList.begin(),upperList.end());
	
	cout << "Ordered lower hull list: ";
	printList(upperList);
	
	cout << "\n\n";
	
	upperOut.push_back(getCoord(upperList,0));
	upperOut.push_back(getCoord(upperList,1));
	
	// Upper hull
	int sizeOfList = upperList.size();
	for (int i = 2; i < sizeOfList; i++)
	{
		// push L[i] onto upperList;
		upperOut.push_back(getCoord(upperList,i));
		
		while (upperOut.size() > 2 && findTurn(upperOut[i-2],upperOut[i-1],upperOut[i]) != 2) {
			delCoord(upperOut,upperOut.size()-2);
			sizeOfList--;
		}
		cout << "List size - 2 = " << upperList.size() - 2 << endl;
		cout << "Upper list.size() = " << upperList.size() << endl;
		cout << "Upper i = " << i << endl;
		
		cout << "Size of Hull: " << sizeOfList << endl;
	} // end upper hull
	cout << "Returned upper list:\n";
	printList(upperOut);
	return upperOut;
	cout << "END LOWER HULL\n-----------------------------\n";
}

