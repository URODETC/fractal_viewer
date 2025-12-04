#pragma once
#include <SFML/System.hpp>
class Camera {
    public:
    Camera(sf::Vector2f center = {-.5f, 0}, sf::Vector2f scale = {3,2});

    void move(const sf::Vector2f& delta);
    void zoom(float factor, const sf::Vector2f& mouseNorm);

    sf::Vector2f getCenter() const;
    sf::Vector2f getScale() const;

    private:
    sf::Vector2f center;
    sf::Vector2f scale;
};