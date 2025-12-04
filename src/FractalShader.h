#pragma once
#include <SFML/Graphics.hpp>
#include <Camera.h>
#include <string>

class FractalShader {
    public:
    FractalShader(const std::string& path, const sf::Vector2u& windowSize);

    void update(const Camera& camera);
    void draw(sf::RenderTarget& target);
    void setBaseIterations(int base, int perZoom);

    private:
    sf::Shader shader;
    sf::RectangleShape screen;

    bool shaderLoaded = false;
    int baseIterations = 200;
    int iterationsPerLog = 120;
};