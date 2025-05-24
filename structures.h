#include <cmath>

class Vector2 {
protected:
    float x, y;

public:
    Vector2(float x, float y);

    Vector2 operator+(Vector2 another);

    float getX();

    float getY();

    float length();

    void setX(float x);

    void setY(float y);

    void norm();
};


class Position {
protected:
    Vector2* position;
public:
    Position(float x, float y);

    void setPosition(Vector2& vector);

    void setPosition(float x, float y);

    Vector2 getPosition();

    virtual ~Position();
};


class Move : public Position {
protected:
    Vector2* way;
    float speed;

public:
    Move(Vector2 position, float speed);

    Move(Vector2 position, Vector2 way, float speed);

    Vector2 getWay();

    void setWay(Vector2 way);

    void move();

    virtual ~Move();
};


class GameBorderData {
    protected:
        int width, height;

    public:
        GameBorderData(int width, int height);
};