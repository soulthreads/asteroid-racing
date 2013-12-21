#include "element.h"
#include "button.h"

Button::Button(const string &label, Rect rect, vec4 bgColor, vec4 fgColor, function<void()> f)
    : Element (rect, bgColor, fgColor, f)
{
    text = textUnit {vec2 (r.x+r.w/2.0, r.y-r.h/2.0),
            fgColor, 1, A_CENTER, A_CENTER, label};

    id = "button" + label;
}

vector<textUnit> Button::getTexts()
{
    vector<textUnit> ret {text};
    return ret;
}
