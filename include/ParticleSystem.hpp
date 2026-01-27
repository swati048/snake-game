#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

// Represents a single particle in the system
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;      // Current lifetime remaining
    float maxLifetime;   // Initial lifetime for fade calculation
    float size;
    
    Particle(const sf::Vector2f& pos, const sf::Vector2f& vel, 
             const sf::Color& col, float life, float sz)
        : position(pos), velocity(vel), color(col), 
          lifetime(life), maxLifetime(life), size(sz) {}
};

// Manages a collection of particles for various effects
class ParticleSystem {
public:
    ParticleSystem();
    // Creates a burst effect (for food consumption)
    void createBurst(const sf::Vector2f& position, int count, const sf::Color& color);
    // Creates an explosion effect (for death/collision)
    void createExplosion(const sf::Vector2f& position, int count, const sf::Color& baseColor);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void clear();
    

private:
    std::vector<Particle> particles;
    std::mt19937 rng;
    std::uniform_real_distribution<float> angleDist;
    std::uniform_real_distribution<float> speedDist;
};

// Displays a temporary score popup text
class ScorePopup {
public:
    ScorePopup();
    void create(const sf::Vector2f& position, int points, const sf::Font& font);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isActive() const { return active; }

private:
    sf::Text text;
    float lifetime;
    float maxLifetime;
    bool active;
    sf::Vector2f velocity;
};