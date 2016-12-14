#include "UIRenderSystem.h"
#include "Game.h"
#include "TextureCache.h"
#include "RenderSystem.h"
#include "StateLoader.h"
#include "StatSystem.h"

#include <SDL.h>
#include <string>
#include <iostream>

/* Render main menu. */
void RenderTitle(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background != nullptr) {
		RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	}
	Texture* overlay = TextureCache_GetTexture(Constants::MainMenuO_);
	if (overlay != nullptr) {
		RenderSystem_Render_xywh(game->renderer, 0, 0, overlay->w, overlay->h, NULL, overlay);
	}
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		Texture* selection;
		if (selectionIndex == game->titleState.selection) {
			std::string base;
			if (selectionIndex == 0 && game->playState.unlockedLevels > 1)
				base = game->titleState.selectionStrings[Constants::TitleScreenSelections_];
			else
				base = game->titleState.selectionStrings[selectionIndex];
			base.append("_base");
			selection = TextureCache_GetTexture(base.c_str());
		} else {
			std::string select;
			if (selectionIndex == 0 && game->playState.unlockedLevels > 1)
				select = game->titleState.selectionStrings[Constants::TitleScreenSelections_];
			else
				select = game->titleState.selectionStrings[selectionIndex];
			select.append("_select");
			selection = TextureCache_GetTexture(select.c_str());
		}
		
		if (selection == nullptr)
			continue;

		int renderX = Constants::ScreenWidth_ / 2 - selection->w / 2;
		int renderY = (selectionIndex + 2)* (selection->h + 35) - 50;
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection);
	}
	SDL_RenderPresent(game->renderer);
}


/* Render intro with proper fade. */
void RenderIntro(Game* game, uint32 elapsed) {
	// Check if the intro is done
	game->introState.elapsed += elapsed;
	game->introState.alpha = 1 - (((float)game->introState.elapsed) / ((float)Constants::IntroTime_));
	if ( game->introState.elapsed  >= Constants::IntroTime_) {
		game->gameState = GameState_Title;
	}

	// Render intro
	Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	Texture* menuOverlay = TextureCache_GetTexture(Constants::MenuOverlay_);

	if (background) {
	  RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	}
	if (fader) { 
		RenderSystem_Render_xywh(game->renderer, 0, 0, menuOverlay->w, menuOverlay->h, NULL,menuOverlay);
		SDL_SetTextureAlphaMod(fader->sdltexture, (game->introState.alpha * 255));
		RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, NULL,  fader);
	}
	SDL_RenderPresent(game->renderer);
}


/* Render intro with transitions between cut scenes. */
void RenderZoneIntro(Game* game, uint32 elapsed, bool* keysdown, bool* keysup) {
	// Render fade
	if (game->zoneIntroState.startScene.current ==game->zoneIntroState.startScene.slideCount) {
		game->zoneIntroState.elapsed += elapsed;
		CameraSystem_Update(&game->playState.cameraSystem);
		SDL_RenderClear(game->renderer);
		game->zoneIntroState.alpha = 1 - ((float)game->zoneIntroState.elapsed / Constants::ZoneIntroTime_);
		
		// Render intro
		Texture* fader = TextureCache_GetTexture(Constants::TitleFader_);
		SDL_RenderClear(game->renderer);
		if (fader) { 
			RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed, game->playState.levelSelection);
			Texture* name = TextureCache_GetTexture(Constants::ZoneName_);
			if (!name) {
				std::cerr << "Error: Unable to load the zone's name texture" << std::endl;
				return;
			}
			int renderX = Constants::ScreenWidth_ / 2 - name->w / 2;
			int renderY = Constants::ScreenHeight_/2 - name->h / 2;
			RenderSystem_Render_xywh(game->renderer, renderX, renderY, name->w, name->h, NULL, name);
			SDL_SetTextureAlphaMod(fader->sdltexture, (game->zoneIntroState.alpha * 255));
			RenderSystem_Render_xywh(game->renderer, 0, 0, fader->w, fader->h, NULL,  fader);
		}
		SDL_RenderPresent(game->renderer);
		if (game->zoneIntroState.elapsed >= Constants::ZoneIntroTime_) {
			game->gameState = GameState_Play;
		}
	} else { // Render cut scene
		if (keysdown[SDLK_SPACE % Constants::NumKeys_] && keysup[SDLK_SPACE % Constants::NumKeys_]) {
			if (game->zoneIntroState.startScene.current + 1 != game->zoneIntroState.startScene.slideCount) {
				keysdown[SDLK_SPACE  % Constants::NumKeys_] = false;
				keysup[SDLK_SPACE  % Constants::NumKeys_] = false;
				game->zoneIntroState.sliding = true;
			} else {
				game->zoneIntroState.startScene.current++;
			}
		}

		if (game->zoneIntroState.sliding) {
			game->zoneIntroState.elapsed += elapsed;
			if (game->zoneIntroState.elapsed >= Constants::SlideTime_) {
				float xOffset = ((float)(game->zoneIntroState.elapsed - Constants::SlideTime_) / Constants::OvershootTime_) * ((float)Constants::SlideOvershoot_);
				Texture* scene = game->zoneIntroState.startScene.slides[game->zoneIntroState.startScene.current + 1];
				if (scene) {
					RenderSystem_Render_xywh(game->renderer, xOffset - Constants::SlideOvershoot_, 0, scene->w, scene->h, NULL, scene);
				}
				SDL_RenderPresent(game->renderer);
				if (xOffset - Constants::SlideOvershoot_ > 0) {
					game->zoneIntroState.sliding = false;
					game->zoneIntroState.elapsed = 0;
					game->zoneIntroState.startScene.current++;
				}
			} else {
				float xOffset = ((float)game->zoneIntroState.elapsed / Constants::SlideTime_) * ((float)Constants::ScreenWidth_ + Constants::SlideOvershoot_);
				SDL_RenderClear(game->renderer);
				Texture* scene1 = game->zoneIntroState.startScene.slides[game->zoneIntroState.startScene.current];
				if (scene1) {
					RenderSystem_Render_xywh(game->renderer, 0, -xOffset, scene1->w, scene1->h, NULL, scene1);
				}
				Texture* scene2 = game->zoneIntroState.startScene.slides[game->zoneIntroState.startScene.current + 1];
				if (scene2) {
					RenderSystem_Render_xywh(game->renderer, Constants::ScreenWidth_ - xOffset, 0, scene2->w, scene2->h, NULL, scene2);
				}
				SDL_RenderPresent(game->renderer);
			}
		} else {
			SDL_RenderClear(game->renderer);
			Texture* scene = game->zoneIntroState.startScene.slides[game->zoneIntroState.startScene.current];
			if (scene) {
				RenderSystem_Render_xywh(game->renderer, 0, 0, scene->w, scene->h, NULL, scene);
			}
			SDL_RenderPresent(game->renderer);
		}
	}
}

/* Render options based on current selection. */
void RenderOptions(Game* game, uint32 elapsed) {
	//std::cout<< "in redner options" << std::endl;
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	Texture* soundBar = TextureCache_GetTexture(Constants::SoundBar_);

	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);

	Texture* overlay = TextureCache_GetTexture(Constants::OptionsO_);
	RenderSystem_Render_xywh(game->renderer, 0, 0, overlay->w, overlay->h, NULL, overlay);

	int renderX = Constants::ScreenWidth_ / 2 - soundBar->w / 2;
	RenderSystem_Render_xywh(game->renderer, renderX, 400, soundBar->w, soundBar->h, NULL, soundBar);

	//draw sound amount
	float ratio = (float) game->optionState.musicVolume / Constants::MaxVolume_;
	const SDL_Rect currentRect = {renderX + 8, 444, static_cast<int>((Constants::ScreenWidth_/5)*ratio), Constants::ScreenHeight_/16};
	int r = 157;
	int r2 = 131;
	int g = 131;
	int g2 = 167;
	int b = 255;
	r = r2 + (r - r2)*ratio;
	g = g2 + (g - g2)*ratio;
	SDL_SetRenderDrawColor(game->renderer, r, g, b, 1);
	SDL_RenderFillRect(game->renderer, &currentRect);

	//draw choices
	for (int selectionIndex = 0; selectionIndex < Constants::OptionScreenSelections_; selectionIndex++) {
		Texture* selection;
		if (selectionIndex == game->optionState.selection) {
			std::string base = game->optionState.selectionStrings[selectionIndex];
			base.append("_base");
			selection = TextureCache_GetTexture(base.c_str());
		} else {
			std::string select = game->optionState.selectionStrings[selectionIndex];
			select.append("_select");
			selection = TextureCache_GetTexture(select.c_str());
		}
		int renderY = (selectionIndex+3) * (selection->h + 35);
		renderX = Constants::ScreenWidth_ / 2 - selection->w / 2;
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection);
	}
	SDL_RenderPresent(game->renderer);
}

/* Render high scores. */
void RenderHighScore(Game* game, uint32 elapsed) {
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	Texture* overlay = TextureCache_GetTexture(Constants::StatsO_);
	RenderSystem_Render_xywh(game->renderer, 0, 0, overlay->w, overlay->h, NULL, overlay);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(highScoreIndex));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = Constants::ScreenWidth_ / 2 - score->w / 2;
		int renderY = (3 + highScoreIndex) * (score->h + 35);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score);
	}
	SDL_RenderPresent(game->renderer);
}

/* Render level scores. */
void RenderLevelStats(Game* game, uint32 elapsed) {
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
	if (game->gameState == GameState_Win){
		background = TextureCache_GetTexture(Constants::WinBackground_);
	} else if (game->gameState == GameState_Lose){
		background = TextureCache_GetTexture(Constants::LoseBackground_);
	}
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	//Header
	Texture* score = TextureCache_GetTexture("levelheader");
	int renderX = (Constants::ScreenWidth_ / 8 )- score->w / 2;
	int renderY = (0 * (Constants::ScreenHeight_ / 20)) + 300;;
	RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score);

	int displayed = 1;
	for (int highScoreIndex = 0; highScoreIndex < NumScoreTypes_; highScoreIndex++) {
		if (numPossibleScores[highScoreIndex] != 0 || (highScoreIndex == Deaths_ 
			|| highScoreIndex == Fallen_ || highScoreIndex == GameTime_)){
		std::string name = "level_score_";
		name.append(std::to_string(highScoreIndex));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = (Constants::ScreenWidth_ / 8 )- score->w / 2;
		int renderY = (displayed * (Constants::ScreenHeight_ / 20)) + 300;
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score);
		displayed++;
		}
	}
	SDL_RenderPresent(game->renderer);
}


/* Render the paused state with given texure and speech bubbles. */
void RenderPauseState(Game* game, uint32  elapsed) {
	(void) elapsed;
	SDL_RenderClear(game->renderer);
	RenderSystem_Update(&game->playState.renderSystem, game->renderer, 0, game->playState.levelSelection);
	Texture* texture = game->pauseState.pauseTextures[game->pauseState.pauseIndex];
	Texture* speech = TextureCache_GetTexture("speech-bubble");
	if (speech == NULL) {
		return;
	}
	if (texture != NULL) {
		int bubbleX = Constants::ScreenWidth_ / 2 - speech->w / 2;
		int bubbleY = Constants::ScreenHeight_ / 2 - speech->h / 2;
		RenderSystem_Render_xywh(game->renderer, bubbleX, bubbleY, speech->w, speech->h, NULL, speech);
		int renderX = Constants::ScreenWidth_ / 2 - texture->w / 2;
		int renderY = Constants::ScreenHeight_ / 2 - texture->h / 2;
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, texture->w, texture->h, NULL, texture);
	} else {
		game->gameState = GameState_Play;
	}
	SDL_RenderPresent(game->renderer);
}


void RenderLevelSelect(Game* game, uint32 elapsed) {
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	Texture* overlay = TextureCache_GetTexture(Constants::LevelsO_);
	RenderSystem_Render_xywh(game->renderer, 0, 0, overlay->w, overlay->h, NULL, overlay);
	for (int i = 0; i <= game->playState.unlockedLevels; i++) {
		std::string lvl = "";
		lvl.append(std::to_string(i));
		lvl.append("_level");
		Texture* texture = TextureCache_GetTexture(lvl.c_str());
		if (game->levelSelectState.selection == i) {
			lvl.append("_select");
			texture = TextureCache_GetTexture(lvl.c_str());
		}
		int renderX = (Constants::ScreenWidth_ - texture->w) / 2;
		int renderY = (i+3) * (texture->h + 25);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, texture->w, texture->h, NULL, texture);
	}
	SDL_RenderPresent(game->renderer);
}


/* Select the correct render based on state. */
void UIRenderSystem_Render(int gameState, Game* game, uint32 elapsed, bool* keysdown, bool* keysup) {
	switch (gameState) {
		case GameState_Intro:
			RenderIntro(game, elapsed);
			break;
		case GameState_Title:
			RenderTitle(game);
			break;
		case GameState_ZoneIntro:
			RenderZoneIntro(game, elapsed, keysdown, keysup);
			break;
		case GameState_HighScore:
			RenderHighScore(game, elapsed);
			break;
		case GameState_Options:
			RenderOptions(game, elapsed);
			break;
		case GameState_Win:
			RenderLevelStats(game,elapsed);
			break;
  		case GameState_Lose:
  			RenderLevelStats(game,elapsed);
  			break;
  		case GameState_Pause:
  			RenderPauseState(game, elapsed);
  			break;
  		case GameState_LevelSelect:
  			RenderLevelSelect(game, elapsed);
  			break;
  		default:
			break;
	}
}
