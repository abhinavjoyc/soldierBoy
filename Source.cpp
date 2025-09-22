#include<iostream>
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<SDL3_image/SDL_image.h>
#include<vector>
#include "gameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<array>

struct SDLstate
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int height;
	int width;
	int logw;
	int logh;
};
void cleanup(SDLstate& state);
void initialize(SDLstate& state);

const size_t	LAYER_IDX_LEVEL = 0;
const size_t	LAYER_IDX_CHARACTERS = 1;
struct GameState {


	std::array<std::vector<GameObject>, 2> layers;
	int playerIndex;
	GameState(){
		playerIndex = 0;
		}
};

struct resource {

	const int ANIM_PLAYER_IDLE = 0;
	std::vector<Animation> playerAnims;
	std::vector<SDL_Texture*> textures;
	SDL_Texture* texIdle;
	SDL_Texture* loadTexture(SDL_Renderer *renderer, std::string &filepath) {

		SDL_Texture* tex = IMG_LoadTexture(renderer, filepath.c_str());
		SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
		textures.push_back(tex);
		return tex;
	}

	void load(SDLstate& state) {
		playerAnims.resize(5);
		playerAnims[ANIM_PLAYER_IDLE] = Animation(5, 1.6f);
		std::string path = "data/soldierBoy.png";
		texIdle = loadTexture(state.renderer,path );
	}
	void unload() {
		for (SDL_Texture* Tex : textures) {
			SDL_DestroyTexture(Tex);
		}

	}


};
void drawObject(const SDLstate& state, GameState& gs, GameObject& obj, float deltatime);

int main(int argc, char* argv[])
{
	SDLstate state;
	initialize(state);
	resource res;
	res.load(state);
	// game data 
	GameState gs;
	GameObject player;
	player.type = ObjectType::player;
	player.texture = res.texIdle;
	player.animations = res.playerAnims;
	player.currentAnimations = res.ANIM_PLAYER_IDLE;
	gs.layers[LAYER_IDX_CHARACTERS].push_back(player);
	SDL_SetRenderLogicalPresentation(state.renderer, state.logw, state.logh, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	const bool* key = SDL_GetKeyboardState(nullptr);
	 uint64_t prev = SDL_GetTicks();

	std::cout << "testing ";
	bool running = true;
			bool flip = false;
		while (running) 
		{
			uint64_t now = SDL_GetTicks();
			float delta = (now - prev)/1000.0f;
			prev = now;
			float x = delta;


			SDL_Event event{ 0 };

			while (SDL_PollEvent(&event))
			{
				switch (event.type) 
					{

						case SDL_EVENT_QUIT: 
						{
							running = false;
							break;

						}
						case SDL_EVENT_WINDOW_RESIZED :
						{
							state.width = event.window.data1;
							state.height = event.window.data2;
						}

				}
			
			}
			
			int moveAmount=0;
			if (key[SDL_SCANCODE_A]) {

				moveAmount += -75;
				
				flip = true;

			}
			if (key[SDL_SCANCODE_D]) {

				moveAmount += +75;
				flip = false;
				

			}

			for (auto& layer : gs.layers) {
				for (GameObject& obj : layer)
				{
					if (obj.currentAnimations != -1)
					{

						obj.animations[obj.currentAnimations].step(delta);
					}
					

				}

			}
			SDL_SetRenderDrawColor(state.renderer, 255, 200, 300, 200);
			SDL_RenderClear(state.renderer);

			
			for (auto& layer : gs.layers) {
				for (GameObject& obj : layer)
				{
					drawObject(state, gs, obj, delta);
					
				}
				
			}
			SDL_RenderPresent(state.renderer);
		}
		res.unload();
		cleanup(state);
		return 0;
			
		}
	
	
		

void initialize(SDLstate& state)
{
	state.width = 1600;
	state.height = 900;
	state.logw = 640;
	state.logh = 320;
	state.window = SDL_CreateWindow("soldierBoy", state.width, state.height, SDL_WINDOW_RESIZABLE);
	state.renderer = SDL_CreateRenderer(state.window, nullptr);

}
void cleanup(SDLstate& state) 
{
	SDL_DestroyWindow(state.window);
	SDL_DestroyRenderer(state.renderer);
	
	SDL_Quit();

}


void drawObject(const SDLstate &state, GameState& gs, GameObject& obj, float deltatime) {
	
	float sprtiteSize = 32;
	float srcX = (obj.currentAnimations != -1)
		? obj.animations[obj.currentAnimations].currentFrame()* sprtiteSize
		: 0.0f;
	SDL_FRect src
	{
		.x = srcX,
		.y = 0,
		.w = sprtiteSize,
		.h = sprtiteSize

	};
	SDL_FRect dest
	{
		.x = obj.position.x,
		.y = obj.position.y ,
		//.x = 100,
		//.y = 200 - sprtiteSize,
		.w = sprtiteSize,
		.h = sprtiteSize

	};
	SDL_FlipMode flipmode = (obj.direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	SDL_RenderTextureRotated(state.renderer,obj.texture, &src, &dest, 0, nullptr, flipmode);
}