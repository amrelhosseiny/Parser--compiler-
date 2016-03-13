#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <List>

bool exp();
bool statSeq();



using namespace std;

string token;

short int state;

short int start = 1, num = 2, id = 3, assign = 4, comment = 6, special = 7, reserved = 8, error = 10;

int index = 0;

ofstream ofile;

vector <string> tokens;

//string array that will contain finsihed tokens

//scanner method
void done() {

	if (state == start) {
		ofile << "unexpected output at token \n" << token;
	}
	else if (state == num) {
		ofile <<"number\n";
	}
	else if (state == id) {
		ofile <<"identifier\n";
	}
	else if (state == comment) {
		//ofile << token << " : Comment \n";
	}
	else if (state == special) {
		ofile << token << "\n";
	}
	else if (state == reserved) {
		ofile << token << "\n";
	}
	else if (state = error) {
		ofile << "ERROR, unexpected output at :" << token;
	}

}



//start of parser methods
bool match(string s) {
	if (s == tokens.at(index)) {
		index++;
		return true;
	}
	return false;
}

bool factor() {
	if (match("number")) {
		ofile << "Factor Found \n";
		return true;
	}
	else if (match("identifier")) {
		ofile << "Factor Found \n";
		return true;
	}
	if (match("(")) {
		if (exp()) {
			if (match(")")) {
				ofile << "Factor Found \n";
				return true;
			}
		}
	}
	return false;
}

bool mulOp() {

	if (match("*") || match("/")) {
		ofile << "Multiplication Operator Found \n";
		return true;
	}
	return false;
}

bool term() {
	if (factor()) {
		ofile << "Term Expression Found \n";
		return true;
	}
	if (term()) {
		if (mulOp()) {
			if (factor()) {
				ofile << "Term Expression Found \n";
				return true;
			}
		}
	}
	return false;
}

bool addOp() {

	if (match("+") || match("-")) {
		ofile << "Add Operator Found \n";
		return true;
	}
	return false;
}

bool compOp() {

	if ( match("=") || match("<") || match("<") ) {
		ofile << "Comparison Operator Found \n";
		return true;
	}
	return false;
}

bool simpleExp() {
	if (term()) {
		ofile << "Simple Expression Found \n";
		return true;
	}
	if (simpleExp()) {
		if (addOp()) {
			if (term()) {
				ofile << "Simple Expression Found \n";
				return true;
			}
		}
	}
	return false;
}

bool exp() {
	if (simpleExp()) {
		if (compOp()) {
			if (simpleExp()) {
				ofile << "Expression Found \n";
				return true;
			}
		}
			ofile << "Expression Found \n";
			return true;
	}
	return false;
}

bool ifStat() {
	if (match("if")) {
		if (exp()) {
			if (match("then")) {
				if (statSeq()) {
					if (match("end")) {
						ofile << "If Statment Found \n";
						return true;
					}
					else if (match("else")) {
						if (statSeq()) {
							if (match("end")) {
								ofile << "If Statment Found \n";
								return true;
							}
						}
					}

				}
			}
		}
	}
	return false;
}

bool repStat() {
	if (match("repeat")) {
		if (statSeq()) {
			if (match("until")) {
				if (exp()) {
					ofile << "Repeat Statment Found \n";
					return true;
				}
			}
		}
	}
	
	return false;
}

bool assignStat() {
	if (match("identifier")) {
		if (match(":=")) {
			if (exp()) {
				ofile << "Assign Statment Found \n";
				return true;
			}
		}
	}
	return false;
}

bool readStat() {
	if (match("read")) {
		if (match("identifier")) {
			ofile << "Read Statment Found \n";
			return true;

		}
	}
	return false;
}

bool writeStat() {
	if (match("write")) {
		if (exp()) {
			ofile << "Write Statment Found \n";
			return true;
		}
	}
	return false;
}

bool stat() {
	if (ifStat() || repStat() || assignStat() || readStat() || writeStat()) {
		ofile << "Statment Found \n";
		return true;
	}
	return false;
}

bool statSeq() {
	if (stat()){
		if (match(";")) {
			if (stat()) {
				ofile << "Statment Sequnce Found \n";
				return true;
			}
		}
		ofile << "Statment Sequnce Found \n";
		return true;
	}
	return false;
}

bool prog() {

	if (statSeq()) {
		ofile << "Program Found \n";
		return true;
	}
	return false;
}


void main() {

	
	//scanner part
	ifstream myReadFile;
	myReadFile.open("tiny_sample_code.txt");
	string input, line;
	cout << "Welcome to Scanner & Parser assignment \n ";
	cout << "A file \"Scanner Output.txt\" will be generated with the scanner output \n";
	cout << "A file \"Parser Output\" will be generated with the parser output \n";

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) { myReadFile >> line; input += " "; input += line; }
	} else cout << "Error opening file, Please check file name and location.";

	ofile.open("Scanner Output.txt");
	char x;
	state = start;

	for (int i = 0; i < input.size(); i++) {
		x = input[i];
		if (x == ' ' && state != comment) {
			state = start;
		}
		else if (x == '{') {
			token = x;
			state = comment;
		}
		else if (state == start) {
			if (isdigit(x)) {
				state = num;
				token = "";
				token.push_back(x);
				if (!isdigit(input[i + 1])) {
					done();
					state = start;
				}
			}
			else if (x == '+' || x == '-' || x == '/' || x == '*' || x == ';' || x == '=' || x == '<' || x == '>' || x == '(' || x == ')') {
				token = x;
				state = special;
				done();
				state = start;
			}
			else if (x == ':') {

				if (input[i + 1] == '=') {
					token = "";
					token.push_back(x);
					token.push_back(input[i + 1]);
					state = special;
					done();
					input[i + 1] = ' ';
					state = start;
				}
				else {
					token = x;
					state = error;
					done();
					state = start;
				}
			}
			else if (isalpha(x) || x == '_') {
				token = "";
				if (x == 'i' && input[i + 1] == 'f') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					//token = x + input[i + 1];

					state = reserved;
					done();
					input[i + 1] = ' ';
					state = start;
				}
				else if (x == 't' && input[i + 1] == 'h' && input[i + 2] == 'e' && input[i + 3] == 'n') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					state = start;
				}
				else if (x == 'e' && input[i + 1] == 'l' && input[i + 2] == 's' && input[i + 3] == 'e') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					state = start;
				}
				else if (x == 'r' && input[i + 1] == 'e' && input[i + 2] == 'a' && input[i + 3] == 'd') {
					//	//token = x + input[i + 1] + input[i + 2] + input[i + 3];
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					state = start;
				}
				else if (x == 'e' && input[i + 1] == 'n' && input[i + 2] == 'd') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					state = start;
				}
				else if (x == 'r' && input[i + 1] == 'e' && input[i + 2] == 'p' && input[i + 3] == 'e' && input[i + 4] == 'a' && input[i + 5] == 't') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					token.push_back(input[i + 4]);
					token.push_back(input[i + 5]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					input[i + 4] = ' ';
					input[i + 5] = ' ';
					state = start;
				}
				else if (x == 'u' && input[i + 1] == 'n' && input[i + 2] == 't' && input[i + 3] == 'i' && input[i + 4] == 'l') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					token.push_back(input[i + 4]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					input[i + 4] = ' ';
					state = start;
				}
				else if (x == 'w' && input[i + 1] == 'r' && input[i + 2] == 'i' && input[i + 3] == 't' && input[i + 4] == 'e') {
					token.push_back(x);
					token.push_back(input[i + 1]);
					token.push_back(input[i + 2]);
					token.push_back(input[i + 3]);
					token.push_back(input[i + 4]);
					state = reserved;
					done();
					input[i + 1] = ' ';
					input[i + 2] = ' ';
					input[i + 3] = ' ';
					input[i + 4] = ' ';
					state = start;
				}
				else {
					token.push_back(x);
					if (isalnum(input[i + 1]) || input[i + 1] == '_') {
						state = id;
					}
					else {
						state = id;
						done();
						state = start;
					}
				}
			}

			else {
				token = x;
				state = error;
				done();
				state = start;
			}
		}
		else if (state == comment) {
			if (x == '}') {
				token += x;
				done();
				state = start;
			}
			else {
				token += x;
				state = comment;
			}
		}
		else if (state == num) {
			token.push_back(x);
			if (isdigit(input[i + 1])) {
				state = num;
			}
			else {
				done();
				state = start;
			}
		}
		else if (state == id) {
			token.push_back(x);
			if (isalnum(input[i + 1]) || input[i + 1] == '_') {
				state = id;
			}
			else {
				done();
				state = start;
			}
		}
	}

	myReadFile.close();
	ofile.close();
	//end of scanner part





	//Parser part
	myReadFile.open("Scanner Output.txt");
	input = "";
	line = "";

	while (getline(myReadFile, input)) { tokens.push_back(input); }
	ofile.open("Parser Output.txt");

	if (prog()) {
		ofile << "End Of Parser Succesfully";
	}
	myReadFile.close();
	ofile << "\n End of Parser.";
	ofile.close();
}