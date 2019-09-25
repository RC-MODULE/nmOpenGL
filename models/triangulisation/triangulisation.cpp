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
	trn2splt.a = trn2splt.a * scale;
	trn2splt.b = trn2splt.b * scale;
	trn2splt.c = trn2splt.c * scale;

	return trn2splt.isFitIntoRectangle(MAX_DIFF, MAX_DIFF);
}

void split(Triangle input, vector<Triangle>& output) {
	///test
	//cout << "split was called" << endl;
	Triangle trn1(input.a, input.b, input.c);
	Triangle trn2;
	if()
	if (!trn1.isFitIntoRectangle(MAX_DIFF, MAX_DIFF)) {
		split(trn1, output);
	}
	else {
		output.push_back(trn1);
	}
/*	input.sort_by_length();
	//input.print_trn();
	for (int i = 0; i<3; i++) {
		trn1.A[i] = input.A[i];
		trn1.C[i] = input.C[i];
		trn1.B[i] = input.mdl_pnt_AB[i];

		trn2.A[i] = input.mdl_pnt_AB[i];
		trn2.B[i] = input.B[i];
		trn2.C[i] = input.C[i];
	}
	//cout << "trn1" << endl;
	//trn1.print_trn();
	//cout << "trn2" << endl;
	//trn2.print_trn();

	if (trn1.sort_by_length()) {
		split(trn1, output);
	}
	else {
		output.push_back(trn1);
	}
	if (trn2.sort_by_length()) {
		split(trn2, output);
	}
	else {
		output.push_back(trn2);
	}*/
	return;
};


int main(int argc, char *argv[]){
	string path;
	string path2wrt;
	string buff;
	/*if (argc < 3) {
		return 0xBAD00BAD;
	}*/
	float scale;
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
		if (!read_trn_from_str(saveTrian, splitTrian, scale)) {
			writeTrian.clear();
			split(splitTrian, writeTrian);
			//write_splited_trn(output, trn2wrt, trn2save[0]);
		}else {
			//write_saved_trn(output,trn2save);
			//write_scaled_trn(output, trn2splt, trn2save[0]);
		}
	}
	/*triangle trn2splt;
	vector<triangle> trn2wrt;
	vector<string> trn2save;
	int i = 0;
	while (input) {
		trn2save.clear();
		save_trn(input, trn2save);
		if (!input) {
			break;
		}
		if (read_trn_from_str(trn2save, trn2splt, scale)) {
			trn2wrt.clear();
			split(trn2splt, trn2wrt);
			write_splited_trn(output, trn2wrt, trn2save[0]);
		}
		else {
			//write_saved_trn(output,trn2save);
			write_scaled_trn(output, trn2splt, trn2save[0]);
		}
		//cout << i << endl;
		//i++;
	}
	output << "endsolid OpenSCAD_Model";
	output.close();
	input.close();
	cout << "ALL PROCESSING HAS BEEN DONE ENJOY THE NEW STL!!!" << endl;*/
}
/*class triangle{
public:
	float A[3];
	float B[3];
	float C[3];
	float mdl_pnt_AB[3];
	
	bool sort_by_length(){
		//the perpose of the method is to sort sides of triangle that way that the longest side would be AB
		float length_AB = (A[0]-B[0])*(A[0]-B[0]) + (A[1]-B[1])*(A[1]-B[1]) + (A[2]-B[2])*(A[2]-B[2]) ;
		float length_AC = (A[0]-C[0])*(A[0]-C[0]) + (A[1]-C[1])*(A[1]-C[1]) + (A[2]-C[2])*(A[2]-C[2]) ; 
		float length_BC = (B[0]-C[0])*(B[0]-C[0]) + (B[1]-C[1])*(B[1]-C[1]) + (B[2]-C[2])*(B[2]-C[2]) ;
			
		vector<float> v2sort = {length_AB,length_AC,length_BC};
		sort(v2sort.begin(), v2sort.end());
		if(length_AB == v2sort[2]){
			//AB side is the greatest side no action needed
		}else{
			if(length_AC == v2sort[2]){
				for(int i=0;i<3;i++){
					swap(C[i],B[i]);
				}
			}else{
				for(int i=0;i<3;i++){
					swap(A[i],B[i]);
					swap(B[i],C[i]);
				}
			}
		}
		//set mdl point
		for(int i=0;i<3;i++){
			mdl_pnt_AB[i] = A[i] + (B[i] - A[i])*0.5;	
		}
		if(v2sort[2] > MAX_DIFF*MAX_DIFF){
			return true;
		}else{
            return false;
		}
	}
	void print_trn(){
		cout << "A ";
		for(int i=0;i<3;i++){
			cout << A[i] << " ";
		}
		cout << endl;
		cout << "B ";
		for(int i=0;i<3;i++){
			cout << B[i] << " ";
		}
		cout << endl;
		cout << "C ";
		for(int i=0;i<3;i++){
			cout << B[i] << " ";
		}
		cout << endl;
	}
};

bool read_trn_from_str(const vector<string>& input, triangle& trn2splt,float scale){
	string A[3];
	string B[3];
	string C[3];
	string buff[3];
	////get rid of " " before verex word
	for(int j=0;j<3;j++){
		for(int i=6;i<input[2+j].size();i++){
			buff[j] += input[2+j][i];
		}
	}
	string buff_2[3];	
	for(int j=0;j<3;j++){
		for(int i=6;i<buff[j].size();i++){
			buff_2[j] += buff[j][i];
		}
	}
	////
	for(int i=0,j=-1;i<buff_2[0].size();i++){
		if(buff_2[0][i] == ' '){
			j++;
		}else A[j] += buff_2[0][i];
	}
	for(int i=0,j=-1;i<buff_2[1].size();i++){
		if(buff_2[1][i] == ' '){
			j++;
		}else B[j] += buff_2[1][i];
	}
	for(int i=0,j=-1;i<buff_2[2].size();i++){
		if(buff_2[2][i] == ' '){
			j++;
		}else C[j] += buff_2[2][i];
	}
	///convert 
	for(int i=0;i<3;i++){
		trn2splt.A[i] = scale*stof(A[i],0);
	}
	for(int i=0;i<3;i++){
		trn2splt.B[i] = scale*stof(B[i],0);
	}
	for(int i=0;i<3;i++){
		trn2splt.C[i] = scale*stof(C[i],0);
	}
	return trn2splt.sort_by_length();
}

bool readTriangle(fstream &input, triangle &trn2splt, float scale){
	string vertex_pass;
	string A[3];
	string B[3];
	string C[3];
	getline(input,vertex_pass);
	input >> vertex_pass;
	for(int i=0;i<3;i++){
		input >> A[i];
		//cout << A[i] << " ";
	}
	input >> vertex_pass;
	cout << endl;
	for(int i=0;i<3;i++){
		input >> B[i];
		//cout << B[i] << " ";
	}
	cout << endl;
	input >> vertex_pass;
	for(int i=0;i<3;i++){
		input >> C[i];
		//cout << C[i] << " ";
	}
	///convert 
	for(int i=0;i<3;i++){
		trn2splt.A[i] = scale*stof(A[i],0);
	}
	for(int i=0;i<3;i++){
		trn2splt.B[i] = scale*stof(B[i],0);
	}
	for(int i=0;i<3;i++){
		trn2splt.C[i] = scale*stof(C[i],0);
	}
	return trn2splt.sort_by_length()	;
};
void split(triangle input, vector<triangle>& output){
    ///test
    //cout << "split was called" << endl;
    triangle trn1,trn2;
    input.sort_by_length();
    //input.print_trn();
    for(int i =0;i<3;i++){
        trn1.A[i] = input.A[i];
        trn1.C[i] = input.C[i];
        trn1.B[i] = input.mdl_pnt_AB[i];

        trn2.A[i] = input.mdl_pnt_AB[i];
        trn2.B[i] = input.B[i];
        trn2.C[i] = input.C[i];
    }
    //cout << "trn1" << endl;
    //trn1.print_trn();
    //cout << "trn2" << endl;
    //trn2.print_trn();

    if(trn1.sort_by_length()){
        split(trn1,output);
    }else{
        output.push_back(trn1);
    }
    if(trn2.sort_by_length()){
        split(trn2,output);
    }else{
        output.push_back(trn2);
    }
    return;
};
void write_1_trn_into_file(ofstream& output, const triangle& trn2wrt){
	output << "      vertex ";
	output << trn2wrt.A[0] << ' ' << trn2wrt.A[1] << ' ' << trn2wrt.A[2] << endl;
	output << "      vertex ";
	output << trn2wrt.B[0] << ' ' << trn2wrt.B[1] << ' ' << trn2wrt.B[2] << endl;
	output << "      vertex ";
	output << trn2wrt.C[0] << ' ' << trn2wrt.C[1] << ' ' << trn2wrt.C[2] << endl;
};

void write_splited_trn(ofstream& output, const vector<triangle>& trn2wrt, const string& normal){
	for(triangle item : trn2wrt){
		output << normal << endl;
		output << "    outer loop" << endl;
		write_1_trn_into_file(output,item);
		output << "    endloop" << endl;
   	output << "  endfacet" << endl;
	}
};

void write_scaled_trn(ofstream& output, const triangle trn2wrt, const string& normal){
		output << normal << endl;
		output << "    outer loop" << endl;
		write_1_trn_into_file(output,trn2wrt);
		output << "    endloop" << endl;
   	output << "  endfacet" << endl;
};

void write_saved_trn(ofstream& output, const vector<string>& trn2save){
	for(int i=0;i<7;i++){
		output << trn2save[i] << endl;
	}
};
void save_trn(fstream& input, vector<string>& vec2save){
	//long position = input.tellp();
	string buff;
	for(int i=0;i<7;i++){
		getline(input,buff);
		vec2save.push_back(buff);
	}
	//input.seekp(position);
};



int main()
{
  string path;
	string path2wrt;
	string buff;
	float scale;
  cout << "WELCOME TO THE TRIANGULISATION PROGRAM!!!!!" << endl;
  cout << "Please enter the path to file for transformation" << endl;
  cin >> path; 
  fstream input(path);
	if(input){
		getline(input,buff);
		cout << "file was opened" << endl;
	}
	else cout << "file does not exist" << endl;
  cout << "Please enter the path to file for writting the resault stl " << endl;
  cin >> path2wrt;
  cout << "Please enter the scale" << endl;
  cin >> scale;

	ofstream output(path2wrt);
	output << buff << endl;

	triangle trn2splt;
	vector<triangle> trn2wrt;
	vector<string> trn2save;
	int i =0;	
	while(input){
    trn2save.clear();
		save_trn(input,trn2save);
		if(!input){
			break;
		}
		if(read_trn_from_str(trn2save,trn2splt,scale)){
			trn2wrt.clear();
			split(trn2splt,trn2wrt);
			write_splited_trn(output,trn2wrt,trn2save[0]);
		}else{
			//write_saved_trn(output,trn2save);
			write_scaled_trn(output,trn2splt,trn2save[0]);
		}
		//cout << i << endl;
		//i++;
	}
	output << "endsolid OpenSCAD_Model";
	output.close();
	input.close();
  cout << "ALL PROCESSING HAS BEEN DONE ENJOY THE NEW STL!!!" << endl;
}//end of maine*/
