#include "objects.h"


Collision::Collision(int _width, int _height) : GameBorderData(_width, _height) {}

Ball::Ball(int gameWidth, int gameHeight, float speed) :
    Move(Vector2(gameWidth / 2, gameHeight / 2), speed),
    Collision(gameWidth, gameHeight) {
    draw = sf::CircleShape(radius);
    draw.setFillColor(sf::Color(255, 255, 255));
    updateDrawPosition();
}

sf::CircleShape Ball::getDraw() { return draw; }

void Ball::updateDrawPosition() {
    draw.setPosition(position->getX() - radius, position->getY() - radius);
}

void Ball::setTouchesBorder() {
    if ((position->getX() - radius) <= 0 || (position->getX() + radius) >= width)
        way->setX(-way->getX());
    if ((position->getY() - radius) <= 0 || (position->getY() + radius) >= height)
        way->setY(-way->getY());
}

void Ball::setTouchesSlider(Slider& slider) {
    Vector2 sliderPosition = slider.getPosition();

    float x_distance = abs(sliderPosition.getX() - position->getX()), half_width = static_cast<float>(slider.getWidth()) / 2;
    float y_distance = abs(sliderPosition.getY() - position->getY()), half_height = static_cast<float>(slider.getHeight()) / 2;

    bool horizontalCond = x_distance <= (radius + half_width);
    bool verticalCond = y_distance <= (radius + half_height);

    if (!(verticalCond && horizontalCond))
        return;

    auto newWay = getWay() + slider.getWay();

    if (newWay.length() == 0)
        setWay(Vector2(0, 1));
    else {
        newWay.norm();
        setWay(newWay);
    }

    if ((sliderPosition.getX() - half_width) <= position->getX() && (sliderPosition.getX() + half_width) >= position->getX()) {
        position->setY(position->getY() + (position->getY() > sliderPosition.getY() ? 1 : -1) * (2 * radius + half_height - y_distance));
        way->setY(-way->getY());
    }
    else if ((sliderPosition.getY() - half_height) <= position->getY() && (sliderPosition.getY() + half_height) >= position->getY()) {
        position->setX(position->getX() + (position->getX() > sliderPosition.getX() ? 1 : -1) * (2 * radius + half_width - x_distance));
        way->setX(-way->getX());
    }
    else {
        way->setY(-way->getY());
        way->setX(-way->getX());
    }
}

void Ball::setTouchesBrick(BaseBlock& block) {
    Vector2 blockPosition = block.getPosition();

    float x_distance = abs(blockPosition.getX() - position->getX()), half_width = static_cast<float>(block.getSize()) / 2;
    float y_distance = abs(blockPosition.getY() - position->getY()), half_height = static_cast<float>(block.getSize()) / 2;

    bool horizontalCond = x_distance <= (radius + half_width);
    bool verticalCond = y_distance <= (radius + half_height);

    if (!(verticalCond && horizontalCond))
        return;

    block.hit();

    if (block.isGiveSpeed())
        speed += 0.4f;

    if ((blockPosition.getX() - half_width) <= position->getX() && (blockPosition.getX() + half_width) >= position->getX()) {
        position->setY(position->getY() + (position->getY() > blockPosition.getY() ? 1 : -1) * (2 * radius + half_height - y_distance));
        way->setY(-way->getY());
    }
    else if ((blockPosition.getY() - half_height) <= position->getY() && (blockPosition.getY() + half_height) >= position->getY()) {
        position->setX(position->getX() + (position->getX() > blockPosition.getX() ? 1 : -1) * (2 * radius + half_width - x_distance));
        way->setX(-way->getX());
    }
    else {
        way->setY(-way->getY());
        way->setX(-way->getX());
    }
}

void Ball::update() {
    move();
    setTouchesBorder();
    updateDrawPosition();
}

void Ball::update(Slider& slider) {
    update();

    setTouchesSlider(slider);
}

void Ball::setSpeed(float newSpeed) {
    speed = newSpeed;
}

Slider::Slider(int gameWidth, int gameHeight, float speed) : Move(Vector2(gameWidth / 2, 5 * gameHeight / 6), Vector2(0, 0), speed), GameBorderData(gameWidth, gameHeight){
    draw = sf::RectangleShape(sf::Vector2f(selfWidth, selfHeight));
    draw.setFillColor(sf::Color::White);
    updateDrawPosition();
}

int Slider::getWidth() { return selfWidth; }

int Slider::getHeight() { return selfHeight; }

sf::RectangleShape Slider::getDraw() { return draw; }

void Slider::updateDrawPosition() {
    draw.setPosition(position->getX() - static_cast<float>(selfWidth) / 2, position->getY());
}

void Slider::move() {
    auto newPosX = position->getX() + way->getX() * speed;

    if (0 <= (newPosX - static_cast<float>(selfWidth) / 2) && (newPosX + static_cast<float>(selfWidth) / 2) <= width) {
        position->setX(newPosX);
    }
}

void Slider::update() {
    move();
    updateDrawPosition();
}

void Slider::expandWidth(float factor) {
    selfWidth = static_cast<int>(selfWidth * factor);
    draw.setSize({ float(selfWidth), float(selfHeight) });
    updateDrawPosition();
}


Bonus::Bonus(float _x, float _y, float _speed): Move(Vector2(_x, _y), Vector2(0, 1), _speed){}

TrajectoryBonus::TrajectoryBonus(float x, float y, float s): Bonus(x, y, s) {}

SpeedResetBonus::SpeedResetBonus(float x, float y, float s): Bonus(x, y, s) {}

PaddleExpandBonus::PaddleExpandBonus(float x, float y, float s): Bonus(x, y, s) {}

IncreasedStickinessBonus::IncreasedStickinessBonus(float x, float y, float s): Bonus(x, y, s) {}

NoFloorPenaltyBonus::NoFloorPenaltyBonus(float x, float y, float s): Bonus(x, y, s) {}

BaseBlock::BaseBlock(float _x, float _y) : Position(_x, _y) {
    health = 1;
    draw = sf::RectangleShape(sf::Vector2f(selfWidth, selfHeight));
    draw.setFillColor(sf::Color(200, 58, 93));
    draw.setPosition(position->getX() - size / 2, position->getY() - size / 2);
}

sf::RectangleShape BaseBlock::getDraw() { return draw; }

bool BaseBlock::isGiveSpeed() { return giveSpeed; }

void BaseBlock::hit() { health -= 1; }

int BaseBlock::getHealth() { return health; }

int BaseBlock::getSize() { return size; }

StrongBlock::StrongBlock(float _x, float _y) : BaseBlock(_x, _y) {
    health = 3;
    draw.setFillColor(sf::Color(53, 200, 43));
}

UnbreakableBlock::UnbreakableBlock(float _x, float _y) : BaseBlock(_x, _y) {
    draw.setFillColor(sf::Color(100, 100, 100));
}

void UnbreakableBlock::hit() { return; }

SpeedBlock::SpeedBlock(float _x, float _y) : BaseBlock(_x, _y) {
    giveSpeed = true;
    draw.setFillColor(sf::Color(123, 32, 200));
}

BonusBlock::BonusBlock(float _x, float _y) : BaseBlock(_x, _y) {
    draw.setFillColor(sf::Color(32, 32, 200));
}