#include "GuiPanel.h"
GuiPanel::GuiPanel(float width) : panelWidth(width)
{
    background.setFillColor(sf::Color(0, 0, 0, 140));
    background.setSize({panelWidth, 1000});

    font.loadFromFile("assets/Roboto-Regular.ttf");
}

void GuiPanel::setFPS(float fpsVal) { fps = fpsVal; }
void GuiPanel::setCursorCoord(long double x, long double y) { cursorX = x; cursorY = y; }
void GuiPanel::setZoom(long double z) { zoom = z; }

void GuiPanel::addButton(const std::string& text, std::function<void()> onClick)
{
    float y = 100 + buttons.size() * 50;
    buttons.push_back({ sf::FloatRect{10, y, panelWidth - 20, 40}, text, onClick });
}

void GuiPanel::addSlider(const std::string& label, float min, float max, float* ptr)
{
    float y = 300 + sliders.size() * 60;
    sliders.push_back({ sf::FloatRect{10, y, panelWidth - 20, 20}, min, max, ptr, false, label });
}

bool GuiPanel::isMouseOver(const sf::FloatRect& r, sf::Vector2f mp) const
{
    return r.contains(mp);
}

void GuiPanel::update(const sf::RenderWindow& window, sf::Vector2f mousePos)
{
    // check slider dragging
    for (auto& s : sliders) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!s.dragging && isMouseOver(s.rect, mousePos))
                s.dragging = true;
        } else {
            s.dragging = false;
        }

        if (s.dragging) {
            float t = (mousePos.x - s.rect.left) / s.rect.width;
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            *s.valuePtr = s.minValue + t * (s.maxValue - s.minValue);
        }
    }

    // check button clicks
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for (auto& b : buttons) {
            if (isMouseOver(b.rect, mousePos)) {
                b.onClick();
            }
        }
    }
}

void GuiPanel::draw(sf::RenderWindow& window)
{
    background.setPosition(window.getSize().x - panelWidth, 0);
    background.setSize({ panelWidth * 1.f, (float)window.getSize().y });
    window.draw(background);

    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(16);

    float x0 = window.getSize().x - panelWidth + 10;

    text.setString("FPS: " + std::to_string((int)fps));
    text.setPosition(x0, 10);
    window.draw(text);

    text.setString("Cursor: " + std::to_string((double)cursorX) +
                   ", " + std::to_string((double)cursorY));
    text.setPosition(x0, 35);
    window.draw(text);

    text.setString("Zoom : " + std::to_string((double)zoom));
    text.setPosition(x0, 60);
    window.draw(text);

    // buttons
    for (auto& b : buttons) {
        sf::RectangleShape r;
        r.setPosition(x0 + (b.rect.left - 10), b.rect.top);
        r.setSize({ b.rect.width, b.rect.height });
        r.setFillColor(sf::Color(40, 40, 40));

        window.draw(r);

        text.setString(b.text);
        text.setPosition(r.getPosition().x + 5, r.getPosition().y + 5);
        window.draw(text);
    }

    // sliders
    for (auto& s : sliders) {
        sf::RectangleShape bar;
        bar.setPosition(x0 + (s.rect.left - 10), s.rect.top);
        bar.setSize({ s.rect.width, 5 });
        bar.setFillColor(sf::Color(150, 150, 150));
        window.draw(bar);

        float v = (*s.valuePtr - s.minValue) / (s.maxValue - s.minValue);
        float knobX = bar.getPosition().x + v * s.rect.width;

        sf::CircleShape knob(7);
        knob.setFillColor(sf::Color::White);
        knob.setPosition(knobX - 7, s.rect.top - 4);
        window.draw(knob);

        text.setString(s.label + ": " + std::to_string(*s.valuePtr));
        text.setPosition(bar.getPosition().x, bar.getPosition().y - 20);
        window.draw(text);
    }
}