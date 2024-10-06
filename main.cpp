#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <windows.h>


class Key {
private:
public:
	int clicks = 0;
	std::string name;
	SDL_Keycode code;

	Key(const SDL_Keycode& code) : code(code) {	
		name = SDL_GetKeyName(code);		
		std::cout << name << std::endl;
	}
};


class Window {
private:
	int delay;
	std::vector<Key> keys;
	SDL_Window *window;
	SDL_Renderer *renderer;

	void quit() {
		SDL_DestroyWindow(window);
        SDL_Quit();
	}

	void start(const int width, const int height, const int fpsCap) {
		delay = 1000 / fpsCap; 
		keys.push_back(Key(SDLK_z));
		keys.push_back(Key(SDLK_x));
		std::cout << "Keys:\n";
		for(const Key& i : keys) {
			const std::string keyName = SDL_GetKeyName(i.code);
			std::ofstream file(keyName + ".txt");
		}
		SDL_Init(SDL_INIT_VIDEO);	
    	window = SDL_CreateWindow("keys overlay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    	loop();
	}

	void saveKeypress(Key& key) {
	  	key.clicks++;
	    std::ofstream file(key.name + ".txt");
		if (file.is_open()) {
			file << key.clicks;
			    file.close();
		} else {
			std::cerr << "failed to open the file" << std::endl;
		}
	   	std::cout << key.name << " ("  << key.clicks << ")\n";
	}

	void eventCheck(const SDL_Event event) {
	    if (event.type == SDL_QUIT) {
	         quit();
	    }
	    if (event.type == SDL_KEYDOWN) {
	    	const SDL_Keycode keyCode = event.key.keysym.sym;
	    	if (keyCode == 27) {
	    		quit();
	    	}
	    	for (Key& i : keys) {
				if (i.code == keyCode) {
	    			saveKeypress(i);
	    		}
	    	}
    	}
	}

	void loop() {	
		SDL_Event event;
		Uint32 frameStart, frameTime;
		while (true) {
			frameStart = SDL_GetTicks();
	        while (SDL_PollEvent(&event)) {
	            eventCheck(event);
	        }	
	        frameTime = SDL_GetTicks() - frameStart;
	        if (delay > frameTime) {
                SDL_Delay(delay - frameTime);
            }
    	}
	}

public:
	Window(const int width, const int height, const int fpsCap) {
		start(width, height, fpsCap);
	}
};


class App {
private:
	void start() {
		Window window(800, 600, 240);
	}

public:
	App() {
		start();
	}
};


int main(int argc, char* argv[]) {
	App app;
    return 0;
}
