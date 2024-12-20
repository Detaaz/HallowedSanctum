#pragma once
#include "entity.h"

// Forward declaration to avoid circular dependency 
class Enemy;

class Player : public Entity {
private:
	float speed;
	int playerHealth;
	int facingDirection = 1;
	float attackCooldownTimer = 0.0f;
	const float attackCooldownTime = 1.0f;


public:
	void Update(double dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
	
	void movement(double dt);
	int attackDirection(int dir);
	void makeAttack(int direction, const std::vector<std::unique_ptr<Enemy>>& enemies);
	int takeDamage();
	void Reset() override;
	bool aliveCheck() const;
	int getHealth() const;
	void recoverHealth(int recoverAmount);


	Player();
	void Render(sf::RenderWindow& window) const override;
	bool validMove(sf::Vector2f pos);
};