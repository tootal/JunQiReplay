#ifndef REPLAY_H
#define REPLAY_H

#include <QDateTime>

class Replay
{
public:
    // 军旗中四方角色，用颜色名称指代
    // 按走棋顺序排列
    enum class Role : int {
        Orange,
        Purple,
        Green,
        Blue,
        Count // 总数
    };
    // 游戏结果
    enum class Result : int {
        Win,
        Lose,
        Tie,
        Count // 总数
    };

    // 从JGS颜色数据映射到角色
    static Role fromJGSRole(quint8 role);
    // 从JGS文件解析出复盘
    static Replay fromJGSFile(const QString &filePath);
    // 从缓存的Json数据解析出Replay
    static Replay fromJson(const QByteArray &json);

    // 通过角色获取颜色, 玩家不存在使用灰色
    static QColor colorFrom(Role c);
    // 获取对家角色
    static Role thatRole(Role c);

    // 复盘数据转换为Json数据用于缓存
    QByteArray toJson() const;

    bool hasError() const;

    // 通过结果获取色值
    QColor resultColor() const;
    QBrush resultBrush() const;

    // 获取玩家色值
    QColor thisColor() const;
    QBrush thisBrush() const;
    // 获取玩家色值
    QColor thatColor() const;
    QBrush thatBrush() const;
    // 获取玩家色值
    QColor prevColor() const;
    QBrush prevBrush() const;
    // 获取玩家色值
    QColor nextColor() const;
    QBrush nextBrush() const;

    // 获取玩家颜色
    Role thisRole() const;
    // 获取对家颜色
    Role thatRole() const;
    // 获取上家颜色
    Role prevRole() const;
    // 获取下家颜色
    Role nextRole() const;

    // 获取指定角色的玩家名字
    QString name(Role c) const;
    // 获取玩家名字
    QString thisName() const;
    // 获取对家名字
    QString thatName() const;
    // 获取上家名字
    QString prevName() const;
    // 获取下家名字
    QString nextName() const;
    // 结果展示字符串
    QString resultString() const;

    Role role;
    QDateTime time;
    int steps;
    QString names[4];
    Result result;
    QString filePath;
    QString errorString;
};

#endif // REPLAY_H
