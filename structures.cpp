#include "structures.h"

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

Vector2 Vector2::operator+(Vector2 another) {
    return Vector2(x + another.getX(), y + another.getY());
}

float Vector2::getX() { return x; }

float Vector2::getY() { return y; }

float Vector2::length() { return sqrt(x * x + y * y); }

void Vector2::setX(float _x) { x = _x; }

void Vector2::setY(float _y) { y = _y; }

void Vector2::norm() {
    float _length = length();
    x /= _length;
    y /= _length;
}

Position::Position(float _x, float _y) { position = new Vector2(_x, _y); }

void Position::setPosition(Vector2& vector) {
    setPosition(vector.getX(), vector.getY());
}

void Position::setPosition(float _x, float _y) {
    position->setX(_x);
    position->setY(_y);
}

Vector2 Position::getPosition() { return Vector2(position->getX(), position->getY()); }

Move::Move(Vector2 _position, float _speed) : speed(_speed), Position(_position.getX(), _position.getY())
{
    way = new Vector2(0, 1);
    way->norm();
}

Move::Move(Vector2 _position, Vector2 _way, float _speed) : Move(_position, _speed) {
    setWay(_way);
}

Vector2 Move::getWay() {
    return Vector2(way->getX(), way->getY());
}

void Move::setWay(Vector2 _way) {
    way->setX(_way.getX());
    way->setY(_way.getY());
}

void Move::move() {
    position->setX(position->getX() + way->getX() * speed);
    position->setY(position->getY() + way->getY() * speed);
}

GameBorderData::GameBorderData(int _width, int _height) : width(_width), height(_height) {}