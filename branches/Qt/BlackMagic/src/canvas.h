#ifndef CANVAS_H
#define CANVAS_H

#include "QSFMLCanvas.hpp"

class QWidget;


class Canvas : public QSFMLCanvas
{
public:
    Canvas(QWidget* parent=NULL);

private:
    virtual void OnInit();
    virtual void OnUpdate();

    sf::String test;
    sf::View view;
};

#endif // CANVAS_H
