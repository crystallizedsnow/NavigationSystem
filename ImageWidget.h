
#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>
#include <vector>
#include  <utility>
#include <QPainter>
#include "PointEdge.h"
#include "KNTree.h"

#include <QGraphicsSceneMouseEvent>
#include <QPolygonF>
enum Enum_ZoomState{
    NO_STATE,
    RESET,
    ZOOM_IN,
    ZOOM_OUT
};

class StarItem : public QGraphicsItem
{
public:
    StarItem(const QPointF& position, const QColor& color,QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QPointF m_position;
    QColor m_color;
};

// class ImageWidget :public QObject, QGraphicsItem
class ImageWidget :public QGraphicsItem
{
    //Q_OBJECT
public:
    ImageWidget(QPixmap *pixmap);
    ~ImageWidget();
    QRectF  boundingRect() const;//边界设置
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    wheelEvent(QGraphicsSceneWheelEvent *event);//鼠标滚轮
    //QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    qreal   getScaleValue() const;
    void    ResetItemPos();//重置大小
    void    setQGraphicsViewWH(int nwidth,int nheight);
    void    getTextLabel(QLabel* l1,QLabel* l2,QLabel* l3);
    void   getTextLabel2(QLabel* l4,QLabel* l5);
    void    newMap();
    void    drawMap();
    void    drawMap1();
    void    drawMap2();
    void    drawMap3();
    void    drawMap11();
    void    drawMap22();
    void    drawMap33();
    void    drawWay(vector<int> path);
    void    drawWayRe(vector<int>& path,double x,double y);
    void    eraseWay(bool choice);
    void    recover_map();
    int    getNearest(double x,double y);
    void    drawNear100(double x,double y);
    void    setStart(bool choice);
    void    setEnd(bool choince);
    bool    is_scale_change();
    void    setStartPoint(const QPointF& startPoint);
    void    setNowPoint(const QPointF& nowPoint);
    void    setEndPoint(const QPointF& endPoint);
    void    newRoad();//刷新路况
    QPointF   parseStringToPoint(const QString& coordString);
    Graph*    getGraph();
    void drawMapcolor();
private:
    qreal       m_scaleValue;//图片大小
    qreal       m_scaleDafault;//默认图片大小
    qreal       m_pre_scaleValue;//上次的大小
    QPixmap     m_pix;//加载的图片
    Graph*      g;//地图
    int         m_zoomState;//状态
    bool        scale_change;//图片图层是否变化
    bool        m_isMove;//图片是否可以移动
    bool        choose_start;//是否要选择起点
    bool        choose_end;//是否要选择终点
    QPointF     m_startPos;//图片起点
    QLabel*     label_now;
    QLabel*     label_start;
    QLabel*     label_end;
    QLabel*     label_dist;
    QLabel*     label_time;//显示内容的几个标签
    QPainter*   myPainter;//画笔
    vector<int> path_in_graph;//图片中存储的路径
    vector<int> top100_in_graph;//图片中最近的100个点
    vector<int> remain_in_graph;//剩余的路径
    pair<int,int>current_pos;//现在的位置

    StarItem *m_starItem = nullptr;
    StarItem *n_starItem = nullptr;
    StarItem *e_starItem = nullptr;//位置标记图案
    int signal=0;

};
#endif // IMAGEWIDGET_H
