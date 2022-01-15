#include "replay.h"

#include <QBrush>
#include <QFileInfo>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "junqi.h"

Replay::Role Replay::fromJGSRole(quint8 role)
{
    int roleCount = static_cast<int>(Role::Count);
    int roleMap = (roleCount - role) % roleCount;
    return static_cast<Replay::Role>(roleMap);
}

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

    replay.role = fromJGSRole(bytes[0x0f]);
    replay.steps = (quint8)bytes[0x1b] * 0x100 + (quint8)bytes[0x1a];

    int pos = 0x20;
    int type = 0;
    for (int j = 0; j < 4; j++) {
        int color = (quint8)bytes[pos];
        if (color != 0xff) {
            type++;
            int len = 20;
            while (static_cast<quint8>(bytes[pos + 8 + len - 1]) == 0) {
                len--;
            }
            auto name = bytes.mid(pos + 8, len);
            replay.names[color] = QTextCodec::codecForName("GBK")->toUnicode(name);
        }
        pos += 88;
    }
    int flag = static_cast<quint8>(bytes[0x19c + 10 * (replay.steps - 1) + 3]);
    if (flag == 2) replay.result = Replay::Result::Tie;
    else {
        Role loseRole = fromJGSRole(bytes[0x19c + 10 * (replay.steps - 2) + 2]);
        if (type == 2) {
            if (replay.role == loseRole) replay.result = Replay::Result::Lose;
            else replay.result = Replay::Result::Win;
        } else {
            if (replay.role == loseRole || replay.role == thatRole(loseRole))
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
    replay.role = static_cast<Role>(obj.value("role").toInt());
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

QColor Replay::colorFrom(Role c)
{
    switch (c) {
    case Role::Orange:
        return JunQi::orange;
    case Role::Blue:
        return JunQi::blue;
    case Role::Green:
        return JunQi::green;
    case Role::Purple:
        return JunQi::purple;
    default:
        return QColor();
    }
}

Replay::Role Replay::thatRole(Role c)
{
    return static_cast<Role>(
                static_cast<int>(c) ^ 2);
}

QByteArray Replay::toJson() const
{
    QJsonObject obj;
    obj.insert("filePath", filePath);
    obj.insert("role", static_cast<int>(role));
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

QColor Replay::resultColor() const
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

QBrush Replay::resultBrush() const
{
    return resultColor();
}

QColor Replay::thisColor() const
{
    if (thisName().isEmpty()) return QColor(Qt::gray);
    else return colorFrom(thisRole());
}

QBrush Replay::thisBrush() const
{
    return thisColor();
}

QColor Replay::thatColor() const
{
    if (thatName().isEmpty()) return QColor(Qt::gray);
    else return colorFrom(thatRole());
}

QBrush Replay::thatBrush() const
{
    return thatColor();
}

QColor Replay::prevColor() const
{
    if (prevName().isEmpty()) return QColor(Qt::gray);
    else return colorFrom(prevRole());
}

QBrush Replay::prevBrush() const
{
    return prevColor();
}

QColor Replay::nextColor() const
{
    if (nextName().isEmpty()) return QColor(Qt::gray);
    else return colorFrom(nextRole());
}

QBrush Replay::nextBrush() const
{
    return nextColor();
}

Replay::Role Replay::thisRole() const
{
    return role;
}

Replay::Role Replay::thatRole() const
{
    return thatRole(role);
}

Replay::Role Replay::prevRole() const
{
    return static_cast<Role>(
        (static_cast<int>(role) + 1) % static_cast<int>(Role::Count));
}

Replay::Role Replay::nextRole() const
{
    return static_cast<Role>(
                (static_cast<int>(role) + 3) % 4);
}

QString Replay::name(Role c) const
{
    return names[static_cast<int>(c)];
}

QString Replay::thisName() const
{
    return name(role);
}

QString Replay::thatName() const
{
    return name(thatRole());
}

QString Replay::prevName() const
{
    return name(prevRole());
}

QString Replay::nextName() const
{
    return name(nextRole());
}

QString Replay::resultString() const
{
    if (result == Result::Win) return QObject::tr("Win");
    else if (result == Result::Tie) return QObject::tr("Tie");
    else if (result == Result::Lose) return QObject::tr("Lose");
    return "Unknown";
}
