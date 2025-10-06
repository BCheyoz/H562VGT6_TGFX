#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Color.hpp>

Screen2View::Screen2View()
{
    btn = false;
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::changeTxt()
{
	if(btn == true){
		btn = false;
	    textArea1_2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
	}
	else {
		btn = true;
	    textArea1_2.setColor(touchgfx::Color::getColorFromRGB(200, 200, 200));
	}
}
