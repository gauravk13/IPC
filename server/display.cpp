#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
int group_id;
while(1) {
  
	cout<< "\npress ctrl+ C to exit \n";
	cout<< "\n 1. display messages of group Id : ";
	cin>>group_id;
	string file_name = to_string(group_id);
	file_name.append(".txt");
	 ifstream myReadFile;
	 myReadFile.open(file_name);
	 char output[100];
	 if (myReadFile.is_open()) {
		 while (!myReadFile.eof()) {
	 		 myReadFile >> output;
   	 		 cout<<output;
 		}
	}
	myReadFile.close();
}
	return 0;
}
