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
	cout << "adding " << add << "to total of " << total << endl;
	total += add;
	cout << "total is now " << total << endl;
	return total;
}

void Copy(const char* path, int pos, std::string value){
	//copies file into another file up to position, writes the value
	///then copies the rest of the file over
	cout << "in copy" << endl;
	FILE* scoreFile = fopen(path, "r+");
	if (scoreFile == NULL) {
		std::cerr << "Error: The score file " << path << " was NULL" << std::endl;
		return;
	}
	//newfile name
	std::string newpath = path;
	newpath.append("1");
	const char* newPath = newpath.c_str();

	FILE* newScoreFile = fopen(path, "w");
	if (newScoreFile == NULL) {
		std::cerr << "Error: The score file " << newPath << " was NULL" << std::endl;
		return;
	}
	cout << "after file opening" << endl;

	int c = 0;
	int filepos = 0;	
	//copy everything before the position
	while (filepos < pos){
		c = fgetc(scoreFile);
		fputc(c, newScoreFile);
		filepos++;
	}
	cout << "writing to positioons" << endl;
	//write the value over
	fseek(newScoreFile, filepos-1, SEEK_SET);
	fputs(value.c_str(), newScoreFile);
	//start copying from the ';'
	while ((c=fgetc(scoreFile)) != ';') {
		//run to ignore the previous value
	}
	cout << "after the ; " << endl;
	fputc(c, newScoreFile);
	while ((c=fgetc(scoreFile)) != EOF) {
		fputc(c, newScoreFile);
	}

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
	pp = 0;
	read = false;
	while ((c=fgetc(scoreFile)) != EOF) {
		if (c =='=') {
			if (strcmp(str, type) == 0) {
				if (strcmp(path, levelpath)==0 || strcmp(path, hatspath) == 0 ||
					strcmp(path, scorepath) == 0){
					c = fgetc(scoreFile);
					pp++;
					if ((char)c == '0'){//We add to the total
						addTotal = true;
					}
					std::string total = value;
					total.append(";");
					//pp++;
					fseek(scoreFile, pp, SEEK_SET);
					fputs(total.c_str(), scoreFile); //writing value to text file
				} else {
					addTotal = true;
					int currentTotal = Add(scoreFile,value);
					std::string total = std::to_string(currentTotal);

					//total.append(";");

					//fseek(scoreFile, pp - 1, SEEK_SET);
					//fputs(total.c_str(), scoreFile);
					Copy(path, pp -1 , total);
				}
			} else if (strcmp(str, "total") == 0) {
				if (addTotal == true){
					cout << "PP IS: " << pp << " FOR WORD total " << endl;
					int currentTotal = Add(scoreFile,value);
					std::string total = std::to_string(currentTotal);
					total.append(";");
					fseek(scoreFile, pp -1, SEEK_SET);
					fputs(total.c_str(), scoreFile); //writing value to text file
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
		cout << "pp is " << pp << endl;
	}
	cout << "SUCCESS: Stored value in file." << endl;
	fclose (scoreFile);
	//Format(path);
	//update scores now 
	if (strcmp(path, scorepath) != 0){
		Scoreboard_Update();
	}
	//remove("assets/score/hats.txt");
	return;
}

