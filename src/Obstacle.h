#ifndef OBSTACLE_H
#define OBSTACLE_H

struct Obstacle {
    int x, y, width, height;
    int speed;

    Obstacle(int xPos, int yPos, int obstacleWidth, int obstacleHeight, int obstacleSpeed)
        : x(xPos), y(yPos), width(obstacleWidth), height(obstacleHeight), speed(obstacleSpeed) {}

    void move();
};

#endif // OBSTACLE_H
