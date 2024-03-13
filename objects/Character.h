#ifndef CHARACTER_H
#define CHARACTER_H

#include "Ground.h"
#include "ObjectsList.h"

enum CHARACTER_TYPE
{
    MAIN_BIRD,
    BAT,

    CHARACTER_COUNT
};

enum DIRECTION
{
    LEFT,
    RIGHT
};

class Character : public Object
{
private:
    LTexture sTexture[4];

    CHARACTER_TYPE mCharacterType;
    DIRECTION direction;

    int initImgChangeVelWhenAir;
    int imgCount;
    int curTimeRender;
    int curIMGRender;
    int imgChangeVel;
    //The angle
    double mAngle;
    //The velocity
    int mVelX, mVelY;
    double mVelAngle;

    double limitAngleUpper;
    double limitAngleLower;

    //check whether character died
    bool died;

    void loadIMG();
    void setPosToBorderPos();
    void increaseTimeRender();

    bool checkOutBorder();
    bool checkGroundCollision();

    bool isStayingOnGround();
    bool move();
    bool playingRender();
    bool dyingRender();

protected:
    Character(int x, int y, CHARACTER_TYPE character);
    ~Character();

    void initCharacter(int x, int y);

    //use when have gravitation
    void decreaseVelAndAngle();

    void onDied();
    bool isDied() const;
    bool renderCharacter();

    void setVelX(int value);
    void setVelY(int value);
    void setVelAngle(double value);
    void addVelX(int value);
    void addVelY(int value);
    void addVelAngle(double value);
};

Character::Character(int x, int y, CHARACTER_TYPE character) : Object(false)
{
    mCharacterType = character;

    initCharacter(x, y);
    loadIMG();
}

Character::~Character()
{
    for (int i = 0; i < imgCount; i++) {
        sTexture[i].free();
    }
}

void Character::initCharacter(int x, int y)
{
    mTexture = &sTexture[0];

    died = false;

    curTimeRender = 0;
    curIMGRender = 0;
    imgChangeVel = 5;

    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    mAngle = 0;
    //Create the necessary SDL_Rects
    //mColliders.resize(11);

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    mVelAngle = 0;

    if (direction == RIGHT) {
        limitAngleUpper = 0;
        limitAngleLower = -45;
    }
    else if (direction == LEFT) {
        limitAngleUpper = 45;
        limitAngleLower = 0;
    }
}

void Character::onDied()
{
    if (died) {
        return;
    }

    died = true;

    //continue moving
    mVelX = gInitVelocityYScene - gVelocityYScene;
    mVelY = 15;

    if (direction == RIGHT) {
        limitAngleUpper = 90;
    }
    else {
        limitAngleLower = -90;
    }
}

void Character::loadIMG()
{
    //image: https://flappybird.io/,
    switch (mCharacterType) {
        case MAIN_BIRD:
            sTexture[0].loadFromFile(imagePath + "mainBird0.png", true, 124, 197, 205);
            sTexture[1].loadFromFile(imagePath + "mainBird1.png", true, 124, 197, 205);
            sTexture[2].loadFromFile(imagePath + "mainBird2.png", true, 124, 197, 205);
            sTexture[3] = sTexture[1];
            imgCount = 4;

            direction = RIGHT;
            initImgChangeVelWhenAir = 2;

            break;
        case BAT:
            sTexture[0].loadFromFile(imagePath + "bat0.png", true, 34,177,76);
            sTexture[1].loadFromFile(imagePath + "bat1.png", true, 34,177,76);
            sTexture[2].loadFromFile(imagePath + "bat2.png", true, 34,177,76);
            sTexture[3] = sTexture[1];
            imgCount = 4;

            direction = LEFT;
            initImgChangeVelWhenAir = 6;

            break;
        default:
            break;
    }
}

void Character::setPosToBorderPos()
{
    if (mPosX < 0) {
        mPosX = 0;
    }
    if (mPosX + getWidth() > SCREEN_WIDTH) {
        mPosX = SCREEN_WIDTH - getWidth();
    }
    if (mPosY + getHeight() < 0) {
        mPosY = -getHeight();
    }
}

void Character::increaseTimeRender()
{
    curTimeRender++;
    if (curTimeRender >= imgChangeVel) {
        curTimeRender = 0;
        curIMGRender++;
        if (curIMGRender >= imgCount) {
            curIMGRender = 0;
        }
    }

     mTexture = &sTexture[curIMGRender];
}

bool Character::playingRender()
{
    bool moved = false;
    if (move()) {
        moved = true;
    }
    //change wing flapping speed
    if (isStayingOnGround()) {
        imgChangeVel = 5;
    }
    else {
        imgChangeVel = initImgChangeVelWhenAir;
    }

    mTexture->render(mPosX, mPosY, NULL, mAngle);
    increaseTimeRender();

    return moved;
}

bool Character::dyingRender()
{
    //start falling
    decreaseVelAndAngle();

    if (mVelX > gVelocityYScene && isStayingOnGround()) {
        //Velocity X decreases due to friction
        mVelX--;
    }

    bool moved = move();
    mTexture->render(mPosX, mPosY, NULL, mAngle);
    return moved;
}

bool Character::isStayingOnGround()
{
    return (mPosY + getHeight() == groundPosY + 10);
}

bool Character::checkGroundCollision()
{
    if (mPosY + getHeight() >= groundPosY + 10) {
        return true;
    }
    return false;
}

bool Character::checkOutBorder()
{
    if (mPosX < 0) {
        return true;
    }
    if (mPosX + getWidth() > SCREEN_WIDTH) {
        return true;
    }
    if (mPosY + getHeight() < 0) {
        return true;
    }
    return false;
}

void Character::decreaseVelAndAngle()
{
    if (!isStayingOnGround()) {
        addVelY(-1);
        //Decrease Y velocity because of gravitation
    }
    addVelAngle(-0.5);
}

bool Character::move()
{
    bool moved = false;
    if (mVelX > gVelocityYScene) moved = true;

    mPosX += mVelX;
    mPosY -= mVelY;
    mAngle -= mVelAngle;

    if (mAngle > limitAngleUpper) {
        mAngle = limitAngleUpper;
    }
    if (mAngle < limitAngleLower) {
        mAngle = limitAngleLower;
    }

    //do not lower head
    if (mAngle == 0) {
        mVelAngle = 0;
    }

    if (checkGroundCollision()) {
        //not staying on the ground
        if (!isStayingOnGround()) {
            //set pos to ground pos
            mPosY = groundPosY + 10 - getHeight();
            moved = true;
        }
        else {
            //continue to fall
            mPosY += mVelY;
        }
        //reset Y velocity
        mVelY = 0;
    }
    else {
        moved = true;
    }

    if (checkOutBorder()) {
        // set pos to border pos
        setPosToBorderPos();
        if (isStayingOnGround()) {
            //reset Y velocity
            mVelY = 0;
        }
    }

    return moved;
}

bool Character::isDied() const
{
    return died;
}

bool Character::renderCharacter() {
    bool rendered = true;
    if (!died) {
        playingRender();
    }
    else {
        rendered = dyingRender();
    }

    return rendered;
}

void Character::setVelX(int value)
{
    mVelX = value;
}

void Character::setVelY(int value)
{
    mVelY = value;
}

void Character::addVelX(int value)
{
    mVelX += value;
}

void Character::addVelY(int value)
{
    mVelY += value;
}

void Character::setVelAngle(double value)
{
    mVelAngle = value;
}

void Character::addVelAngle(double value)
{
    mVelAngle += value;
}
#endif