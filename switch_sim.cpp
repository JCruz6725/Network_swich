#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
/*
*  John L. Cruz Jr
*  10/24/2021
*  Network Arc
*/


void readVector(vector<string> vec) {
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i];
		cout << "\n";
	}
}


void read2DVector(vector<vector<string>> vec) {
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < vec[i].size(); j++) {
			cout << vec[i][j];
			cout << " ";
		}
		cout << "\n";
	}
}


vector<string> retfile(string path) {

	vector<string> lines;
	fstream myfile;
	myfile.open(path, ios::in);

	char ch;
	string temp_line;
	while (true) {
		myfile >> ch;
		if (myfile.eof()) {
			break;
		}

		if (ch == '*') {
			lines.push_back(temp_line);
			temp_line.clear();
		}

		else {
			temp_line.push_back(ch);
		}
		//cout << ch;
	}
	myfile.close();
	return lines;
}


vector<vector<string>> string_file_to_data(vector<string> raw_data ) {
	vector<vector<string>> data;
	vector<string> frame;
	string line, chs;
	char temp;

	for (int line_num = 0; line_num < raw_data.size(); line_num++) {
		line = raw_data[line_num];
		vector<char> num_char{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		char qwe = line[2];

		if (line[2] == 'P') {
			frame.push_back(line.substr(0, 2));
			frame.push_back(line.substr(2, 2));
			chs.push_back(line[4]);
			frame.push_back(chs);
			chs.clear();
			chs.push_back(line[7]);
			frame.push_back(chs);
			chs.clear();
			data.push_back(frame);
			frame.clear();
		}
		else {
			frame.push_back(line.substr(0, 3));
			frame.push_back(line.substr(3, 2));
			chs.push_back(line[5]);
			frame.push_back(chs);
			chs.clear();
			chs.push_back(line[8]);
			frame.push_back(chs);
			chs.clear();
			data.push_back(frame);
			frame.clear();
		}
	}
	return data;
}


vector<vector<string>> net_switch(vector<vector<string>> incoming_frame) {
	// check if frame is comming
	vector <string> port_names{"P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8" };

	vector<string> port_v;
	vector<vector<string>> ports_data;
	vector<vector<string>> ports_to_computers_mapper;
	vector<string> temp;

	for (int i = 0; i < port_names.size(); i++) {
		port_v.push_back(port_names[i]);
		ports_data.push_back(port_v);
		port_v.clear();
	}

	ports_to_computers_mapper = ports_data;

	for (int frame_num = 0; frame_num < incoming_frame.size(); frame_num++) {
		string frame_number = incoming_frame[frame_num][0];
		string port = incoming_frame[frame_num][1];
		string from = incoming_frame[frame_num][2];
		string to = incoming_frame[frame_num][3];



		// the logs all cpu's in the respective port by taking in the current frames source adres 
		bool flag = false;
		for (int i = 0; i < ports_to_computers_mapper.size(); i++) {
			if (ports_to_computers_mapper[i][0] == port) {
				flag = false;
				for (int j = 0; j < ports_to_computers_mapper[i].size(); j++) {
					if (ports_to_computers_mapper[i][j] == from) {
						flag = true;
						break;
					}
				}
				if (!flag) {
					ports_to_computers_mapper[i].push_back(from);		
				}
			}
		}


		//check if in same port.
		bool is_same_port = false;
		bool is_same_port_from = false;
		bool is_same_port_to = false;
		for (int i = 0; i < ports_to_computers_mapper.size(); i++) {

			for (int j = 0; j < ports_to_computers_mapper[i].size(); j++) {
				if (ports_to_computers_mapper[i][j] == to) { is_same_port_to = true; }
				if (ports_to_computers_mapper[i][j] == from) { is_same_port_from = true; }				
			}
			if (is_same_port_to == true && is_same_port_from == true) {
				is_same_port = true;
				break; 
			}
			is_same_port_from = false;
			is_same_port_to = false;
		}
		if (is_same_port) { break; }


		// check if destination computer has already sent a frame out 
		// and has been logged with its port number
		bool flag2 = false;
		for (int i = 0; i < ports_to_computers_mapper.size(); i++) {
			for (int j = 0; j < ports_to_computers_mapper[i].size(); j++) {
				if (ports_to_computers_mapper[i][0] != port && ports_to_computers_mapper[i][j] == to) {
					flag2 = true;
					ports_data[i].push_back(frame_number);
					break;
				}
			}
		}


		// if frames source hasnt been logged then send frame to all ports 
		if (!flag2) {
			for (int i = 0; i < ports_to_computers_mapper.size(); i++) {
				for (int j = 0; j < ports_to_computers_mapper[i].size(); j++) {
					//if (ports_to_computers_mapper[i][0] != port && ports_to_computers_mapper[i][j] == to) {
					//	ports_data[i].push_back(frame_number);
					//	break;
					//}
					if ((ports_to_computers_mapper[i][0] != port && ports_to_computers_mapper[i][j] != from)) {
						ports_data[i].push_back(frame_number);
						break;
					}
				}
			}
		}
	}
	read2DVector(ports_to_computers_mapper);
	cout << "\n";
	//read2DVector(ports_data);
	return ports_data;
}


int main()
{
	//string path = "test.txt";
	string path = "frames.txt";
	string out_path = "out.txt";
	vector<string> raw_data_lines = retfile(path);
	vector<vector<string>>  data = string_file_to_data(raw_data_lines);
	vector<vector<string>>  switch_data;

	read2DVector(data);
	cout << "\n";
	switch_data = net_switch(data);

	fstream out_file; 
	out_file.open(out_path, ios_base::out);
	
	for (int i = 0; i < switch_data.size(); i++) {
		for (int j = 0; j < switch_data[i].size(); j++) {
			out_file << switch_data[i][j] << "\t";
		}
		out_file << "\n";
		out_file << "\n";
	}

	out_file.close();
	read2DVector(switch_data);

	return 0;
}