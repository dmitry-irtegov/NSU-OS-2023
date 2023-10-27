#define _FILE_OFFSET_BITS 64
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <algorithm>

using namespace std;

bool checker(string name, string pattern) {
	int nameId = 0;
	int patternId = 0;
	bool fits = false;
	while (patternId < pattern.size() && nameId < name.size()) {
		if (pattern[patternId] != '*' && pattern[patternId] != '?') { // not & or *
			if (pattern[patternId] != name[nameId]) {
				break;
			}
			else {
				nameId++;
				patternId++;
				continue;
			}
		}
		else if (pattern[patternId] == '?') { //?
			nameId++;
			patternId++;
			continue;
		}
		else { //'*'
			while (patternId < pattern.size() && pattern[patternId] == '*') {
				patternId++;
			}
			if (pattern.size() == patternId) {// pattern ended with '*'
				fits = true;
				break;
			}

			else if (patternId >= pattern.size()) {  //"*?"
				break;
			}
			int questionsCount = 0;
			while (patternId + questionsCount < pattern.size() && pattern[patternId + questionsCount] == '?') {
				questionsCount++;
			}
			if (pattern[patternId + questionsCount] == '*') {//*?*
				patternId += questionsCount;
				nameId += questionsCount;
				continue;
			}

			string NOLEFT = pattern.substr(patternId + questionsCount);
			string closestSubstring = NOLEFT.substr(0, NOLEFT.find_first_of("*?"));


			if (name.find_first_of(closestSubstring, nameId + questionsCount) == -1) {
				break;
			}
			else {
				nameId = name.find_first_of(closestSubstring, nameId + questionsCount);
				nameId += closestSubstring.size();
				patternId += questionsCount + closestSubstring.size();
			}
		}
	}

	if (name.size() == nameId) {
		while (pattern.size() > patternId) {
			if ('*' != pattern[patternId]){
				break;
			}
			patternId++;
		}
		if (pattern.size() == patternId) {
			fits = true;
		}
	}
	return fits;
}

int main(int argc, char **argv) {

	string pattern;
	struct dirent *cur;
	DIR *dir;
	int fits = 0;

	// open current directory
	if ((dir = opendir(".")) == NULL) {
        perror("Couldn't open current directory");
        return 1;
	}
	cout<<"pattern is:"<<endl;
	cin>>pattern;
	for (int i = 0; i<pattern.size(); i++) {
		if (pattern[i] == '/') {
			cout<<"Using / is restricted"<<endl;
            		return 0;
		}
	}

	while ((cur = readdir(dir)) != NULL) {
		string curName(cur->d_name);
		fits+=checker(curName,pattern);      
        }
    
	if (!fits) {
		cout<<pattern<<" did not matched any file in the current directory"<<endl;
	}
	else{
		cout<<"found " << fits<<" matches"<<endl;
	}
	closedir(dir);
	return 0;
}
