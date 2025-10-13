#include <gui/screen6_screen/Screen6View.hpp>

#include "FirmwareStateMachine.hpp"

Screen6View::Screen6View() : tickCount(0), tankLvl(0), movePx(2)
{
}

void Screen6View::setupScreen()
{
    Screen6ViewBase::setupScreen();
}

void Screen6View::tearDownScreen()
{
    Screen6ViewBase::tearDownScreen();
}

void Screen6View::waveAnimationDone(){
	waveAnimated.stopAnimation();
	waveAnimated.startAnimation(!waveAnimated.isReverse(), false, true);
}

void Screen6View::handleTickEvent(){
	tickCount++;
	uint16_t sysTankLvl = FwMng::getInstance()->getTankLvl();

	if (tickCount % 2 == 0 && tankLvl != sysTankLvl){ // 2 ticks = 32ms
		if(tankLvl > sysTankLvl ){ // descendre le niveau
			tankLvl--;
			if(waveAnimated.getY() < TANK_ANIMATION_MAX_Y){
				if(waveAnimated.getY() + movePx > TANK_ANIMATION_MAX_Y){
					movePx = 1;
				}

				waveAnimated.setY(waveAnimated.getY() + movePx);
				box1.setY(box1.getY() + movePx);
				box1.setHeight(box1.getHeight() - movePx);
			}
		}
		else { // monte le niveau
			tankLvl++;
			if(waveAnimated.getY() > TANK_ANIMATION_MIN_Y){
				if(waveAnimated.getY() - movePx < TANK_ANIMATION_MIN_Y){
					movePx = 1;
				}

				waveAnimated.setY(waveAnimated.getY() - movePx);
				box1.setY(box1.getY() - movePx);
				box1.setHeight(box1.getHeight() + movePx);
			}
		}

	    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", tankLvl);
	    textArea1.invalidate();

		// la bar de progrssion fait 150 pixels pour 100%
		// déplace 1.5 pixels etant impossible on bouge de 1 puis de 2.
		if(movePx == 1){
			movePx = 2;
		}
		else {
			movePx = 1;
		}
	}

	Screen6ViewBase::handleTickEvent();
}
