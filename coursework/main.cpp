#include <SFML/Graphics.hpp>
#include "player.h"
#include "enemy.h"
#include "LevelSystem.h"

using namespace sf;
using namespace std;

/*
Add player health bar/text

create a loop so that enemies keep spawning for as long as the player is alive? ensure they die at some point
*/

// Player Object
Player player;

// Vector to store enemies
std::vector<std::unique_ptr<Enemy>> enemies;

Font font;
Text mainText;
Text headerText;
Text bodyText;
Text bodyText1;

int playerScore = 0;
int state = 0;

int enemiesDead = 0;
float enemyRespawnTimer = 3.0f;
const float enemyRespawnTime = 3.0f;

static void startScreen(RenderWindow& window) {
	headerText.setFont(font);
	headerText.setColor(Color::Black);
	headerText.setOutlineColor(Color::White);
	headerText.setOutlineThickness(1.5f);
	headerText.setCharacterSize(48);
	headerText.setPosition(200, 200);
	headerText.setString("Hallowed Sanctum");
	window.draw(headerText);

	bodyText.setFont(font);
	bodyText.setColor(Color::Black);
	bodyText.setOutlineColor(Color::White);
	bodyText.setOutlineThickness(1.f);
	bodyText.setCharacterSize(24);
	bodyText.setPosition(280, 350);
	bodyText.setString("Press Enter to start.");
	window.draw(bodyText);

	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		state = 1;
		return;
	}

	// Quit via ESC key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

}

void Load() {
	playerScore = 0;
	// load level
	ls::loadLevelFile("res/levels/level1.txt", 25.0f);

	// Print the level to the console
	for (size_t y = 0; y < ls::getHeight(); ++y) {
		for (size_t x = 0; x < ls::getWidth(); ++x) {
			cout << ls::getTile({ x, y });
		}
		cout << endl;
	}

	// load font
	if (!font.loadFromFile("res/fonts/Poppins-Black.ttf"))
		cerr << "Could not load font :(" << endl;

	player.Reset();

	enemies.clear();

	// Find enemy spawn points
	std::vector<sf::Vector2ul> spawnPoint = ls::findTiles(ls::ENEMY);

	// Converting spawn tiles to "real" coordinate positions
	std::vector<sf::Vector2f> spawnPos;
	for (const auto& tilePos : spawnPoint) {
		sf::Vector2f _pos = ls::getTilePosition(tilePos);
		spawnPos.push_back(_pos);
	}

	// Randomising the number of enemies to spawn
	srand(time(NULL));
	int enemyCount = 1 + rand() % 8;

	for (int i = 0; i < enemyCount; ++i) {
		auto enemy = std::make_unique<Enemy>();

		enemy->Reset();
		enemy->setPosition(spawnPos[i]);

		enemies.push_back(std::move(enemy));
	}

}

// end screen needs to be after load for reset to work

static void endScreen(RenderWindow& window) {
	headerText.setFont(font);
	headerText.setColor(Color::Black);
	headerText.setOutlineColor(Color::White);
	headerText.setOutlineThickness(1.5f);
	headerText.setCharacterSize(48);
	headerText.setPosition(250, 200);
	headerText.setString("Game Over!");
	window.draw(headerText);

	bodyText.setFont(font);
	bodyText.setColor(Color::Black);
	bodyText.setOutlineColor(Color::White);
	bodyText.setOutlineThickness(1.f);
	bodyText.setCharacterSize(24);

	bodyText.setPosition(300, 275);
	bodyText.setString("You scored: " + std::to_string(playerScore) + "!");
	window.draw(bodyText);


	bodyText.setPosition(280, 350);
	bodyText.setString("Press Enter to retry!");
	window.draw(bodyText);

	

	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		playerScore = 0;
		enemiesDead = 0;
		enemyRespawnTimer = enemyRespawnTime;
		player.Reset();
		enemies.clear();
		state = 0;
		Load();
		return;
	}

	// Quit via ESC key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

}

static void respawnEnemies() {
	enemies.clear();

	// Find enemy spawn points
	std::vector<sf::Vector2ul> spawnPoint = ls::findTiles(ls::ENEMY);

	// Converting spawn tiles to "real" coordinate positions
	std::vector<sf::Vector2f> spawnPos;
	for(const auto & tilePos : spawnPoint) {
		sf::Vector2f _pos = ls::getTilePosition(tilePos);
		spawnPos.push_back(_pos);
	}

	// Randomising the number of enemies to spawn
	srand(time(NULL));
	int enemyCount = 1 + rand() % 8 ;

	for (int i = 0; i < enemyCount; ++i) {
		auto enemy = std::make_unique<Enemy>();

		//enemy->Reset();
		enemy->setPosition(spawnPos[i]);

		enemies.push_back(std::move(enemy));
	}
	enemiesDead = 0;
	player.recoverHealth(15);
	
}



void Update(RenderWindow &window) {
	static Clock clock;
	float dt = clock.restart().asSeconds();

	Event event;
	while (window.pollEvent(event)) {
		if (event.type == Event::Closed) {
			window.close();
			return;
		}
	}

	// Quit via ESC key
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}
	
	// Players update
	player.Update(dt, enemies);

	// Player health txt
	bodyText.setOutlineColor(Color::White);
	bodyText.setPosition(600, 520);
	bodyText.setString("Health: " + std::to_string(player.getHealth()));
	window.draw(bodyText);

	// Checks if the game is over
	bool _playerAlive = true;
	_playerAlive = player.aliveCheck();
	if (!_playerAlive) {
		state = 2;
	}

	// Update all enemies
	for (auto& enemy : enemies) {
		enemy->Update(dt, player, enemies);
		bool _isDead = enemy->isDead();
		bool _pointCounted = enemy->pointsCounted();

		if (_isDead && !_pointCounted) {
			playerScore += 15;
			enemy->countPoint();
			enemiesDead++;
		}
		

		if (enemiesDead == enemies.size()) {
			// Gives the player time between "waves" of enemy spawns
			if (enemyRespawnTimer > 0.0f) {
				// setting timer to delta time
				enemyRespawnTimer -= static_cast<float>(dt);
			}

			if (enemyRespawnTimer <= 0.0f) {
				respawnEnemies();
				enemyRespawnTimer = enemyRespawnTime;
			}

		}

	}
}

void Render(RenderWindow& window) {
	ls::Render(window);
	player.Render(window);
	for (auto& enemy : enemies) {
		enemy->Render(window);
	}
}

int main() {
	RenderWindow window(VideoMode(800, 600), "Hallowed Sanctum");
	Load();

	while (window.isOpen()) {

		if (state == 0) {
			window.clear();
			startScreen(window);
			window.display();
		}

		if (state == 1) {
			window.clear();
			Update(window);
			Render(window);
			window.display();
		}

		if (state == 2) {
			window.clear();
			endScreen(window);
			window.display();
		}
	}
	return 0;
}
