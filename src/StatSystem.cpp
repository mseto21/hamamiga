#include "StatSystem.h"
#include "Constants.h"

#include <cstdio>
#include <stdio.h>
#include <iostream>

using namespace std;

const uint32 MaxBuffSize_ = 128;
const char* scorepath = "assets/score/score.txt";
const char* levelpath = "assets/score/levels.txt";
const char* hatspath = "assets/score/hats.txt";

int pp = 0; //reset whenever reading from file
bool read = false; //flag for checking if read from total

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
		pp++;
	}
	pp++;
	read = true;
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
	total += add;
	return total;
}

void Copy(const char* path, int pos, const char * value, bool add){
	//copies file into another file up to position, writes the value
	///then copies the rest of the file over
	FILE* scoreFile = fopen(path, "r");
	if (scoreFile == NULL) {
		std::cerr << "Error: The score file " << path << " was NULL" << std::endl;
		return;
	}
	//newfile name
	std::string newpath = path;
	newpath.append("1");
	const char* newPath = newpath.c_str();

	FILE* newScoreFile = fopen(newPath, "w");
	if (newScoreFile == NULL) {
		std::cerr << "Error: The score file " << newPath << " was NULL" << std::endl;
		return;
	}

	int c = 0;
	int filepos = 0;	
	//copy everything before the position
	while (filepos < pos){
		c = fgetc(scoreFile);
		fputc(c, newScoreFile);
		filepos++;
	}
	if (add == true){
		int currentTotal = Add(scoreFile,value);
		std::string total = std::to_string(currentTotal);
		//write the value over
		fseek(newScoreFile, filepos, SEEK_SET);
		fputs(total.c_str(), newScoreFile);
	} else { //just write the value if we're not adding to total
		Add(scoreFile,value); //removing the ';'
		fseek(newScoreFile, filepos, SEEK_SET);
		fputs(value, newScoreFile);
	}
	//start copying from the ';'
	fputc((int) ';', newScoreFile);
	while ((c=fgetc(scoreFile)) != EOF) {
		fputc(c, newScoreFile);
	}
	
	fclose(scoreFile);
	fclose(newScoreFile);

	remove(path);
	rename(newPath, path);

}

int GetPosNum(const char* path, const char* type){
FILE* scoreFile = fopen(path, "r");
	if (scoreFile == NULL) {
		std::cerr << "Error: The score file " << path << " was NULL" << std::endl;
		return -1;
	}
	int c = 0;
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	int pos = 0;
	int typePos = 0;
	pp = 0;

	while ((c=fgetc(scoreFile)) != EOF) {
		if (c =='=') {
			if (strcmp(str, type) == 0) {
				typePos = pp+1;
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
	fclose (scoreFile);
	return typePos;
}

void Scores_Update(const char* path, const char* type, const char* value) {
	bool add = true;
	const char * total = "total";
	int typePos = GetPosNum(path, type);
	int totalPos = GetPosNum(path, total);
	//update scores now
	//If we don't want to add to the total set the position to zero
	if (strcmp(path, levelpath)==0 || strcmp(path, hatspath) == 0 ||
					strcmp(path, scorepath) == 0){
		add = false;
	}
	Copy(path, typePos, value, add);
	if (add == true){
		Copy(path, totalPos, value, add);
	}

	if (strcmp(path, scorepath) != 0){
		Scoreboard_Update();
	}

	return;
}

