#include "KNTree.h"

void KNode::setinfo(int n, double x1, double y1) {
    no = n;
    x = x1;
    y = y1;
};
bool cmp11(KNode a, KNode b)
{
    return a.x < b.x;
}
bool cmp12(KNode a, KNode b)
{
    return a.y < b.y;
}
bool equal1(KNode a, KNode b)
{
    return a.x == b.x && a.y == b.y;
}

bool cmp13(NoDist a, NoDist b)
{
    return a.dis < b.dis;
}

KNTree::KNTree(PointRoad* points)
{
    tree = new KNode[10000];
    for (int i = 0; i < 10000; i++)
    {
        pointsort[i].setinfo(i, points[i].x, points[i].y);
    }
    head = build_kdTree(pointsort, 10000);
    //cout << "head:" << head << endl;
    //cout<< "size:" << sizet << endl;
}

void KNTree:: chooseSplit(KNode* tree_set, int& split, KNode& splitChoice, int size)//选取中位数作为切分点
{
    //计算x维度上的方差（这里的方差不是常说的方差，而是E(X^2)-(E(x))^2
    double temp1 = 0, temp2 = 0;
    for (int i = 0; i < size; i++)
    {
        temp1 += 1.0 / (double)size * pow(tree_set[i].x, 2);
        temp2 += 1.0 / (double)size * tree_set[i].x;
    }
    double v1 = temp1 - temp2 * temp2;
    //计算y维度上方差
    temp1 = temp2 = 0;
    for (int i = 0; i < size; i++)
    {
        temp1 += 1.0 / (double)size * pow(tree_set[i].y, 2);
        temp2 += 1.0 / (double)size * tree_set[i].y;
    }
    double v2 = temp1 - temp2 * temp2;
    split = v1 > v2 ? 0 : 1;//取方差大的为切分维度
    if (split == 0)//按x轴坐标从小到大排序
    {
        sort(tree_set, tree_set + size, cmp11);
    }
    else //按y轴坐标从小到大排序
    {
        sort(tree_set, tree_set + size, cmp12);
    }
    splitChoice.x = tree_set[size / 2].x;
    splitChoice.y = tree_set[size / 2].y;//选取中位数作为切分点
    splitChoice.no = tree_set[size / 2].no;
}
int KNTree:: build_kdTree(KNode* subtree, int size)
{
    if (size == 0)
    {
        return -1;
    }
    KNode* tree_left = new KNode[5000];
    KNode* tree_right = new KNode[5000];
    int split = 0;
    sizet++;
    KNode splitChoice;
    chooseSplit(subtree, split, splitChoice, size);
    int sizeleft = 0, sizeright = 0;
    if (split == 0)//切分x轴
    {
        for (int i = 0; i < size; i++)
        {
            //小于平面的放在左空间
            if (!equal1(subtree[i], splitChoice) && subtree[i].x <= splitChoice.x)
            {
                tree_left[sizeleft].x = subtree[i].x;
                tree_left[sizeleft].y = subtree[i].y;
                tree_left[sizeleft].no = subtree[i].no;
                sizeleft++;
            }
            //大于平面的放在右空间
            else if (!equal1(subtree[i], splitChoice) && subtree[i].x > splitChoice.x)
            {
                tree_right[sizeright].x = subtree[i].x;
                tree_right[sizeright].y = subtree[i].y;
                tree_right[sizeright].no = subtree[i].no;
                sizeright++;
            }
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            if (!equal1(subtree[i], splitChoice) && subtree[i].y <= splitChoice.y)
            {
                tree_left[sizeleft].x = subtree[i].x;
                tree_left[sizeleft].y = subtree[i].y;
                tree_left[sizeleft].no = subtree[i].no;
                sizeleft++;
            }
            else if (!equal1(subtree[i], splitChoice) && subtree[i].y > splitChoice.y)
            {
                tree_right[sizeright].x = subtree[i].x;
                tree_right[sizeright].y = subtree[i].y;
                tree_right[sizeright].no = subtree[i].no;
                sizeright++;
            }
        }
    }
    tree[splitChoice.no].split = split;
    tree[splitChoice.no].no = splitChoice.no;
    tree[splitChoice.no].x = splitChoice.x;
    tree[splitChoice.no].y = splitChoice.y;
    tree[splitChoice.no].left = build_kdTree(tree_left, sizeleft);
    //cout << splitChoice.no << endl;
    tree[splitChoice.no].right = build_kdTree(tree_right, sizeright);
    delete[]tree_left;
    delete[]tree_right;
    return splitChoice.no;
}
double distance1(KNode a, KNode b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

NoDist* KNTree::getTopOne(double x, double y)
{
    KNode target(x, y);
    NoDist* topPoint = new NoDist;//找到的点集
    topPoint->dis = 1000000;
    searchNearest(head, target, 1, topPoint);
    return topPoint;
}

NoDist* KNTree:: gettop100(double x, double y)
{
    KNode target(x, y);
    NoDist* topPoint = new NoDist[100];//找到的点集
    for (int i = 0; i < 100; i++) {
        topPoint[i].dis = 1000000;
    }
    searchNearest(head, target, 100, topPoint);
    return topPoint;
}

void KNTree::searchNearest(int Kd, KNode target, int num, NoDist* topPoint)//根结点，目标坐标，距离变量，最临近点数
{
    //topPoint为距离目标点最近的点集，按照从小到大的顺序排列
    // 1.向下搜索直到叶子结点
    stack<int> search_path;
    int pSearch = Kd;//标记搜索到的结点
    KNode nearest;
    double dist = 0;//选中点和目标点距离
    int findn = 0;//找到的最小值个数
    int* mark = new int[10000];
    for (int i = 0; i < 10000; i++)
    {
        mark[i] = 0;//标记结点是否访问过
    }
    while (pSearch != -1)
    {
        //pSearch加入到search_path中;
        mark[pSearch] = 1;
        search_path.push(pSearch);

        if (tree[pSearch].split == 0)
        {
            if (target.x <= tree[pSearch].x) /* 如果小于就进入左子树 */
            {
                pSearch = tree[pSearch].left;
            }
            else
            {
                pSearch = tree[pSearch].right;
            }
        }
        else {
            if (target.y <= tree[pSearch].y) /* 如果小于就进入左子树 */
            {
                pSearch = tree[pSearch].left;
            }
            else
            {
                pSearch = tree[pSearch].right;
            }
        }
    }
    //2.回溯搜索路径

    int pBack = 0;//指向回溯到的叶子
    while (search_path.size() != 0)
    {
        //取出search_path最后一个结点赋给pBack
        pBack = search_path.top();
        search_path.pop();//待检查的点

        if (tree[pBack].left == -1 && tree[pBack].right == -1) /* 如果pBack为叶子结点 */
        {
            dist = distance1(tree[pBack], target);
            if (topPoint[num-1].dis > dist)//只需要跟最远的比较
            {
                if (findn < num)
                {
                    topPoint[findn].no = tree[pBack].no;
                    topPoint[findn].dis = dist;
                    mark[topPoint[findn].no] = 1;
                    sort(topPoint, topPoint + num, cmp13);//按距离从小到大排序
                    findn++;
                }
                else
                {
                    topPoint[num-1].no = tree[pBack].no;
                    topPoint[num-1].dis = dist;
                    mark[topPoint[num-1].no] = 1;
                    sort(topPoint, topPoint + num, cmp13);
                }
            }
        }
        else/*不是叶子结点*/
        {
            int s = tree[pBack].split;
            if (s == 0)
            {
                dist = distance1(tree[pBack], target);
                if (findn < num || fabs(tree[pBack].x - target.x) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if (topPoint[num-1].dis > dist)
                    {
                        if (findn < num)
                        {
                            topPoint[findn].no = tree[pBack].no;
                            topPoint[findn].dis = dist;
                            mark[topPoint[findn].no] = 1;
                            sort(topPoint, topPoint + num, cmp13);
                            findn++;
                        }
                        else
                        {
                            topPoint[num-1].no = tree[pBack].no;
                            topPoint[num-1].dis = dist;
                            mark[topPoint[num-1].no] = 1;
                            sort(topPoint, topPoint + num, cmp13);
                        }
                    }
                    if (target.x >= tree[pBack].x || findn < num)
                    {

                        pSearch = tree[pBack].left; // 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索
                        while (mark[pSearch] == 0 && pSearch != -1)
                        {
                            //pSearch加入到search_path中;
                            mark[pSearch] = 1;
                            search_path.push(pSearch);

                            if (tree[pSearch].split == 0)
                            {
                                if (target.x <= tree[pSearch].x) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                            else
                            {
                                if (target.y <= tree[pSearch].y) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                        }
                    }
                    if (target.x <= tree[pBack].x || findn < num) /* 如果target位于pBack的左子空间，或最小值数组没满且右子空间没搜索，那么就要跳到右子空间去搜索 */
                    {
                        pSearch = tree[pBack].right;
                        while (mark[pSearch] == 0 && pSearch != -1)
                        {
                            //pSearch加入到search_path中;
                            mark[pSearch] = 1;
                            search_path.push(pSearch);

                            if (tree[pSearch].split == 0)
                            {
                                if (target.x <= tree[pSearch].x) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                            else
                            {
                                if (target.y <= tree[pSearch].y) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                dist = distance1(tree[pBack], target);
                if (findn < num || fabs(tree[pBack].y - target.y) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
                {
                    if (topPoint[num-1].dis > dist)
                    {
                        if (findn < num)
                        {
                            topPoint[findn].no = tree[pBack].no;
                            topPoint[findn].dis = dist;
                            sort(topPoint, topPoint + num, cmp13);
                            mark[topPoint[findn].no] = 1;
                            findn++;
                        }
                        else
                        {
                            topPoint[num-1].no = tree[pBack].no;
                            topPoint[num-1].dis = dist;
                            sort(topPoint, topPoint + num, cmp13);
                            mark[topPoint[num-1].no] = 1;
                        }
                    }

                    if (target.y <= tree[pBack].y || findn < num) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
                    {
                        pSearch = tree[pBack].right;
                        while (mark[pSearch] == 0 && pSearch != -1)
                        {
                            //pSearch加入到search_path中;
                            mark[pSearch] = 1;
                            search_path.push(pSearch);

                            if (tree[pSearch].split == 0)
                            {
                                if (target.x <= tree[pSearch].x) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                            else
                            {
                                if (target.y <= tree[pSearch].y) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                        }
                    }
                    if (target.y >= tree[pBack].y || findn < num)
                    {
                        pSearch = tree[pBack].left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
                        while (mark[pSearch] == 0 && pSearch != -1)
                        {
                            //pSearch加入到search_path中;
                            mark[pSearch] = 1;
                            search_path.push(pSearch);

                            if (tree[pSearch].split == 0)
                            {
                                if (target.x <= tree[pSearch].x) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                            else
                            {
                                if (target.y <= tree[pSearch].y) /* 如果小于就进入左子树 */
                                {
                                    pSearch = tree[pSearch].left;
                                }
                                else
                                {
                                    pSearch = tree[pSearch].right;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
