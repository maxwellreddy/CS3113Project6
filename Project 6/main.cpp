// Maxwell Reddy
// CS 3113
// Assignment 6

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#include <vector> 
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "YouWin.h"
#include "GameOver.h"
#include "Title.h"

Scene* currentScene;

Scene* sceneList[4];

Mix_Chunk* fireSound;

void SwitchToScene(Scene* scene) {
	currentScene = scene;
	currentScene->Initialize();
}

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool start = false;
bool restarting = false;
bool livesUpdated = false;

int fireIndex = 0;
int fireCount = 0;
int remainingFire = 12;
int remLives = 5;
int firingTimer = 10;

bool left = false;
bool right = false;
bool up = false;
bool down = false;
bool shift = false;
bool firing = false;

bool end = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

void Initialize() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	displayWindow = SDL_CreateWindow("Project 6: Touhou ~ Marisa Kirisame's Awesome Battle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 800, 600);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	fireSound = Mix_LoadWAV("Player Shot Sound.wav");

	viewMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	sceneList[0] = new Title();
	sceneList[1] = new Level1();
	sceneList[2] = new YouWin();
	sceneList[3] = new GameOver();
	SwitchToScene(sceneList[0]);
}

void ProcessInput() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_LEFT:

				left = true;

				break;

			case SDLK_RIGHT:
				right = true;
				break;
			case SDLK_UP:
				up = true;
				break;
			case SDLK_DOWN:
				down = true;
				break;
			case SDLK_LSHIFT:
				shift = true;
				break;

			case SDLK_RETURN:
				if (start == false) {
					currentScene->state.start = true;
					start = true;
				}
				break;

			case SDLK_z:
				// Makes the player shoot
				firing = true;

			
			}

			break; // SDL_KEYDOWN
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_z:
				firing = false;
				break;
			case SDLK_LEFT:
				left = false;
				break;
			case SDLK_RIGHT:
				right = false;
				break;
			case SDLK_UP:
				up = false;
				break;
			case SDLK_DOWN:
				down = false;
				break;
			case SDLK_LSHIFT:
				shift = false;
				break;
			}

			break; // SDL_KEYDOWN
		}
	}


	const Uint8* keys = SDL_GetKeyboardState(NULL);

	// Left and right keys move the player
	
	if (start == true && currentScene->state.start == false && end == false) {
		if (left == true && currentScene->state.player->isDead == false) {
			if (right == true && up == false && down == false) {
				currentScene->state.player->velocity.x = 0.0f;
				currentScene->state.player->velocity.y = 0.0f;
			}
			else if (up == true && right == false && down == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = -1.414f;
					currentScene->state.player->velocity.y = 1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = -2.828f;
					currentScene->state.player->velocity.y = 2.828f;
				}
			}
			else if (down == true && up == false && right == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = -1.414f;
					currentScene->state.player->velocity.y = -1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = -2.828f;
					currentScene->state.player->velocity.y = -2.828f;
				}
			}
			else if (down == false && up == false && down == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = -2.0f;
					currentScene->state.player->velocity.y = 0.0f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = -4.0f;
					currentScene->state.player->velocity.y = 0.0f;
				}
			}
			
		}
		
		else if (right == true && currentScene->state.player->isDead == false) {
			if (left == true && up == false && down == false) {
				currentScene->state.player->velocity.x = 0.0f;
				currentScene->state.player->velocity.y = 0.0f;
			}
			else if (up == true && left == false && down == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = 1.414f;
					currentScene->state.player->velocity.y = 1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 2.828f;
					currentScene->state.player->velocity.y = 2.828f;
				}
			}
			else if (down == true && up == false && left == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = 1.414f;
					currentScene->state.player->velocity.y = -1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 2.828f;
					currentScene->state.player->velocity.y = -2.828f;
				}
			}
			else if (left == false && up == false && down == false) {
				if (shift == true) {
					currentScene->state.player->velocity.x = 2.0f;
					currentScene->state.player->velocity.y = 0.0f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 4.0f;
					currentScene->state.player->velocity.y = 0.0f;
				}
			}
		}
		else if (up == true && currentScene->state.player->isDead == false) {
			if (down == true && right == false && left == false) {
				currentScene->state.player->velocity.x = 0.0f;
				currentScene->state.player->velocity.y = 0.0f;
			}
			else if (left == true && right == false && down == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = -1.414f;
					currentScene->state.player->velocity.y = 1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = -2.828f;
					currentScene->state.player->velocity.y = 2.828f;
				}
			}
			else if (right == true && down == false && left == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = 1.414f;
					currentScene->state.player->velocity.y = 1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 2.828f;
					currentScene->state.player->velocity.y = 2.828f;
				}
			}
			else if (down == false && right == false && left == false) {
				if (shift == true) {
					currentScene->state.player->velocity.x = 0.0f;
					currentScene->state.player->velocity.y = 2.0f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 0.0f;
					currentScene->state.player->velocity.y = 4.0f;
				}

			}
		}
		else if (down == true && currentScene->state.player->isDead == false) {
			if (up == true && right == false && left == false) {
				currentScene->state.player->velocity.x = 0.0f;
				currentScene->state.player->velocity.y = 0.0f;
			}
			else if (left == true && right == false && up == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = -1.414f;
					currentScene->state.player->velocity.y = -1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = -2.828f;
					currentScene->state.player->velocity.y = -2.828f;
				}
			}
			else if (right == true && up == false && left == false) {

				if (shift == true) {
					currentScene->state.player->velocity.x = 1.414f;
					currentScene->state.player->velocity.y = -1.414f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 2.828f;
					currentScene->state.player->velocity.y = -2.828f;
				}
			}
			else if (up == false && right == false && left == false) {
				if (shift == true) {
					currentScene->state.player->velocity.x = 0.0f;
					currentScene->state.player->velocity.y = -2.0f;
				}
				else if (shift == false) {
					currentScene->state.player->velocity.x = 0.0f;
					currentScene->state.player->velocity.y = -4.0f;
				}

			}
			
		
		}
		else if (currentScene->state.start == false){
			currentScene->state.player->velocity.x = 0.0f;
			currentScene->state.player->velocity.y = 0.0f;
		}
		
		if (firing == true && firingTimer >= 5) {
			if (start == true && currentScene->state.gameFinished == false) {
				if ((fireCount < 12 || (fireCount == 12 && currentScene->state.fire[fireIndex].isActive == false)) && currentScene->state.player->isDead == false) {
					Mix_PlayChannel(-1, fireSound, 0);
					currentScene->state.fire[fireIndex].position = currentScene->state.player->position;

					currentScene->state.fire[fireIndex].entityType = PLAYERSHOT;
					currentScene->state.fire[fireIndex].velocity = glm::vec3(0.0f, 10.0f, 0);
					currentScene->state.fire[fireIndex].textureID = Util::LoadTexture("PlayerShot.png");
					currentScene->state.fire[fireIndex].height = 1.0f;
					currentScene->state.fire[fireIndex].width = 1.0f;
					currentScene->state.fire[fireIndex].isActive = false;
					currentScene->state.fire[fireIndex].isTouched = false;
					fireIndex += 1;
					if (fireIndex == 12) {
						fireIndex = 0;
					}
					if (fireCount != 12) {
						fireCount += 1;
					}
					firingTimer = 0;
				}
			}
			
		}
	}
	
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	deltaTime += accumulator;
	if (deltaTime < FIXED_TIMESTEP) {
		accumulator = deltaTime;
		return;
	}

	while (deltaTime >= FIXED_TIMESTEP) {
		currentScene->Update(deltaTime);
		if (end == false) {
			for (int i = 0; i < fireCount; i++) {
				currentScene->state.fire[i].playerX = currentScene->state.player->position.x;
				if (currentScene->state.fire[i].isTouched == false) {
					currentScene->state.fire[i].isActive = true;
				}
				currentScene->state.fire[i].Update(deltaTime, currentScene->state.enemy, 1, NULL, 0, NULL, 0);
			}
		}

		deltaTime -= FIXED_TIMESTEP;


	}

	firingTimer += 1;
	accumulator = deltaTime;

	viewMatrix = glm::mat4(1.0f);
}


void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	program.SetViewMatrix(viewMatrix);
	


	currentScene->Render(&program);

	if (end == false) {
		for (int i = 0; i < fireCount; i++) {
			currentScene->state.fire[i].Render(&program);
		}
	}
	
	SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		if (currentScene->state.nextScene >= 0) {
			SwitchToScene(sceneList[1]);
		}
		else if (start == true && currentScene->state.start == false && end == false) {
			if (currentScene->state.player->lives <= 0) {
				SwitchToScene(sceneList[3]);
				end = true;
			}
			else if (currentScene->state.enemy->spellcard == 5) {
				SwitchToScene(sceneList[2]);
				end = true;
			}
		}
		Render();
	}

	Shutdown();
	return 0;
}
