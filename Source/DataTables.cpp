#include <Book/DataTables.hpp>
#include <Book/Spaceship.hpp>
#include <Book/Planet.hpp>


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
	std::vector<PlanetData> data(Planet::TypeCount);

	data[Planet::Earth].speed = -30.f;
	data[Planet::Earth].texutre = Textures::Earth;
	data[Planet::Earth].directions.push_back(Direction(+0.f, 200.f));
	//data[Planet::Earth].directions.push_back(Direction(-45.f, 160.f));
	//data[Planet::Earth].directions.push_back(Direction(+45.f, 80.f));

	//data[Planet::Jupiter].speed = 40.f;
	//data[Planet::Jupiter].texutre = Textures::Jupiter;
	//data[Planet::Jupiter].directions.push_back(Direction(+85.f, 40.f));
	//data[Planet::Jupiter].directions.push_back(Direction(-85.f, 80.f));
	//data[Planet::Jupiter].directions.push_back(Direction(+85.f, 40.f));

	//data[Planet::Mars].speed = 80.f;
	//data[Planet::Mars].texutre = Textures::Mars;
	//data[Planet::Mars].directions.push_back(Direction(+45.f, 60.f));
	//data[Planet::Mars].directions.push_back(Direction(0.f, 30.f));
	//data[Planet::Mars].directions.push_back(Direction(-45.f, 60.f));
	//data[Planet::Mars].directions.push_back(Direction(0.f, 30.f));

	data[Planet::Pluton].speed = -35.f;
	data[Planet::Pluton].texutre = Textures::Pluton;
	data[Planet::Pluton].directions.push_back(Direction(+0.f, 250.f));
	//data[Planet::Pluton].directions.push_back(Direction(-45.f, 30.f));
	//data[Planet::Pluton].directions.push_back(Direction(0.f, 80.f));
	//data[Planet::Pluton].directions.push_back(Direction(-40.f, 20.f));

	return data;
}
