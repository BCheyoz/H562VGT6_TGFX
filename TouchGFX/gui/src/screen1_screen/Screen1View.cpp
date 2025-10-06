#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{
    digitalHours = 17;
    digitalMinutes = 29;
    digitalSeconds = 10;
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateData()
{
	//updateData @ 62 tick = 992ms

	if (++digitalSeconds >= 60)
	{
		digitalSeconds = 0;
		if (++digitalMinutes >= 60)
		{
			digitalMinutes = 0;
			if (++digitalHours >= 24)
			{
				digitalHours = 0;
			}
		}
	}

	digitalClock1.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);

	int currentValue = boxProgress1.getValue();

	currentValue = currentValue + 10;

	if (currentValue >= 100){
		currentValue = 0;
	}

	boxProgress1.setValue(currentValue);
}
