#include "Entity.h"
#include <string>

#define PI 3.14159265358979323846

Entity::Entity()
{
	position = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);

	modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
	if (isActive == false || other->isActive == false) {
		return false;
	}

	float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
	float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

	if (xdist < 0 && ydist < 0) return true;

	return false;
}

bool Entity::CheckCollisionR(Entity* other) {
	if (isActive == false || other->isActive == false) {
		return false;
	}


	float xdist = fabs(position.x - other->position.x);
	float ydist = fabs(position.y - other->position.y);

	float squaredDistance = pow(xdist, 2) + pow(ydist, 2);
	float distance = pow(squaredDistance, 0.5);
	if (distance < 0.2) {
		return true;
	}
	return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity* object = &objects[i];
		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object->position.y);
			float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
			if (velocity.y > 0) {
				if (entityType == PLAYER && object->entityType == ENEMY && object->isDead == false) {
					lives -= 1;
					if (lives > 0) {
						position = glm::vec3(2.0f, -3.0f, 0);
						velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == PLAYER && isDead == false) {
					object->lives -= 1;
					if (lives > 0) {
						object->position = glm::vec3(2.0f, -3.0f, 0);
						object->velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						object->isDead = true;
					}
				}
				else if (entityType == PLAYERSHOT && object->entityType == ENEMY && isActive == true) {
					isActive = false;
					isTouched = true;
					if (object->spellcard == 1) {
						object->spellcard1hp -= 1;
					}
					else if (object->spellcard == 2) {
						object->spellcard2hp -= 1;
					}
					else if (object->spellcard == 3) {
						object->spellcard3hp -= 1;
					}
					else if (object->spellcard == 4) {
						object->spellcard4hp -= 1;
					}
				}
				
			}
			else if (velocity.y < 0) {
				if (entityType == PLAYER && object->entityType == ENEMY && object->isDead == false) {
					lives -= 1;
					if (lives > 0) {
						position = glm::vec3(2.0f, -3.0f, 0);
						velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == PLAYER && isDead == false) {
					object->lives -= 1;
					if (lives > 0) {
						object->position = glm::vec3(2.0f, -3.0f, 0);
						object->velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						object->isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == ENEMY) {

				}
			}
			
		}
	}
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity* object = &objects[i];
		if (CheckCollision(object))
		{
			float xdist = fabs(position.x - object->position.x);
			float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
			if (velocity.x >= 0) {
				/*
				if (entityType== PLAYERSHOT && object->entityType == ENEMY && object->isDead == false) {
					isActive = false;
					object->isDead = true;
				}
				*/
				if (entityType == PLAYER && object->entityType == ENEMY && object->isDead == false) {
					lives -= 1;
					if (lives > 0) {
						position = glm::vec3(2.0f, -3.0f, 0);
						velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == PLAYER && isDead == false) {
					object->lives -= 1;
					if (lives > 0) {
						object->position = glm::vec3(2.0f, -3.0f, 0);
						object->velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						object->isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == ENEMY) {

				}
			}
			else if (velocity.x < 0) {

				if (entityType == PLAYER && object->entityType == ENEMY && object->isDead == false) {
					lives -= 1;
					if (lives > 0) {
						position = glm::vec3(0.0f, -3.0f, 0);
						velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == PLAYER && isDead == false) {
					object->lives -= 1;
					if (lives > 0) {
						object->position = glm::vec3(0.0f, -3.0f, 0);
						object->velocity = glm::vec3(0.0f, 0.0f, 0);
					}
					else {
						object->isDead = true;
					}
				}
				else if (entityType == ENEMY && object->entityType == ENEMY) {
					
				}
			}
		}
	}
}

void Entity::CheckCollisionsRadius(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity* object = &objects[i];
		if (CheckCollisionR(object))
		{
			if (entityType == PLAYER && object->entityType == BULLET && object->isActive == true) {
				lives -= 1;
				object->isActive = false;
				object->isTouched = true;
				gotHit = true;
			}
		}
	}
}

void Entity::Update(float deltaTime, Entity* object, int objectCount, Entity* object2, int objectCount2, 
	Entity* object3, int objectCount3){
	if (isActive == false) {
		return;
	}

	else {
		if (entityType == BULLET && object->spellcard == 3 && timer == 50 + (bulletNumber * 12)) {
			playerX = object2->position.x;
			playerY = object2->position.y;
		}
		else if (entityType == BULLET && object->spellcard == 3 && timer > 50 + (bulletNumber * 12)) {
			glm::vec3 direction = glm::vec3(playerX, playerY, 0) - position;
			float angle;

			angle = atan(direction.y / direction.x);

			float xDirection;
			float yDirection;

			
			if (direction.x < 0) {
				xDirection = -cos(angle) * 2.5;
				yDirection = -sin(angle) * 2.5;
			}

			else {
				xDirection = cos(angle) * 2.5;
				yDirection = sin(angle) * 2.5;
			}
			
			velocity = glm::vec3(xDirection, yDirection, 0);
		}
	
		velocity.y += acceleration.y * deltaTime;
		position.y += velocity.y * deltaTime;

		if (entityType == PLAYER || entityType == BULLET || entityType == PLAYERSHOT) {
			CheckCollisionsY(object, objectCount);
		}

		position.x += velocity.x * deltaTime;

		if (entityType == PLAYER || entityType == BULLET) {
			CheckCollisionsX(object, objectCount);
			
		}

		if (entityType == PLAYER) {
			CheckCollisionsRadius(object2, objectCount2);
			CheckCollisionsRadius(object3, objectCount3);
		}
	}

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	if (entityType == PLAYERSHOT) {
		if (position.y > 5.0f) {
			isActive = false;
		}
	}

	else if (entityType == BULLET) {
		if (position.y > 5.0f) {
			isActive = false;

		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 1.0f));

		if (object->spellcard == 3 && timer >= 50 + (bulletNumber * 10)) {
			glm::vec3 direction = object2->position - position;
			float angle;

			angle = atan(direction.y / direction.x);

			float xDirection;
			float yDirection;

			if (direction.x < 0) {
				xDirection = -cos(angle) * 2.5;
				yDirection = -sin(angle) * 2.5;
			}

			else {
				xDirection = cos(angle) * 2.5;
				yDirection = sin(angle) * 2.5;
			}
			velocity = glm::vec3(xDirection, yDirection, 0);
		}
	}

	else if (entityType == BORDER) {
		if (borderType == 0 || borderType == 1) {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 1.0f, 1.0f));
		}
		else {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 10.0f, 1.0f));
		}
	}

	else if (entityType == HEALTH) {
		if (object->spellcard == 1) {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(object->spellcard1hp * 0.1f, 0.5f, 1.0f));
		}
		else if (object->spellcard == 2) {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(object->spellcard2hp * 0.1f, 0.5f, 1.0f));
		}
		else if (object->spellcard == 3) {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(object->spellcard3hp * 0.1f, 0.5f, 1.0f));
		}
		else if (object->spellcard == 4) {
			modelMatrix = glm::scale(modelMatrix, glm::vec3(object->spellcard4hp * 0.07f, 0.5f, 1.0f));
		}
	}

	else if (entityType == BACKGROUND) {
		modelMatrix = glm::scale(modelMatrix, glm::vec3(9.3f, 9.f, 1.0f));
	}
}

void Entity::Render(ShaderProgram* program) {
	if (isActive == false) {
		return;
	}

	program->SetModelMatrix(modelMatrix);
	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}




