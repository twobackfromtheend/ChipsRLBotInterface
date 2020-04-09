#include "simulation/game.h"
#include "simulation/ball.h"
#include "simulation/field.h"

#include <iostream>

std::string Game::mode = "mode_not_set";


void Game::set_mode(std::string gamemode) {

  mode = gamemode;

  std::cout << "setting mode: " << mode << std::endl;

  if (mode == std::string("soccar")) {
    Field::initialize_soccar();
    Ball::radius = Ball::soccar_radius;
    Ball::collision_radius = Ball::soccar_collision_radius;
    Ball::I = 0.4f * Ball::m * Ball::radius * Ball::radius;
  }
  else if (mode == std::string("hoops")) {
    Field::initialize_hoops();
    Ball::radius = Ball::hoops_radius;
    Ball::collision_radius = Ball::hoops_collision_radius;
    Ball::I = 0.4f * Ball::m * Ball::radius * Ball::radius;
  }
  else if (mode == std::string("dropshot")) {
    Field::initialize_dropshot();
    Ball::radius = Ball::dropshot_radius;
    Ball::collision_radius = Ball::dropshot_collision_radius;
    Ball::I = 0.4f * Ball::m * Ball::radius * Ball::radius;
  } else {
		std::cout << "invalid gametype, use one of the following:" << std::endl;
		std::cout << "\"soccar\"" << std::endl;
		std::cout << "\"hoops\"" << std::endl;
		std::cout << "\"dropshot\"" << std::endl;
  }

}


#ifdef GENERATE_PYTHON_BINDINGS
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>

void init_game(pybind11::module & m) {
	pybind11::class_<Game>(m, "Game")
        .def_static("set_mode", &Game::set_mode);
}
#endif
