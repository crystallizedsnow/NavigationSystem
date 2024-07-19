
#ifndef POINTEDGE_H
#define POINTEDGE_H
#include<iostream>
#include<ctime>
#include<random>
#include <chrono>
#include<cmath>
#include<vector>
#include<algorithm>
#include<fstream>
#include<utility>
#include<stack>
#include<set>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;
static default_random_engine e;
static int pointHNum = 0;
class PointRoad
{
public:
    int no;//点序号
    double x;
    double y;//点坐标
    int group;//点所属分组
};
class PointH
{
public:
    int noH;
    int no;
    double x;
    double y;
    int group;
};
class Edge
{
public:
    Edge* next;
    int point;//连上的一点序号
    double distance;//边长
    int car_capacity;//车容量
    int car_num;//实际车辆数
    double car_density;//车流密度(辆/公里）
    double pass_time;//过路时间
    double ratio;
    int level;//道路等级（值为车道数）
    int highWay;
    Edge(int p);
    void calpass_time(int high);
};

class EdgeL
{
public:
    Edge* head;
    Edge* fence;
    Edge* tail;
    int len;
    double dis;
    EdgeL();
    void addEdge(int k1, int v, PointRoad* points,int highWay);//传入两边点的序号,第一个点是数组中的点序号

    void calDistance(PointRoad* points, int k1, int k2);
    void add(int v);
    void next();
    void first();
};

class Graph
{
public:
    PointRoad* points;//点集
    EdgeL* edges;//存储边的数组
    EdgeL* edgesH;//存储高速公路边数组
    EdgeL* edges2;//存储主干路的数组
    int pnum;//点数目
    int ednum;//边数
    int ednumH;//高速公路边数
    int ednum2;//2级公路数目
    int** higheNo;//高速公路点序号
    int** secondeNo;//二级道路的序号
    vector<int>pathPoint;//导航道路
    vector <int>remainPathSet;//剩余道路
    Graph();
    Graph(int pnum);
    vector<int> dijkstra(int start, int end);
    double inspire_function(int p1, int p2);
    vector<int> astar(int start, int end);
    vector<int> dijkstra_time(int start,int end);
    vector<int> astar_time(int start,int end);
    bool cmpDis(double x,double y,int p1,int p2,double len);
    vector<int>remainRoad(double x,double y,int&direction,
           double &totalwayDis,double&remainDis,int&wrongtype);
};
void initial_point(int pnum, PointRoad* points);
int initial_edge(int pnum, PointRoad* points, EdgeL* edges);
void initial_PointH(int** higheNo, int rows, int cols);
int initial_edgesH(int** higheNo, PointRoad* points, EdgeL* edges, EdgeL* edgesH);
int initial_Point2(int** secondeNo);
int initial_edges2(int** secondeNo,PointRoad* points,EdgeL* edges,EdgeL* edges2);
//void initial_map_1(Graph* map);
//void initial_map_2(Graph* map);



#endif // POINTEDGE_H
