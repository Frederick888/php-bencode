#include "bdict.h"

BDict::BDict(int maxGear) {
    this->maxGear = maxGear;
    this->currentGear = 1;
    this->speed = 0;
}

void BDict::shift(int gear) {
    if (gear < 1 || gear > maxGear) {
        return;
    }
    currentGear = gear;
}

void BDict::accelerate() {
    speed += (5 * this->getCurrentGear());
}

void BDict::brake() {
    speed -= (5 * this->getCurrentGear());
}

int BDict::getCurrentSpeed() {
    return speed;
}

int BDict::getCurrentGear() {
    return currentGear;
}

