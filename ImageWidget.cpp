#include "ImageWidget.h"
#include <QDebug>
#include<QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <QString>
#include <math.h>
double changeNum1=2.5;
double changeNum2=5;

StarItem::StarItem(const QPointF& position,const QColor& color, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_position(position),m_color(color)
{
}
QRectF StarItem::boundingRect() const
{
    return QRectF(-10, -10, 20, 20); // 调整边界矩形的大小
}


ImageWidget::ImageWidget(QPixmap *pixmap)
{
    m_pix = *pixmap;
    //If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    setAcceptDrops(true);//true接收悬停事件
    m_scaleValue = 0;
    m_scaleDafault = 0;
    m_pre_scaleValue=m_scaleValue;
    m_isMove = false;
}
void StarItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPolygonF starPolygon;
    const int numberOfPoints = 5;
    const qreal outerRadius = 10;
    const qreal innerRadius = 4;

    for (int i = 0; i < numberOfPoints * 2; ++i)
    {
        qreal radius = (i % 2 == 0) ? outerRadius : innerRadius;
        qreal angle = 2 * M_PI * i / (numberOfPoints * 2);
        starPolygon << QPointF(radius * sin(angle), -radius * cos(angle)); // 绘制五角星
    }

    painter->setBrush(m_color); // 设置五角星的颜色
    painter->drawPolygon(starPolygon); // 绘制五角星
}



void ImageWidget::getTextLabel(QLabel* l1,QLabel* l2,QLabel* l3)
{
    this->label_now=l1;
    this->label_start=l2;
    this->label_end=l3;
}
void ImageWidget::getTextLabel2(QLabel* l4,QLabel* l5)
{
    this->label_dist=l4;
    this->label_time=l5;
}
QRectF ImageWidget::boundingRect() const
{
    return QRectF(-m_pix.width()/2 , -m_pix.height()/2 ,
                  m_pix.width(), m_pix.height());
}

bool ImageWidget::is_scale_change()
{
    if(m_pre_scaleValue<changeNum1&&m_scaleValue>=changeNum1)
        return true;
    else if(m_pre_scaleValue<changeNum2&&m_scaleValue>=changeNum2)
        return true;
    else if(m_pre_scaleValue>=changeNum1&&m_scaleValue<changeNum1)
        return true;
    else if(m_pre_scaleValue>=changeNum2&&m_scaleValue<changeNum2)
        return true;
    else
        return false;
}
void ImageWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                        QWidget *)
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
}

void ImageWidget::setStartPoint(const QPointF& startPoint)
{
    // 删除之前的五角星对象（如果存在）
    if (m_starItem)
    {
        if(!path_in_graph.empty())
        {
            path_in_graph.clear();
            drawMap();
        }
        delete m_starItem;
        m_starItem = nullptr;
    }
    QPointF globalPos = mapToScene(startPoint);
    // 创建新的五角星对象，并设置位置为局部坐标系中的坐标
    m_starItem = new StarItem(globalPos,QColor(251, 79, 20));

    // 将五角星对象添加到图形场景中，并在指定位置绘制五角星
    m_starItem->setPos(globalPos);

    scene()->addItem(m_starItem);

    // 刷新图形场景以显示新添加的五角星
    update();
}
void ImageWidget::setEndPoint(const QPointF& endPoint)
{
    // 删除之前的五角星对象（如果存在）
    if (e_starItem)
    {
        if(!path_in_graph.empty())
        {
            path_in_graph.clear();
            drawMap();
        }
        delete e_starItem;
        e_starItem = nullptr;
    }
    QPointF globalPos = mapToScene(endPoint);
    // 创建新的五角星对象，并设置位置为局部坐标系中的坐标
    e_starItem = new StarItem(globalPos,Qt::blue);

    // 将五角星对象添加到图形场景中，并在指定位置绘制五角星
    e_starItem->setPos(globalPos);

    scene()->addItem(e_starItem);

    // 刷新图形场景以显示新添加的五角星
    update();
}
void ImageWidget::setNowPoint(const QPointF& nowPoint)
{
    if (n_starItem)
    {
        if(!top100_in_graph.empty())
        {
            top100_in_graph.clear();
            drawMap();
        }
        delete n_starItem;
        n_starItem = nullptr;
    }
    QPointF globalPos = mapToScene(nowPoint);
    // 创建新的五角星对象，并设置位置为局部坐标系中的坐标
    n_starItem = new StarItem(globalPos,Qt::green);

    // 将五角星对象添加到图形场景中，并在指定位置绘制五角星
    n_starItem->setPos(globalPos);

    scene()->addItem(n_starItem);

    // 刷新图形场景以显示新添加的五角星
    update();
}
void ImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        QPointF mouse_pos=event->pos();
        m_isMove = true;//标记鼠标左键被按下
        if(event->modifiers()==Qt::ShiftModifier)
        {
            QString coordString = "(" + QString::number(mouse_pos.x()) + "," + QString::number(mouse_pos.y()) + ")";
            QPointF nowPoint = parseStringToPoint(coordString);
            setNowPoint(nowPoint);
            label_now->setText("("+QString::number(mouse_pos.x()+500)+","+QString::number(mouse_pos.y()+500)+")");
        }
        if(choose_start)
        {
            QString coordString = "(" + QString::number(mouse_pos.x()) + "," + QString::number(mouse_pos.y()) + ")";
            QPointF startPoint = parseStringToPoint(coordString);
            setStartPoint(startPoint);
            label_start->setText("("+QString::number(mouse_pos.x()+500)+","+QString::number(mouse_pos.y()+500)+")");
            choose_start=false;
        }

        else if(choose_end)
        {
            QString coordString = "(" + QString::number(mouse_pos.x()) + "," + QString::number(mouse_pos.y()) + ")";
            QPointF endPoint = parseStringToPoint(coordString);
            setEndPoint(endPoint);
            label_end->setText("("+QString::number(mouse_pos.x()+500)+","+QString::number(mouse_pos.y()+500)+")");
            choose_end=false;
        }

    }
    else if(event->button() == Qt::RightButton)
    {
        ResetItemPos();//右击鼠标重置大小
    }
}

void ImageWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 如果未按下鼠标左键，执行其他操作（如检查地图位置是否超出边界）
    qreal x = 2 * m_pix.width();
    qreal y = 2 * m_pix.height();
    if (pos().x() <= (-x))
    {
        setPos(qreal(-x + 1), pos().y());
        m_isMove = false;
    }
    if (pos().x() >= x)
    {
        setPos(QPointF(x - 1, pos().y()));
        m_isMove = false;
    }
    if (pos().y() <= (-y))
    {
        setPos(pos().x(), qreal(-y + 1));
        m_isMove = false;
    }
    if (pos().y() >= y)
    {
        setPos(pos().x(), qreal(y - 1));
        m_isMove = false;
    }
    if(m_isMove)
    {
        QPointF point1 = (event->pos() - m_startPos)*m_scaleValue;
        moveBy(point1.x(), point1.y());
        if(m_starItem){
            QPointF starLocalPos1 = m_starItem->pos();
            QPointF newStarPos = starLocalPos1 + point1;
            m_starItem->setPos(newStarPos);
        }
        if(n_starItem){
            QPointF starLocalPos2 = n_starItem->pos();
            QPointF newNowPos = starLocalPos2 + point1;
            n_starItem->setPos(newNowPos);
        }
        if(e_starItem){
            QPointF starLocalPos3 = e_starItem->pos();
            QPointF newEndPos = starLocalPos3 + point1;
            e_starItem->setPos(newEndPos);
        }
    }
}

void ImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isMove = false;//标记鼠标左键已经抬起
}


void ImageWidget::wheelEvent(QGraphicsSceneWheelEvent *event)//鼠标滚轮事件
{
    //选点缩放=缩放+移动
    QPointF mousePos = mapToScene(event->pos());

    if((event->delta() > 0)&&(m_scaleValue >= 2*changeNum2))//最大放大到原始图像的10倍
    {
        return;
    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDafault))//图像缩小到自适应大小之后就不继续缩小
    {
        //ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
    }
    else
    {
        //①缩放
        qreal qrealOriginScale = m_scaleValue;
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            m_pre_scaleValue=m_scaleValue;
            m_scaleValue*=1.1;//每次放大10%
            if(is_scale_change())
            {
                drawMap();
            }
        }
        else
        {
            m_pre_scaleValue=m_scaleValue;
            m_scaleValue*=0.9;//每次缩小10%
            if(is_scale_change())
            {
                drawMap();
            }
        }
        setScale(m_scaleValue);
        qreal scaleRatio = m_scaleValue / qrealOriginScale;
        // 计算五角星的局部坐标
        if(m_starItem){
            QPointF starLocalPos1 = m_starItem->pos();
            QPointF newStarPos = mousePos + (starLocalPos1 - mousePos) * scaleRatio;
            m_starItem->setPos(newStarPos);
        }
        if(n_starItem){
            QPointF starLocalPos2 = n_starItem->pos();
            QPointF newNowPos = mousePos + (starLocalPos2 - mousePos) * scaleRatio;
            n_starItem->setPos(newNowPos);
        }
        if(e_starItem){
            QPointF starLocalPos3 = e_starItem->pos();
            // 根据缩放比例调整五角星的坐标，使其相对于图像固定位置
            QPointF newEndPos = mousePos + (starLocalPos3 - mousePos) * scaleRatio;
            e_starItem->setPos(newEndPos);
        }
        //②移动
        if(event->delta() > 0)
        {
            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
        else
        {
            moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
        }
    }
}

void ImageWidget::setQGraphicsViewWH(int nwidth, int nheight)
//将主界面的控件QGraphicsView的width和height传进本类中，并根据图像的长宽和控件的长宽的比例来使图片缩放到适合控件的大小
{
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;
    if(temp1>temp2)
    {
        m_scaleDafault = temp2;
    }
    else
    {
        m_scaleDafault = temp1;
    }
    setScale(m_scaleDafault);
    m_scaleValue = m_scaleDafault;
    cout<<"scale:"<<m_scaleValue<<endl;
}

void ImageWidget::ResetItemPos()//重置图片位置
{
    m_scaleValue = m_scaleDafault;//缩放比例回到一开始的自适应比例
    setScale(m_scaleDafault);//缩放到一开始的自适应大小
    if (m_starItem)
    {
        delete m_starItem;
        m_starItem = nullptr;
    }
    if(e_starItem)
    {
        delete e_starItem;
        e_starItem=nullptr;
    }
    if(n_starItem)
    {
        delete n_starItem;
        n_starItem=nullptr;
    }
    if(!path_in_graph.empty())
        path_in_graph.clear();
    if(!remain_in_graph.empty())
        remain_in_graph.clear();
    if(!top100_in_graph.empty())
        top100_in_graph.clear();
    drawMap();
    setPos(0,0);

}

qreal ImageWidget::getScaleValue() const
{
    return m_scaleValue;
}
void ImageWidget::newMap()
{
    e.seed(time(0));
    g=new Graph(10000);
    drawMap();
    cout<<"successfully draw map"<<endl;
}
void ImageWidget::drawMap()
{
    if(m_scaleValue>changeNum2)
    {
        drawMap1();
    }
    else if(m_scaleValue>changeNum1)
    {
        drawMap2();
    }
    else
    {
        drawMap3();
    }
}
void ImageWidget::drawMapcolor()
{
    if(m_scaleValue>changeNum2)
    {
        drawMap11();
    }
    else if(m_scaleValue>changeNum1)
    {
        drawMap22();
    }
    else
    {
        drawMap33();
    }
}
void ImageWidget::drawMap1()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    //myPainter.drawPoint(100,120);
    for(int i = 0;i<10000;i++)
    {
        myPainter.drawPoint(g->points[i].x,g->points[i].y);
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);

    for(int i = 0;i<10000;i++)
    {
        for(g->edges[i].first();g->edges[i].fence!=nullptr;g->edges[i].next()){
            //qDebug()<<g.points[i].x<<" "<<g.points[i].y<<" "<<g.points[g.edges[i].fence->point].x<<" "<<g.points[g.edges[i].fence->point].x;
            myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edges[i].fence->point].x,g->points[g->edges[i].fence->point].y);
        }
    }
    if(!path_in_graph.empty())
    {
        //EdgeL* edges=map->edges;
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            qDebug()<<"drawing";
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    if(!top100_in_graph.empty())
    {

        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    if(!path_in_graph.empty())
    {
        //EdgeL* edges=map->edges;
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            qDebug()<<"drawing";
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    if(!remain_in_graph.empty())
    {

        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
        myPainter.drawLine(current_pos.first,current_pos.second,g->points[remain_in_graph[remain_in_graph.size() - 1]].x,g->points[remain_in_graph[remain_in_graph.size() - 1]].y);
    }
    update();
}

void ImageWidget::drawMap2()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<106;j++)
        {
            myPainter.drawPoint(g->points[g->secondeNo[i][j]].x,g->points[g->secondeNo[i][j]].y);
        }
    }
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<9;j++)
        {
            myPainter.drawPoint(g->points[g->higheNo[i][j]].x,g->points[g->higheNo[i][j]].y);
        }
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);
    for(int i = 0;i<10000;i++)
    {
        for(g->edges2[i].first();g->edges2[i].fence!=nullptr;g->edges2[i].next()){
            //qDebug()<<"now point number"<<i;
            //qDebug()<<g->points[i].x<<" "<<g->points[i].y<<" "<<g->points[g->edgesH[i].fence->point].x<<" "<<g->points[g->edgesH[i].fence->point].x;
            myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edges2[i].fence->point].x,g->points[g->edges2[i].fence->point].y);
        }
    }
    for(int i = 0;i<10000;i++)
    {
        for(g->edgesH[i].first();g->edgesH[i].fence!=nullptr;g->edgesH[i].next()){
            //qDebug()<<"now point number"<<i;
            //qDebug()<<g->points[i].x<<" "<<g->points[i].y<<" "<<g->points[g->edgesH[i].fence->point].x<<" "<<g->points[g->edgesH[i].fence->point].x;
            myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edgesH[i].fence->point].x,g->points[g->edgesH[i].fence->point].y);
        }
    }
    if(!path_in_graph.empty())
    {
        //EdgeL* edges=map->edges;
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            qDebug()<<"drawing";
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    if(!top100_in_graph.empty())
    {

        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    if(!remain_in_graph.empty())
    {

        myPainter.setPen(Qt::red);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
        myPainter.drawLine(current_pos.first,current_pos.second,g->points[remain_in_graph[remain_in_graph.size() - 1]].x,g->points[remain_in_graph[remain_in_graph.size() - 1]].y);
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    update();
}
void ImageWidget::drawMap3()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<9;j++)
        {
            myPainter.drawPoint(g->points[g->higheNo[i][j]].x,g->points[g->higheNo[i][j]].y);
        }
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);
    if(m_scaleValue<=5)
    {
        for(int i = 0;i<10000;i++)
        {
            for(g->edgesH[i].first();g->edgesH[i].fence!=nullptr;g->edgesH[i].next()){
                //qDebug()<<"now point number"<<i;
                //qDebug()<<g->points[i].x<<" "<<g->points[i].y<<" "<<g->points[g->edgesH[i].fence->point].x<<" "<<g->points[g->edgesH[i].fence->point].x;
                myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edgesH[i].fence->point].x,g->points[g->edgesH[i].fence->point].y);
            }
        }
    }

    if(!path_in_graph.empty())
    {
        //EdgeL* edges=map->edges;
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            qDebug()<<"drawing";
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    if(!top100_in_graph.empty())
    {

        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    if(!remain_in_graph.empty())
    {

        myPainter.setPen(Qt::red);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
        myPainter.drawLine(current_pos.first,current_pos.second,g->points[remain_in_graph[remain_in_graph.size() - 1]].x,g->points[remain_in_graph[remain_in_graph.size() - 1]].y);
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    update();
}
void ImageWidget::drawMap11()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    //myPainter.drawPoint(100,120);
    for(int i = 0;i<10000;i++)
    {
        myPainter.drawPoint(g->points[i].x,g->points[i].y);
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);

    for(int i = 0;i<10000;i++)
    {
        for(g->edges[i].first();g->edges[i].fence!=nullptr;g->edges[i].next()){
            if(g->edges[i].fence->ratio>0.8)
            {
                mypen.setColor(Qt::red);
                myPainter.setPen(mypen);
            }
            else if(g->edges[i].fence->ratio>0.5)
            {
                mypen.setColor(Qt::yellow);
                myPainter.setPen(mypen);
            }
            else
            {
                mypen.setColor(Qt::green);
                myPainter.setPen(mypen);
            }
            myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edges[i].fence->point].x,g->points[g->edges[i].fence->point].y);
        }
    }

    if(!top100_in_graph.empty())
    {
        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    if(!path_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    if(!remain_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[remain_in_graph[i-1]].y);
        }
    }
    update();
}

void ImageWidget::drawMap22()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    //二级道路点
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<106;j++)
        {
            myPainter.drawPoint(g->points[g->secondeNo[i][j]].x,g->points[g->secondeNo[i][j]].y);
        }
    }
    //三级道路点
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<9;j++)
        {
            myPainter.drawPoint(g->points[g->higheNo[i][j]].x,g->points[g->higheNo[i][j]].y);
        }
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);
    for(int i = 0;i<10000;i++)
    {
        for(g->edges[i].first();g->edges[i].fence!=nullptr;g->edges[i].next()){
            if(g->edges[i].fence->highWay>1)
            {
                if(g->edges[i].fence->ratio>0.8)
                {
                    mypen.setColor(Qt::red);
                    myPainter.setPen(mypen);
                }
                else if(g->edges[i].fence->ratio>0.5)
                {
                    mypen.setColor(Qt::yellow);
                    myPainter.setPen(mypen);
                }
                else
                {
                    mypen.setColor(Qt::green);
                    myPainter.setPen(mypen);
                }
                myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edges[i].fence->point].x,g->points[g->edges[i].fence->point].y);
            }
        }
    }
    if(!path_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    if(!top100_in_graph.empty())
    {

        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    if(!remain_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[remain_in_graph[i-1]].y);
        }
    }
    update();
}
void ImageWidget::drawMap33()
{
    m_pix.fill(Qt::white);
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::black);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::black);
    myPainter.setPen(mypen);
    for(int i=0;i<25;i++)
    {
        for(int j=0;j<9;j++)
        {
            myPainter.drawPoint(g->points[g->higheNo[i][j]].x,g->points[g->higheNo[i][j]].y);
        }
    }
    mypen.setWidth(1);
    myPainter.setPen(mypen);
    if(m_scaleValue<=5)
    {
        for(int i = 0;i<10000;i++)
        {
            for(g->edges[i].first();g->edges[i].fence!=nullptr;g->edges[i].next()){
                if(g->edges[i].fence->highWay==3)
                {
                    if(g->edges[i].fence->ratio>0.8)
                    {
                        mypen.setColor(Qt::red);
                        myPainter.setPen(mypen);
                    }
                    else if(g->edges[i].fence->ratio>0.5)
                    {
                        mypen.setColor(Qt::yellow);
                        myPainter.setPen(mypen);
                    }
                    else
                    {
                        mypen.setColor(Qt::green);
                        myPainter.setPen(mypen);
                    }
                    myPainter.drawLine(g->points[i].x,g->points[i].y,g->points[g->edges[i].fence->point].x,g->points[g->edges[i].fence->point].y);
                }
            }
        }
    }
    if(!path_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = path_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[path_in_graph[i]].x,g->points[path_in_graph[i]].y,g->points[path_in_graph[i-1]].x,g->points[path_in_graph[i-1]].y);
        }
    }
    if(!top100_in_graph.empty())
    {

        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(3);
        mypen.setColor(Qt::gray);
        for(int i=0;i<100;i++)
        {
            myPainter.drawPoint(g->points[top100_in_graph[i]].x,g->points[top100_in_graph[i]].y);
        }
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        myPainter.setPen(Qt::gray);
        QPen mypen;
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = g->edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(g->points[npos].x,g->points[npos].y,g->points[search->point].x,g->points[search->point].y);
            search = search->next;
        }
    }
    if(!remain_in_graph.empty())
    {
        myPainter.setPen(Qt::blue);
        QPen mypen;
        mypen.setWidth(2);
        mypen.setColor(Qt::blue);
        myPainter.setPen(mypen);
        for (int i = remain_in_graph.size() - 1; i > 0; i--)
        {
            myPainter.drawLine(g->points[remain_in_graph[i]].x,g->points[remain_in_graph[i]].y,g->points[remain_in_graph[i-1]].x,g->points[remain_in_graph[i-1]].y);
        }
    }
    update();
}


void ImageWidget::eraseWay(bool choice)//0擦除点，1擦除路径
{
    {
        if(choice)
        {
            if(!path_in_graph.empty())
                path_in_graph.clear();
            if(!remain_in_graph.empty())
                remain_in_graph.clear();
        }
        else
        {
            if(!top100_in_graph.empty())
                top100_in_graph.clear();
        }
    }
    drawMap();
}

void ImageWidget::drawWay(vector<int> path)
{
    PointRoad* points=g->points;
    EdgeL* edges=g->edges;
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::blue);
    QPen mypen;
    mypen.setWidth(2);
    mypen.setColor(Qt::blue);
    myPainter.setPen(mypen);
    for (int i = path.size() - 1; i > 0; i--)
    {
        myPainter.drawLine(points[path[i]].x,points[path[i]].y,points[path[i-1]].x,points[path[i-1]].y);
    }
    double dist_sum = 0;
    double time_cost=0;
    for (int i = path.size() - 1; i > 0; i--)
    {
        int npos = path[i];
        EdgeL ne = edges[npos];
        Edge* search = ne.head;
        while (search)
        {
            if (search->point == path[i - 1])
            {
                dist_sum = dist_sum + search->distance;
                time_cost=time_cost+search->pass_time;
                break;
            }
            else
                search = search->next;
        }
    }
    cout << "total dist:" << dist_sum << "km"<<endl;
    cout << "total time:" << time_cost / 3600 << "h" << endl;
    label_dist->setText("距离:"+QString::number(dist_sum)+"km");
    label_time->setText("预计用时:"+QString::number(time_cost/3600)+"h");
    cout << endl;
    if(!path_in_graph.empty())
        path_in_graph.clear();
    for(int i=0;i<path.size();i++)
    {
        path_in_graph.push_back(path[i]);
    }
    update();

}
int ImageWidget::getNearest(double x,double y)
{
    KNTree tree(g->points);
    NoDist* topPoint = tree.getTopOne(x,y);
    return topPoint->no;
}
void ImageWidget::drawNear100(double x,double y)
{
    KNTree tree(g->points);
    NoDist* topPoint = tree.gettop100(x,y);
    PointRoad* points=g->points;
    EdgeL* edges=g->edges;
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::gray);
    QPen mypen;
    mypen.setWidth(3);
    mypen.setColor(Qt::gray);
    myPainter.setPen(mypen);
    for(int i=0;i<100;i++)
    {
        myPainter.drawPoint(points[topPoint[i].no].x,points[topPoint[i].no].y);
    }
    if(!top100_in_graph.empty())
    {
        top100_in_graph.clear();
    }
    for (int i = 0; i < 100; i++)
    {
        top100_in_graph.push_back(topPoint[i].no);
    }
    for (int i = 0;i<top100_in_graph.size();i++)
    {
        mypen.setWidth(1);
        mypen.setColor(Qt::gray);
        myPainter.setPen(mypen);
        int npos = top100_in_graph[i];
        EdgeL ne = edges[npos];
        Edge* search = ne.head->next;
        while (search)
        {
            myPainter.drawLine(points[npos].x,points[npos].y,points[search->point].x,points[search->point].y);
            search = search->next;
        }
    }
    update();
}

void ImageWidget::setStart(bool choice)
{
    choose_start=choice;
}
void ImageWidget::setEnd(bool choice)
{
    choose_end=choice;
}
QPointF ImageWidget::parseStringToPoint(const QString& coordString)
{
    QStringList coordList = coordString.mid(1, coordString.length() - 2).split(','); // 去除括号并分割字符串

    if (coordList.size() == 2) // 确保坐标字符串包含两个值
    {
        bool xOk, yOk;
        qreal x = coordList[0].trimmed().toDouble(&xOk); // 转换x坐标值为浮点数
        qreal y = coordList[1].trimmed().toDouble(&yOk); // 转换y坐标值为浮点数

        if (xOk && yOk) // 确保转换成功
        {
            return QPointF(x, y); // 返回QPointF对象表示转换后的坐标值
        }
    }

    // 如果无法解析或转换坐标，返回原点作为默认值
    return QPointF(0, 0);
}

ImageWidget::~ImageWidget() {
    delete m_starItem;
    delete n_starItem;
    delete e_starItem;
}

void ImageWidget::drawWayRe(vector<int>& path,double x,double y)
{
    PointRoad* points=g->points;
    QPainter myPainter(&m_pix);
    myPainter.setPen(Qt::green);
    QPen mypen;
    mypen.setWidth(2);
    mypen.setColor(Qt::green);
    myPainter.setPen(mypen);
    for (int i = path.size() - 1; i > 0; i--)
    {
        myPainter.drawLine(points[path[i]].x,points[path[i]].y,points[path[i-1]].x,points[path[i-1]].y);
    }
    qDebug()<<"path[max-1]:"<<path[path.size()-1];
    myPainter.drawLine(x,y,points[path[0]].x,points[path[0]].y);
    if(!remain_in_graph.empty())
        remain_in_graph.clear();//清空原来的
    for(int i = path.size()-1;i>0;i--)
    {
        remain_in_graph.push_back(path[i]);
    }
    current_pos.first=x;
    current_pos.second=y;
    update();
}

void ImageWidget::newRoad()
{
    e.seed(time(nullptr));
    for(int i=0;i<g->pnum;i++)
    {
        EdgeL* edges=g->edges;
        EdgeL ne = edges[i];
        Edge* search = ne.head->next;
        while(search)
        {
            search->calpass_time(search->highWay);
            search=search->next;
        }
    }
    for(int i=0;i<g->pnum;i++)
    {
        EdgeL* edges=g->edges2;
        EdgeL ne = edges[i];
        Edge* search = ne.head->next;
        while(search)
        {
            search->calpass_time(search->highWay);
            search=search->next;
        }
    }
    for(int i=0;i<g->pnum;i++)
    {
        EdgeL* edges=g->edgesH;
        EdgeL ne = edges[i];
        Edge* search = ne.head->next;
        while(search)
        {
            search->calpass_time(search->highWay);
            search=search->next;
        }
    }
    drawMapcolor();
}
Graph* ImageWidget::getGraph()
{
    return g;
}
