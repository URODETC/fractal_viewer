#include "FractalShader.h"

#include <iostream>
#include <cmath>

FractalShader::FractalShader(const std::string& path, const sf::Vector2u& windowSize) {
    screen.setSize(sf::Vector2f(windowSize));
    screen.setPosition(0,0);
    shaderLoaded = shader.loadFromFile(path, sf::Shader::Fragment);
    if (!shaderLoaded) {
        std::cerr << "Failed to load shader: " << path << std::endl;
    }

    if (shaderLoaded) {
        shader.setUniform("u_resolution", sf::Glsl::Vec2(windowSize));
        shader.setUniform("u_escapeRadius", 4.0f);
    }
}

void FractalShader::setBaseIterations(int base, int perZoom) {
    baseIterations = base;
    iterationsPerLog = perZoom;
}

void FractalShader::update(const Camera& camera) {
    if (!shaderLoaded) return;
    double scaleX = camera.getScale().x;
    double zoomLevel = 0.0;
    if (scaleX > 0) zoomLevel = std::max(0.0, -std::log10(scaleX));

    int maxI = baseIterations + int(zoomLevel * iterationsPerLog);
    if (maxI < 32) maxI = 32;
    if (maxI > 5000) maxI = 5000;

    shader.setUniform("u_center", camera.getCenter());
    shader.setUniform("u_scale", camera.getScale());
    shader.setUniform("u_maxIterations", maxI);
}

void FractalShader::draw(sf::RenderTarget& target)
{
    target.draw(screen, &shader);
}

void FractalShader::ChangeC(float a, float b) {
    shader.setUniform("u_ca", a);
    shader.setUniform("u_cb", b);
}
