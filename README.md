# Navigation System
This navigation system is based on QT framework. It can simulate maps in the real world and provide navigation functions. It can tell you the top 100 nearest points from the specified coordinates. And it uses A* and dijkstra algorithm to provide navigation function. This system is developed using QT 4.11.1.    
## Installations
You can use it by [navigation_system.exe](https://github.com/crystallizedsnow/NavigationSystem/blob/main/nevegation_system.exe)
<img width="902" alt="image" src="https://github.com/user-attachments/assets/6b99d342-3d4b-4fde-9d1f-f2f81c8967c3">.
## Stimulation
Divide the space into squares and put one point at each square randomly. Connect the points with their four neibour points(up,down,left right) as edges, forming the roadnet.    
## Algorithms
**KDTree** to find the top 100 closest points to any point you select.<br>
**Dijkstra** to find the shortest route.<br>
**A\*** to find the shortest route. Proving that A* is faster than Dijkstra when solving finding closest route task.<br>
## Co-author
[crystallizedsnow](https://github.com/crystallizedsnow)  
[dawn-wisdom](https://github.com/dawn-wisdom)

