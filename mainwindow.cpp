
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("简易导航系统");//设置窗口名
    /* ui->pushButton:刷新地图的按钮
     * ui->pushButton_2 展示临近100个点的按钮
     * ui->pushButton_3 开始导航的按钮
     * ui->pushButton_4 设置起点的按钮
     * ui->pushButton_5 设置终点的按钮
     * ui->label   “当前位置”
     * ui->label_2 显示当前位置的坐标
     * ui->label_3 显示起点坐标
     * ui->label_4 显示终点坐标
    */
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::mapRefresh);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::showNear100);

    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::setStart);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::setEnd);
    // 创建定时器对象
    QTimer* timer = new QTimer(this);

    // 修改定时器对象的精度
    timer->setTimerType(Qt::PreciseTimer);

    // 按钮 ui->pushButton_6的点击事件
    // 点击按钮启动或者关闭定时器, 定时器启动, 周期性得到当前时间
    connect(ui->pushButton_6, &QPushButton::clicked, this, [=]()
    {
        // 启动定时器
        if(timer->isActive())
        {
            timer->stop();  // 关闭定时器
            m_Image->drawMap();//恢复黑色
            ui->pushButton_6->setText("开始查询车流量");
        }
        else
        {
          ui->pushButton_6->setText("停止查询车流量");
          roadRefresh();
           timer->start(1000); // 1000ms == 1s
        }
    });
    connect(timer, &QTimer::timeout, this, &MainWindow::roadRefresh);
//    connect(ui->pushButton_6,&QPushButton::clicked,this,&MainWindow::roadRefresh);
    ui->subButton1->hide();
    ui->subButton2->hide();
    ui->subButton3->hide();
    ui->subButton5->hide();
    ui->pushButton_6->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->label_7->hide();
    connect(ui->pushButton_3, &QPushButton::clicked, [=](){
        ui->subButton1->setVisible(!ui->subButton1->isVisible());
        ui->subButton2->setVisible(!ui->subButton2->isVisible());
        ui->subButton3->setVisible(!ui->subButton3->isVisible());
        ui->subButton5->setVisible(!ui->subButton5->isVisible());
        ui->label_5->setVisible(!ui->label_5->isVisible());
        ui->label_6->setVisible(!ui->label_6->isVisible());
        ui->label_7->setVisible(!ui->label_7->isVisible());
       });
    //connect(ui->subButton1,&QPushButton::clicked,this,&MainWindow::nevigation);
    connect(ui->subButton1,&QPushButton::clicked,this,std::bind(&MainWindow::nevigation, this,1));
    connect(ui->subButton2,&QPushButton::clicked,this,std::bind(&MainWindow::nevigation, this,2));
    connect(ui->subButton3,&QPushButton::clicked,this,std::bind(&MainWindow::nevigation, this,3));
    connect(ui->subButton5,&QPushButton::clicked,this,&MainWindow::findRemainRoad);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void showError(const QString &errorMessage) {
    QMessageBox::critical(nullptr, "Error", errorMessage, QMessageBox::Ok);
}

void MainWindow::nevigation(int wayChoice)
{
    //读取标签中的值，然后找到与该坐标最近的一个点
    Graph* map=m_Image->getGraph();
    QString text_st=ui->label_3->text();
    QString text_ed=ui->label_4->text();
    double x1=0,y1=0,x2=0,y2=0;
    int start = text_st.indexOf("(") + 1; // 查找左括号的索引并加1得到开始位置
    int end = text_st.indexOf(")"); // 查找右括号的索引
    QStringList parts = text_st.mid(start, end - start).split(","); // 提取括号内的部分，并按逗号分割为x和y
    if (parts.size() == 2) {
        QString px1 = parts.at(0);
        QString py1 = parts.at(1);
        x1=px1.toDouble();
        y1=py1.toDouble();
    }
    start = text_ed.indexOf("(") + 1; // 查找左括号的索引并加1得到开始位置
    end = text_ed.indexOf(")"); // 查找右括号的索引
    QStringList parts2 = text_ed.mid(start, end - start).split(","); // 提取括号内的部分，并按逗号分割为x和y
    if (parts2.size() == 2) {
        QString px2 = parts2.at(0);
        QString py2 = parts2.at(1);
        x2=px2.toDouble();
        y2=py2.toDouble();
    }
    if(parts.size() != 2||parts2.size() != 2)
    {
        QString errorMessage = "输入出错，无法开始导航";
        showError(errorMessage);
    }
    else
    {
        qDebug()<<x1<<"  "<<y1;
        qDebug()<<x2<<"  "<<y2;
        m_Image->eraseWay(1);
        int st=m_Image->getNearest(x1,y1);
        int ed=m_Image->getNearest(x2,y2);
        qDebug()<<"st:"<<st<<"  ed:"<<ed;
        vector<int> path;
        switch (wayChoice)
        {
        case 1:
            path=map->dijkstra(st,ed);
            break;
        case 2:
            path=map->astar(st,ed);
            break;
        case 3:
            path=map->dijkstra_time(st,ed);//计算时间的函数
            break;
        case 4:
            path=map->astar(st,ed);//计算时间的函数
            break;
        default:
            break;
        }
        m_Image->drawWay(path);

    }
    m_Image->setStart(0);
    m_Image->setEnd(0);
}

void MainWindow::mapRefresh()
{
    ui->pushButton_6->setVisible(1);
    qDebug()<<"map initialed";
    refresh();
    m_Image->newMap();
    m_Image->setStart(0);
    m_Image->setEnd(0);
    ui->label_2->setText("按住shit键并用鼠标左键点击");
    ui->label_3->setText("起点坐标");
    ui->label_4->setText("终点坐标");
    ui->label_5->setText("路程");
    ui->label_6->setText("预计用时");
    ui->label_7->setText("请按住shift键选择你当前到达的位置");
}

void MainWindow::refresh()
{
    QPixmap ConvertPixmap(1000,1000);
    ConvertPixmap.fill(Qt::white);
    int width = ui->ImageGraphic->width();//获取界面控件Graphics View的宽度
    int height = ui->ImageGraphic->height();//获取界面控件Graphics View的高度
    QGraphicsScene  *qgraphicsScene = new QGraphicsScene;//创建场景
    qgraphicsScene->setSceneRect(QRectF(-width/2,-height/2,width,height));//场景中央坐标为（0,0）
    qDebug()<<"width="<<width<<" heigth="<<height<<" size="<< ui->ImageGraphic->size();

    m_Image = new ImageWidget(&ConvertPixmap);
    m_Image->getTextLabel(ui->label_2,ui->label_3,ui->label_4);
    m_Image->getTextLabel2(ui->label_5,ui->label_6);
    //将界面控件Graphics View的width和height传进类m_Image中
    m_Image->setQGraphicsViewWH(width,height);

    //将QGraphicsItem类对象放进QGraphicsScene中
    qgraphicsScene->addItem(m_Image);

    ui->ImageGraphic->fitInView(m_Image,Qt::KeepAspectRatio);

    //ui->ImageGraphic->setSceneRect(QRectF(-(width/2),-(height/2),width,height));//使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大），防止图片放大后重新缩小的时候视窗太大而不方便观察图片
    //ui->ImageGraphic->setMinimumSize(width,height);
    ui->ImageGraphic->setScene(qgraphicsScene);
    ui->ImageGraphic->setFocus();//将界面的焦点设置到当前Graphics View控件
    ui->ImageGraphic->ensureVisible(m_Image);
    ui->ImageGraphic->show();
}

void MainWindow::showNear100()
{
    QString text_st=ui->label_2->text();
    double x=0,y=0;
    int start = text_st.indexOf("(") + 1; // 查找左括号的索引并加1得到开始位置
    int end = text_st.indexOf(")"); // 查找右括号的索引
    QStringList parts = text_st.mid(start, end - start).split(","); // 提取括号内的部分，并按逗号分割为x和y
    if (parts.size() == 2) {
        m_Image->eraseWay(0);
        QString px = parts.at(0);
        QString py = parts.at(1);
        x=px.toDouble();
        y=py.toDouble();
        //获取当前位置
        m_Image->drawNear100(x,y);
    }
    else
    {
        QString errorMessage = "无法获得当前位置！";
        showError(errorMessage);
    }
}

void MainWindow::setStart()
{
    m_Image->setStart(1);
    m_Image->setEnd(0);
}

void MainWindow::setEnd()
{
    m_Image->setStart(0);
    m_Image->setEnd(1);
}

void MainWindow::navigate1(double x1,double y1)
{

    //读取标签中的值，然后找到与该坐标最近的一个点
    Graph* map=m_Image->getGraph();
    QString text_ed=ui->label_4->text();
    double x2=0,y2=0;
    int start = text_ed.indexOf("(") + 1; // 查找左括号的索引并加1得到开始位置
    int end = text_ed.indexOf(")"); // 查找右括号的索引
    QStringList parts2 = text_ed.mid(start, end - start).split(","); // 提取括号内的部分，并按逗号分割为x和y
    if (parts2.size() == 2) {
        QString px2 = parts2.at(0);
        QString py2 = parts2.at(1);
        x2=px2.toDouble();
        y2=py2.toDouble();
    }
    if(parts2.size() != 2)
    {
        QString errorMessage = "输入出错，无法开始导航";
        showError(errorMessage);
    }
    else
    {
        qDebug()<<x1<<"  "<<y1;
        qDebug()<<x2<<"  "<<y2;
        m_Image->eraseWay(1);
        int st=m_Image->getNearest(x1,y1);
        int ed=m_Image->getNearest(x2,y2);
        qDebug()<<"st:"<<st<<"  ed:"<<ed;
        vector<int> path;
        path=map->dijkstra(st,ed);
        m_Image->drawWay(path);
    }

}
void MainWindow::findRemainRoad()
{
    Graph* map=m_Image->getGraph();
    //获取当前位置(shift+点击)
    QString text_st=ui->label_2->text();
    double x=0,y=0;
    int start = text_st.indexOf("(") + 1; // 查找左括号的索引并加1得到开始位置
    int end = text_st.indexOf(")"); // 查找右括号的索引
    QStringList parts = text_st.mid(start, end - start).split(","); // 提取括号内的部分，并按逗号分割为x和y
    if (parts.size() == 2) {
        QString px = parts.at(0);
        QString py = parts.at(1);
        x=px.toDouble();
        y=py.toDouble();
    }
    else
    {
        QString errorMessage = "无法获得当前位置！";
        showError(errorMessage);
        return;
    }
//   m_Image->eraseWay(1);
   vector<int>remainRoadSet;//剩余路集合
   int direction=0;//下一条路方向
   double totalDis=0;//总共剩余距离
   double remainDis=0;//这条路还剩距离
   int wrongtype=0;//0没错，1没导航，2选点距离路径太远
   remainRoadSet=map->remainRoad(x,y,direction,totalDis,remainDis,wrongtype);
   if(wrongtype==1)
   {
       showError("请先开始导航再计算剩余距离");
       return;
   }
   else if(wrongtype==2)
   {
       showError("您已偏航，正在为您采用dijkstra最短路径方式重新导航");
       navigate1(x,y);
       return;
   }
   m_Image->drawWayRe(remainRoadSet,x,y);

   switch(direction)
   {
       case 1 :{  ui->label_7->setText("还剩"+QString::number(remainDis)+"千米走完这条路\n路口转向北行\n总共还剩"+QString::number(totalDis)+"千米到达终点");break;}
       case 2 :{  ui->label_7->setText("还剩"+QString::number(remainDis)+"千米走完这条路\n路口转向南行\n总共还剩"+QString::number(totalDis)+"千米到达终点");break;}
       case 3 :{  ui->label_7->setText("还剩"+QString::number(remainDis)+"千米走完这条路\n路口转向西行\n总共还剩"+QString::number(totalDis)+"千米到达终点");break;}
       case 4 :{  ui->label_7->setText("还剩"+QString::number(remainDis)+"千米走完这条路\n路口转向东行\n总共还剩"+QString::number(totalDis)+"千米到达终点");break;}
   }
}

void MainWindow::roadRefresh()
{
   m_Image->newRoad();
   m_Image->setStart(0);
   m_Image->setEnd(0);
   ui->label_2->setText("");
   ui->label_3->setText("");
   ui->label_4->setText("");

}
