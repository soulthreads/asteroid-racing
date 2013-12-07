#ifndef BUTTON_H
#define BUTTON_H

#include "element.h"

class Button : public Element
{
private:
    textUnit text;

public:
    Button(string label, Rect rect, vec4 bgColor, vec4 fgColor, Functor f);
    vector<textUnit> getTexts();
};

#endif // BUTTON_H
