#include <SFML/Graphics.hpp>
#include <vector>

#include "objects.h"

class Game {
private:
    int score = 0;
    const int width = 1080, height = 720;

    sf::Keyboard::Scancode pressed = sf::Keyboard::Scancode::Unknown;

    Ball* ball;
    Slider* slider;
    std::vector<std::vector<BaseBlock*>> blocks;

    bool floorTouchedFlag = false;

    struct BonusItem {
        Bonus* bonus;
        sf::CircleShape shape;
    };
    std::vector<BonusItem> bonuses;

    bool noFloorPenaltyActive = false;
    bool stickinessActive = false;
    float stickinessFactor = 0.5f;

public:
    sf::Font font;
    sf::Text scoreText;

    Game();

    void checkBlocks();

    void checkEvents(sf::RenderWindow& window, sf::Event& event);

    void render(sf::RenderWindow& window);

    void update(sf::RenderWindow& window);

    void run();

    ~Game();
};
