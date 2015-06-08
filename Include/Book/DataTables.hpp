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
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{}

	float angle;
	float distance;
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