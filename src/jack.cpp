#include "jack.h"
#include "level.h"
#include "sdlutil.h"

using namespace std;

Jack::Jack(string filename) {
    this->jack = SDLUtil::loadImage(filename);
    this->x_position = JACK_WIDTH + Level::LEVEL_X_OFFSET;
    this->y_position = Level::LEVEL_HEIGHT + Level::LEVEL_Y_OFFSET - Jack::JACK_HEIGHT - 38;
    this->speed = 0;
    verticalSpeed = 0;
    jumping = false;
	lastMove=0;
	lastButOneMove=0;
}

Jack::~Jack() {
    if(jack != NULL) {
        SDL_FreeSurface(jack);
    }
}

int setLimit(int value, int limit1, int range) {
    int limit2=limit1+range;
    if(value<limit1)
        return limit1;
    if(value>=limit2)
        return (limit2-1);
    return value;
}

void Jack::drawSelf(SDL_Surface *surface) {
    this->x_position = setLimit(x_position, Level::LEVEL_X_OFFSET,Level::LEVEL_WIDTH-JACK_WIDTH);
    this->y_position = setLimit(y_position, Level::LEVEL_Y_OFFSET,Level::LEVEL_HEIGHT-JACK_HEIGHT - 38);
    SDLUtil::applySurface(this->x_position, this->y_position, this->jack, surface);
    return;
}

void Jack::move() {
    x_position += speed;
    return;
}

void Jack::jump(Level* level) {
    y_position += verticalSpeed;
    verticalSpeed += ACCELERATION;
    if(jumping == true && y_position >= Level::LEVEL_Y_OFFSET + Level::LEVEL_HEIGHT - 57 -38 - (level->grid[(x_position - Level::LEVEL_X_OFFSET)/38]*38)) {
        jumping = false;
        verticalSpeed = 1;
    }
    if(y_position >= Level::LEVEL_Y_OFFSET + Level::LEVEL_HEIGHT - 57 -38 - (level->grid[(x_position - Level::LEVEL_X_OFFSET)/38]*38)) {
        y_position = Level::LEVEL_Y_OFFSET + Level::LEVEL_HEIGHT - 57 -38 - (level->grid[(x_position - Level::LEVEL_X_OFFSET)/38]*38);
        verticalSpeed -= ACCELERATION;
    }
    if(y_position >= Level::LEVEL_Y_OFFSET + Level::LEVEL_HEIGHT - 57 -38 - (level->grid[(x_position+38 - Level::LEVEL_X_OFFSET)/38]*38)) {
        y_position = Level::LEVEL_Y_OFFSET + Level::LEVEL_HEIGHT - 57 -38 - (level->grid[(x_position+38 - Level::LEVEL_X_OFFSET)/38]*38);
        verticalSpeed -= ACCELERATION;
    }
    //verticalSpeed++;
    return;
}

void Jack::pushMove(int v) {
		if(lastMove==0) {
			speed=v;
			lastMove=v;
		}
	else if (lastButOneMove==0) {
			speed=v;
			lastButOneMove=v;
		}
	return;
}

void Jack::popMove(int v) {
	if(lastButOneMove==v) {
		lastButOneMove=0;
		speed=lastMove;
	}
	else if(lastMove==v) {
		if(lastButOneMove!=0) {
			speed=lastButOneMove;
			lastMove=lastButOneMove;
			lastButOneMove=0;
		}
		else {
			lastMove=0;
			speed=0;
		}
	}
}
