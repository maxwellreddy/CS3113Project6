#include "Level1.h"
#define FIRE_COUNT 12
#define SPELL1_COUNT 224
#define SPELL1_COUNT_2 120
#define SPELL2_COUNT 200
#define SPELL2_COUNT_2 176
#define SPELL3_COUNT 96
#define SPELL4_COUNT 240
#define SPELL4_COUNT_2 80

#define PI 3.14159265358979323846

int seed = 0;

int spell1Index = 0;
int spell1Count = 0;
int spell1Index2 = 0;
int spell1Count2 = 0;

int spell2Index = 0;
int spell2Count = 0;
int spell2Index2 = 0;
int spell2Count2 = 0;

int spell3Index = 0;
int spell3Count = 0;

int spell4Index = 0;
int spell4Count = 0;
int spell4Index2 = 0;
int spell4Count2 = 0;

int spell4timer = 45;
int spell4side = 0;


bool spell1End = false; 
bool spell2End = false;
bool spell3End = false;
bool spell4End = false;
bool spell4flag = false;
bool spell4flag2 = false;

Mix_Music* music;
Mix_Chunk* enemySound;
Mix_Chunk* playerDeath;

void Level1::Initialize() {
	state.nextScene = -1;
	state.start = false;

	state.background = new Entity();
	state.background->entityType = BACKGROUND;
	state.background->textureID = Util::LoadTexture("Background.png");
	state.background->isActive = true;

// Initializes the player Marisa
	state.player = new Entity();
	state.player->entityType = PLAYER;
	state.player->position = glm::vec3(0.0f, -3.0f, 0);
	state.player->textureID = Util::LoadTexture("Player.png");
	state.player->height = 1.0f;
	state.player->width = 1.0f;
	state.player->lives = 8;
	state.player->isActive = true;

	// Initializes the boss
	state.enemy = new Entity();
	state.enemy->textureID = Util::LoadTexture("Enemy1.png");
	state.enemy->position = glm::vec3(0.0f, 2.5f, 0.0f);
	state.enemy->height = 0.8f;
	state.enemy->width = 0.8f;
	state.enemy->entityType = ENEMY;

	state.borders = new Entity[4];
	for (int i = 0; i < 4; i++) {
		state.borders[i].textureID = Util::LoadTexture("Border.png");
		state.borders[i].entityType = BORDER;
		state.borders[i].borderType = i;
		state.borders[i].isActive = true;
	}

	state.borders[0].position = glm::vec3(0.0f, 4.0f, 0.0f);
	state.borders[1].position = glm::vec3(0.0f, -4.0f, 0.0f);
	state.borders[2].position = glm::vec3(4.5f, 0.0f, 0.0f);
	state.borders[3].position = glm::vec3(-4.5f, 0.0f, 0.0f);

	state.fire = new Entity[FIRE_COUNT];
	state.spellcard1set1 = new Entity[SPELL1_COUNT];
	state.spellcard1set2 = new Entity[SPELL1_COUNT_2];
	state.spellcard2set1 = new Entity[SPELL2_COUNT];
	state.spellcard2set2 = new Entity[SPELL2_COUNT_2];
	state.spellcard3set1 = new Entity[SPELL3_COUNT];
	state.spellcard4set1 = new Entity[SPELL4_COUNT];
	state.spellcard4set2 = new Entity[SPELL4_COUNT_2];

	state.lifeMeter = new Entity[8];
	for (int i = 0; i < 8; i++) {
		state.lifeMeter[i].textureID = Util::LoadTexture("Life.png");
		state.lifeMeter[i].position = glm::vec3(4.5f, -2.0f + (0.5f * i), 0.0f);
		state.lifeMeter[i].isActive = true;
	}

	state.health = new Entity();
	state.health->textureID = Util::LoadTexture("Health.png");
	state.health->position = glm::vec3(-4.0f, 3.75f, 0.0f);
	state.health->entityType = HEALTH;
	state.health->isActive = true;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 0, 4096);
	music = Mix_LoadMUS("Adventures in Inferno - 21 - Future Century Stella.wav");

	Mix_PlayMusic(music, -1);

	enemySound = Mix_LoadWAV("se_tan01.wav");
	playerDeath = Mix_LoadWAV("se_pldead00.wav");
}
void Level1::Update(float deltaTime) {
	state.timer += 1;
	state.timer2 += 1;
	state.timer3 += 1;
	seed += 1;

	if (state.player->position.x >= 4.0f && state.player->velocity.x >= 0.0f) {
		state.player->velocity.x = 0;
		state.player->position.x = 4.0f;
	}
	if (state.player->position.x <= -4.0f && state.player->velocity.x <= 0.0f) {
		state.player->velocity.x = 0;
		state.player->position.x = -4.0f;
	}
	if (state.player->position.y >= 3.5f && state.player->velocity.y >= 0.0f) {
		state.player->velocity.y = 0;
		state.player->position.y = 3.5f;
	}
	if (state.player->position.y <= -3.5f && state.player->velocity.y <= 0.0f) {
		state.player->velocity.y = 0;
		state.player->position.y = -3.5f;
	}



	for (int i = 0; i < 4; i++) {
		state.borders[i].Update(deltaTime, NULL, 0, NULL, 0, NULL, 0);
	}

	for (int i = 0; i < state.player->lives; i++) {
		state.lifeMeter[i].Update(deltaTime, NULL, 0, NULL, 0, NULL, 0);
	}

	srand(time(NULL) + seed);
	
	float randomX = (float)rand() / (float)(RAND_MAX / 200.0f);
	float randomY = (float)rand() / (float)(RAND_MAX / 200.0f);	

	int negX = rand() % 2;
	int negY = rand() % 2;
	
	if (negX == 1) {
		negX = -1;
	}
	else {
		negX = 1;
	}

	if (negY == 1) {
		negY = -1;
	}
	else {
		negY = 1;
	}

	//Spellcard 1
	if (state.enemy->spellcard == 1 && state.timer % 30 == 0) {
		int spell1IndexCopy = spell1Index;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell1IndexCopy; i < spell1IndexCopy + 16; i++) {
			state.spellcard1set1[i].entityType = BULLET;
			state.spellcard1set1[i].position = state.enemy->position;

			glm::vec3 randomPos = glm::vec3(randomX, randomY, 0);
			glm::vec3 direction = randomPos - state.enemy->position;
			float angle;

			angle = atan(direction.y / direction.x) + (2.0f * PI) * (i * (1.0 / 16.0));

			float xDirection;
			float yDirection;

			if (direction.x < 0) {
				xDirection = -cos(angle);
				yDirection = -sin(angle);
			}

			else {
				xDirection = cos(angle);
				yDirection = sin(angle);
			}

			state.spellcard1set1[i].velocity = glm::vec3(xDirection, yDirection, 0);
			state.spellcard1set1[i].textureID = Util::LoadTexture("Bullet 1.png");
			state.spellcard1set1[i].height = 1.0f;
			state.spellcard1set1[i].width = 1.0f;
			state.spellcard1set1[i].isActive = false;
			state.spellcard1set1[i].isTouched = false;

			spell1Index += 1;
				
			if (spell1Index == SPELL1_COUNT) {
				spell1Index = 0;
			}
			if (spell1Count != SPELL1_COUNT) {
				spell1Count += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 1 && state.timer > 30) {
		for (int i = 0; i < spell1Count; i++) {
			if (state.spellcard1set1[i].isTouched == false) {
				state.spellcard1set1[i].isActive = true;
			}
			
		}
		state.timer = 0;
	}

	if (state.enemy->spellcard == 1 && state.timer2 % 60 == 0) {
		int spell1IndexCopy2 = spell1Index2;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell1IndexCopy2; i < spell1IndexCopy2 + 40; i++) {
			state.spellcard1set2[i].entityType = BULLET;
			state.spellcard1set2[i].position = state.enemy->position;

			glm::vec3 direction = state.player->position - state.enemy->position;
			float angle;

			angle = atan(direction.y / direction.x) + (2.0f * PI) * (i * (1.0 / 40.0));

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



			state.spellcard1set2[i].velocity = glm::vec3(xDirection, yDirection, 0);
			state.spellcard1set2[i].textureID = Util::LoadTexture("Bullet 2.png");
			state.spellcard1set2[i].height = 1.0f;
			state.spellcard1set2[i].width = 1.0f;
			state.spellcard1set2[i].isActive = false;
			state.spellcard1set2[i].isTouched = false;

			spell1Index2 += 1;

			if (spell1Index2 == 120) {
				spell1Index2 = 0;
			}
			if (spell1Count2 != 120) {
				spell1Count2 += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 1 && state.timer2 > 60) {
		for (int i = 0; i < spell1Count2; i++) {
			if (state.spellcard1set2[i].isTouched == false) {
				state.spellcard1set2[i].isActive = true;
			}

		}
		state.timer2 = 0;
	}

	for (int i = 0; i < spell1Count; i++) {
		state.spellcard1set1[i].Update(deltaTime, state.enemy, 1, NULL, 0, NULL, 0);
	}

	for (int i = 0; i < spell1Count2; i++) {
		state.spellcard1set2[i].Update(deltaTime, state.enemy, 1, NULL, 0, NULL, 0);
	}

	if (state.enemy->spellcard1hp <= 0 && spell1End == false) {
		state.enemy->spellcard = 2;
		for (int i = 0; i < spell1Count; i++) {
			state.spellcard1set1[i].isActive = false;
		}
		for (int i = 0; i < spell1Count2; i++) {
			state.spellcard1set2[i].isActive = false;
		}
		state.timer = 1;
		state.timer2 = 1;

		spell1End = true;
	}

	//Spellcard 2

	if (state.enemy->spellcard == 2 && state.timer % 45 == 0) {
		int spell2IndexCopy = spell2Index;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell2IndexCopy; i < spell2IndexCopy + 20; i++) {
			int negX2 = rand() % 2;

			if (negX2 == 1) {
				negX2 = -1;
			}
			else {
				negX2 = 1;
			}

			int negX3 = rand() % 2;

			if (negX3 == 1) {
				negX3 = -1;
			}
			else {
				negX3 = 1;
			}

			float randomX2 = (float)rand() / (float)(RAND_MAX / 0.5f) * negX2;
			float randomPosX = (float)rand() / (float)(RAND_MAX / 3.75f) * negX3;
			
			state.spellcard2set1[i].entityType = BULLET;
			state.spellcard2set1[i].position = glm::vec3(randomPosX, 4, 0);
			float yDirection = 1 - pow(randomX2, 2);
			yDirection = pow(yDirection, 0.5);
			state.spellcard2set1[i].velocity = glm::vec3(randomX2, -2, 0);
			state.spellcard2set1[i].textureID = Util::LoadTexture("Bullet 1.png");
			state.spellcard2set1[i].height = 1.0f;
			state.spellcard2set1[i].width = 1.0f;
			state.spellcard2set1[i].isActive = false;
			state.spellcard2set1[i].isTouched = false;

			spell2Index += 1;

			if (spell2Index == SPELL2_COUNT) {
				spell2Index = 0;
			}
			if (spell2Count != SPELL2_COUNT) {
				spell2Count += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 2 && state.timer > 45) {
		for (int i = 0; i < spell2Count; i++) {
			if (state.spellcard2set1[i].isTouched == false) {
				state.spellcard2set1[i].isActive = true;
			}

		}
		state.timer = 0;
	}

	if (state.enemy->spellcard == 2 && state.timer2 % 90 == 0) {
		int spell2IndexCopy2 = spell2Index2;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell2IndexCopy2; i < spell2IndexCopy2 + 8; i++) {
			state.spellcard2set2[i].entityType = BULLET;
			state.spellcard2set2[i].position = glm::vec3(state.player->position.x, 4, 0);

			state.spellcard2set2[i].velocity = glm::vec3(0, -0.7f * (i - spell2IndexCopy2), 0);
			state.spellcard2set2[i].textureID = Util::LoadTexture("Bullet 2.png");
			state.spellcard2set2[i].height = 1.0f;
			state.spellcard2set2[i].width = 1.0f;
			state.spellcard2set2[i].isActive = false;
			state.spellcard2set2[i].isTouched = false;

			spell2Index2 += 1;

			if (spell2Index2 == SPELL2_COUNT_2) {
				spell2Index2 = 0;
			}
			if (spell2Count2 != SPELL2_COUNT_2) {
				spell2Count2 += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 2 && state.timer2 > 90) {
		for (int i = 0; i < spell2Count2; i++) {
			if (state.spellcard2set2[i].isTouched == false) {
				state.spellcard2set2[i].isActive = true;
			}

		}
		state.timer2 = 0;
	}

	for (int i = 0; i < spell2Count; i++) {
		state.spellcard2set1[i].Update(deltaTime, state.enemy, 1, NULL, 0, NULL, 0);
	}

	for (int i = 0; i < spell2Count2; i++) {
		state.spellcard2set2[i].Update(deltaTime, state.enemy, 1, NULL, 0, NULL, 0);
	}

	if (state.enemy->spellcard2hp <= 0 && spell2End == false) {
		state.enemy->spellcard = 3;
		for (int i = 0; i < spell2Count; i++) {
			state.spellcard2set1[i].isActive = false;
		}
		for (int i = 0; i < spell2Count2; i++) {
			state.spellcard2set2[i].isActive = false;
		}
		state.timer = 1;
		state.timer2 = 1;

		spell2End = true;
	}



	//Spellcard 3

	float randomPosX3 = (float)rand() / (float)(RAND_MAX / 1.5f);
	float randomPosY3 = ((float)rand() / (float)(RAND_MAX / 2.0f)) + 1;

	if (state.enemy->spellcard == 3 && state.timer % 60 == 0) {
		int spell3IndexCopy = spell3Index;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell3IndexCopy; i < spell3IndexCopy + 12; i++) {
			glm::vec3 randomPos = glm::vec3(randomX, randomY, 0);
			glm::vec3 randomSpawnPos = glm::vec3(randomPosX3 * negX, randomPosY3, 0);
			state.spellcard3set1[i].entityType = BULLET;
			state.spellcard3set1[i].position = randomSpawnPos;
			glm::vec3 direction = randomPos - randomSpawnPos;
			float angle;

			angle = atan(direction.y / direction.x) + (2.0f * PI) * (i * (1.0 / 12.0));

			float hypotenuse = pow(direction.x, 2) + pow(direction.x, 2);
			float displacement = pow(hypotenuse, 0.5);
			float n = displacement / 1;

			float xDirection;
			float yDirection;

			if (direction.x < 0) {
				xDirection = -cos(angle);
				yDirection = -sin(angle);
			}

			else {
				xDirection = cos(angle);
				yDirection = sin(angle);
			}

			state.spellcard3set1[i].velocity = glm::vec3(xDirection, yDirection, 0);
			state.spellcard3set1[i].textureID = Util::LoadTexture("Bullet 1.png");
			state.spellcard3set1[i].height = 1.0f;
			state.spellcard3set1[i].width = 1.0f;
			
			state.spellcard3set1[i].bulletNumber = i - spell3IndexCopy;
			state.spellcard3set1[i].timer = 0;
			state.spellcard3set1[i].isActive = false;
			state.spellcard3set1[i].isTouched = false;

			spell3Index += 1;

			if (spell3Index == SPELL3_COUNT) {
				spell3Index = 0;
			}
			if (spell3Count != SPELL3_COUNT) {
				spell3Count += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 3 && state.timer > 60) {
		for (int i = 0; i < spell3Count; i++) {
			if (state.spellcard3set1[i].isTouched == false) {
				state.spellcard3set1[i].isActive = true;
			}

		}
		state.timer = 0;
	}

	if (state.enemy->spellcard3hp <= 0 && spell3End == false) {
		state.enemy->spellcard = 4;
		for (int i = 0; i < spell3Count; i++) {
			state.spellcard3set1[i].isActive = false;
		}

		state.timer = 1;
		state.timer2 = 1;

		spell3End = true;
	}

	for (int i = 0; i < spell3Count; i++) {
		state.spellcard3set1[i].timer += 1;
		state.spellcard3set1[i].Update(deltaTime, state.enemy, 1, state.player, 1, NULL, 0);
	}

	// Spellcard 4
	if (state.enemy->spellcard4hp <= 140 && state.enemy->spellcard4hp > 70) {
		spell4timer = 30;
	}
	else if (state.enemy->spellcard4hp <= 70 && state.enemy->spellcard4hp > 0) {
		spell4timer = 15;
	}
	if (state.enemy->spellcard == 4 && state.timer % spell4timer == 0) {
		int spell4IndexCopy = spell4Index;
		Mix_PlayChannel(-1, enemySound, 0);
		for (int i = spell4IndexCopy; i < spell4IndexCopy + 6; i++) {
			state.spellcard4set1[i].entityType = BULLET;
			state.spellcard4set1[i].position = state.enemy->position;
			int negX2 = rand() % 2;
			if (negX2 == 1) {
				negX2 = -1;
			}
			else {
				negX2 = 1;
			}

			float randomX2 = (float)rand() / (float)(RAND_MAX / 1.25f) * negX2;
			float randomY2 = (float)rand() / (float)(RAND_MAX / 1.25f);

			float yDirection = 1 - pow(randomX2, 2);
			yDirection = pow(yDirection, 0.5);

			state.spellcard4set1[i].velocity = glm::vec3(randomX2, randomY2, 0);
			state.spellcard4set1[i].acceleration = glm::vec3(0, -2.0f, 0);
			state.spellcard4set1[i].textureID = Util::LoadTexture("Bullet 2.png");
			state.spellcard4set1[i].height = 1.0f;
			state.spellcard4set1[i].width = 1.0f;

			state.spellcard4set1[i].timer = 0;
			state.spellcard4set1[i].isActive = false;
			state.spellcard4set1[i].isTouched = false;

			spell4Index += 1;

			if (spell4Index == SPELL4_COUNT) {
				spell4Index = 0;
			}
			if (spell4Count != SPELL4_COUNT) {
				spell4Count += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 4 && state.timer > spell4timer) {
		for (int i = 0; i < spell4Count; i++) {
			if (state.spellcard4set1[i].isTouched == false) {
				state.spellcard4set1[i].isActive = true;
			}

		}
		state.timer = 0;
	}

	if (state.enemy->spellcard == 4 && state.timer2 == 45) {
		state.enemy->velocity = glm::vec3(-2.0f, -2.0f, 0);
	}
	if (state.enemy->spellcard == 4 && state.timer2 == 90) {
		state.enemy->velocity = glm::vec3(2.0f, -2.0f, 0);
	}
	if (state.enemy->spellcard == 4 && state.timer2 == 135) {
		state.enemy->velocity = glm::vec3(2.0f, 2.0f, 0);
	}
	if (state.enemy->spellcard == 4 && state.timer2 == 180) {
		state.enemy->velocity = glm::vec3(-2.0f, 2.0f, 0);
		state.timer2 = 0;
	}



	if (state.enemy->spellcard == 4 && state.timer3 == 90) {
		int spell4IndexCopy2 = spell4Index2;
		glm::vec3 spell4Position;
		if (spell4side == 0) {
			spell4Position = glm::vec3(4.0f, 4.0f, 0);
			spell4side = 1;
		}
		else if (spell4side == 1) {
			spell4Position = glm::vec3(-4.0f, 4.0f, 0);
			spell4side = 0;
		}
		for (int i = spell4IndexCopy2; i < spell4IndexCopy2 + 5; i++) {
			state.spellcard4set2[i].entityType = BULLET;
			state.spellcard4set2[i].position = spell4Position;


			glm::vec3 direction = state.player->position - state.spellcard4set2[i].position;
			float angle;

			angle = atan(direction.y / direction.x);

			float xDirection;
			float yDirection;

			if (direction.x < 0) {
				xDirection = -cos(angle) * ((i - spell4IndexCopy2 + 1) * 0.75f);
				yDirection = -sin(angle) * ((i - spell4IndexCopy2 + 1) * 0.75f);
			}

			else {
				xDirection = cos(angle) * ((i - spell4IndexCopy2 + 1) * 0.75f);
				yDirection = sin(angle) * ((i - spell4IndexCopy2 + 1) * 0.75f);
			}

			state.spellcard4set2[i].velocity = glm::vec3(xDirection, yDirection, 0);
			state.spellcard4set2[i].textureID = Util::LoadTexture("Bullet 1.png");
			state.spellcard4set2[i].height = 1.0f;
			state.spellcard4set2[i].width = 1.0f;
			state.spellcard4set2[i].isActive = false;
			state.spellcard4set2[i].isTouched = false;

			spell4Index2 += 1;

			if (spell4Index2 == SPELL4_COUNT_2) {
				spell4Index2 = 0;
			}
			if (spell4Count2 != SPELL4_COUNT_2) {
				spell4Count2 += 1;
			}
		}
	}

	else if (state.enemy->spellcard == 4 && state.timer3 > 90) {
		for (int i = 0; i < spell4Count2; i++) {
			if (state.spellcard4set2[i].isTouched == false) {
				state.spellcard4set2[i].isActive = true;
			}

		}
		state.timer3 = 0;
	}

	if (state.enemy->spellcard4hp <= 0 && spell4End == false) {
		state.enemy->spellcard = 5;
		for (int i = 0; i < spell4Count; i++) {
			state.spellcard4set1[i].isActive = false;
		}

		spell4End = true;
	}

	for (int i = 0; i < spell4Count; i++) {
		state.spellcard4set1[i].Update(deltaTime, state.enemy, 1, state.player, 1, NULL, 0);
	}

	for (int i = 0; i < spell4Count2; i++) {
		state.spellcard4set2[i].Update(deltaTime, state.enemy, 1, state.player, 1, NULL, 0);
	}

	if (state.player->gotHit == true) {
		Mix_PlayChannel(-1, playerDeath, 0);
		state.player->gotHit = false;
	}

	if (state.enemy->spellcard == 1) {
		state.player->Update(deltaTime, state.enemy, 1, state.spellcard1set1, SPELL1_COUNT, state.spellcard1set2, SPELL1_COUNT_2);
	}
	else if (state.enemy->spellcard == 2) {
		state.player->Update(deltaTime, state.enemy, 1, state.spellcard2set1, SPELL2_COUNT, state.spellcard2set2, SPELL2_COUNT_2);
	}
	else if (state.enemy->spellcard == 3) {
		state.player->Update(deltaTime, state.enemy, 1, state.spellcard3set1, SPELL3_COUNT, NULL, 0);
	}
	else if (state.enemy->spellcard == 4) {
		state.player->Update(deltaTime, state.enemy, 1, state.spellcard4set1, SPELL4_COUNT, state.spellcard4set2, SPELL4_COUNT_2);
	}
	state.enemy->Update(deltaTime, state.fire, FIRE_COUNT, NULL, 0, NULL, 0);
	state.health->Update(deltaTime, state.enemy, 1, NULL, 0, NULL, 0);
	state.background->Update(deltaTime, NULL, 0, NULL, 0, NULL, 0);
}

void Level1::Render(ShaderProgram* program) {
	state.background->Render(program);
	state.player->Render(program);
	state.enemy->Render(program);

	for (int i = 0; i < spell1Count; i++) {
		state.spellcard1set1[i].Render(program);
	}

	for (int i = 0; i < spell1Count2; i++) {
		state.spellcard1set2[i].Render(program);
	}

	for (int i = 0; i < spell2Count; i++) {
		state.spellcard2set1[i].Render(program);
	}

	for (int i = 0; i < spell2Count2; i++) {
		state.spellcard2set2[i].Render(program);
	}

	for (int i = 0; i < spell3Count; i++) {
		state.spellcard3set1[i].Render(program);
	}

	for (int i = 0; i < spell4Count; i++) {
		state.spellcard4set1[i].Render(program);
	}

	for (int i = 0; i < spell4Count2; i++) {
		state.spellcard4set2[i].Render(program);
	}

	for (int i = 0; i < 4; i++) {
		state.borders[i].Render(program);
	}

	for (int i = 0; i < state.player->lives; i++) {
		state.lifeMeter[i].Render(program);
	}

	state.health->Render(program);


}

