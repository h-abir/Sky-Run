#include "coin.h"
#include<stdlib.h>
#include<time.h>

void Coin::move() {
    x -= speed;

    if (x + width < 0) {
        x = 1280; // You may need to adjust this value based on your window size
        y = rand() % 800;
    }
}
