#include "FileLoader.h"
#include "Zone.h"
#include "TileMap.h"
#include "Animation.h"
#include "EntityCache.h"
#include "TextureCache.h"

#include "ComponentBag.h"
#include "RectangleComponent.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "InputComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "CameraComponent.h"
#include "HatComponent.h"
#include "AIComponent.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>

using namespace std;

const uint32 MaxBuffSize_ = 128;
const uint8 TYPE_STR = 0;
const uint8 TYPE_INT = 1;
const uint8 TYPE_FLOAT = 2;

/* Read the name of the chapter from the zone into the zone struct. */
int ReadName(FILE* chapterFile, Zone* zone) {
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	uint8 pos = 0;
	int lineNumber = 0;

	int c;
	// Loop until we reach an end of line.
	while ((c=fgetc(chapterFile)) != ';') {
		if (c == '\n') {
			lineNumber++;
		} else {
			if (c != '\t')
				str[pos++] = c;
		}
	}
	memcpy(&zone->name, str, sizeof(str));
	cout << "SUCCESS: Name successfully loaded!" << endl;
	return lineNumber;
}


/* Read in the tileset that we're using for this zone. */
int ReadTileset(FILE* chapterFile, SDL_Renderer* renderer) {
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	uint8 pos = 0;
	int lineNumber = 0;

	int c;
	// Loop until we reach an end of line.
	while ((c=fgetc(chapterFile)) != ';') {
		if (c == '\n') {
			lineNumber++;
		} else {
			if (c != '\t')
				str[pos++] = c;
		}
	}
	TextureCache_CreateTexture(renderer, str, "tileset");
	cout << "SUCCESS: tileset successfully loaded!" << endl;
	return lineNumber;
}


/* Read in each tile with the correct parameters. */
int ReadTileMap(FILE* chapterFile, Zone* zone) {
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	uint8 pos = 0;
	int lineNumber = 0;

	int c;
	// Loop until we reach an end of line.
	while ((c=fgetc(chapterFile)) != ';') {
		if (c == '\n') {
			lineNumber++;
		} else {
			if (c != '\t')
				str[pos++] = c;
		}
	}

	// Open tilemap file.
	FILE* tilemapFile = fopen(str, "r");
	if (tilemapFile == NULL) {
		std::cerr << "Error: The chapter file " << str << " was NULL" << std::endl;
		return lineNumber;
	}

	int t;
	TileMap* tileMap = &zone->tileMap;
	int xIndex = 0;
	int yIndex = 0;

	// Hold the tile data.
	Tile tile;
	tile.tid = 0;
	tile.solid = false;
	tile.moving = false;
	char tilestr[MaxBuffSize_];
	memset(&tilestr, 0, MaxBuffSize_);
	uint8 tilepos = 0;
	bool getParams = false;

	while ((t=fgetc(tilemapFile)) != EOF) {
		switch (t) {
			case '\n': // New line
				// Hacky for now
				tileMap->map[yIndex][xIndex] = tile;
				tilepos = 0;
				tile.tid = 0;
				tile.solid = false;
				tile.moving = false;
				memset(&tilestr, 0, MaxBuffSize_);
				xIndex++;
				getParams = false;

				tileMap->w = xIndex;
				yIndex++;
				xIndex = 0;
				break;
			case ' ': // Flush tile
				tileMap->map[yIndex][xIndex] = tile;
				tilepos = 0;
				tile.tid = 0;
				tile.solid = false;
				tile.moving = false;
				memset(&tilestr, 0, MaxBuffSize_);
				xIndex++;
				getParams = false;
				break;
			case ':': // Tile parameters
				if (tilepos != 0) {
					int tid = stoi(tilestr);
					tile.tid = tid;
					getParams = true;
				} else {
					cerr << "Error: Expected tile id but none was given." << endl;
				}
				break;
			case '0': // Null tile
				tile.tid = 0;
				break;
			case 0: // Null character error; needs better error checking
				cerr << "Error: Encountered a null character in the tilemap file at line " << lineNumber << "." << endl;
				break;
			case 's':
				if (getParams)
					tile.solid = true; // TO-DO: Make this not temporary.
				break;
			case 'm':
				if (getParams)
					tile.moving = true;// TO-DO: Make this not temporary.
				break;
			default: // Add to integer string
				tilestr[tilepos++] = t;
				break;
		}
	}
	tileMap->h = yIndex;
	fclose(tilemapFile);
	cout << "SUCCESS: Tilemap successfully loaded!" << endl;
	return lineNumber;
}


/* Helper isInteger method. */
int GetType(char* str) {
	int type = TYPE_INT;
	int decimalCount = 0;
	int i = 0;
	while (str[i] != 0) {
		if (str[i] == '.') {
			if (decimalCount == 1) {
				return TYPE_STR;
			}
			type = TYPE_FLOAT;
			decimalCount++;
		} else if (!isdigit(str[i])) {
			return TYPE_STR;
		} 
		i++;
	}
	return type;
}


/* Read entities into the entity cache. */
int ReadEntity(FILE* chapterFile, ComponentBag* cBag, SDL_Renderer* renderer) {
	int lineNumber = 0;
	Entity* entity = EntityCache_GetNewEntity();
	uint32 eid = entity->eid;

	// Command buffer
	char cmd[MaxBuffSize_];
	memset(&cmd, 0, MaxBuffSize_);
	uint8 cmdpos = 0;

	// Parameter buffer
	char param[MaxBuffSize_];
	memset(&param, 0, MaxBuffSize_);
	uint8 parampos = 0;
	bool getParams = false;

	queue<int> int_parameters;
	queue<string> str_parameters;
	queue<float> float_parameters;

	int c;

	// Loop until we reach an end of line.
	while ((c=fgetc(chapterFile)) != ';') {
		if (getParams) {
			if (c == ')') { // Add component for entity
				if (strcmp(cmd, "animation") == 0) {
					Animation animation;
					uint16 frames = (uint16)int_parameters.front();
					int_parameters.pop();
					float frameTime = float_parameters.front();
					float_parameters.pop();
					int spriteW = int_parameters.front();
					int_parameters.pop();
					int spriteH = int_parameters.front();
					int_parameters.pop();
					cout << "Adding animation to entity " << eid << ":(" << frames << "," << frameTime << "," << spriteW << "," << spriteH << ")" << endl;
					Animation_Initialize(&animation, frames, frameTime, spriteW, spriteH);
					AnimationComponent_Add(cBag->animationComponent, eid, animation);
				} else if (strcmp(cmd, "health") == 0) {
					int health = int_parameters.front();
					int_parameters.pop();
					cout << "Adding health to entity " << eid << ":(" << health << ")" << endl;
					HealthComponent_Add(cBag->healthComponent, eid, health);
				} else if (strcmp(cmd, "input") == 0) {
					cout << "Adding input to entity " << eid << "..." << endl;
					InputComponent_Add(cBag->inputComponent, eid);
				} else if (strcmp(cmd, "movement") == 0) {
					float xVelocity = float_parameters.front();
					float_parameters.pop();
					float yVelocity = float_parameters.front();
					float_parameters.pop();
					int xAccel = int_parameters.front();
					int_parameters.pop();
					int yAccel = int_parameters.front();
					int_parameters.pop();
					cout << "Adding movement to entity " << eid << ":(" << xVelocity << "," << yVelocity << "," << xAccel << "," << yAccel << ")" << endl;
					MovementComponent_Add(cBag->movementComponent, eid, xVelocity, yVelocity, xAccel, yAccel);
				} else if (strcmp(cmd, "physics") == 0) {
					int mass = int_parameters.front();
					int_parameters.pop();
					cout << "Adding physics to entity " << eid << ":(" << mass << ")" << endl;
					PhysicsComponent_Add(cBag->physicsComponent, eid, mass);
				} else if (strcmp(cmd, "rectangle") == 0) {
					int x = int_parameters.front();
					int_parameters.pop();
					int y = int_parameters.front();
					int_parameters.pop();
					int w = int_parameters.front();
					int_parameters.pop();
					int h = int_parameters.front();
					int_parameters.pop();
					cout << "Adding rectangle to entity " << eid << ":(" << x << "," << y << "," << w << "," << h << ")" << endl;
					RectangleComponent_Add(cBag->rectangleComponent, eid, x, y, w, h);
				} else if (strcmp(cmd, "texture") == 0) {
					string path = str_parameters.front();
					str_parameters.pop();
					string tag = str_parameters.front();
					str_parameters.pop();
					cout << "Adding texture to entity " << eid << ":(" << path << "," << tag << ")" << endl;
					TextureComponent_Add(cBag->textureComponent, eid, TextureCache_CreateTexture(renderer, path.c_str(), tag.c_str()));
				} else if (strcmp(cmd, "camera") == 0) {
					cout << "Adding camera to entity " << eid << "..." << endl;
					CameraComponent_Add(cBag->cameraComponent, eid);
				} else if (strcmp(cmd, "hat") == 0) {
					Hat hat;
					cout << "Adding hat to entity " << eid << "..." << endl;
					HatComponent_Add(cBag->hatComponent, eid, hat);
				} else if (strcmp(cmd, "ai") == 0) {
					int range = int_parameters.front();
					int_parameters.pop();
					int facing = int_parameters.front();
					int_parameters.pop();
					cout << "Adding AI to entity " << eid << "..." << endl;
					AIComponent_Add(cBag->aiComponent, eid, range, facing);
				} else {
					cerr << "Error: The given command is invalid: " << cmd << "." << endl;
				}
				memset(&cmd, 0, MaxBuffSize_);
				cmdpos = 0;
				getParams = false;
			} else if (c == ',') { // Add parameter to queue
				int type = GetType(param);
				switch (type) {
					case TYPE_INT:
						int_parameters.push(atoi(param));
						break;
					case TYPE_FLOAT:
						float_parameters.push(atof(param));
						break;
					case TYPE_STR:
						str_parameters.push(param);
						break;
				}
				memset(&param, 0, MaxBuffSize_);
				parampos = 0;
			} else if (c != '\t') {
					param[parampos++] = c;
			}
		} else if (c == '(') { // Add correct Component to the entity
			getParams = true;
		} else {
			if (c == '\n') // Increase line #
				lineNumber++;
			else if (c != '\t') // Add to command if not tab
				cmd[cmdpos++] = c;
		}
	}
	cout << "SUCCESS: Entity " << eid << " successfully loaded!" << endl;
	return lineNumber;
}


/* Read in a zone and its parts. */
int ReadZone(Zone* zone, FILE* chapterFile, ComponentBag* cBag, SDL_Renderer* renderer) {
	int c;
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	uint8 pos = 0;
	int lineNumber = 0;

	while ((c=fgetc(chapterFile)) != EOF) {
		if (c == '=') {
			if (strcmp(str, "name") == 0) {
				lineNumber += ReadName(chapterFile, zone);
			} else if (strcmp(str, "tileset") == 0) {
				lineNumber += ReadTileset(chapterFile, renderer);
			} else if (strcmp(str, "tilemap") == 0) {
				lineNumber += ReadTileMap(chapterFile, zone);
			} else if (strcmp(str, "entity") == 0) {
				lineNumber += ReadEntity(chapterFile, cBag, renderer);
			} else if (strcmp(str, "zone") == 0) {
				// Embedded zone, won't worry about that right now.
			} else if (strcmp(str, "END") == 0) {
				lineNumber++;
				cout << "SUCCESS: Zone successfully loaded!" << endl;
				return lineNumber;
			} else {
				std::cerr << "Error: Invalid token. Read " << str << " at line " << lineNumber << std::endl;
			}
			pos = 0;
			memset(&str, 0, MaxBuffSize_);
		} else {
			if (c == '\n') {
				lineNumber++;
			} else {
				if (c != '\t')
					str[pos++] = c;
			}
		}
	}
	cerr << "Error: Missing END tag. Why you no read syntax?" << endl;
	return lineNumber;
}


/* Begin reading a chapter file. */
void FileLoader_Load(Zone* zone, const char* path, ComponentBag* cBag, SDL_Renderer* renderer) {
	if (!zone) {
		std::cerr << "Error: The given zone was uninitialized." << std::endl;
		return;
	}

	FILE* chapterFile = fopen(path, "r");
	if (chapterFile == NULL) {
		std::cerr << "Error: The chapter file " << path << " was NULL" << std::endl;
		return;
	}

	int c;
	char str[MaxBuffSize_];
	memset(&str, 0, MaxBuffSize_);
	uint8 pos = 0;
	int lineNumber = 1; // For debugging purposes.

	while ((c=fgetc(chapterFile)) != EOF) {
		if (c =='=') {
			if (strcmp(str, "zone") == 0) {
				lineNumber += ReadZone(zone, chapterFile, cBag, renderer);
				pos = 0;
				memset(&str, 0, MaxBuffSize_);
			} else {
				std::cerr << "Error: A \'Zone\' tag was not found initially" << std::endl;
				return;
			}
		} else {
			if (c == '\n') {
				lineNumber++;
			} else {
				str[pos] = c;
				pos++;
			}
		}
	}
	cout << "SUCCESS: File successfully loaded!" << endl;
	fclose (chapterFile);
}



