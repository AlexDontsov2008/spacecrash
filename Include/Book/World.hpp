#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include <Book/ResourceHolder.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/SceneNode.hpp>
#include <Book/SpriteNode.hpp>
#include <Book/Spaceship.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Command.hpp>
#include <Book/Planet.hpp>
#include <Book/TextNode.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
	public:
		explicit							World(sf::RenderWindow& window);
		void								update(sf::Time dt);
		void								draw();
		
		CommandQueue&						getCommandQueue();


	private:
		void								loadTextures();
		void								buildScene();
		void								adaptPlayerPosition();
		void								adaptPlayerVelocity();
		void								handleCollisions();

		void								spawnEnemies();
		sf::FloatRect						getViewBounds() const;
		sf::FloatRect						getBattlefieldBounds() const;
		void								addPlanets();
		void								addPlanet(Planet::Type type, float relX, float relY);

		// Method's work with Health and Points Text Bars.
		void								initialHealthDisplay();
		void								initialPointsDisplay();
		void								initialText();

		void								updateHealthDisplay();
		void								updatePointsDisplay(sf::Time dt);
		void								updateText(sf::Time dt);

		// Method's work with world add/delete texture.
		void								updateWorldBounds();
		void								addNewWorldArea();
		bool								isNeedNewWorld();
		bool								isNeedDeleteOldWorld();


	private:
		enum Layer
		{
			Background,
			Space,
			LayerCount
		};


		struct SpawnPoint
		{
			SpawnPoint(Planet::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
			{}

			Planet::Type	type;
			float			x;
			float			y;
		};


	private:
		sf::RenderWindow&					mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;
		FontHolder							mFonts;

		SceneNode							mSceneGraph;
		std::array<SceneNode*, LayerCount>	mSceneLayers;
		CommandQueue						mCommandQueue;

		sf::FloatRect						mWorldBounds;
		sf::Vector2f						mSpawnPosition;
		float								mScrollSpeed;
		Spaceship*							mPlayerSpaceship;

		std::vector<SpawnPoint>				mPlanetSpawnPoints;

		SpriteNode*							mBackgroundSprite;
		TextNode*							mHealthDisplay;
		TextNode*							mPointsDisplay;
		float								mPoints;
		// Top world texture y coord.
		float								mOldWorldCoordY;
};

#endif // BOOK_WORLD_HPP
