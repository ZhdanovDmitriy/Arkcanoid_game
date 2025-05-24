#include <iostream>
#include <SFML/Graphics.hpp>

#include "structures.h"


class AbstractObjectMovableDraw {
public:
    virtual void updateDrawPosition() = 0;
    virtual void update() = 0;
};


class Slider;
class BaseBlock;


class Collision : public GameBorderData {
public:
    Collision(int width, int height);

    virtual void setTouchesBorder() = 0;
    virtual void setTouchesSlider(Slider& slider) = 0;
    virtual void setTouchesBrick(BaseBlock& block) = 0;
};


class Ball : public Move, public Collision, public AbstractObjectMovableDraw {
private:
    const int radius = 10;

    sf::CircleShape draw;

public:
    Ball(int gameWidth, int gameHeight, float speed);

    sf::CircleShape getDraw();

    virtual void updateDrawPosition();

    virtual void setTouchesBorder();
    virtual void setTouchesSlider(Slider& slider);
    virtual void setTouchesBrick(BaseBlock& block);

    virtual void update();
    void update(Slider& slider);
};


class Slider : public Move, public GameBorderData, public AbstractObjectMovableDraw {
private:
    const int selfHeight = 10;
    int selfWidth = 200;

    sf::RectangleShape draw;

public:
    Slider(int gameWidth, int gameHeight, float speed);

    int getWidth();
    int getHeight();

    void move();

    sf::RectangleShape getDraw();

    virtual void updateDrawPosition();

    virtual void update();
};


enum class BonusType {
    None = -1,
    add_ball = 0,
    slicy,
    slider_width,
    add_ball_speed,
    change_trajectory,
};


class Bonus : public Move {
protected:
    BonusType type = BonusType::None;

public:
    Bonus(float x, float y, float speed);

    Bonus(float x, float y, float speed, BonusType type);

};


class BaseBlock : public Position {
protected:
    static const int size = 54;
    const int selfHeight = size;
    const int selfWidth = size;
    bool giveSpeed = false;

    int health;
    sf::RectangleShape draw;

public:
    BaseBlock(float x, float y);

    sf::RectangleShape getDraw();

    bool isGiveSpeed();

    virtual void hit();

    int getHealth();

    static int getSize();
};


class StrongBlock : public BaseBlock {
public:
    StrongBlock(float x, float y);
};


class UnbreakableBlock : public BaseBlock {
public:
    UnbreakableBlock(float x, float y);

    virtual void hit();
};


class SpeedBlock : public BaseBlock {
public:
    SpeedBlock(float x, float y);
};

class BonusBlock : public BaseBlock {
public:
    BonusBlock(float x, float y);
};