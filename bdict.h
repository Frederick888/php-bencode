#ifndef BENCODE_BDICT_H
#define BENCODE_BDICT_H

// A simple try of the objective extension
class BDict {
public:
    BDict(int maxGear);
    void shift(int gear);
    void accelerate();
    void brake();
    int getCurrentSpeed();
    int getCurrentGear();
private:
    int maxGear;
    int currentGear;
    int speed;
};

#endif
