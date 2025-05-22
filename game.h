#include <SFML/Graphics.hpp>
#include <vector>

#include "objects.h"

class Game {
private:
    const int width = 1080, height = 720;
    int score = 0;

    sf::Keyboard::Scancode pressed = sf::Keyboard::Scancode::Unknown;
    sf::Text scoreText;


    Ball* ball;
    Slider* slider;
    std::vector<std::vector<BaseBlock*>> blocks;

public:
    Game();

    void checkBlocks();

    void checkEvents(sf::RenderWindow& window, sf::Event& event);

    void render(sf::RenderWindow& window);

    void update(sf::RenderWindow& window);

    void run();
};
