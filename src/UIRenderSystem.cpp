#include "UIRenderSystem.h"
#include "Game.h"
#include "TextureCache.h"
#include "RenderSystem.h"

#include <SDL.h>
#include <string>
#include <iostream>

void RenderTitle(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	for (int selectionIndex = 0; selectionIndex < Constants::TitleScreenSelections_; selectionIndex++) {
		Texture* selection;
		if (selectionIndex == game->titleState.selection) {
			std::string base = game->titleState.selectionStrings[selectionIndex];
			base.append("_base");
			selection = TextureCache_GetTexture(base.c_str());
		} else {
			std::string select = game->titleState.selectionStrings[selectionIndex];
			select.append("_select");
			selection = TextureCache_GetTexture(select.c_str());
		}
		int renderX = Constants::ScreenWidth_ / 2 - selection->w / 2;
		int renderY = selectionIndex * (Constants::ScreenHeight_ / Constants::TitleScreenSelections_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection);
	}
	SDL_RenderPresent(game->renderer);
}


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
			RenderSystem_Update(&game->playState.renderSystem, game->renderer, elapsed);
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
					RenderSystem_Render_xywh(game->renderer, -xOffset, 0, scene1->w, scene1->h, NULL, scene1);
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


void RenderLose(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::LoseBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	SDL_RenderPresent(game->renderer);
}


void RenderWin(Game* game) {
	Texture* background = TextureCache_GetTexture(Constants::WinBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	SDL_RenderPresent(game->renderer);
}


void RenderOptions(Game* game, uint32 elapsed) {
	//std::cout<< "in redner options" << std::endl;
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
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
		int renderX = Constants::ScreenWidth_ / 2 - selection->w / 2;
		int renderY = selectionIndex * (Constants::ScreenHeight_ / Constants::OptionScreenSelections_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, selection->w, selection->h, NULL, selection);
	}
	SDL_RenderPresent(game->renderer);
}

void RenderHighScore(Game* game, uint32 elapsed) {
	(void) elapsed;
	Texture* background = TextureCache_GetTexture(Constants::TitleBackground_);
	SDL_RenderClear(game->renderer);
	if (background) RenderSystem_Render_xywh(game->renderer, 0, 0, background->w, background->h, NULL, background);
	for (int highScoreIndex = 0; highScoreIndex < Constants::MaxHighScores_; highScoreIndex++) {
		std::string name = "high_score_";
		name.append(std::to_string(game->highScoreState.scores[highScoreIndex]));
		Texture* score = TextureCache_GetTexture(name.c_str());
		int renderX = Constants::ScreenWidth_ / 2 - score->w / 2;
		int renderY = highScoreIndex * (Constants::ScreenHeight_ / Constants::MaxHighScores_);
		RenderSystem_Render_xywh(game->renderer, renderX, renderY, score->w, score->h, NULL, score);
	}
	SDL_RenderPresent(game->renderer);
}

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
			RenderWin(game);
			break;
  		case GameState_Lose:
  			RenderLose(game);
			break;
	}
}