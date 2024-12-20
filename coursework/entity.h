#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <LevelSystem.h>

class Entity {

protected:
	std::unique_ptr<sf::Shape> shape;
	sf::Vector2f position;
	Entity(std::unique_ptr<sf::Shape> shp);

	int health;
public:
	Entity() = delete;
	virtual ~Entity() = default;

	
	virtual void Reset() = 0;

	virtual void Update(const double dt);
	virtual void SetSpawn(std::vector<sf::Vector2ul> spawnPoint);
	virtual void Render(sf::RenderWindow& window) const = 0;

	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& pos);
	void move(const sf::Vector2f& pos);

	// Handle attacks and damage
	int takeDamage(int health);
};


