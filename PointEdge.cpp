#include "PointEdge.h"
Edge::Edge(int p)
{
    next = nullptr;
    point = p;
    distance = 100000000;
}
void Edge:: calpass_time(int high)
{
    highWay = high;
    double c = 0;//系数
    switch (high)
    {
    case 1:
    {
        uniform_int_distribution<int>u1(1, 2);
        level = u1(e);//车道数
        c = 50;
        break;
    }
    case 2:
    {
        uniform_int_distribution<int>u2(2, 3);
        level = u2(e);//车道数
        c = 75;
        break;
    }
    case 3:
    {
        uniform_int_distribution<int>u3(2, 4);
        level = u3(e);//车道数
        c = 100;
        break;
    }
    default:
        break;
    }
    car_capacity = level * distance * 100;//车容量为100辆/km/车道
    uniform_real_distribution<double>v(0 * level, 100 * level);
    car_density = v(e);//假设车流密度从0-100辆/km
    car_num = (int)car_density * (int)distance;//车辆数
    const double ratio_limit = 0.75;//函数分界点
    const double E = 2.718281828;
    //double ratio = (double)car_num / (double)car_capacity;//车辆数比车容量
    ratio = (double)car_num / (double)car_capacity;//车辆数比车容量
    if (ratio <= ratio_limit) {//fx=1
        pass_time = 1 * c * distance;
    }
    else {//fx=1+ex
        pass_time = (1 + E * ratio) * c * distance;
    }
}

EdgeL::EdgeL()
{
    head = fence = tail = new Edge(-1);
    len = 1;
    dis = 0;
}
void EdgeL::addEdge(int k1, int v, PointRoad* points,int highWay)//传入两边点的序号,第一个点是数组中的点序号
{
    if (head == nullptr)
    {
        head = tail = new Edge(v);
        len++;
        return;
    }
    tail->next = new Edge(v);
    tail = tail->next;
    len++;
    calDistance(points, k1, v);
    tail->calpass_time(highWay);
}
void EdgeL::calDistance(PointRoad* points, int k1, int k2)
{
    tail->distance = sqrt(pow((points[k1].x - points[k2].x), 2) + pow((points[k1].y - points[k2].y), 2));
}
void EdgeL::add(int v)
{
    tail->next = new Edge(v);
    tail = tail->next;
    len++;
}
void EdgeL::next()
{
    fence = fence->next;
}
void EdgeL::first()
{
    fence = head->next;
}
Graph::Graph()
{}
Graph::Graph(int pointNum)
{
    cout<<"successfully initial map"<<endl;
    pnum = pointNum;
    points = new PointRoad[pnum];//道路节点
    edges = new EdgeL[pnum];//高速路、主干路、次干路
    edgesH = new EdgeL[pnum];//高速路
    edges2=new EdgeL[pnum];//高速路和主干路
    initial_point(pnum, points);
    ednum = initial_edge(pnum, points, edges);
    higheNo = new int* [25];
    secondeNo=new int* [25];
    for (int i = 0; i < 25; ++i)
    {
        higheNo[i] = new int[9];
    }
    initial_PointH(higheNo,25,9);
    ednumH=initial_edgesH(higheNo,points,edges,edgesH);//加到原来的图中
    for (int i = 0; i < 25; ++i)
    {
        secondeNo[i] = new int[110];
    }
    initial_Point2(secondeNo);
    initial_edges2(secondeNo,points,edges,edges2);
}

void initial_point(int pnum, PointRoad* points)
{
    for (int nodeNum = 0; nodeNum < pnum; nodeNum++)
    {
        int groupNo = nodeNum / 400;
        int no_in_group = nodeNum - groupNo * 400;
        int no_in_group_x = no_in_group % 20;//计算各组点的x轴序号
        int no_in_group_y = no_in_group / 20;
        int boundX = no_in_group_x * 10 + groupNo % 5 * 200;
        int boundY = no_in_group_y * 10 + groupNo / 5 * 200;
        uniform_real_distribution<double> u(boundX, boundX + 10);
        double x = u(e);
        uniform_real_distribution<double> v(boundY, boundY + 10);
        double y = v(e);
        points[nodeNum].no = nodeNum;
        points[nodeNum].x = x;
        points[nodeNum].y = y;
    }
}


void initial_PointH(int ** higheNo,int rows,int cols)
{
    uniform_int_distribution<int> u(0, 6);
    int x1 = u(e);
    int y1 = u(e);
    uniform_int_distribution<int> v(8, 12);
    int x2 = v(e);
    int y2 = v(e);
    uniform_int_distribution<int> w(15, 19);
    int x3 = w(e);
    int y3 = w(e);
    for (int group = 0; group < rows; group++)
    {
        higheNo[group][0] = y1 * 20 + x1 + group * 400;
        higheNo[group][1] = y1 * 20 + x2 + group * 400;
        higheNo[group][2] = y1 * 20 + x3 + group * 400;
        higheNo[group][3] = y2 * 20 + x1 + group * 400;
        higheNo[group][4] = y2 * 20 + x2 + group * 400;
        higheNo[group][5] = y2 * 20 + x3 + group * 400;
        higheNo[group][6] = y3 * 20 + x1 + group * 400;
        higheNo[group][7] = y3 * 20 + x2 + group * 400;
        higheNo[group][8] = y3 * 20 + x3 + group * 400;
    }
}

void addEdgehelp1(int add_no, int k1,PointRoad* points,EdgeL* edges)
{
    edges[k1].addEdge(k1, add_no,points,1);
    edges[add_no].addEdge(add_no, k1,points,1);
}

void addEdgehelp2(int add_no, int k1, PointRoad* points, EdgeL* edges,EdgeL* edgesH)
{
    edges[k1].addEdge(k1, add_no, points,3);
    edges[add_no].addEdge(add_no, k1, points,3);
    edgesH[k1].addEdge(k1, add_no, points,3);
    edgesH[add_no].addEdge(add_no, k1, points,3);
}

void addEdgehelp3(int add_no, int k1,PointRoad* points,EdgeL*edges,EdgeL* edges2)
{
    edges[k1].addEdge(k1, add_no,points,2);
    edges[add_no].addEdge(add_no, k1,points,2);
    edges2[k1].addEdge(k1, add_no,points,2);
    edges2[add_no].addEdge(add_no, k1,points,2);
}

int initial_edge(int pnum, PointRoad* points, EdgeL* edges)
{
    int e_num = 0;
    for (int nodeNum = 0; nodeNum < pnum; nodeNum++)
    {
        int groupNo = nodeNum / 400;
        int no_in_group = nodeNum - groupNo * 400;
        int no_in_group_x = no_in_group % 20;//计算各组点的x轴序号
        int no_in_group_y = no_in_group / 20;
        if (no_in_group_x != 0)//每个点和前一个点相连
        {
            addEdgehelp1(nodeNum, nodeNum - 1, points,edges);
            e_num++;
        }
        if (no_in_group_y != 0)//每一个点和上面对应点相连
        {
            addEdgehelp1(nodeNum, nodeNum - 20, points,edges);
            e_num++;
        }
        //cout<<nodeNum<<endl;
    }
    return e_num;
}

int initial_edgesH(int** higheNo, PointRoad* points, EdgeL* edges,EdgeL* edgesH)
{
    int ednum = 0;
    for (int i = 0; i < 25; i++)
    {
        if (i % 5 != 0) {
            addEdgehelp2(higheNo[i - 1][2], higheNo[i][0],points,edges,edgesH);
            addEdgehelp2(higheNo[i - 1][5], higheNo[i][3], points,edges, edgesH);
            addEdgehelp2(higheNo[i - 1][8], higheNo[i][6], points,edges, edgesH);

            ednum = ednum + 3;
        }
        if (i / 5 != 0) {
            addEdgehelp2(higheNo[i - 5][6], higheNo[i][0], points,edges,edgesH);
            addEdgehelp2(higheNo[i - 5][7], higheNo[i][1], points,edges,edgesH);
            addEdgehelp2(higheNo[i - 5][8], higheNo[i][2], points,edges,edgesH);

            ednum = ednum + 3;
        }
        for (int m = 0; m < 9; m++) {
            if (m % 3 != 0) {
                addEdgehelp2(higheNo[i][m - 1], higheNo[i][m],points,edges, edgesH);
                ednum = ednum++;
            }
            if (m / 3 != 0) {
                addEdgehelp2(higheNo[i][m - 3], higheNo[i][m],points,edges,edgesH);
                ednum = ednum++;
            }
        }
    }
    return ednum;
}

int initial_Point2(int** secondeNo)
{
    int cnt=0;
    for(int group=0;group<25;group++)
    {
        cnt=0;
        int start_pno=group*400;
        //第一个区域取 （start_x+50,start+150) (start_y+50,start_y+150)
       //5行5列到5行15列 5行5列到15行5列...
        for(int j=5;j<=15;j++)
        {
            secondeNo[group][cnt++]=start_pno+20*(5-1)+j;
        }
        for(int i=6;i<=15;i++)
        {
            secondeNo[group][cnt++]=start_pno+15+(i-1)*20;
        }
        for(int j=15;j>=5;j--)
        {
            secondeNo[group][cnt++]=start_pno+20*(15-1)+j;
        }

        for(int i=15;i>=6;i--)
        {
            secondeNo[group][cnt++]=start_pno+5+(i-1)*20;
        }
        //cout<<"cnt1:"<<cnt<<endl;
        //第二个区域取 （start_x+70,start+120) (start_y+70,start_y+120)
        //7行7列到7行12列 7行12列到12行12列...
        for(int j=7;j<=12;j++)
        {
            secondeNo[group][cnt++]=start_pno+20*(7-1)+j;
        }
        for(int i=7;i<=12;i++)
        {
            secondeNo[group][cnt++]=start_pno+12+(i-1)*20;
        }
        for(int j=12;j>=7;j--)
        {
            secondeNo[group][cnt++]=start_pno+20*(12-1)+j;
        }
        for(int i=12;i>=7;i--)
        {
            secondeNo[group][cnt++]=start_pno+7+(i-1)*20;
        }
        //cout<<"cnt2"<<cnt<<endl;
        //第三个区域取（x，x+200）y=100
        for(int j=0;j<20;j++)
        {
            secondeNo[group][cnt++]=start_pno+9*20+j;
        }
        //cout<<"cnt3"<<cnt<<endl;
        //第四个区域取（y，y+200） x=100
        for(int i=0;i<20;i++)
        {
            secondeNo[group][cnt++]=start_pno+10+i*20;
        }
        //cout<<"cnt4"<<cnt<<endl;
    }
    cout<<"total num_p2 for a group"<<cnt<<endl;
    return cnt;
}
int initial_edges2(int** secondeNo,PointRoad* points,EdgeL* edges,EdgeL* edges2)
{
    //int cnt=initial_Point2(secondeNo);
    cout<<"normally"<<endl;
    int cntEdge=0;
    for(int group=0;group<25;group++)
    {
        //cnt=106 42+20+20+20
        for(int i=0;i<41-1;i++)
        {
            addEdgehelp3(secondeNo[group][i],secondeNo[group][i+1],points,edges,edges2);
            cntEdge++;
        }
        for(int i=42;i<65-1;i++)
        {
            addEdgehelp3(secondeNo[group][i],secondeNo[group][i+1],points,edges,edges2);
            cntEdge++;
        }
        for(int i=66;i<85-1;i++)
        {
            addEdgehelp3(secondeNo[group][i],secondeNo[group][i+1],points,edges,edges2);
            cntEdge++;
        }
        for(int i=86;i<105-1;i++)
        {
            addEdgehelp3(secondeNo[group][i],secondeNo[group][i+1],points,edges,edges2);
            cntEdge++;
        }

    }
    return cntEdge;
}
vector<int> Graph::dijkstra(int start, int end)
{
    cout << "dijkstra:\n";
    int* node_before = new int[pnum];//记录该结点路径的前驱节点
    int pos = start;
    double* len = new double[pnum];//记录当前状态各点距离
    int* mark = new int[pnum];//标记是否访问
    double shortestDis = edges[pos].head->next->distance;//记录当前最短路径
    int shortestpoint = 0;//记录要加入visited集合的点
    for (int i = 0; i < pnum; i++)
    {
        len[i] = 100000000;
    }
    for (int i = 0; i < pnum; i++)
    {
        mark[i] = -1;
    }
    mark[start] = 1;
    len[start]=0;
    //与起点相连接的点的前驱节点为起点
    for (edges[start].first(); edges[start].fence != nullptr; edges[start].next())
    {
        len[edges[start].fence->point] = edges[start].fence->distance;
        node_before[edges[start].fence->point] = pos;
    }
    //起点不可以与终点一致
    while (shortestpoint != end)
    {
        shortestDis = 1000000000;
        shortestpoint = -1;
        for (int i = 0; i < pnum; i++)
        {
            if (mark[i] == 1)
                continue;
            if (shortestDis > len[i])
            {
                shortestpoint = i;
                shortestDis = len[i];
            }
        }
        mark[shortestpoint] = 1;
        pos = shortestpoint;//找到距离最近的点加入其中
        for (edges[pos].first(); edges[pos].fence != nullptr; edges[pos].next())
        {
            if (mark[edges[pos].fence->point] == 1)
                continue;
            if (len[edges[pos].fence->point] > len[pos] + edges[pos].fence->distance)
            {
                node_before[edges[pos].fence->point] = pos;
                len[edges[pos].fence->point] = len[pos] + edges[pos].fence->distance;
            }
        }
    }
    cout << "shortestLen:" << len[end] << endl;
    vector<pair<double, double>>path_coordinate;//记录路径坐标
    //vector<int>pathPoint;
    if(!pathPoint.empty())
        pathPoint.clear();
    for (pos = end; pos != start; pos = node_before[pos])
    {
        path_coordinate.push_back({ points[pos].x,points[pos].y });
        pathPoint.push_back(pos);
    }
    pathPoint.push_back(start);
    path_coordinate.push_back({ points[pos].x,points[pos].y });
    return pathPoint;
}

double Graph::inspire_function(int p1, int p2) //选用欧几里得距离作为启发函数,即计算起点到该点的欧几里得距离
{
    return sqrt((points[p1].x - points[p2].x) * (points[p1].x - points[p2].x) + (points[p1].y - points[p2].y) * (points[p1].y - points[p2].y));
}

vector<int> Graph::astar(int start, int end)
{
    cout << "Astar:\n";
    int pos = start;
    int* node_before = new int[pnum];//记录该结点路径的前驱节点
    double* len = new double[pnum];//记录当前状态各点F值
    double* dist = new double[pnum];//记录当前状态各点路程
    int* mark = new int[pnum];//标记是否访问
    double shortestF = edges[pos].head->next->distance + inspire_function(end, edges[pos].head->next->point);//记录当前最短F值（F=总路程+两点欧几里得距离）
    int shortestpoint = 0;//记录要加入visited集合的点
    vector<int>path;//记录走过的最短路径包括的点
    double shortestDist = edges[pos].head->next->distance;
    for (int i = 0; i < pnum; i++)
    {
        len[i] = 100000000;
    }
    for (int i = 0; i < pnum; i++)
    {
        dist[i] = 0;
    }
    for (int i = 0; i < pnum; i++)
    {
        mark[i] = -1;
    }
    mark[start] = 1;
    len[start]=0;
    for (edges[pos].first(); edges[pos].fence != nullptr; edges[pos].next())
    {
        len[edges[pos].fence->point] = edges[pos].fence->distance + inspire_function(end, edges[pos].fence->point);
        dist[edges[pos].fence->point] = edges[pos].fence->distance;
        node_before[edges[pos].fence->point] = pos;
    }
    while (shortestpoint != end)
    {
        shortestF = 1000000000;
        shortestpoint = -1;
        for (int i = 0; i < pnum; i++)
        {
            if (mark[i] == 1)
                continue;
            if (shortestF > len[i])
            {
                shortestpoint = i;
                shortestF = len[i];
                shortestDist = dist[i];
            }
        }
        mark[shortestpoint] = 1;
        pos = shortestpoint;
        for (edges[pos].first(); edges[pos].fence != nullptr; edges[pos].next())
        {
            if (mark[edges[pos].fence->point] == 1)
                continue;
            if (len[edges[pos].fence->point] > len[pos] + edges[pos].fence->distance)
            {
                len[edges[pos].fence->point] = len[pos] + edges[pos].fence->distance+ inspire_function(end, edges[pos].fence->point);
                dist[edges[pos].fence->point] = dist[pos] + edges[pos].fence->distance;
                node_before[edges[pos].fence->point] = pos;
            }
        }
    }
    cout << "shortestLen(A*):" << dist[end] << endl;
    vector<pair<double, double>>path_coordinate;//记录路径坐标
    //vector<int>pathPoint;
    if(!pathPoint.empty())
        pathPoint.clear();
    for (pos = end; pos != start; pos = node_before[pos])
    {
        path_coordinate.emplace_back(points[pos].x, points[pos].y);
        pathPoint.push_back(pos);
    }
    pathPoint.push_back(start);
    path_coordinate.push_back({ points[pos].x,points[pos].y });
    return pathPoint;
}

vector<int> Graph::dijkstra_time(int start, int end)//根据时间来寻路
{
    //cout << "距离：" << sqrt(pow(points[start].x - points[end].x,2)+pow( points[start].y - points[end].y,2));
    double disSum = 0;
    int* node_before = new int[pnum];//记录该结点路径的前驱节点
    int pos = start;
    double* ptime = new double[pnum];//记录当前状态各点时间
    int* mark = new int[pnum];//标记是否访问
    double shortestTime = edges[pos].head->next->pass_time;//记录当前最短时间路径
    int shortestpoint = 0;//记录要加入visited集合的点
    for (int i = 0; i < pnum; i++)
    {
        ptime[i] = 100000000;
    }
    for (int i = 0; i < pnum; i++)
    {
        mark[i] = -1;
    }
    mark[start] = 1;
    ptime[start]=0;
    //与起点相连接的点的前驱节点为起点
    for (edges[pos].first(); edges[pos].fence != nullptr; edges[pos].next())
    {
        ptime[edges[pos].fence->point] = edges[pos].fence->pass_time;
        node_before[edges[pos].fence->point] = pos;
    }
    while (shortestpoint != end)
    {
        shortestTime = 1000000000;
        shortestpoint = -1;
        for (int i = 0; i < pnum; i++)
        {
            if (mark[i] == 1)
                continue;
            if (shortestTime > ptime[i])
            {
                shortestpoint = i;
                shortestTime = ptime[i];
            }
        }
        mark[shortestpoint] = 1;
        pos = shortestpoint;
        for (edges[pos].first(); edges[pos].fence != nullptr; edges[pos].next())
        {
            if (mark[edges[pos].fence->point] == 1)
                continue;
            if (ptime[edges[pos].fence->point] > ptime[pos] + edges[pos].fence->pass_time)
            {
                node_before[edges[pos].fence->point] = pos;
                ptime[edges[pos].fence->point] = ptime[pos] + edges[pos].fence->pass_time;
            }
        }
    }
    cout << "shortestTime:" << ptime[end] / 3600 << "h" << endl;
    vector<pair<double, double>>path_coordinate;//记录路径坐标
    //vector<int>pathPoint;
    if(!pathPoint.empty())
        pathPoint.clear();
    for (pos = end; pos != start; pos = node_before[pos])
    {
        path_coordinate.push_back({ points[pos].x,points[pos].y });
        pathPoint.push_back(pos);
    }
    pathPoint.push_back(start);
    path_coordinate.push_back({ points[pos].x,points[pos].y });
    return pathPoint;
}


bool Graph::cmpDis(double x,double y,int p1,int p2,double len)//返回是否点(x,y)在点p1，p2组成的边上
{
    if(sqrt((x-points[p1].x)*(x-points[p1].x)+(y-points[p1].y)*(y-points[p1].y))+ sqrt((x-points[p2].x)*(x-points[p2].x)+(y-points[p2].y)*(y-points[p2].y))-len<3)//线上一点到两点距离和约等于两点距离
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<int>Graph::remainRoad(double x,double y,int&direction,double &totalwayDis,double&remainDis,int&wrongtype)
{
    if(pathPoint.empty()==true)
    {
        wrongtype=1;
        return remainPathSet;
        //返回错误信息先导航
    }
    if(remainPathSet.empty()!=true)
    {
        remainPathSet.clear();
    }
    bool flag=false;//标志是否找到所在路
    totalwayDis=0;//计算剩下总路程

    pair<int,int>temp;//记录找到的边的两端点
    vector<int>::reverse_iterator it1=pathPoint.rbegin();
    vector<int>::reverse_iterator it2=pathPoint.rbegin()+1;//前驱指针
    while(it2!=pathPoint.rend())
    {
        for(edges[*it1].first();edges[*it1].fence!=NULL;edges[*it1].next())
        {
            if(edges[*it1].fence->point!=*it2)//若点搜索到的连边不在路径上
            {
                continue;
            }
            if(cmpDis(x,y,*it1,edges[*it1].fence->point,edges[*it1].fence->distance))//返回是否点(x,y)在点p1，p2组成的边上
            {
                temp.first=*it1;
                temp.second=edges[*it1].fence->point;
                flag=true;
                break;
            }
        }
        if(flag==true)
        {break;}
        it1++;
        it2++;
    }
    if(flag==false)
    {
        wrongtype=2;
        return remainPathSet;
        //返回错误信息重新设置现在位置
    }
    remainDis=(x-points[temp.second].x)*(x-points[temp.second].x)+(y-points[temp.second].y)*(y-points[temp.second].y);//这条边剩余路径
    direction=0;//1北2南3西4东
    if(it2!=pathPoint.rend())//若残边之后还有边
    {
        it1++;
        it2++;//探路下一条边方向
        bool maindir=((points[*it1].x-points[*it2].x)-(points[*it1].y<points[*it2].y)>0)?true:false;//true为x主导，false为y主导方向
        if(maindir)//左西右东
        {
            if(points[*it1].x<points[*it2].x)//东
            {
                direction=4;
            }
            else//西
            {
                direction=3;
            }
        }
        else//上北下南
        {
            if(points[*it1].y<points[*it2].y)//南
            {
                direction=2;
            }
            else//北
            {
                direction=1;
            }
        }

        totalwayDis+=remainDis;
        while(it1!=pathPoint.rend())
        {
            for(edges[*it1].first();edges[*it1].fence!=NULL;edges[*it1].next())
            {
                if(edges[*it1].fence->point!=*it2)//若点搜索到的连边不在路径上
                {
                    continue;
                }
                totalwayDis+=edges[*it1].fence->distance;
                remainPathSet.push_back(*it1);
                break;
            }
            it1++;
            it2++;
        }
        cout<<pathPoint[0]<<endl;
        remainPathSet.push_back(pathPoint[0]);
    }
    return remainPathSet;
}
