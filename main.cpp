#include "game/game.h"
#include "learn/qlearn.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>

class Runner {
private:
	std::shared_ptr<SDLWrapper> sdl;
	std::shared_ptr<GLWrapper> gl;
	std::shared_ptr<DriveGame> game;
	std::shared_ptr<QLearn> qlearn;

	bool running;

public:
	Runner();

	auto Run() -> void;
	auto HandleEvent(SDL_Event* ev) -> void;
	auto Update(float dt) -> void;
	auto Draw() -> void;
};

Runner::Runner()
{
	sdl = std::make_shared<SDLWrapper>("Drive AI", 800, 600);
	gl = std::make_shared<GLWrapper>();
	game = std::make_shared<DriveGame>(sdl, gl);
	qlearn = std::make_shared<QLearn>(8, 4);

	qlearn->SetDiscountFactor(0.7);
	qlearn->SetExploreChance(0.1);
	qlearn->SetLearningRate(0.4);

	std::vector<unsigned int> layers = {8, 6};
	qlearn->GenerateNetwork(layers);
}

auto Runner::Run() -> void
{
	this->running = true;
	auto lastTime = std::chrono::system_clock::now();
	auto now = lastTime;

	SDL_Event event;
	while (this->running) {
		lastTime = now;
		now = std::chrono::system_clock::now();

		while (this->sdl->PollEvents(&event)) {
			this->HandleEvent(&event);
		}

		float delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() / 1000.f;
		this->Update(delta);
		this->Draw();

		usleep(1000);
	}	
}

auto Runner::HandleEvent(SDL_Event* ev) -> void
{
	switch (ev->type) {
	case SDL_QUIT:
		running = false;
		break;

	case SDL_KEYDOWN:
		switch (ev->key.keysym.sym) {
		case SDLK_ESCAPE:
			this->running = false;
		}

	/* FALLTHROUGH */
	default:
		this->game->HandleEvent(ev);
		break;
	}
}

int times = 0;
auto Runner::Update(float dt) -> void
{
	std::vector<float> inputs;
	std::vector<float> sensors;
	this->game->GetCar()->GetDistances(sensors, 50, this->game->GetTrack());

	for (float& sense : sensors) {
		sense /= 50.0f;
	}

	qlearn->GetKeys(sensors, inputs);

	this->game->SetInputs(inputs);
	// for (int i = 0; i < 10; i++)
	this->game->Update(dt);	

	float reward = 1;
	if (game->GetCar()->IsDead()) {
		reward = -1;
		game->GetCar()->Revive();
	}

	Trial trial;
	trial.prev_state = sensors;

	this->game->GetCar()->GetDistances(sensors, 50, this->game->GetTrack());
	trial.new_state = sensors;

	trial.inputs = inputs;
	trial.quality = reward;

	qlearn->AddTrial(trial);

	if (times++ >= 240) {
		times = 0;
		qlearn->Learn();
	}
}

auto Runner::Draw() -> void
{
	this->game->Draw();
}

int main() {
	Runner runner;

	runner.Run();

	return 0;
}
