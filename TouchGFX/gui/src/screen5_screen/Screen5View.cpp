#include <gui/screen5_screen/Screen5View.hpp>
#include "FirmwareGateway.h"
#include "FirmwareStateMachine.hpp"

Screen5View::Screen5View()
{

}

void Screen5View::setupScreen()
{
    Screen5ViewBase::setupScreen();
}

void Screen5View::tearDownScreen()
{
    Screen5ViewBase::tearDownScreen();
}

void Screen5View::updateScreen5Data(){

	int16_t ctn = getCtn0()/100;
	if(ctn <= 0) ctn = 0;
	if (ctn < 100){
		TmpBasGauge.setValue(ctn);
		TmpBasGauge.invalidate();
	}

	ctn = getCtn1()/100;
	if(ctn <= 0) ctn = 0;
	if (ctn < 100){
		TmpHautGauge.setValue(ctn);
		TmpHautGauge.invalidate();
	}

    Unicode::snprintf(textArea6Buffer, TEXTAREA6_SIZE, "%d", fanFeedbackSpeed());
    textArea6.invalidate();

    FwMng *fwp = FwMng::getInstance();
    SmartGridToggleButton.forceState(fwp->isSmartState() == 0 ? false : true);
    HpHCToggleButton.forceState(fwp->isJNState() == 0 ? false : true);
    SmartGridToggleButton.invalidate();
    HpHCToggleButton.invalidate();
}

void Screen5View::enablePopup(bool enable){
	customContainer11.setVisible(enable);
	invalidate();
}


