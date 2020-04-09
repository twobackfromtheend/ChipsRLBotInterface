#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

void init_linalg(pybind11::module & m);

void init_ball(pybind11::module & m);
void init_field(pybind11::module & m);
void init_geometry(pybind11::module & m);
void init_game(pybind11::module & m);

PYBIND11_MODULE(rlutilities, m) {
  pybind11::module linear_algebra = m.def_submodule("linear_algebra");
	init_linalg(linear_algebra);
  pybind11::module simulation = m.def_submodule("simulation");
	init_ball(simulation);
	init_field(simulation);
	init_geometry(simulation);
	init_game(simulation);
}
