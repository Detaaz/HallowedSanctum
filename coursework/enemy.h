#pragma once
#include "entity.h"

// Forward declaration to avoid circular dependency 
class Player;

class Enemy : public Entity {
private:
	float speed;
	int enemyHealth;

	float attackCooldownTimer = 0.0f;
	const float attackCooldownTime = 1.0f;

	float attackWaitTimer = 0.5f;
	const float attackWaitTime = 0.5f;

	bool _pointsCounted = false;
	bool _isDead = false;


public:

	void Reset(/*const std::vector<std::unique_ptr<Enemy>>& enemies*/);
	void Update(double dt, Player& player, const std::vector<std::unique_ptr<Enemy>>& enemies);
	Enemy();
	void Render(sf::RenderWindow& window) const override;
	bool validMove(sf::Vector2f pos);
	void takeDamage();
	void makeAttack(double dt, Player& player);

	bool isDead() const;
	void countPoint();
	bool pointsCounted() const;


	void Enemy::movement(double dt, const Player& player,const std::vector<std::unique_ptr<Enemy>>& enemies);
};