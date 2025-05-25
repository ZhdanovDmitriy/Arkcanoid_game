#include "game.h"
#include <cstdlib>

Game::Game() {
    ball = new Ball(width, height, 0.2f);
    slider = new Slider(width, height, 0.5f);

    const int blockSize = BaseBlock::getSize() / 2;

    for (int rowIndex = 0; rowIndex < 3; rowIndex++) {
        std::vector<BaseBlock*> row;

        for (int colIndex = 0; colIndex < width / blockSize; colIndex++) {
            BaseBlock* block;
            int probability = rand() % 10;
            int pos_x = blockSize + colIndex * 2 * blockSize;
            int pos_y = blockSize + rowIndex * 2 * blockSize;

            switch (probability) {
            case 0:
                block = new UnbreakableBlock(pos_x, pos_y);
                break;
            case 1:
                block = new StrongBlock(pos_x, pos_y);
                break;
            case 2:
                block = new SpeedBlock(pos_x, pos_y);
                break;
            case 3:
                block = new BonusBlock(pos_x, pos_y);
                break;
            default:
                block = new BaseBlock(pos_x, pos_y);
                break;
            }

            row.push_back(block);
        }

        blocks.push_back(row);
    }

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font arial.ttf\n";
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setString("Score: 0");
    {
        auto bounds = scoreText.getLocalBounds();
        float x = width - bounds.width - 40.f;
        float y = height - bounds.height - 10.f;
        scoreText.setPosition(x, y);
    }
}

void Game::checkBlocks() {
    for (int rowIndex = 0; rowIndex < blocks.size(); rowIndex++) {
        for (int colIndex = 0; colIndex < blocks[rowIndex].size(); colIndex++) {
            auto block = blocks[rowIndex][colIndex];

            if (block == nullptr)
                continue;

            ball->setTouchesBrick(*block);

            if (block->getHealth() == 0) {
                blocks[rowIndex][colIndex] = nullptr;
                score += 1;
                scoreText.setString("Score: " + std::to_string(score));
            }
        }
    }
}

void Game::checkEvents(sf::RenderWindow& window, sf::Event& event) {
    if (event.type == sf::Event::Closed)
        window.close();
    else if (event.type == sf::Event::KeyPressed) {
        if (pressed != sf::Keyboard::Scancode::Unknown)
            return;

        auto button = event.key.scancode;
        if (button == sf::Keyboard::Scan::A) {
            slider->setWay(Vector2(-1, 0));
            pressed = button;
        }
        else if (button == sf::Keyboard::Scan::D) {
            slider->setWay(Vector2(1, 0));
            pressed = button;
        }
    }
    else if (event.type == sf::Event::KeyReleased) {
        auto button = event.key.scancode;
        if (button == pressed) {
            slider->setWay(Vector2(0, 0));
            pressed = sf::Keyboard::Scancode::Unknown;
        }
    }
}

void Game::render(sf::RenderWindow& window) {
    window.draw(scoreText);
    window.draw(ball->getDraw());
    window.draw(slider->getDraw());

    for (int rowIndex = 0; rowIndex < blocks.size(); rowIndex++) {
        for (int colIndex = 0; colIndex < blocks[rowIndex].size(); colIndex++) {
            auto block = blocks[rowIndex][colIndex];

            if (block == nullptr)
                continue;

            window.draw(block->getDraw());
        }
    }
}

void Game::update(sf::RenderWindow& window) {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event))
            checkEvents(window, event);

        window.clear();
        ball->update(*slider);
        {
            auto circle = ball->getDraw();
            float ballBottom = circle.getPosition().y + circle.getRadius() * 2.f;
            if (!floorTouchedFlag && ballBottom >= height) {
                score -= 1;
                scoreText.setString("Score: " + std::to_string(score));
                auto bounds = scoreText.getLocalBounds();
                scoreText.setPosition(
                    width - bounds.width - 40.f,
                    height - bounds.height - 10.f
                );
                floorTouchedFlag = true;
            }
            if (ballBottom < height) {
                floorTouchedFlag = false;
            }
        }

        checkBlocks();
        slider->update();
        render(window);
        window.display();
    }
}

Game::~Game() {
    delete ball;
    delete slider;
    for (auto& row : blocks) {
        for (auto& block : row) {
            delete block;
        }
    }
}

void Game::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Arcanoid");

    update(window);
}