#include "player.h"
#include "enemy.h"
#include "levelsystem.h"

using namespace sf;
using namespace std;

Player::Player() : speed(200.0), playerHealth(100), Entity(make_unique<CircleShape>(10.0)) {}

void Player::Update(double dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
	facingDirection = attackDirection(facingDirection);
	movement(dt);

	/*if (Keyboard::isKeyPressed(Keyboard::J)) {
		playerHealth = takeDamage(playerHealth);
	}*/

	/*if (Keyboard::isKeyPressed(Keyboard::K)) {
		cout << "health = " << playerHealth;
		cout << endl;
	}*/

	// Stops player from being able to attack every game tick
	if (attackCooldownTimer > 0.0f) {
		// setting timer to delta time
		attackCooldownTimer -= static_cast<float>(dt);
	}
	
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (attackCooldownTimer <= 0.0f){
			makeAttack(facingDirection, enemies);
			attackCooldownTimer = attackCooldownTime;
		}
	}

	Entity::Update(dt);
}

int Player::getHealth() const {
	return playerHealth;
}

void Player::recoverHealth(int recoverAmount) {
	playerHealth += recoverAmount;
}

bool Player::aliveCheck() const {
	if (playerHealth > 0) {
		return true;
	}
	else {
		return false;
	}
}

int Player::attackDirection(int dir) {
	// Allows the player to control their attack direction seperate from their movement.
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		return 1;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down)) {
		return 2;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Left)) {
		return 3;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right)) {
		return 4;
	}
	else {
		return dir;
	}
}

void Player::movement(double dt) {
	//move in 4 directions
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		if (validMove(position + Vector2f(0, -speed * dt/* - 25.0*/)))
			move(Vector2f(0, -speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::S)) {
		if (validMove(position + Vector2f(0, speed * dt + 20.0)))
			move(Vector2f(0, speed * dt));
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		if (validMove(position + Vector2f(-speed * dt/* - 20.0*/, 0)))
			move(Vector2f(-speed * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		if (validMove(position + Vector2f(speed * dt + 20.0, 0))) //0.50
			move(Vector2f(speed * dt, 0));
	}
}

void Player::makeAttack(int direction, const std::vector<std::unique_ptr<Enemy>>& enemies) {
	sf::Vector2f playerPos = getPosition();

	sf::Vector2f directionStartMod;
	sf::Vector2f directionFinMod;
	int swingMultiplier = 3;
	// Changing swing start points based on facing
	if (direction == 1) {
		// Up
		directionStartMod = sf::Vector2f(-25.0f, -25.0f) * static_cast<float>(swingMultiplier);
		directionFinMod = sf::Vector2f(0.0f, 25.0f) * static_cast<float>(swingMultiplier);
	}
	if (direction == 2) {
		// Down
		directionStartMod = sf::Vector2f(-25.0f, 0.0f) * static_cast<float>(swingMultiplier);
		directionFinMod = sf::Vector2f(25.0f, 25.0f) * static_cast<float>(swingMultiplier);
	}
	if (direction == 3) {
		// Left
		directionStartMod = sf::Vector2f(-25.0f, -25.0f) * static_cast<float>(swingMultiplier);
		directionFinMod = sf::Vector2f(0.0f, 25.0f) * static_cast<float>(swingMultiplier);
	}
	if (direction == 4) {
		// Right
		directionStartMod = sf::Vector2f(0.0f, -25.0f) * static_cast<float>(swingMultiplier);
		directionFinMod = sf::Vector2f(25.0f, 25.0f) * static_cast<float>(swingMultiplier);
	}


	// Calculating the start and end point of the swing area 
	sf::Vector2f swingStart = playerPos + directionStartMod;
	sf::Vector2f swingEnd = playerPos + directionFinMod;

	cout << "Player Position: " << playerPos << endl;
	cout << "Swing Start: " << swingStart << endl;
	cout << "Swing End: " << swingEnd << endl;

	int enemyIndex = 0;
	for (auto& enemy : enemies) {
		sf::Vector2f enemyPos = enemy->getPosition();

		// Check if enemy falls within swing box
		if (enemyPos.x >= swingStart.x && enemyPos.x <= swingEnd.x && enemyPos.y >= swingStart.y && enemyPos.y <= swingEnd.y) {
			cout << "Enemy " << enemyIndex << "hit";
			cout << endl;
			enemy->takeDamage();
		}

		enemyIndex++;
	}
}

void Player::Reset() {
	shape->setFillColor(Color::Green);
	shape->setOrigin(Vector2f(0, 0));
	playerHealth = 100;
	aliveCheck();

	std::vector<sf::Vector2ul> spawnPoint = ls::findTiles(ls::START);
	SetSpawn(spawnPoint);
	
}

int Player::takeDamage() {
	int enemyAttack = 15;
	playerHealth = playerHealth - enemyAttack;

	return playerHealth;
}

void Player::Render(RenderWindow& window) const {
	window.draw(*shape);
}

bool Player::validMove(sf::Vector2f pos)
{
	return (ls::getTileAt(pos) != ls::WALL);
}