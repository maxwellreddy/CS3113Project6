#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"

struct GameState {
	Entity* player;
	Entity* enemy;
	Entity* fire;
	Entity* spellcard1set1;
	Entity* spellcard1set2;
	Entity* spellcard2set1;
	Entity* spellcard2set2;
	Entity* spellcard3set1;
	Entity* spellcard4set1;
	Entity* spellcard4set2;
	Entity* lifeMeter;
	Entity* borders;
	Entity* health;
	Entity* background;
	int nextScene;
	bool start = false;
	bool gameFinished = false;
	int timer = 0;
	int timer2 = 0;
	int timer3 = 0;
	int lives = 5;
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};
