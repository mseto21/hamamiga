#include "StatSystem.h"
#include "Constants.h"

#include <cstdio>
#include <iostream>

using namespace std;

const uint32 MaxBuffSize_ = 128;
const char* scorepath = "assets/score/score.txt";
const char* levelpath = "assets/score/levels.txt";
const char* hatspath = "assets/score/hats.txt";

int ReadTotal(FILE* scoreFile) {
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	int pos = 0;
	int c;
	// Loop until we reach an end of line.
	while ((c=fgetc(scoreFile)) != ';') {
		if (c == '\n') {
		} else {
			if (c != '\t')
				str[pos++] = c;
		}
	}
	return atoi(str);
}

int totalFromFile(const char * path){
	int total = -1;
FILE* scoreFile = fopen(path, "r+");
	if (scoreFile == NULL) {
		std::cerr << "Error: The score file " << path << " was NULL" << std::endl;
		return total;
	}
	int c = 0;
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	int pos = 0;
	while ((c=fgetc(scoreFile)) != EOF) {
		if (c =='=') {
			if (strcmp(str, "total") == 0) {
				total = ReadTotal(scoreFile);
			}
		} else {
			if (c == '\n') {
				pos = 0;
				memset(&str, 0, MaxBuffSize_);
			} else {
				str[pos] = c;
				pos++;
			}
		}
	}
	fclose (scoreFile);
	//update scores now 
	return total;;
}

//Calculates total scores for scoreboard
void Scoreboard_Update(){
//Getting total from hat
	int hatTotal = totalFromFile("assets/score/hats.txt");
	std::string hatT = std::to_string(hatTotal);
	cout << "hat total is: " << hatT << endl;
//Getting total from level
	int levelTotal = totalFromFile("assets/score/levels.txt");
	std::string levelT = std::to_string(levelTotal);
	cout << "level total is: " << levelT << endl;
//Getting total from deaths
	int deathTotal = totalFromFile("assets/score/deaths.txt");
	std::string deathT = std::to_string(deathTotal);
	cout << "death total is: " << deathT << endl;
//Storing in scoreboard
	Scores_Update(scorepath, (char*) "hats", hatT.c_str());
	Scores_Update(scorepath, (char*) "levels", levelT.c_str());
	Scores_Update(scorepath, (char*) "deaths", deathT.c_str());

}

int Add(FILE* scoreFile, const char* value){
	int add = atoi(value);
	int total = ReadTotal(scoreFile);
	cout << "adding " << add << "to total of " << total << endl;
	total += add;
	cout << "total is now " << total << endl;
	return total;
}

void Scores_Update(const char* path, const char* type, const char* value) {
FILE* scoreFile = fopen(path, "r+");
	if (scoreFile == NULL) {
		std::cerr << "Error: The score file " << path << " was NULL" << std::endl;
		return;
	}
	int c = 0;
	bool addTotal = false;
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	int pos = 0;
	int pp = 0;
	while ((c=fgetc(scoreFile)) != EOF) {
		if (c =='=') {
			if (strcmp(str, type) == 0) {
				if (strcmp(path, levelpath)==0 || strcmp(path, hatspath) == 0 ||
					strcmp(path, scorepath) == 0){
					c = fgetc(scoreFile);
					if ((char)c == '0'){//We add to the total
						addTotal = true;
					}
					pos = pp +1;
					fseek(scoreFile, pos, SEEK_SET);
					fputs(value, scoreFile); //writing value to text file
				} else {
					addTotal = true;
					pos = pp +1;
					int currentTotal = Add(scoreFile,value);
					std::string total = std::to_string(currentTotal);
					cout << "writing value " << currentTotal << "to total" << endl;
					fseek(scoreFile, pos, SEEK_SET);
					fputs(total.c_str(), scoreFile);
				}
			} else if (strcmp(str, "total") == 0) {
				if (addTotal == true){
					pos = pp +2;
					int currentTotal = Add(scoreFile,value);
					std::string total = std::to_string(currentTotal);
					cout << "writing value " << currentTotal << "to total" << endl;
					fseek(scoreFile, pos, SEEK_SET);
					fputs(total.c_str(), scoreFile); //writing value to text file
					cout << "puts at pos " << pos << endl;
					break;
				}
			}
		} else {
			if (c == '\n') {
				pos = 0;
				memset(&str, 0, MaxBuffSize_);
			} else {
				str[pos] = c;
				pos++;
			}
		}
		pp++;
	}
	cout << "SUCCESS: Stored value in file." << endl;
	fclose (scoreFile);
	//update scores now 
	if (strcmp(path, scorepath) != 0){
		Scoreboard_Update();
	}
	return;
}

