
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ImageWidget.h"
#include "PointEdge.h"
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <QMessageBox>
#include <QGraphicsView>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refresh();
public slots:
    void mapRefresh();
    void nevigation(int wayChoice);
    void showNear100();
    void setStart();
    void setEnd();
    void findRemainRoad();
    void navigate1(double x1,double y1);
    void roadRefresh();
private:
    Ui::MainWindow *ui;
    ImageWidget *m_Image;
};
void initial_map(Graph* map);
void showErrorMessageBox(const QString &errorMessage);
#endif // MAINWINDOW_H
