#include "enemy.h"
#include "levelsystem.h"
#include "player.h"

using namespace sf;
using namespace std;

Enemy::Enemy() : speed(150.0), enemyHealth(100), Entity(make_unique<CircleShape>(10.0)) {}

void Enemy::Update(double dt, Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies) {
	// health using health int stops nullptr error
	int health = enemyHealth;
	if (health > 0) {
		shape->setFillColor(Color::Red);
		if (health <= 50) {
			shape->setFillColor(Color::Yellow);
		}
		// Checks if enemy can make an attack, does if possible
		makeAttack(dt, player);

		// Handles all movement for the enemies, including avoiding collision with other enemies
		movement(dt, player, enemies);

	}
	else {
		shape->setFillColor(Color::Blue);
		_isDead = true;
	}
	
	Entity::Update(dt);
}

bool Enemy::isDead() const {
	return _isDead;
} 

void Enemy::countPoint() {
	_pointsCounted = true;
}

bool Enemy::pointsCounted() const {
	return _pointsCounted;
}

void Enemy::makeAttack(double dt, Player& player) {
	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f enemyPos = getPosition();
	sf::Vector2f chaseDirection = playerPos - enemyPos;

	float chaseDistance = std::sqrt(chaseDirection.x * chaseDirection.x + chaseDirection.y * chaseDirection.y);

	/*float attackWaitTimer = 0.5f;
	const float attackWaitTime = 0.5f;*/

	// Stops enemy from being able to attack every game tick
	if (attackCooldownTimer > 0.0f) {
		// setting timer to delta time
		attackCooldownTimer -= static_cast<float>(dt);
	}

	// resets wait timer if the player moves too far away
	if (chaseDistance >= 50.f) {
		attackWaitTimer = attackWaitTime;
		return;
	}

	// Delay timer counts down while player is in range
	if (attackWaitTimer > 0.0f) {
		attackWaitTimer -= static_cast<float>(dt);
	}
	else {
		// Performs attack if timer hits 0
		if (attackCooldownTimer <= 0.0f) {
			player.takeDamage();
			attackCooldownTimer = attackCooldownTime;
			attackWaitTimer = attackWaitTime;
		}
	}	
}

void Enemy::takeDamage() {
	int playerAttack = 15;
	enemyHealth = enemyHealth - playerAttack;

}

void Enemy::movement(double dt, const Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies) {
	sf::Vector2f totalMovement(0.0f, 0.0f);
	
	// Handles chasing player and avoiding sitting on them
	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f enemyPos = getPosition();
	sf::Vector2f chaseDirection = playerPos - enemyPos;

	float chaseDistance = std::sqrt(chaseDirection.x * chaseDirection.x + chaseDirection.y * chaseDirection.y);
	// Normalises the direction vector
	if (chaseDistance != 0.0f) {
		chaseDirection.x /= chaseDistance;
		chaseDirection.y /= chaseDistance;
	}

	// Stop enemy from moving if they're too close to the player, avoids standing on top of player,
	if (chaseDistance > 25.0f) {
		//Handles moving towards the player, cast to float to stop issue with double
		totalMovement += chaseDirection * static_cast<float>(speed * dt);
	}
	else if (chaseDistance < 25.0f) {
		totalMovement += -chaseDirection * static_cast<float>(speed * dt);
	}

	// Avoiding collisions between enemies
	sf::Vector2f totalRepulsion(0.0f, 0.0f);

	for (const auto& otherEnemy : enemies) {
		// Skip if current enemy and otherEnemy are the same
		if (this == otherEnemy.get()) continue;

		sf::Vector2f otherEnemyPos = otherEnemy->getPosition();
		sf::Vector2f enemyPos = getPosition();
		sf::Vector2f diff = otherEnemyPos - enemyPos;

		float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

		if (distance > 0.0f && distance < 25.0f) {
			sf::Vector2f repulse = (diff / distance) * (15.0f - distance) * 10.0f;
			totalRepulsion += repulse * static_cast<float>(5.f * dt);
		}

	}
	totalMovement += totalRepulsion;

	move(totalMovement);
}

void Enemy::Reset(/*const std::vector<std::unique_ptr<Enemy>>& enemies*/) {
	shape->setFillColor(Color::Red);
	//shape->setOrigin(Vector2f(0, 0));
}

void Enemy::Render(RenderWindow& window) const {
	window.draw(*shape);
}

bool Enemy::validMove(sf::Vector2f pos)
{
	return (ls::getTileAt(pos) != ls::WALL);
}
