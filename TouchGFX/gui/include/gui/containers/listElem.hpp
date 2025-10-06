#ifndef LISTELEM_HPP
#define LISTELEM_HPP

#include <gui_generated/containers/listElemBase.hpp>
#include <touchgfx/Bitmap.hpp>
#include <texts/TextKeysAndLanguages.hpp>

class listElem : public listElemBase
{
public:
    listElem();
    virtual ~listElem() {}

    virtual void initialize();

    void setupListElement(const Bitmap& iconBMP, TEXTS iconTextID);
    void updateWildcard(uint8_t val);

protected:
};

#endif // LISTELEM_HPP
