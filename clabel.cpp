#include "clabel.h"

CLabel::CLabel(QWidget *parent): QLabel (parent)
{

}

CLabel::CLabel(const QString &name, QWidget *parent): QLabel (parent)
{
    this->setText(name);
}

void CLabel::mousePressEvent(QMouseEvent *e)
{
//    qDebug() << "Press: " << e->pos() << endl;
    first = e->pos();
}

void CLabel::mouseReleaseEvent(QMouseEvent *e)
{
//    qDebug() << "Release: " << e->pos() << endl;
    last = e->pos();
//    painter->drawRect(QRect(first, current));

}

void CLabel::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug() << "Moving: " << e->pos() << endl;
    current = e->pos();
    if(selectionmode){
        update();
    }
}

void CLabel::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(selectionmode && !playMode){
        painter.setPen(QColor(0, 0, 255));
        painter.drawPixmap(QRect(0, 0, map.width(), map.height()), map, QRect(0, 0, map.width(), map.height()));
        painter.drawRect(QRect(first, current));
    }else if(! selectionmode && !playMode){
        painter.eraseRect(QRect(first, last));
        painter.drawPixmap(QRect(0, 0, map.width(), map.height()), map, QRect(0, 0, map.width(), map.height()));
    }else {
        painter.drawPixmap(QRect(0, 0, map.width(), map.height()), map, QRect(0, 0, map.width(), map.height()));
    }
}

void CLabel::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "Pressed: " << e->key() << endl;
}

void CLabel::clear(QPainter *p)
{
    p->eraseRect(QRect(first, last));
}

void CLabel::setSelectionMode(bool s)
{
    selectionmode = s;
}

void CLabel::setPlayMode(bool s)
{
    playMode = s;
}

bool CLabel::selected() const
{
    if(selectionmode){
        return true;
    }else {
        return false;
    }
}

bool CLabel::paused() const
{
    if(playMode == false){
        return true;
    }else{
        return false;
    }
}

void CLabel::setPixelMap(const QPixmap &map)
{
    this->map = map;
}

QPixmap CLabel::Roi()
{
    return map.copy(QRect(first, last));
}





