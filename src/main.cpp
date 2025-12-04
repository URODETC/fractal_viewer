#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <sstream>
#include "Camera.h"
#include "FractalShader.h"

int main() {
    sf::VideoMode vm(1920, 1080);
    sf::RenderWindow window(vm, "Fractal Viewer");
    window.setFramerateLimit(120);

    Camera camera({-0.5f, 0.0f}, {3.0f, 3.0f * (float(vm.height) / float(vm.width))});
    FractalShader fractalShader("shaders/mandelbrot.frag", window.getSize());
    fractalShader.setBaseIterations(300, 140);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text infoText("", font, 16);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(8.f, 8.f);

    sf::Clock fpsClock;
    float fps = 60.0f;
    sf::Vector2i lastMousePos;
    bool dragging = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::Resized) {
                sf::Vector2u newSize(event.size.width, event.size.height);
                camera = Camera(camera.getCenter(), {
                                    camera.getScale().x, camera.getScale().x * (float(newSize.y) / float(newSize.x))
                                });
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.85f : 1.15f;
                sf::Vector2f mouseNorm(event.mouseWheelScroll.x / float(window.getSize().x),
                                       event.mouseWheelScroll.y / float(window.getSize().y));
                camera.zoom(zoomFactor, mouseNorm);
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                dragging = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                dragging = false;
            }
        }

        if (dragging) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (mousePos != lastMousePos) {
                sf::Vector2f delta(
                    float(lastMousePos.x - mousePos.x) / float(window.getSize().x),
                    -float(lastMousePos.y - mousePos.y) / float(window.getSize().y)
                );
                camera.move(delta);
                lastMousePos = mousePos;
            }
        }



        fractalShader.update(camera);

        window.clear();
        fractalShader.draw(window);

        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        double mx = camera.getCenter().x + (mouse.x / double(window.getSize().x) - 0.5) * camera.getScale().x;
        double my = camera.getCenter().y + (mouse.y / double(window.getSize().y) - 0.5) * camera.getScale().y;

        float dt = fpsClock.restart().asSeconds();
        if (dt > 0.0f) fps = 0.95f*fps + 0.05f*(1.0f/dt);

        std::ostringstream ss;
        ss.setf(std::ios::fixed); ss.precision(8);
        ss << "Mouse: (" << mx << ", " << my << ")\n";
        ss << "Scale: " << camera.getScale().x << " (world width)\n";
        ss << "FPS: " << int(fps);
        infoText.setString(ss.str());
        window.draw(infoText);

        window.display();
    }
    return 0;
}
