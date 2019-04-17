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

	qlearn->SetDiscountFactor(0.2);
	qlearn->SetExploreChance(0.2);
	qlearn->SetLearningRate(0.4);

	std::vector<unsigned int> layers = {12, 8, 6};
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
		if (delta > 0.05f) delta = 0.05f;
		for (int i = 0; i < 10; i++)
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
			break;
		case SDLK_l:
			printf("\033[3;10HLearning   \n");
			qlearn->SetExploreChance(0.2);
			break;
		case SDLK_k:
			printf("\033[3;10HExploiting \n");
			qlearn->SetExploreChance(0);
			break;
		case SDLK_j:
			printf("\033[3;10HExploringish \n");
			qlearn->SetExploreChance(0.9f);
			break;
		}

	/* FALLTHROUGH */
	default:
		this->game->HandleEvent(ev);
		break;
	}
}

int times = 0;
std::vector<float> inputs(4);
auto Runner::Update(float dt) -> void
{
	std::vector<float> sensors;
	this->game->GetCar()->GetDistances(sensors, 50, this->game->GetTrack());

	if (sensors.size() != 8) { throw "NOPE"; }

	for (float& sense : sensors) {
		sense /= 50.0f;
		sense = 1.0f - sense;
	}

	if (times % 8 == 0)
		qlearn->GetKeys(sensors, inputs);

	printf("\033[1;10H");
	printf("Up: %.1f, Down: %.1f, Left: %.1f, Right: %.1f\n", inputs[0], inputs[1], inputs[2], inputs[3]);

	this->game->SetInputs(inputs);
	float prescore = game->GetScore();
	this->game->Update(dt);	
	float postscore = game->GetScore();

	float reward = 0;
	if (postscore > prescore) {
		//The car crossed a reward gate
		reward = 100;
	}

	if (game->GetCar()->IsDead()) {
		reward = -100;
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
		// for (Trial& trial : qlearn->trials) {
		// 	for (float& in : trial.prev_state) {
		// 		if (std::isnan(in)) { throw "AHHH"; }
		// 	}
		// 	for (float& in : trial.inputs) {
		// 		if (std::isnan(in)) { throw "AHHH"; }
		// 	}
		// 	for (float& in : trial.new_state) {
		// 		if (std::isnan(in)) { throw "AHHH"; }
		// 	}
		// }

		times = 0;
		qlearn->Learn();
	}
}

auto Runner::Draw() -> void
{
	this->game->Draw();
}

int main() {
	srand(time(NULL));

	Runner runner;

	runner.Run();

	return 0;
}
