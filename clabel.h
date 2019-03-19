#ifndef CLABEL_H
#define CLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>


typedef struct {
    int x, y, width, height;
} CRect;

class CLabel: public QLabel
{
//    QPainter painter;
    Q_OBJECT
    QPoint first, current, last;
    bool selectionmode = false;
    bool playMode = true;
    QPixmap map;
//    QPixmap crop;
public:
    CLabel(QWidget* parent);
    CLabel(const QString& name, QWidget* parent);
    void mousePressEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void paintEvent(QPaintEvent* e);
    void keyPressEvent(QKeyEvent* e);
    void clear(QPainter* p);
    void setSelectionMode(bool s = true);
    void setPlayMode(bool s = false);
    bool selected() const;
    bool paused() const;
    void setPixelMap(const QPixmap& map);
    QPixmap Roi();
};

#endif // CLABEL_H
