#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"animation.h"
#include<SDL3/SDL.h>


enum class PlayerState {
	idle , running , jumping
};

enum class ObjectType {
	player , level , enemy
};
struct PlayerData
{

	PlayerState state;
	PlayerData() {
		state = PlayerState::idle;


	}
};
struct LevelData {

};
struct EnemyData {

};
union ObjectData
{
	PlayerData player;
	LevelData level;
	EnemyData enemy;
};

struct GameObject 
{

	ObjectType type;
	ObjectData data;
	glm::vec2 position, velocity, acceleration;
	float direction;
	float maxSpeed;
	std::vector<Animation> animations;
	int currentAnimations;
	SDL_Texture* texture;


	GameObject() :data{ .level = LevelData()}
	{
		type = ObjectType::level;
		direction = 1;
		maxSpeed=0;
		position = velocity = acceleration = glm::vec2(0);
		int currentAnimation = -1;
		texture = nullptr;

	}



};