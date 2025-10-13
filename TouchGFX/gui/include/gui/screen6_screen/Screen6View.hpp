#ifndef SCREEN6VIEW_HPP
#define SCREEN6VIEW_HPP

#include <gui_generated/screen6_screen/Screen6ViewBase.hpp>
#include <gui/screen6_screen/Screen6Presenter.hpp>

class Screen6View : public Screen6ViewBase
{
public:
    Screen6View();
    virtual ~Screen6View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void waveAnimationDone() final;
    virtual void handleTickEvent();
protected:

private :
    uint16_t tickCount;
    uint16_t tankLvl;
    uint8_t movePx;

    const uint16_t TANK_ANIMATION_MAX_Y = 185;
    const uint8_t TANK_ANIMATION_MIN_Y = 35;

};

#endif // SCREEN6VIEW_HPP
