#ifndef BUTTON_H
#define BUTTON_H

class Element;

class Button : public Element
{
private:
    textUnit text;

public:
    Button(const string &label, Rect rect, vec4 bgColor, vec4 fgColor, function<void()> f);
    vector<textUnit> getTexts();
};

#endif // BUTTON_H
