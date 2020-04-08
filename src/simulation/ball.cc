#include "simulation/ball.h"
#include "simulation/field.h"

#include "misc/json.h"

const float Ball::restitution = 0.6f;
const float Ball::drag = -0.0305f;
const float Ball::mu = 2.0f;

const float Ball::v_max = 4000.0f;
const float Ball::w_max = 6.0f;

const float Ball::m = 30.0f;
const float Ball::soccar_radius = 91.25f;
const float Ball::hoops_radius = 91.25f;
const float Ball::dropshot_radius = 100.45f;
const float Ball::soccar_collision_radius = 93.15f;
const float Ball::hoops_collision_radius = 93.15f;
const float Ball::dropshot_collision_radius = 103.6f;

float Ball::radius = Ball::soccar_radius;
float Ball::collision_radius = Ball::soccar_collision_radius;
float Ball::I = 0.4f * Ball::m * Ball::radius * Ball::radius;
vec3 Ball::gravity = vec3{ 0.0, 0.0, -650.0f };

Ball::Ball() {
	time = 0.0f;
	x = vec3{ 0.0f, 0.0f, 1.1f * collision_radius };
	v = vec3{ 0.0f, 0.0f, 0.0f };
	w = vec3{ 0.0f, 0.0f, 0.0f };
}

sphere Ball::hitbox() {
	return sphere{ x, collision_radius };
}

void Ball::step(float dt) {

	ray contact = Field::collide(hitbox());

	if (norm(contact.direction) > 0.0) {

		vec3 p = contact.start;
		vec3 n = contact.direction;

		vec3 L = p - x;

		float m_reduced = 1.0f / ((1.0f / m) + dot(L, L) / I);

		vec3 v_perp = fminf(dot(v, n), 0.0f) * n;
		vec3 v_para = v - v_perp - cross(L, w);

		float ratio = norm(v_perp) / fmaxf(norm(v_para), 0.0001f);

		vec3 J_perp = -(1.0f + restitution) * m * v_perp;
		vec3 J_para = -fminf(1.0f, mu * ratio) * m_reduced * v_para;

		vec3 J = J_perp + J_para;

		w += cross(L, J) / I;
		v += (J / m) + drag * v * dt;
		x += v * dt;

		float penetration = collision_radius - dot(x - p, n);
		if (penetration > 0.0f) {
			x += 1.001f * penetration * n;
		}

	}
	else {

		v += (drag * v + gravity) * dt;
		x += v * dt;

	}

	w *= fminf(1.0, w_max / norm(w));
	v *= fminf(1.0, v_max / norm(w));
	time += dt;
}

vec3 closest_point_on_obb(const vec3 & v, const obb & hitbox) {
	vec3 v_local = dot(v - hitbox.center, hitbox.orientation);
	v_local[0] = clip(v_local[0], -hitbox.half_width[0], hitbox.half_width[0]);
	v_local[1] = clip(v_local[1], -hitbox.half_width[1], hitbox.half_width[1]);
	v_local[2] = clip(v_local[2], -hitbox.half_width[2], hitbox.half_width[2]);
	return dot(hitbox.orientation, v_local) + hitbox.center;
}

float scale(float dv) {
	const int n = 6;
	float values[n][2] = { {   0.0f, 0.65f},
						  { 500.0f, 0.65f},
						  {2300.0f, 0.55f},
						  {4600.0f, 0.30f} };

	float input = clip(dv, 0.0f, 4600.0f);

	for (int i = 0; i < (n - 1); i++) {
		if (values[i][0] <= input && input < values[i + 1][0]) {
			float u = (input - values[i][0]) / (values[i + 1][0] - values[i][0]);
			return lerp(values[i][1], values[i + 1][1], u);
		}
	}

	return -1.0f;
}

std::string Ball::to_json() {
  return nlohmann::json{
    {"x", {x[0], x[1], x[2]}},
    {"v", {v[0], v[1], v[2]}},
    {"w", {w[0], w[1], w[2]}}
  }.dump();
}

#ifdef GENERATE_PYTHON_BINDINGS
#include <pybind11/pybind11.h>
void init_ball(pybind11::module & m) {
	pybind11::class_<Ball>(m, "Ball")
		.def(pybind11::init<>())
		.def(pybind11::init<const Ball &>())
		.def_readwrite("location", &Ball::x)
		.def_readwrite("velocity", &Ball::v)
		.def_readwrite("angular_velocity", &Ball::w)
		.def_readwrite("time", &Ball::time)
		.def_readonly_static("restitution", &Ball::restitution)
		.def_readonly_static("drag", &Ball::drag)
		.def_readonly_static("friction", &Ball::mu)
		.def_readonly_static("mass", &Ball::m)
		.def_readonly_static("moment_of_inertia", &Ball::I)
		.def_readonly_static("max_speed", &Ball::v_max)
		.def_readonly_static("max_omega", &Ball::w_max)
		.def_readonly_static("radius", &Ball::radius)
		.def_readonly_static("collision_radius", &Ball::collision_radius)
		.def("hitbox", &Ball::hitbox)
		.def("to_json", &Ball::to_json)
		.def("step", static_cast<void (Ball::*)(float)>(&Ball::step));
}
#endif
