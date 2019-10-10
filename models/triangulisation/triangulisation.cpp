#define MAX_DIFF 32
#define SCALE 1
#define PATH_2_READ_FILE "c:/Users/User/Documents/test_1/cube3.stl"
#define PATH_2_WRT_FILE "c:/Users/User/Documents/test_1/output.stl"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "triangulisation.h"
using namespace std;

void save_trn(fstream& input, vector<string>& vec2save) {
	//long position = input.tellp();
	string buff;
	for (int i = 0; i<7; i++) {
		getline(input, buff);
		vec2save.push_back(buff);
	}
	//input.seekp(position);
};

bool read_trn_from_str(const vector<string>& input, Triangle& trn2splt, float scale) {
	string A[3];
	string B[3];
	string C[3];
	string buff[3];
	////get rid of " " before verex word
	for (int j = 0; j<3; j++) {
		for (int i = 6; i<input[2 + j].size(); i++) {
			buff[j] += input[2 + j][i];
		}
	}
	string buff_2[3];
	for (int j = 0; j<3; j++) {
		for (int i = 6; i<buff[j].size(); i++) {
			buff_2[j] += buff[j][i];
		}
	}
	////
	for (int i = 0, j = -1; i<buff_2[0].size(); i++) {
		if (buff_2[0][i] == ' ') {
			j++;
		}
		else A[j] += buff_2[0][i];
	}
	for (int i = 0, j = -1; i<buff_2[1].size(); i++) {
		if (buff_2[1][i] == ' ') {
			j++;
		}
		else B[j] += buff_2[1][i];
	}
	for (int i = 0, j = -1; i<buff_2[2].size(); i++) {
		if (buff_2[2][i] == ' ') {
			j++;
		}
		else C[j] += buff_2[2][i];
	}
	///convert 
	trn2splt.a.set(stof(A[0], 0), stof(A[1], 0), stof(A[2], 0));
	trn2splt.b.set(stof(B[0], 0), stof(B[1], 0), stof(B[2], 0));
	trn2splt.c.set(stof(C[0], 0), stof(C[1], 0), stof(C[2], 0));
	trn2splt.a.scal(scale);
	trn2splt.b.scal(scale);
	trn2splt.c.scal(scale);

	return trn2splt.checkLengths(MAX_DIFF);
}

void split(Triangle &input, vector<Triangle>& output) {
	///test
	//cout << "split was called" << endl;
	Triangle trn1(input.a, input.b, input.c);
	Triangle trn2;
	trn1.split(trn2);
	if (!trn1.checkLengths(MAX_DIFF)) {
		split(trn1, output);
	}
	else {
		output.push_back(trn1);
	}
	if (!trn2.checkLengths(MAX_DIFF)) {
		split(trn2, output);
	}
	else {
		output.push_back(trn2);
	}
	return;
};

void write_1_trn_into_file(ofstream& output, const Triangle& trn2wrt) {
	output << "      vertex ";
	output << trn2wrt.a.x << ' ' << trn2wrt.a.y << ' ' << trn2wrt.a.z << endl;
	output << "      vertex ";
	output << trn2wrt.b.x << ' ' << trn2wrt.b.y << ' ' << trn2wrt.b.z << endl;
	output << "      vertex ";
	output << trn2wrt.c.x << ' ' << trn2wrt.c.y << ' ' << trn2wrt.c.z << endl;
};

void write_splited_trn(ofstream& output, const vector<Triangle>& trn2wrt, const string& normal) {
	for (Triangle item : trn2wrt) {
		output << normal << endl;
		output << "    outer loop" << endl;
		write_1_trn_into_file(output, item);
		output << "    endloop" << endl;
		output << "  endfacet" << endl;
	}
};

void write_scaled_trn(ofstream& output, const Triangle &trn2wrt, const string& normal) {
	output << normal << endl;
	output << "    outer loop" << endl;
	write_1_trn_into_file(output, trn2wrt);
	output << "    endloop" << endl;
	output << "  endfacet" << endl;
};

void write_saved_trn(ofstream& output, const vector<string>& trn2save) {
	for (int i = 0; i<7; i++) {
		output << trn2save[i] << endl;
	}
};


int main(int argc, char *argv[]){
	string path;
	string path2wrt;
	string buff;
	/*if (argc < 3) {
		return 0xBAD00BAD;
	}*/
	float scale;

	//path = "d:\\GIT\\nmOpenGL\\models\\NM.stl";
	//path2wrt = "d:\\GIT\\nmOpenGL\\models\\NM_32.stl";
	//scale = 1.5;

	cout << "WELCOME TO THE TRIANGULISATION PROGRAM!!!!!" << endl;
	cout << "Please enter the path to file for transformation" << endl;
	cin >> path;
	fstream input(path);
	if (input) {
		getline(input, buff);
		cout << "file was opened" << endl;
	}
	else cout << "file does not exist" << endl;
	cout << "Please enter the path to file for writting the resault stl " << endl;
	cin >> path2wrt;
	cout << "Please enter the scale" << endl;
	cin >> scale;

	ofstream output(path2wrt);
	output << buff << endl;

	Triangle splitTrian;
	vector<Triangle> writeTrian;
	vector<string> saveTrian;
	int i = 0;
	while (input) {
		saveTrian.clear();
		save_trn(input, saveTrian);
		if (!input) {
			break;
		}
		if (read_trn_from_str(saveTrian, splitTrian, scale)) {
			write_scaled_trn(output, splitTrian, saveTrian[0]);
		}else {
			writeTrian.clear();
			split(splitTrian, writeTrian);
			write_splited_trn(output, writeTrian, saveTrian[0]);
		}
	}
	output << "endsolid OpenSCAD_Model";
	output.close();
	input.close();
	cout << "ALL PROCESSING HAS BEEN DONE ENJOY THE NEW STL!!!" << endl; 
	
}