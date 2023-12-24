#ifndef COIN_H
#define COIN_H

struct Coin {
    int x, y, width, height;
    int speed;

    Coin(int xPos, int yPos, int coinWidth, int coinHeight, int coinSpeed)
        : x(xPos), y(yPos), width(coinWidth), height(coinHeight), speed(coinSpeed) {}

    void move();
};

#endif // COIN_H
