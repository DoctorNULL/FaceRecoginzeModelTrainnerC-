#ifndef libs

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>


using namespace std;
using namespace cv;
using namespace face;

#endif // !libs

#define dataLen 20
#define imgSize 250

enum operation {
	createData,
	Train,
	Look,
	Exit
};

class person
{
protected :
	int id;
	string name;

public :
	person()
	{
		id = 0;
		name = "Noone";
	}

	friend ofstream& operator<< (ofstream& out , person p)
	{
		out << p.id << "-" << p.name << " " << endl;

		return out;
	}

	friend ifstream& operator>> (ifstream& in, person& p)
	{
		int id;
		string name = "" , sid = "";
		char c;
		stringstream s;

		
		in >> c;
		while (!in.fail() && isspace(c))
			in >> c;

		while (!in.fail() && isdigit(c))
		{
			sid += c;
			in >> c;
		}

		in >> c;

		while (!in.fail() && isalpha(c))
		{
			name += c;
			in >> c;
		}

		s << sid;
		s >> id;

		p.id = id;
		p.name = name;

		return in;
	}

	friend ostream& operator<< (ostream& out, person p)
	{
		out << p.id << "-" << p.name << endl;

		return out;
	}

	void set(int oid, string oname = "Noone")
	{
		id = oid;
		name = oname;
	}

	void clone(person& p)
	{
		p.id = id;
		p.name = name;
	}

	int getID()
	{
		return id;
	}


	string getName()
	{
		return name;
	}
};