#include <SFML\Graphics.hpp>
#include <fstream>


class Game {
public:

								Game();
	void						run();

private:

	void						processEvents();
	void						update(sf::Time elapsedTime);
	void						render();

private:

	sf::Clock					mGameClock;
	sf::Time					mTimePerFrame;
	
	sf::RenderWindow			mWindow;	//getSize().x / .y
	sf::View					mView;

	const static int			mTileSize;

};

void Game::run() {

	
	//Creating player.
	sf::Texture playerTexture;
	playerTexture.loadFromFile("playerSpriteList.png");
	Player player(playerTexture);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while(mWindow.isOpen()) {

		sf::Time elapsedTime = mGameClock.restart();
		timeSinceLastUpdate += elapsedTime;
		while(timeSinceLastUpdate > mTimePerFrame) {

			timeSinceLastUpdate -= mTimePerFrame;

			processEvents();
			update(mTimePerFrame);

		}

		sf::FloatRect viewRect = player.getRect();
		viewRect.left += viewRect.width / 2;
		viewRect.top += viewRect.height / 2;
		viewRect.width = mWindow.getSize().x;
		viewRect.height = mWindow.getSize().y;
		mView.reset(viewRect);
		mWindow.setView(mView);


		render();

	}

}

class World {
public:

								World();
	void						resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int tileSize);
	void						loadLevelMap(std::string filename);

	int							getMapHeight();
	int							getMapWidth();

private:

	char**						mLevelMap;
	int							mMapHeight;
	int							mMapWidth;

};

void World::resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int tileSize) {

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(mLevelMap[i][j] == 'B') {

				if(movement.x > 0) rect.left = j * tileSize - rect.width;
				if(movement.x < 0) rect.left = j * tileSize + tileSize;
				if(movement.y > 0) rect.top = i * tileSize - rect.height;
				if(movement.y < 0) rect.top = i * tileSize + tileSize;
			
			}

		}

}

void World::loadLevelMap(std::string filename) {

	//Clearing memory.
	for(int i = 0; i < mMapHeight; ++i)
		delete[] mLevelMap[i];
	delete[] mLevelMap;

	std::ifstream inputFile(filename);

	inputFile >> mMapHeight >> mMapWidth;

	//Allocating memory.
	mLevelMap = new char*[mMapHeight];
	for(int i = 0; i < mMapHeight; ++i)
		mLevelMap[i] = new char[mMapWidth];

	inputFile.get();
	for(int i = 0; i < mMapHeight; ++i) {
		for(int j = 0; j < mMapWidth; ++j)
			inputFile.get(mLevelMap[i][j]);
		inputFile.get();
	}

	inputFile.close();

}

int World::getMapHeight() {
	return mMapHeight;
}

int World::getMapWidth() {
	return mMapWidth;
}

/*
namespace Textures {

	enum ID {
	
		Landscape,
		Airplane,
		Missile
	
	};

}

class TextureHolder {
public:

	void						loadFromFile(Textures::ID id, const std::string& filename);
	sf::Texture&				getTexture(Textures::ID id);

private:

	std::map<Textures::ID,
		std::unique_ptr<sf::Texture>> mTextureMap;


};

void TextureHolder::loadFromFile(Textures::ID id, const std::string& filename) {

	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	texture->loadFromFile(filename);

	mTextureMap.insert(std::make_pair(id, std::move(texture)));

}

sf::Texture& TextureHolder::getTexture(Textures::ID id) {

	auto found = mTextureMap.find(id);
	return *found->second;

}
*/

class Graphics {

};

class Player {
public:

								Player(sf::Texture& texture);
	void						update(World& world, Graphics& graphics, float deltaTime);

	sf::FloatRect				getRect();

private:

	sf::FloatRect				mRect;
	sf::Vector2f				mMovement;

	sf::Sprite					mSpriteWalkUp;
	sf::Sprite					mSpriteWalkDown;
	sf::Sprite					mSpriteWalkLeft;
	sf::Sprite					mSpriteWalkRight;
	float						mCurrentFrame;

	float						mHP;
	float						mMaxHP;

	const float					mSpeed = 0.1;

};

void Player::update(World& world, Graphics& graphics, float deltaTime) {

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	mMovement.x += mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	mMovement.x -= mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	mMovement.y -= mSpeed;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	mMovement.y += mSpeed;

	mRect.left += mMovement.x * deltaTime;
	world.resolveCollision(mRect, mMovement);
	mRect.top += mMovement.y * deltaTime;
	world.resolveCollision(mRect, mMovement);

}

sf::FloatRect Player::getRect() {
	return mRect;
}

int main() {

	Game game;


}

