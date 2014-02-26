using namespace rpg;

class Player {
public:
	//std::string name;
	float dx, dy;
	sf::FloatRect rect;
	sf::Sprite sprite;
	float currentFrame;
	bool isAlive;
	int hp;
	int mp;

	//Player constructor.
	Player(sf::Texture &image, int x, int y) {
		sprite.setTexture(image);
		rect = sf::FloatRect(x, y, 120, 110);					//Character x, y, width, height.
		sprite.setTextureRect(sf::IntRect(0, 15, 120, 110));
		dx = 0;
		dy = 0;
		currentFrame = 0;
		isAlive = true;
		hp = 100;
		mp = 100;
	}

	//Player sprite render.
	void update(float time, char** levelMap) {

		rect.left += dx * time;
		collision(0, levelMap);
		rect.top += dy * time;
		collision(1, levelMap);

		currentFrame += 0.005 * time;
		if(currentFrame > 10) currentFrame -= 10;

		if(dx > 0) sprite.setTextureRect(sf::IntRect(120 * int(currentFrame), 925, 120, 110));
		if(dx < 0) sprite.setTextureRect(sf::IntRect(120 * int(currentFrame), 667, 120, 110));
		if(dy > 0) sprite.setTextureRect(sf::IntRect(120 * int(currentFrame), 535, 120, 110));
		if(dy < 0) sprite.setTextureRect(sf::IntRect(120 * int(currentFrame), 787, 120, 110));

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
		dy = 0;

	}
	

	//direction = 0: horizontal.
	//direction = 1: vertical.
	void collision(int direction, char** levelMap) {
		for(int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; ++i)
			for(int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; ++j) {

				if(levelMap[i][j] == 'B') {
					if((dx > 0) && (direction == 0)) rect.left = j * tileSize - rect.width;
					if((dx < 0) && (direction == 0)) rect.left = j * tileSize + tileSize;
					if((dy > 0) && (direction == 1)) rect.top = i * tileSize - rect.height;
					if((dy < 0) && (direction == 1)) rect.top = i * tileSize + tileSize;
				}
				if(levelMap[i][j] == '0') {
					levelMap[i][j] = ' ';
					mp += 10;
				}

			}
	}
	
};