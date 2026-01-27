#include "ParticleSystem.hpp"
#include <cmath>

ParticleSystem::ParticleSystem() 
    : rng(std::random_device{}()),
      angleDist(0.0f, 6.28318f),  // 0 to 2*PI
      speedDist(50.0f, 150.0f) {
    particles.reserve(100);  // Memory optimization: pre-allocate space
}

void ParticleSystem::createBurst(const sf::Vector2f& position, int count, const sf::Color& color) {
    for (int i = 0; i < count; ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng);
        
        sf::Vector2f velocity(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        );
        
        // Slight color variation for visual interest
        sf::Color particleColor = color;
        particleColor.r = static_cast<sf::Uint8>(std::min(255, color.r + (rand() % 50 - 25)));
        particleColor.g = static_cast<sf::Uint8>(std::min(255, color.g + (rand() % 50 - 25)));
        particleColor.b = static_cast<sf::Uint8>(std::min(255, color.b + (rand() % 50 - 25)));
        
        float lifetime = 0.5f + (rand() % 100) / 200.0f;  // 0.5 to 1.0 seconds
        float size = 3.0f + (rand() % 3);  // 3 to 5 pixels
        
        particles.emplace_back(position, velocity, particleColor, lifetime, size);
    }
}

void ParticleSystem::update(float deltaTime) {
    // Update all particles and remove dead ones
    // Iterate backwards to safely remove elements
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifetime -= deltaTime;
        
        if (it->lifetime <= 0.0f) {
            it = particles.erase(it);
        } else {
            // Update position
            it->position += it->velocity * deltaTime;
            
            // Apply gravity
            it->velocity.y += 200.0f * deltaTime;
            
            // Fade out based on remaining lifetime
            float alpha = (it->lifetime / it->maxLifetime) * 255.0f;
            it->color.a = static_cast<sf::Uint8>(alpha);
            
            ++it;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& particle : particles) {
        sf::CircleShape circle(particle.size);
        circle.setPosition(particle.position);
        circle.setFillColor(particle.color);
        circle.setOrigin(particle.size, particle.size);
        window.draw(circle);
    }
}

void ParticleSystem::clear() {
    particles.clear();
}

void ParticleSystem::createExplosion(const sf::Vector2f& position, int count, const sf::Color& baseColor) {
    for (int i = 0; i < count; ++i) {
        float angle = angleDist(rng);
        float speed = speedDist(rng) * 1.5f;  // Faster than regular burst
        
        sf::Vector2f velocity(
            std::cos(angle) * speed,
            std::sin(angle) * speed
        );
        
        // Create color variation with red/orange tints for death effect
        sf::Color particleColor;
        int colorChoice = rand() % 3;
        
        if (colorChoice == 0) {
            particleColor = sf::Color::Red;
        } else if (colorChoice == 1) {
            particleColor = sf::Color(255, 140, 0);  // Orange
        } else {
            particleColor = baseColor;
        }
        
        // Add some variation
        particleColor.r = static_cast<sf::Uint8>(std::min(255, particleColor.r + (rand() % 50 - 25)));
        particleColor.g = static_cast<sf::Uint8>(std::max(0, particleColor.g + (rand() % 50 - 25)));
        particleColor.b = static_cast<sf::Uint8>(std::max(0, particleColor.b + (rand() % 50 - 25)));
        
        float lifetime = 0.8f + (rand() % 100) / 100.0f;  // 0.8 to 1.8 seconds
        float size = 4.0f + (rand() % 3);  // (larger than regular burst)
        
        particles.emplace_back(position, velocity, particleColor, lifetime, size);
    }
}

// ScorePopup Implementation 

ScorePopup::ScorePopup() 
    : lifetime(0.0f), maxLifetime(1.5f), active(false), velocity(0.0f, -50.0f) {}

void ScorePopup::create(const sf::Vector2f& position, int points, const sf::Font& font) {
    text.setFont(font);
    text.setString("+" + std::to_string(points));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color(255, 215, 0));  // Gold color
    text.setStyle(sf::Text::Bold);
    
    // Center the text at the position
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    text.setPosition(position);
    
    lifetime = maxLifetime;
    active = true;
}

void ScorePopup::update(float deltaTime) {
    if (!active) return;
    
    lifetime -= deltaTime;
    
    if (lifetime <= 0.0f) {
        active = false;
        return;
    }
    
    // Move upward
    text.move(velocity * deltaTime);
    
    // Fade out
    float alpha = (lifetime / maxLifetime) * 255.0f;
    sf::Color color = text.getFillColor();
    color.a = static_cast<sf::Uint8>(alpha);
    text.setFillColor(color);
    
    // Scale effect (grow slightly then shrink)
    float scale = 1.0f + (1.0f - lifetime / maxLifetime) * 0.3f;
    text.setScale(scale, scale);
}

void ScorePopup::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(text);
    }
}