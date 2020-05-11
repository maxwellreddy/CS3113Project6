#pragma once
#include "Scene.h"
#include <time.h>
#include <stdlib.h>
class Level1 : public Scene {
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};