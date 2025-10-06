#include <gui/screen5_screen/Screen5View.hpp>

Screen5View::Screen5View()
{

}

void Screen5View::setupScreen()
{
    Screen5ViewBase::setupScreen();
    Unicode::snprintf(textArea1Buffer, 3, "%d", 5);
    textArea1.invalidate();
}

void Screen5View::tearDownScreen()
{
    Screen5ViewBase::tearDownScreen();
}
