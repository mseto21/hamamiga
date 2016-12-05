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
const char* deathspath = "assets/score/deaths.txt";
const char* killpath = "assets/score/kills.txt";

int pp = 0; //reset whenever reading from file

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
	return atoi(str);
}

int totalFromFile(const char * path, const char * type){
	int total = -1;
FILE* scoreFile = fopen(path, "r");
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
			if (strcmp(str, type) == 0) {
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
	return total;;
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

int GetPosNum(const char* path, const char* type, bool * addTotal){
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
				if (strcmp(path, levelpath)==0 || strcmp(path, hatspath) == 0){
					c = fgetc(scoreFile);
					if ((char)c == '0'){//We add to the total
						*addTotal = true;
					}
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
	fclose (scoreFile);
	return typePos;
}

//Calculates total scores for scoreboard
void Scoreboard_Update(){
	bool add = false;
//Getting total from hat
	int hatTotal = totalFromFile(hatspath, "total");
	std::string hatT = std::to_string(hatTotal);
//Getting total from level
	int levelTotal = totalFromFile(levelpath, "total");
	std::string levelT = std::to_string(levelTotal);
//Getting total from deaths
	int deathTotal = totalFromFile(deathspath, "killed");
	std::string deathT = std::to_string(deathTotal);
	int fallenTotal = totalFromFile(deathspath, "fallen");
	std::string fallenT = std::to_string(fallenTotal);
//Getting total from kills
	int killTotal = totalFromFile(killpath, "total");
	std::string killT = std::to_string(killTotal);
//Storing in scoreboard
	Copy(scorepath, GetPosNum(scorepath, "hats", &add),  hatT.c_str(), add);
	Copy(scorepath, GetPosNum(scorepath, "levels", &add), levelT.c_str(), add);
	Copy(scorepath, GetPosNum(scorepath, "deaths", &add), deathT.c_str(), add);
	Copy(scorepath, GetPosNum(scorepath, "fallen", &add), fallenT.c_str(), add);
	Copy(scorepath, GetPosNum(scorepath, "kills", &add), killT.c_str(), add);

}

void Scores_Update(const char* path, const char* type, const char* value) {
	bool add = false;
	const char * total = "total";
	int typePos = GetPosNum(path, type, &add);
	int totalPos = GetPosNum(path, total, &add);
	//update scores now
	//If we don't want to add to the total set the position to zero
	if (strcmp(path, levelpath)==0 || strcmp(path, hatspath) == 0){
		GetPosNum(path, total, &add);
	} else {
		add = true;
	}
	Copy(path, typePos, value, add);
	if (add == true){
		if (totalPos!= 0){
		Copy(path, totalPos, value, add);
		}
	}

	if (strcmp(path, scorepath) != 0){
		Scoreboard_Update();
	}

	return;
}

