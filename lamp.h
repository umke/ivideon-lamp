#ifndef LAMP_H
#define LAMP_H

#include <QWidget>
#include <QPainter>
#include <QLabel>

enum CommandType {ON = 0x12, OFF = 0x13, COLOR = 0x20};
enum LampState {LampON, LampOFF};

struct CommandData;

class Lamp : public QWidget
{
    Q_OBJECT
public:
    explicit Lamp(QWidget *parent = nullptr);
    void setCommand(const CommandData &);
    void paintEvent(QPaintEvent *event);
    void clearState();
private:
    QColor m_color;
    QPen m_pen;
    QBrush m_brash;
    LampState m_state;
    CommandType m_commandType;
    QLabel m_lableStatus;
    QFont m_font;
private:
    void draw(QPainter *);
signals:

public slots:
};

#endif // LAMP_H
