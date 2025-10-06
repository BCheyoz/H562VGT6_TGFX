#include <gui/screen4_screen/Screen4View.hpp>
#include <touchgfx/events/GestureEvent.hpp>
#include <images/BitmapDatabase.hpp>

Screen4View::Screen4View()
{

}

void Screen4View::setupScreen()
{
    Screen4ViewBase::setupScreen();
	uint8_t elemTxt = T_LISTELEMTXT8;
    for (int i = 0; i < scrollList1ListItems.getNumberOfDrawables(); i++)
	{
		scrollList1ListItems[i].setupListElement(touchgfx::Bitmap(BITMAP_SUP_ID), (TEXTS)elemTxt);
		scrollList1ListItems[i].updateWildcard(i);
		elemTxt++;
	}
}

void Screen4View::tearDownScreen()
{
    Screen4ViewBase::tearDownScreen();
}

void Screen4View::scrollUp(){
	handleGestureEvent(GestureEvent(GestureEvent::SWIPE_VERTICAL, 1, 200, 150));
}

void Screen4View::scrollDown(){
	handleGestureEvent(GestureEvent(GestureEvent::SWIPE_VERTICAL, -1, 200, 150));
}
