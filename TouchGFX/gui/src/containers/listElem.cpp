#include <gui/containers/listElem.hpp>

listElem::listElem()
{

}

void listElem::initialize()
{
    listElemBase::initialize();
}

void listElem::setupListElement(const Bitmap& iconBMP, TEXTS iconTextID)
{
	scalableImage1.setBitmap(iconBMP);
	textArea1.setTypedText(TypedText(iconTextID));
	textArea1.resizeToCurrentText();
    invalidate();
}

void listElem::updateWildcard(uint8_t val){
	Unicode::snprintf(textArea1Buffer, 3, "%d", val);
	textArea1.invalidate();
}
