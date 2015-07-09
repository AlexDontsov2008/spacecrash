#include <Book/DataTables.hpp>
#include <Book/Spaceship.hpp>
#include <Book/Planet.hpp>

#include <ctime>


std::vector<SpaceshipData> initializeSpaceshipData()
{
	std::vector<SpaceshipData> data(Spaceship::TypeCount);

	data[Spaceship::Rocket].hitpoints = 100;
	data[Spaceship::Rocket].speed = 200.f;
	data[Spaceship::Rocket].texture = Textures::SpaceshipMaximumSpeed;

	return data;
}

std::vector<PlanetData> initializePlanetData()
{
	std::srand(std::time(NULL));

	std::vector<PlanetData> data(Planet::TypeCount);

	data[Planet::Earth].speed = 45.f;
	data[Planet::Earth].texutre = Textures::Earth;
	data[Planet::Earth].directions.push_back(Direction(20, 1000.f));

	data[Planet::Pluton].speed = 50.f;
	data[Planet::Pluton].texutre = Textures::Pluton;
	data[Planet::Pluton].directions.push_back(Direction(0.f, 1000.f));

	data[Planet::AsteroidGreate].speed = 70.f;
	data[Planet::AsteroidGreate].texutre = Textures::AsteroidGreate;
	data[Planet::AsteroidGreate].directions.push_back(Direction((rand() % 15) * 1.f, 1000.f));

	data[Planet::AsteroidAverage].speed = 90.f;
	data[Planet::AsteroidAverage].texutre = Textures::AsteroidAverage;
	data[Planet::AsteroidAverage].directions.push_back(Direction(-15, 1000.f));

	data[Planet::AsteroidLittle].speed = 110.f;
	data[Planet::AsteroidLittle].texutre = Textures::AsteroidLittle;
	data[Planet::AsteroidLittle].directions.push_back(Direction(10.f, 1000.f));

	return data;
}
