
#ifndef KNTREE_H
#define KNTREE_H
#pragma once
#include "PointEdge.h"
#include<math.h>
#include <iostream>
#include <algorithm>
#include<fstream>
#include <stack>
using namespace std;
struct p
{
    int no;
    double x;
    double y;
};
void points_initial(PointRoad* points);

class KNode
{
public:
    int no;
    int left;
    int right;
    double x;
    double y;
    int split;
    void setinfo(int n, double x1, double y1);
    KNode() {}
    KNode(double x1, double y1) {
        x = x1;
        y = y1;
    }
};
bool cmp11(KNode a, KNode b);//比较x轴方向上的大小
bool cmp12(KNode a, KNode b);//比较y轴方向上的大小
bool equal1(KNode a, KNode b);
double distance1(KNode a, KNode b);
struct NoDist
{
    int no;
    double dis;
};
bool cmp13(NoDist a, NoDist b);
class KNTree
{
public:
    KNode pointsort[10000];
    KNode* tree;
    int head;
    int sizet;
    KNTree(PointRoad* points);
    void chooseSplit(KNode* tree_set, int& split, KNode& splitChoice, int size);//选取中位数作为切分点
    int build_kdTree(KNode* subtree, int size);
    NoDist* getTopOne(double x,double y);
    NoDist* gettop100(double x, double y);
    void searchNearest(int Kd, KNode target, int num, NoDist* topPoint);
};


#endif // KNTREE_H
