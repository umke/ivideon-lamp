#include "lamp.h"
#include <QDebug>
#include <QPushButton>
#include "datatype.h"

Lamp::Lamp(QWidget *parent) : QWidget(parent)
                            , m_color(0, 0, 0)
                            , m_pen(Qt::SolidLine)
                            , m_brash(Qt::HorPattern)
                            , m_state(LampOFF)
                            , m_commandType(READY)
                            ,m_lableStatus("OFF", this)
{
    setMinimumSize(400, 400);
    m_lableStatus.setAlignment(Qt::AlignHCenter);
    m_font.setPointSize(14);
    m_lableStatus.setFont(m_font);
    m_lableStatus.setMinimumSize(100, 25);

}


void Lamp::setCommand(const CommandData &data)
{
    m_commandType = static_cast<CommandType>(data.type);
    switch (m_commandType) {
    case ON:
        m_lableStatus.setText("ON");
        m_brash.setStyle(Qt::VerPattern);
        m_state = LampON;
        repaint();
        break;
    case OFF:
        m_lableStatus.setText("OFF");
        m_color.setRgb(Qt::black);
        m_brash.setStyle(Qt::HorPattern);
        m_state = LampOFF;
        repaint();
        break;
    case COLOR:
        if (m_state == LampON) {
            m_brash.setStyle(Qt::SolidPattern);
            if (data.value.size() >= 3)
                m_color.setRgb(data.value.at(0), data.value.at(1), data.value.at(2));
            repaint();
        }
        break;
    default:
        qDebug() << "Unknown command from host!";
        break;

    }

}

void Lamp::draw(QPainter *painter)
{
    m_pen.setWidth(4);
    m_pen.setColor(m_color);
    painter->setPen(m_pen);
    m_brash.setColor(m_color);
    painter->setBrush(m_brash);
    painter->drawEllipse(QPoint(width()/2, height()/2), 150, 150);

}

void Lamp::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    draw(&p);

}

void Lamp::clearState()
{
    m_lableStatus.setText("OFF");
    m_color.setRgb(Qt::black);
    m_brash.setStyle(Qt::HorPattern);
    m_state = LampOFF;
    repaint();
}
