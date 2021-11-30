#include "replay.h"

#include <QBrush>

QBrush Replay::brush(Color c)
{
    switch (c) {
    case Color::Yellow:
        return QColor(216, 108, 0);
    case Color::Blue:
        return QColor(40, 104, 160);
    case Color::Green:
        return QColor(115, 158, 5);
    case Color::Purple:
        return QColor(144, 68, 160);
    default:
        return QBrush();
    }
}

QBrush Replay::resultBrush() const
{
    switch (result) {
    case Result::Win:
        return QColor(Qt::darkGreen);
    case Result::Tie:
        return QColor(Qt::darkGray);
    case Result::Lose:
        return QColor(Qt::red);
    default:
        return QColor(Qt::gray);
    }
}

QBrush Replay::thisBrush() const
{
    if (thisName().isEmpty()) return QColor(Qt::gray);
    else return brush(thisColor());
}

QBrush Replay::thatBrush() const
{
    if (thatName().isEmpty()) return QColor(Qt::gray);
    else return brush(thatColor());
}

QBrush Replay::prevBrush() const
{
    if (prevName().isEmpty()) return QColor(Qt::gray);
    else return brush(prevColor());
}

QBrush Replay::nextBrush() const
{
    if (nextName().isEmpty()) return QColor(Qt::gray);
    else return brush(nextColor());
}

Replay::Color Replay::thisColor() const
{
    return color;
}

Replay::Color Replay::thatColor() const
{
    return static_cast<Color>(
        static_cast<int>(color) ^ 2);
}

Replay::Color Replay::prevColor() const
{
    return static_cast<Color>(
        (static_cast<int>(color) + 1) % 4);
}

Replay::Color Replay::nextColor() const
{
    return static_cast<Color>(
                (static_cast<int>(color) + 3) % 4);
}

QString Replay::name(Color c) const
{
    return names[static_cast<int>(c)];
}

QString Replay::thisName() const
{
    return name(color);
}

QString Replay::thatName() const
{
    return name(thatColor());
}

QString Replay::prevName() const
{
    return name(prevColor());
}

QString Replay::nextName() const
{
    return name(nextColor());
}
