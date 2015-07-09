#ifndef BOOK_DATATABLES_HPP
#define BOOK_DATATABLES_HPP

#include <Book/ResourceIdentifiers.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

class Spaceship;

struct Direction
{
	Direction(float angle, float distance, float radius = 0.f)
	: angle(angle)
	, distance(distance)
	, radius(radius)
	{}

	float angle;
	float distance;
	float radius;
};

struct SpaceshipData
{
	int				hitpoints;
	float			speed;
	Textures::ID	texture;
};

struct PlanetData
{
	float					speed;
	Textures::ID			texutre;
	std::vector<Direction>	directions;
};

std::vector<SpaceshipData> initializeSpaceshipData();
std::vector<PlanetData>    initializePlanetData();

#endif