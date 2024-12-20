#include "entity.h"
#include <LevelSystem.h>

using namespace std;
using namespace sf;

Vector2f Entity::getPosition() const { return position; }

void Entity::setPosition(const Vector2f& pos) { position = pos; }

void Entity::move(const Vector2f& pos) { position += pos; }

void Entity::Update(const double dt) { shape->setPosition(position); }

void Entity::SetSpawn(std::vector<sf::Vector2ul> spawnPoint) {
	if (!spawnPoint.empty()) {
		size_t randomSpawn = rand() % spawnPoint.size();
		sf::Vector2ul tilePos = spawnPoint[randomSpawn];

		position = ls::getTilePosition(tilePos);
	}
	else {
		// Spawn in default position if no tile is found 
		std::cerr << "No spawn tiles found! Using default position.\n";
		position = { 50.0f, 50.0f };
		setPosition(position);
	}
}

int Entity::takeDamage(int health) { return health; }

Entity::Entity(unique_ptr<Shape> s) : shape(std::move(s)) {}