#include "canvas.h"
#include <QWidget>
#include "QSFMLCanvas.hpp"

Canvas::Canvas(QWidget* parent):
        QSFMLCanvas(QSize(1600, 1600), 0, parent)
{
    view = sf::View(sf::FloatRect(0, 0, (float)parentWidget()->width(), (float)parentWidget()->height()));
}

void Canvas::OnInit()
{
}

void Canvas::OnUpdate()
{
    view.SetFromRect(sf::FloatRect(0, 0, (float)parentWidget()->width(), (float)parentWidget()->height()));
    sf::Shape line = sf::Shape::Line(0, 16, view.GetRect().GetWidth(), 16, 1, sf::Color::White);
    sf::String test(this->parentWidget()->objectName().toStdString());
    test.Move(view.GetRect().GetWidth()/2-test.GetRect().GetWidth()/2, view.GetRect().GetHeight()/2 - test.GetRect().GetHeight()/2);
    Clear();
    this->Draw(line);
    this->Draw(test);
}
