#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"animation.h"
#include<SDL3/SDL.h>

enum class ObjectType {
	player , level , enemy
};


struct GameObject 
{

	ObjectType type;
	glm::vec2 position, velocity, acceleration;
	float direction;
	std::vector<Animation> animations;
	int currentAnimations;
	SDL_Texture* texture;


	GameObject() {
		type = ObjectType::level;
		direction = 1;
		position = velocity = acceleration = glm::vec2(0);
		int currentAnimation = -1;
		texture = nullptr;

	}



};