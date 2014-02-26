//SFML includes.
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//std includes.
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <sstream>


//FONT - BIG.
//http://patorjk.com/software/taag/

//Not used anymore (not sure, though).
/*
int invincibilityTime = 0;					//2.
int playerStartingX = 0;					//220.
int playerStartingY = 0;					//220.
int tileSize = 0;							//120.
int screenWidth = 0;						//1280.
int screenHeight = 0;						//720.
int gameSpeed = 0;							//200.
*/
//Should be moved to config.
float offsetX = 0;				//Map scrolling
float offsetY = 0;				//offset.
int mapHeight = 0;							//0.
int mapWidth = 0;							//0.




//  _____        _          _                         
// |  __ \      | |        | |                        
// | |  | | __ _| |_ __ _  | |_ _   _ _ __   ___  ___ 
// | |  | |/ _` | __/ _` | | __| | | | '_ \ / _ \/ __|
// | |__| | (_| | || (_| | | |_| |_| | |_) |  __/\__ \
// |_____/ \__,_|\__\__,_|  \__|\__, | .__/ \___||___/
//                               __/ | |              
//                              |___/|_|   
//

//Attributes.
struct attributes {

     int					strength;				//Carry weight, melee damage. 
     int					endurance;				//HP, resistances.
     int					agility;				//Speed, ranged damage.
     int					intelligence;			//MP, magic damage.
     int					luck;					//Dodge chance, critical damage chance.

};

//Resistances.
struct resistances {

     int					poison;
     int					paralize;
     int					lightning;
     int					fire;
     int					ice;
     int					earth;

};

//Health bar.
struct healthBar {

	sf::Sprite				mSprite;
	int						mWidth;
	int						mHeight;

};

//Structure used for storing config values (your C.O.).
struct config {

	float					invincibilityTime;		
	int						playerStartingX;					
	int						playerStartingY;			
	int						mapHeight;				
	int						mapWidth;				
	int						tileSize;						
	int						screenWidth;					
	int						screenHeight;
	int						gameSpeed;
	float					offsetX;				//Map scrolling
	float					offsetY;				//offset.

};




//   _____ _                         
//  / ____| |                        
// | |    | | __ _ ___ ___  ___  ___ 
// | |    | |/ _` / __/ __|/ _ \/ __|
// | |____| | (_| \__ \__ \  __/\__ \
//  \_____|_|\__,_|___/___/\___||___/
//

//Game.hpp
class Game {
public:

							Game();
	void					run();
	void					update();
	void					processEvents();
	void					render();

private:

	sf::RenderWindow		mWindow;
	int						mScreenWidth;					
	int						mScreenHeight;

	float					mInvincibilityTime;
	sf::Clock				mGameClock;
	sf::Clock				mInvincibilityClock;

	char**					mLevelMap;
	int						mPlayerStartingX;					
	int						mPlayerStartingY;

	int						mMapHeight;				
	int						mMapWidth;				
	int						mTileSize;						
	int						mGameSpeed;
	float					mOffsetX;				//Map scrolling
	float					mOffsetY;				//offset.


};

class World {
public:

								World();
	void						resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int direction, int tileSize);
	void						loadLevelMap(std::string filename);

	int							getMapHeight();
	int							getMapWidth();

private:

	char**						mLevelMap;
	int							mMapHeight;
	int							mMapWidth;

};

void World::resolveCollision(sf::FloatRect& rect, sf::Vector2f movement, int direction, int tileSize) {

	for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
		for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

			if(mLevelMap[i][j] == 'B') {
					if((movement.x > 0) && (direction == 0)) rect.left = j * tileSize - rect.width;
					if((movement.x < 0) && (direction == 0)) rect.left = j * tileSize + tileSize;
					if((movement.y > 0) && (direction == 1)) rect.top = i * tileSize - rect.height;
					if((movement.y < 0) && (direction == 1)) rect.top = i * tileSize + tileSize;
			}

		}

}

void World::loadLevelMap(std::string filename) {

	//Clearing memory.
	//for(int i = 0; i < mMapHeight; ++i)
	//	delete[] mLevelMap[i];
	//delete[] mLevelMap;

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

//Entity.hpp
class Entity {
public:

	void					setName(sf::String name);

	void					setRect(sf::FloatRect rect);
	void					setSprite(sf::Sprite sprite);
	void					setTextName(const sf::String& textName, const sf::Font& font, unsigned int characterSize);
	
	std::string				getName();

	sf::FloatRect			getRect();
	sf::Sprite				getSprite();
	sf::Text				getTextName();

protected:	//private

	sf::String				mName;

	sf::FloatRect			mRect;
	sf::Sprite				mSprite;
	sf::Text				mTextName;

};

//Entity.cpp
void Entity::setName(sf::String name) {
	mName = name;
	mTextName.setString(mName);
}

void Entity::setRect(sf::FloatRect rect) {
	mRect = rect;
}

void Entity::setSprite(sf::Sprite sprite) {
	mSprite = sprite;
}

void Entity::setTextName(const sf::String& textName, const sf::Font& font, unsigned int characterSize = 30) {
	mTextName.setString(textName);
	mTextName.setFont(font);
	mTextName.setCharacterSize(characterSize);
}

std::string Entity::getName() {
	return mName;
}

sf::FloatRect Entity::getRect() {
	return mRect;
}

sf::Sprite Entity::getSprite() {
	return mSprite;
}

sf::Text Entity::getTextName() {
	return mTextName;
}

//Character.hpp
class Character : public Entity {
public:

	void					setHealthBar(sf::Texture& image, int width, int height);
	void					updateHealthBar(int offsetX, int offsetY);
	void					drawHealthBar(sf::RenderWindow window);

	void					setMovement(sf::Vector2f movement);
	void					move(sf::Vector2f movement);
	void					setDirection(int direction);

	void					setLevel(int level);
	void					setHP(float HP);
	void					setMaxHP(float maxHP);
	void					setMP(int MP);
	void					setMaxMP(int maxMP);
	void					kill();
	void					revive();
	void					setAttributes(attributes& attributes);
	void					setResistances(resistances& resistances);

	sf::Vector2f			getMovement();
	int						getDirection();

	int						getLevel();
	float					getHP();
	float					getMaxHP();
	float					getMP();
	float					getMaxMP();
	bool					getIsAlive();
	attributes*				getAttributes();
	resistances*			getResistances();

protected:	//private

	sf::Vector2f			mMovement;
	int						mDirection;

	float					mCurrentFrame;

	int						mLevel;
	int						mHP;
	int						mMaxHP;
	float					mMP;
	float					mMaxMP;
	bool					mIsAlive;
	attributes				mAttributes;
	resistances				mResistances;
	healthBar				mHealthBar;

};

//Character.cpp
void Character::setHealthBar(sf::Texture& image, int width = 100, int height = 10) {
	mHealthBar.mSprite.setTexture(image);
	mHealthBar.mWidth = width;
	mHealthBar.mHeight = height;
	mHealthBar.mSprite.setTextureRect(sf::IntRect(0, 0, mHealthBar.mWidth, mHealthBar.mHeight));	
}

void Character::updateHealthBar(int offsetX, int offsetY) {
	if(mHP / mMaxHP >= 0.6)
		mHealthBar.mSprite.setColor(sf::Color::Green);
	else if((mHP / mMaxHP >= 0.35) && (mHP / mMaxHP < 0.6))
		mHealthBar.mSprite.setColor(sf::Color::Yellow);
	else if(mHP / mMaxHP < 0.35)
		mHealthBar.mSprite.setColor(sf::Color::Red);

	mHealthBar.mSprite.setTextureRect(sf::IntRect(mHealthBar.mWidth * (1 - mHP / mMaxHP), 0, mHealthBar.mWidth, mHealthBar.mHeight));
	mHealthBar.mSprite.setPosition(mRect.left - offsetX, mRect.top + mRect.height - offsetY);
}

void Character::drawHealthBar(sf::RenderWindow window) {
	window.draw(mHealthBar.mSprite);
}

void Character::setMovement(sf::Vector2f movement) {
	mMovement = movement;
}

void Character::move(sf::Vector2f movement) {
	mMovement = movement;

	if(movement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
	if(movement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
	if(movement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
	if(movement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

	mSprite.move(mMovement);
}

void Character::setDirection(int direction) {
	mDirection = direction;
}

void Character::setLevel(int level) {
	mLevel = level;
}

void Character::setHP(float HP) {
	mHP = HP;
}

void Character::setMaxHP(float maxHP) {
	mMaxHP = maxHP;
}

void Character::setMP(int MP) {
	mMP = MP;
}

void Character::setMaxMP(int maxMP) {
	mMaxMP = maxMP;
}

void Character::kill() {
	mIsAlive = false;
}

void Character::revive() {
	mIsAlive = true;
}

void Character::setAttributes(attributes& attributes) {
	////////////Copy all members.
}

void Character::setResistances(resistances& resistances) {
	////////////Copy all members.
}

sf::Vector2f Character::getMovement() {
	return mMovement;
}

int Character::getDirection() {
	return mDirection;
}

int Character::getLevel() {
	return mLevel;
}

float Character::getHP() {
	return mHP;
}

float Character::getMaxHP() {
	return mMaxHP;
}

float Character::getMP() {
	return mMP;
}

float Character::getMaxMP() {
	return mMaxMP;
}

bool Character::getIsAlive() {
	return mIsAlive;
}

attributes* Character::getAttributes() {
	return &mAttributes;	//*?
}

resistances* Character::getResistances() {
	return &mResistances;	//*?
}

//Player.
class Player {
public:

	sf::Vector2f			mMovement;
	float					mSpeed;

	sf::FloatRect			mRect;
	sf::Sprite				mSprite;
	sf::Sprite				mHpSprite;
	sf::String				mName;
	sf::Text				mTextName;

	float					mCurrentFrame;
	float					mAnimationSpeed;
	static const int		mFrameCount = 10;

	bool					mIsAlive;
	float					mHP;
	float					mMaxHp;
	int						mMP;

	//Player constructor.
	Player(sf::Texture& texture, sf::Texture& hpImage, int x, int y, sf::Font& font) {

		mSprite.setTexture(texture);
		mRect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.
		
		mName = "Player 1";
		mTextName.setString(mName);
		mTextName.setFont(font);
		mTextName.setCharacterSize(30);
		mTextName.setStyle(sf::Text::Bold);
		mTextName.setColor(sf::Color::Red);
		
		mHpSprite.setTexture(hpImage);
		mHpSprite.setTextureRect(sf::IntRect(0, 0, 100, 10));

		mSprite.setTextureRect(sf::IntRect(0, 15, 120, 110));
	
		mSpeed = 0.1;
		mMovement.x = 0;
		mMovement.y = 0;

		mCurrentFrame = 0;
		mAnimationSpeed = 0.005;

		mIsAlive = true;
		mHP = 100;
		mMaxHp = 100;
		mMP = 100;
	}

	//Player sprite render.
	void update(float deltaTime, char** levelMap, struct config* config) {

		//Player controls (testing).
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	mMovement.y -= mSpeed;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	mMovement.y += mSpeed;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))	mMovement.x -= mSpeed;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	mMovement.x += mSpeed;

		mRect.left += mMovement.x * deltaTime;
		collision(0, levelMap, config);

		mRect.top += mMovement.y * deltaTime;
		collision(1, levelMap, config);

		//Player animation.
		mCurrentFrame += mAnimationSpeed * deltaTime;
		if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

		if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
		if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
		if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
		if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

		mSprite.setPosition(mRect.left - offsetX, mRect.top - offsetY);

		//HP.
		if(mHP / mMaxHp >= 0.6)
			mHpSprite.setColor(sf::Color::Green);
		else if((mHP / mMaxHp >= 0.35) && (mHP / mMaxHp < 0.6))
			mHpSprite.setColor(sf::Color::Yellow);
		else if(mHP / mMaxHp < 0.35)
			mHpSprite.setColor(sf::Color::Red);

		mHpSprite.setTextureRect(sf::IntRect(100 * (1 - mHP / mMaxHp), 0, 100, 10));
		mHpSprite.setPosition(mRect.left - offsetX, mRect.top + mRect.height - offsetY);
		mTextName.setPosition(mRect.left - offsetX, mRect.top - mTextName.getCharacterSize() - offsetY);						//-

		//Camera scrolling.
		if(mRect.left > config->screenWidth / 2 - mRect.width / 2)	offsetX = mRect.left - (config->screenWidth / 2 - mRect.width / 2);
		if(mRect.top > config->screenHeight / 2 - mRect.height / 2)	offsetY = mRect.top - (config->screenHeight / 2 - mRect.height / 2);

		//Stopping the player.
		mMovement.x = 0;
		mMovement.y = 0;

	}
	
	//direction = 0: horizontal.
	//direction = 1: vertical.
	void collision(int direction, char** levelMap, struct config* config) {
		for(int i = mRect.top / config->tileSize; i < (mRect.top + mRect.height) / config->tileSize; ++i)
			for(int j = mRect.left / config->tileSize; j < (mRect.left + mRect.width) / config->tileSize; ++j) {

				if(levelMap[i][j] == 'B') {
					if((mMovement.x > 0) && (direction == 0)) mRect.left = j * config->tileSize - mRect.width;
					if((mMovement.x < 0) && (direction == 0)) mRect.left = j * config->tileSize + config->tileSize;
					if((mMovement.y > 0) && (direction == 1)) mRect.top = i * config->tileSize - mRect.height;
					if((mMovement.y < 0) && (direction == 1)) mRect.top = i * config->tileSize + config->tileSize;
				}
				if(levelMap[i][j] == '0') {
					levelMap[i][j] = ' ';
					mMP += 10;
				}

			}
	}
	
	//Taking damage.
	void takeDamage(float damage) {

		mHP -= damage;

	}

	void heal(float hp) {

		mHP += hp;

	}

	//On-screen stats.
	//sf::Text should be used to display stats.
	void showStats(std::ostringstream* hudHealth, std::ostringstream* hudMana) {

		*hudHealth << mHP;
		*hudMana << mMP;

	}

	sf::FloatRect getRect() {
		return mRect;
	}

};


//Work in progress (stupid AI).
//AI is to be represented as a set of private methods.
class Enemy {
public:
	sf::Vector2f			mMovement;
	int						mDirection;
	float					mSpeed;

	sf::FloatRect			mRect;
	sf::Sprite				mSprite;

	float					mCurrentFrame;
	float					mAnimationSpeed;
	static const int		mFrameCount = 10;

	bool					mIsAlive;
	int						mHP;

	float					mDamage;

	sf::String				mName;
	sf::Text				mTextName;

	//Enemy constructor.
	Enemy(sf::Texture& texture, int x, int y, sf::Font& font) {
		mSprite.setTexture(texture);
		mRect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.

		mName = "Enemy";
		mTextName.setString(mName);
		mTextName.setFont(font);
		mTextName.setCharacterSize(30);
		mTextName.setStyle(sf::Text::Bold);
		mTextName.setColor(sf::Color::Magenta);

		mSprite.setTextureRect(sf::IntRect(0, 15, 120, 110));

		mDirection = rand() % 4;
		mSpeed = 0.05;

		mCurrentFrame = 0;
		mAnimationSpeed = mSpeed * 0.05;

		mIsAlive = true;
		mHP = 100;

		mDamage = 30;
	}

	//Enemy sprite render.
	void update(float deltaTime, char** levelMap, struct config* config) {

		if(mDirection == 0) {mMovement.x = 0;		mMovement.y = -mSpeed;}
		if(mDirection == 1) {mMovement.x = mSpeed;	mMovement.y = 0;}
		if(mDirection == 2) {mMovement.x = 0;		mMovement.y = mSpeed;}
		if(mDirection == 3) {mMovement.x = mSpeed;	mMovement.y = 0;}

		mRect.left += mMovement.x * deltaTime;
		collision(levelMap, config);
		mRect.top += mMovement.y * deltaTime;
		collision(levelMap, config);

		//Enemy animation.
		mCurrentFrame += mAnimationSpeed * deltaTime;
		if(mCurrentFrame > mFrameCount) mCurrentFrame -= mFrameCount;

		if(mMovement.x > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 925, mRect.width, mRect.height));
		if(mMovement.x < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 667, mRect.width, mRect.height));
		if(mMovement.y > 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 535, mRect.width, mRect.height));
		if(mMovement.y < 0) mSprite.setTextureRect(sf::IntRect(mRect.width * int(mCurrentFrame), 787, mRect.width, mRect.height));

		mSprite.setPosition(mRect.left - offsetX, mRect.top - offsetY);
		mTextName.setPosition(mRect.left - offsetX, mRect.top - mTextName.getCharacterSize() - offsetY);

	}
	
	void collision(char** levelMap, struct config* config) {
		for(int i = mRect.top / config->tileSize; i < (mRect.top + mRect.height) / config->tileSize; ++i)
			for(int j = mRect.left / config->tileSize; j < (mRect.left + mRect.width) / config->tileSize; ++j) {
				
				if(levelMap[i][j] == 'B') {
					if(mMovement.x > 0) mRect.left = j * config->tileSize - mRect.width;
					if(mMovement.x < 0) mRect.left = j * config->tileSize + config->tileSize;
					if(mMovement.y > 0) mRect.top = i * config->tileSize - mRect.height;
					if(mMovement.y < 0) mRect.top = i * config->tileSize + config->tileSize;
					int temp = rand() % 4;
					if(temp != mDirection) mDirection = temp;
					//direction = rand() % 4;		//???
				}
						
			}
	}

	void dealDamage(Player& player) {

		float damage = rand() % int(mDamage);
		player.takeDamage(damage);

	}

	sf::FloatRect getRect() {
		return mRect;
	}
	
};


class DropItem {
public:
	sf::FloatRect			mRect;
	sf::Sprite				mSprite;
	float					mCurrentFrame;
	int						mEffectValue;
	bool					mIsAlive;

	//Drop item constructor.
	DropItem(sf::Texture& texture, std::string type, int effect, int x, int y) {
		mSprite.setTexture(texture);
		mRect = sf::FloatRect(x, y, 32, 32);
		mSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
		mCurrentFrame = 0;
		mEffectValue = effect;
		mIsAlive = true;
	}

	//Drop item sprite render.
	void update(float time) {
		
		mSprite.setPosition(mRect.left - offsetX, mRect.top - offsetY);

	}

	void action(Player& player) {
		player.heal(mEffectValue);
		mIsAlive = false;
	}

	sf::FloatRect getRect() {
		return mRect;
	}

};


//  ______                _   _                 
// |  ____|              | | (_)                
// | |__ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |  | |_| | | | | (__| |_| | (_) | | | \__ \
// |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                                             

//   _____           _                    __                  _   _                 
//  / ____|         | |                  / _|                | | (_)                
// | (___  _   _ ___| |_ ___ _ __ ___   | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
//  \___ \| | | / __| __/ _ \ '_ ` _ \  |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//  ____) | |_| \__ \ ||  __/ | | | | | | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |_____/ \__, |___/\__\___|_| |_| |_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//          __/ |                                                                   
//         |___/  
//



//  ______             _               __                  _   _                 
// |  ____|           (_)             / _|                | | (_)                
// | |__   _ __   __ _ _ _ __   ___  | |_ _   _ _ __   ___| |_ _  ___  _ __  ___ 
// |  __| | '_ \ / _` | | '_ \ / _ \ |  _| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
// | |____| | | | (_| | | | | |  __/ | | | |_| | | | | (__| |_| | (_) | | | \__ \
// |______|_| |_|\__, |_|_| |_|\___| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
//                __/ |                                                          
//               |___
//

//Loads level map from a text file (who would've thought?).
//Note: char** sucks. Totally.
char** loadLevelMap(std::string filename) {

	std::ifstream inputFile(filename);
	int mapHeightLocal;
	int mapWidthLocal;
	char trash;												//For '\n'.
	inputFile >> mapHeightLocal >> mapWidthLocal;
	mapHeight = mapHeightLocal;
	mapWidth = mapWidthLocal;

	char** levelMap = new char* [mapHeightLocal];			//!!!Handle memory[1]-.
	for(int i = 0; i < mapHeightLocal; ++i)
		levelMap[i] = new char[mapWidthLocal];				//!!!Handle memory[1]-.
	
	inputFile.get(trash);
	for(int i = 0; i < mapHeightLocal; ++i) {
		for(int j = 0; j < mapWidthLocal; ++j)
			inputFile.get(levelMap[i][j]);
		inputFile.get(trash);
	}
	inputFile.close();

	return levelMap;

}

//Loads a level and sets starting coordinates for the player (work in progress, though).
//filename - name of the level file to be loaded.
//**levelMap - pointer to the existing level map.
char** loadLevel(std::string filename, char** levelMap, struct config* config, int x, int y) {

	//Clearing memory.
	for(int i = 0; i < mapHeight; ++i)
		delete[] levelMap[i];					//!!!Memory handled[1]+.
	delete[] levelMap;							//!!!Memory handled[1]+.

	config->playerStartingX = x;
	config->playerStartingY = y;

	return loadLevelMap(filename);

}

//Loads a config file.
void loadConfigFile(std::string filename, struct config* config) {

	std::ifstream inputFile(filename);
	inputFile >> config->screenWidth;
	inputFile >> config->screenHeight;
	inputFile >> config->tileSize;
	inputFile >> config->playerStartingX;
	inputFile >> config->playerStartingY;
	inputFile >> config->invincibilityTime;
	inputFile >> config->gameSpeed;
	inputFile.close();
	return;

}



//  __  __       _       
// |  \/  |     (_)      
// | \  / | __ _ _ _ __  
// | |\/| |/ _` | | '_ \ 
// | |  | | (_| | | | | |
// |_|  |_|\__,_|_|_| |_|
//                       

int main() {
	
	srand(time(NULL));

	//Game game;
	//game.run();

	//World.
	//World world;
	//world.loadLevelMap("level1.txt");

	//Loading config file.
	config config;
	loadConfigFile("config.txt", &config);

	//Loading level map.
	std::string levelMapName = "level1.txt";
	char** levelMap = loadLevelMap(levelMapName);

	//Creating a window.
	sf::RenderWindow mWindow(sf::VideoMode(config.screenWidth, config.screenHeight), "Badass Tales of BADASSLAND!!!!111");
	//sf::View mView;

	//Clocks.
	sf::Clock gameClock;
	sf::Clock invincibilityClock;

	sf::RectangleShape rectangle(sf::Vector2f(config.tileSize, config.tileSize));	//For tiles (temporary).

	//Creating a font.
	sf::Font font;
	font.loadFromFile("sansation.ttf");

	//HUD Health.
	sf::Text textHealth("", font, 30);
	textHealth.setStyle(sf::Text::Bold);
	textHealth.setColor(sf::Color::Red);
	textHealth.setPosition(10, 10);

	//HUD Mana.
	sf::Text textMana("", font, 30);
	textMana.setStyle(sf::Text::Bold);
	textMana.setColor(sf::Color::Blue);
	textMana.setPosition(10, 45);

	//Enemy sound.
	sf::SoundBuffer emenyHitSoundBuffer;
	emenyHitSoundBuffer.loadFromFile("sound1.ogg");
	//emenyHitSoundBuffer.loadFromFile("Sounds/NPC/ogre/ogre2.wav");
	sf::Sound emenyHitSound(emenyHitSoundBuffer);

	//Loading and setting level tileset.
	sf::Texture tileSet;
	if(!tileSet.loadFromFile("testTileSet.png"))
		return EXIT_FAILURE;
	sf::Sprite tile(tileSet);

	//HP bar.
	sf::Texture hpBar;
	if(!hpBar.loadFromFile("HPBar.png"))
		return EXIT_FAILURE;

	//Creating player.
	sf::Texture playerTexture;
	if(!playerTexture.loadFromFile("playerSpriteList.png"))
		return EXIT_FAILURE;
	Player player(playerTexture, hpBar, config.playerStartingX, config.playerStartingY, font);

	//Creating test enemy.
	sf::Texture enemyTexture;
	if(!enemyTexture.loadFromFile("enemySpriteList.png"))
		return EXIT_FAILURE;
	Enemy enemy(enemyTexture, 400, 360, font);

	//Creating test health potion.
	sf::Texture healthPotionTexture;
	if(!healthPotionTexture.loadFromFile("healthPotion.png"))
		return EXIT_FAILURE;
	DropItem healthPotion(healthPotionTexture, "healthItem", 40, 600, 150);

	//Game cycle.
	while(mWindow.isOpen()) {

		float time = gameClock.getElapsedTime().asMicroseconds();
		gameClock.restart();
		time /= config.gameSpeed;

		sf::Event event;
		while(mWindow.pollEvent(event)) {
			if(event.type == sf::Event::Closed)
				mWindow.close();
		}

		//Updating sprites of all the objects.
		player.update(time, levelMap, &config);
		enemy.update(time, levelMap, &config);
		healthPotion.update(time);



		//Player takes damage from an evil elf and becomes invincible for (invincibilityTime) seconds. Also, sound added. Also, sound de-added.
		if((player.getRect().intersects(enemy.getRect())) && (invincibilityClock.getElapsedTime().asSeconds() > config.invincibilityTime)) {
			enemy.dealDamage(player);
			invincibilityClock.restart();
		}


		//Consumption of the health potion.
		if((player.getRect().intersects(healthPotion.getRect())) && (healthPotion.mIsAlive)) {
			healthPotion.action(player);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::H))	player.mHP += 5;

		//HUD display.
		std::ostringstream hudHealth;
		std::ostringstream hudMana;

		player.showStats(&hudHealth, &hudMana);

		textMana.setString(hudMana.str());
		textHealth.setString(hudHealth.str());

		//Clearing the screen.
		mWindow.clear(sf::Color::White);

		//Rendering level map tiles.
		for(int i = 0; i < mapHeight; ++i) {
			for(int j = 0; j < mapWidth; ++j) {
				
				switch(levelMap[i][j]) {
					case 'B': rectangle.setFillColor(sf::Color::Black); break;
					case '0': rectangle.setFillColor(sf::Color::Green); break;
					default: continue;
				}
				
				/*
				switch(levelMap[i][j]) {
					case 'A': tile.setTextureRect(sf::IntRect(tileSize * 0, tileSize * 0, tileSize * 2, tileSize * 3)); break;//OK.?
					case 'B': tile.setTextureRect(sf::IntRect(tileSize * 2, tileSize * 0, tileSize * 1, tileSize * 3)); break;//OK.?
					case 'C': tile.setTextureRect(sf::IntRect(tileSize * 3, tileSize * 0, tileSize * 1, tileSize * 3)); break;//OK.?
					case 'D': tile.setTextureRect(sf::IntRect(tileSize * 4, tileSize * 0, tileSize * 2, tileSize * 3)); break;//OK.?

					case 'E': tile.setTextureRect(sf::IntRect(tileSize * 0, tileSize * 3, tileSize * 1, tileSize * 1)); break;//OK.
					case 'F': tile.setTextureRect(sf::IntRect(tileSize * 5, tileSize * 3, tileSize * 1, tileSize * 1)); break;//OK.

					case 'G': tile.setTextureRect(sf::IntRect(tileSize * 0, tileSize * 4, tileSize * 1, tileSize * 1)); break;//OK.
					case 'H': tile.setTextureRect(sf::IntRect(tileSize * 5, tileSize * 4, tileSize * 1, tileSize * 1)); break;//OK.

					case 'I': tile.setTextureRect(sf::IntRect(tileSize * 0, tileSize * 5, tileSize * 2, tileSize * 2)); break;
					case 'J': tile.setTextureRect(sf::IntRect(tileSize * 2, tileSize * 6, tileSize * 1, tileSize * 1)); break;
					case 'K': tile.setTextureRect(sf::IntRect(tileSize * 3, tileSize * 6, tileSize * 1, tileSize * 1)); break;
					case 'L': tile.setTextureRect(sf::IntRect(tileSize * 4, tileSize * 5, tileSize * 2, tileSize * 2)); break;

					case 'M': tile.setTextureRect(sf::IntRect(tileSize * 1, tileSize * 03, tileSize * 1, tileSize * 1)); break;//OK.
					//case 'N': tile.setTextureRect(sf::IntRect(360, 201, tileSize, tileSize)); break;
					default: continue;
				}
				*/

				rectangle.setPosition(config.tileSize * j - offsetX, config.tileSize * i - offsetY);
				mWindow.draw(rectangle);
				//tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
				//mWindow.draw(tile);

			}
		}

		//Rendering all the objects.
		mWindow.draw(enemy.mSprite);
		mWindow.draw(enemy.mTextName);
		mWindow.draw(healthPotion.mSprite);

		mWindow.draw(player.mSprite);
		mWindow.draw(player.mHpSprite);
		mWindow.draw(player.mTextName);

		mWindow.draw(textHealth);
		mWindow.draw(textMana);
		mWindow.display();

	}

	//Clearing memory.
	for(int i = 0; i < mapHeight; ++i)
		delete[] levelMap[i];					//!!!Memory handled[1]+.
	delete[] levelMap;							//!!!Memory handled[1]+.

	return 0;

}