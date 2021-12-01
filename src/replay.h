#ifndef REPLAY_H
#define REPLAY_H

#include <QDateTime>

class Replay
{
public:
    // 颜色，在军旗中代表了4方
    enum class Color : int {
        Yellow,
        Blue,
        Green,
        Purple
    };
    // 游戏结果
    enum class Result : int {
        Win,
        Lose,
        Tie
    };

    // 从JGS文件解析出复盘
    static Replay fromJGSFile(const QString &filePath);
    // 从缓存的Json数据解析出Replay
    static Replay fromJson(const QByteArray &json);

    // 通过颜色获取色值, 玩家不存在使用灰色
    static QBrush brush(Color c);
    // 获取对家颜色
    static Color thatColor(Color c);

    // 复盘数据转换为Json数据用于缓存
    QByteArray toJson() const;

    bool hasError() const;

    // 通过结果获取色值
    QBrush resultBrush() const;

    // 获取玩家色值
    QBrush thisBrush() const;
    // 获取玩家色值
    QBrush thatBrush() const;
    // 获取玩家色值
    QBrush prevBrush() const;
    // 获取玩家色值
    QBrush nextBrush() const;

    // 获取玩家颜色
    Color thisColor() const;
    // 获取对家颜色
    Color thatColor() const;
    // 获取上家颜色
    Color prevColor() const;
    // 获取下家颜色
    Color nextColor() const;

    // 获取指定颜色的玩家名字
    QString name(Color c) const;
    // 获取玩家名字
    QString thisName() const;
    // 获取对家名字
    QString thatName() const;
    // 获取上家名字
    QString prevName() const;
    // 获取下家名字
    QString nextName() const;

    Color color;
    QDateTime time;
    int steps;
    QString names[4];
    Result result;
    QString filePath;
    QString errorString;
};

#endif // REPLAY_H
