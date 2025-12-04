#include "Camera.h"

Camera::Camera(sf::Vector2f center, sf::Vector2f scale)
    : center(center), scale(scale) {}

void Camera::move(const sf::Vector2f &delta) {
    center += {delta.x * scale.x, delta.y * scale.y};
}

void Camera::zoom(float factor, const sf::Vector2f &mouseNorm) {
    sf::Vector2f before;
    before.x = center.x + (mouseNorm.x - 0.5f) * scale.x;
    before.y = center.y + (mouseNorm.y - 0.5f) * scale.y;

    scale.x *= factor;
    scale.y *= factor;

    sf::Vector2f after;
    after.x = center.x + (mouseNorm.x - 0.5f) * scale.x;
    after.y = center.y + (mouseNorm.y - 0.5f) * scale.y;

    center.x += (before.x - after.x);
    center.y += (before.y - after.y);
}

sf::Vector2f Camera::getCenter() const { return center; }

sf::Vector2f Camera::getScale() const { return scale; }
