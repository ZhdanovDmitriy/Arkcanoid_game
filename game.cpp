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
    for (auto& row : blocks) {
        for (auto& cell : row) {
            if (!cell) continue;
            ball->setTouchesBrick(*cell);

            if (cell->getHealth() == 0) {
                if (auto bb = dynamic_cast<BonusBlock*>(cell)) {
                    float bx = bb->getPosition().getX();
                    float by = bb->getPosition().getY();
                    Bonus* newB = new Bonus(bx, by, 0.3f);
                    sf::CircleShape s(BaseBlock::getSize() / 4.f);
                    s.setFillColor(sf::Color::Yellow);
                    s.setPosition(bx, by);
                    bonuses.push_back({ newB, s });
                }
                delete cell;
                cell = nullptr;
                score += 1;
                scoreText.setString("Score: " + std::to_string(score));
                auto b = scoreText.getLocalBounds();
                scoreText.setPosition(width - b.width - 10.f,
                    height - b.height - 10.f);
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

    for (auto& row : blocks)
        for (auto& b : row)
            if (b) window.draw(b->getDraw());

    for (auto& bi : bonuses)
        window.draw(bi.shape);
}

void Game::update(sf::RenderWindow& window) {
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event))
            checkEvents(window, event);

        window.clear();
        ball->update(*slider);

        {
            auto csh = ball->getDraw();
            float bottom = csh.getPosition().y + csh.getRadius() * 2;
            if (!floorTouchedFlag && bottom >= height) {
                score--;
                scoreText.setString("Score: " + std::to_string(score));
                auto b = scoreText.getLocalBounds();
                scoreText.setPosition(width - b.width - 10.f,
                    height - b.height - 10.f);
                floorTouchedFlag = true;
            }
            if (bottom < height) floorTouchedFlag = false;
        }

        checkBlocks();
        slider->update();

        for (auto it = bonuses.begin(); it != bonuses.end(); ) {
            it->bonus->move();
            Vector2 pos = static_cast<Position&>(*it->bonus).getPosition();
            it->shape.setPosition(pos.getX(), pos.getY());
            if (it->shape.getGlobalBounds().intersects(slider->getDraw().getGlobalBounds())) {
                Vector2 w = ball->getWay();
                float angle = (std::rand() % 91 - 45) * 3.14159f / 180.f;
                float nx = w.getX() * std::cos(angle) - w.getY() * std::sin(angle);
                float ny = w.getX() * std::sin(angle) + w.getY() * std::cos(angle);
                Vector2 newWay(nx, ny); newWay.norm();
                ball->setWay(newWay);

                delete it->bonus;
                it = bonuses.erase(it);
            }
            else if (pos.getY() > height) {
                delete it->bonus;
                it = bonuses.erase(it);
            }
            else ++it;
        }
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
    for (auto& bi : bonuses)
        delete bi.bonus;
}

void Game::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Arcanoid");

    update(window);
}