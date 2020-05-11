#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <string>
#include "Util.h"

enum EntityType { PLAYER, ENEMY, PLAYERSHOT, BULLET, BORDER, HEALTH, BACKGROUND };

class Entity {
public:
	EntityType entityType;
	glm::vec3 position;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	float width = 1;
	float height = 1;
	float playerX;
	float playerY;

	int bulletNumber;
	int lives;
	int type;
	int enemyType;
	int timer = 0;
	int spellcard = 1;
	int borderType;

	int spellcard1hp = 150;
	int spellcard2hp = 120;
	int spellcard3hp = 120;
	int spellcard4hp = 210;

	GLuint textureID;

	glm::mat4 modelMatrix;

	bool isActive = true;
	bool isDead = false;
	bool isTouched = false;

	bool shoot = false;
	bool gotHit = false;
	Entity* fire;

	Entity();

	bool CheckCollision(Entity* other);
	bool CheckCollisionR(Entity* other);
	void CheckCollisionsY(Entity* objects, int objectCount);
	void CheckCollisionsX(Entity* objects, int objectCount);
	void CheckCollisionsRadius(Entity* objects, int objectCount);
	void Update(float deltaTime, Entity* object, int objectCount, Entity* object2, int objectCount2,
		 Entity* object3, int objectCount3);
	void Render(ShaderProgram* program);
};
