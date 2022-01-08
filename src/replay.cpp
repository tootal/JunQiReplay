#include "replay.h"

#include <QBrush>
#include <QFileInfo>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Replay Replay::fromJGSFile(const QString &filePath)
{
    Replay replay;
    {
        QFileInfo fileInfo(filePath);
        replay.time = fileInfo.fileTime(QFileDevice::FileModificationTime);
        replay.filePath = fileInfo.absoluteFilePath();
    }
    QByteArray bytes;
    {
        QFile file(replay.filePath);
        if (!file.open(QFile::ReadOnly)) {
            replay.errorString = "file open failed!";
            return replay;
        }
        bytes = file.readAll();
    }

    replay.color = static_cast<Replay::Color>(bytes[0x0f]);
    replay.steps = (quint8)bytes[0x1b] * 0x100 + (quint8)bytes[0x1a];

    int pos = 0x20;
    int type = 0;
    for (int j = 0; j < 4; j++) {
        int color = (quint8)bytes[pos];
        if (color != 0xff) {
            type++;
            auto name = bytes.mid(pos + 8, 20);
            replay.names[color] = QTextCodec::codecForName("GBK")->toUnicode(name);
        }
        pos += 88;
    }
    int flag = static_cast<quint8>(bytes[0x19c + 10 * (replay.steps - 1) + 3]);
    if (flag == 2) replay.result = Replay::Result::Tie;
    else {
        Color lose_color = static_cast<Color>(bytes[0x19c + 10 * (replay.steps - 2) + 2]);
        if (type == 2) {
            if (replay.color == lose_color) replay.result = Replay::Result::Lose;
            else replay.result = Replay::Result::Win;
        } else {
            if (replay.color == lose_color || replay.color == thatColor(lose_color))
                replay.result = Replay::Result::Lose;
            else replay.result = Replay::Result::Win;
        }
    }
    return replay;
}

Replay Replay::fromJson(const QByteArray &json)
{
    auto obj = QJsonDocument::fromJson(json).object();
    Replay replay;
    if (obj.isEmpty()) {
        replay.errorString = "json is not an object.";
        return replay;
    }
    replay.filePath = obj.value("filePath").toString();
    replay.color = static_cast<Color>(obj.value("color").toInt());
    replay.time = QDateTime::fromMSecsSinceEpoch(
                    obj.value("time").toVariant().toULongLong());
    replay.steps = obj.value("steps").toInt();
    {
        auto arr = obj.value("names").toArray();
        if (arr.isEmpty() || arr.size() != 4) {
            replay.errorString = "json.names is not an array of 4 items.";
            return replay;
        }
        for (int i = 0; i < 4; i++) {
            replay.names[i] = arr[i].toString();
        }
    }
    replay.result = static_cast<Result>(obj.value("result").toInt());
    return replay;
}

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

Replay::Color Replay::thatColor(Color c)
{
    return static_cast<Color>(
                static_cast<int>(c) ^ 2);
}

QByteArray Replay::toJson() const
{
    QJsonObject obj;
    obj.insert("filePath", filePath);
    obj.insert("color", static_cast<int>(color));
    obj.insert("time", time.toMSecsSinceEpoch());
    obj.insert("steps", steps);
    {
        QJsonArray arr;
        for (int i = 0; i < 4; i++) {
            arr.append(names[i]);
        }
        obj.insert("names", arr);
    }
    obj.insert("result", static_cast<int>(result));
    return QJsonDocument(obj).toJson();
}

bool Replay::hasError() const
{
    return !errorString.isEmpty();
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
    return thatColor(color);
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

QString Replay::resultString() const
{
    if (result == Result::Win) return QObject::tr("Win");
    else if (result == Result::Tie) return QObject::tr("Tie");
    else if (result == Result::Lose) return QObject::tr("Lose");
    return "Unknown";
}
