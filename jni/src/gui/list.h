#ifndef LIST_H
#define LIST_H

class Element;

class List : public Element
{
private:
    vector <string> elements;
    int selected = 0;
    int shift = 0, maxShift = 0;
    float fshift = 0;
    bool moved = false;
    float elementHeight = 0.2;
    bool looped = false;
    bool selectable = true;
    void updateVertices ();
public:
    List(const vector<string> &listElements, Rect rect, vec4 bgColor, vec4 fgColor, function<void()> f);
    void setElements(const vector<string> &listElements);
    vector<textUnit> getTexts ();
    void run (float x, float y);
    void move (float dx, float dy);
    int getSelected() const;
};

#endif // LIST_H
