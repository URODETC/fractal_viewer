#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>

class GuiPanel {
public:
    struct Button {
        sf::FloatRect rect;
        std::string text;
        std::function<void()> onClick;
    };

    struct Slider {
        sf::FloatRect rect;
        float minValue, maxValue;
        float* valuePtr;
        bool dragging = false;
        std::string label;
    };

    GuiPanel(float width = 250.f);

    void update(const sf::RenderWindow& window, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    void addButton(const std::string& text, std::function<void()> onClick);
    void addSlider(const std::string& label, float min, float max, float* value);

    void setFPS(float fps);
    void setCursorCoord(long double x, long double y);
    void setZoom(long double zoom);

private:
    float panelWidth;
    sf::RectangleShape background;

    float fps = 0.f;
    long double cursorX = 0;
    long double cursorY = 0;
    long double zoom = 1;

    sf::Font font;

    std::vector<Button> buttons;
    std::vector<Slider> sliders;

    bool isMouseOver(const sf::FloatRect& r, sf::Vector2f mp) const;
};