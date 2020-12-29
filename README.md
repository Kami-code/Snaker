# Snaker: 一个灵活好用的贪吃蛇游戏


## 一、项目介绍

  **Snaker**是一个开源的贪吃蛇游戏，支持多条蛇同时对战！
  用键盘WASD控制蛇的方向，寻找吃的东西，每吃一口就能得到一定的积分，而且蛇的身子会越吃越长，身子越长玩的难度就越大，不能碰墙，不能咬到自己的身体，等到了一定的分数，就能过关！
  贪吃蛇的唯一的目标就是长成最长的一条蛇！控制小蛇走位，吃掉地图上不定时冒出的食物，就会变长。小心！蛇头碰到其他蛇就会死亡，并且在地图上产生大量食物！

![avatar](https://github.com/Kami-code/Snaker/raw/master/DesktopWindow.png?raw=true)  

![avatar](https://github.com/Kami-code/Snaker/raw/master/GameWindow.png?raw=true)

![avatar](https://github.com/Kami-code/Snaker/raw/master/SettingWindow.png?raw=true)

![avatar](https://github.com/Kami-code/Snaker/raw/master/SaverWindow.png?raw=true)

## 二、系统设计

  由于考虑到AI蛇的训练过程中可能需要调用游戏主程序来完成交互工作，需要提供独立与图形界面之外的接口，这也是面向对象模块化编程的要素。故在设计的过程中严格按照模块之间的独立性、模块的可扩展性进行设计。

### 1.内核类
#### (1)Game class
  Game含以下成员，
```
    Background background;      //存放地图类
    LinkList<Snake> snakeList;  //存放蛇的链表（多人游戏）
    Setting setting;			//存放游戏设置文件
```

#### (2)Snake class
  Snake含以下私有成员，

```
    LinkList<Point> bodyList;       //存放每个关节的结点
    int direction = 4;              //存放真实方向
    int trySetDirection = 4;        //存放尝试方向
    int refreshTime = 50;           //存放刷新率
    int life = 3;                   //存放剩余生命值
```

#### (3)Background class
  Background含以下私有成员，

```
    int width = 30;             //场景横向网格格子数
    int height = 30;            //场景纵向网格格子数
    int **ground = NULL;        //二维数组，存放网格状态
```

#### (4)Saver class
>     Game gameSaved[3];			//目前支持存放3个存档
>     bool saved[3];						//判断存档的存储状态

### 2.图形化部分
  图形化部分提供了Snaker的图形化实现，包括主菜单、设置界面、游戏主界面、战斗结算界面等的架构及其实现。
#### (1)DesktopWindow
  本界面作为游戏的菜单界面，包含其余所有界面作为其成员，并实现所有切换画面和音效的功能。
>     QSound desktopSound;
>     QSound gameWindowSound;
>     GameWindow * gameWindow;
>     GameOverWindow * gameOverWindow;
>     SettingWindow * settingWindow;
>     SaverWindow * saverWindow;

#### (2) GameWindow
  本界面作为游戏主界面，主要实现周期刷新游戏场景，更新游戏元素（蛇、食物、墙）的任务。
>     QPushButton * button[maxButton];
>     WindowMap windowMap;
>     QTimer *timer[maxTimer];
>     bool stopped = true;

#### (3) SettingWindow
  本界面作为游戏设置界面，主要负责与用户的交互，实现音效控制、图形控制、网格大小、蛇的数量等设置。
>    QPushButton * audioButton;
>    QPushButton * figureButton;
>    QPushButton * returnButton;
>    QSlider * gridSlider;;
>    QSlider * snakeSlider;
>    
#### (4) SaverWindow
  本界面作为游戏存档界面，主要实现存档和读档功能。

#### (5) GameOverWindow
  本界面作为游戏结算界面，主要实现结算功能。

##  三、项目实现
### 1.游戏开始菜单

  游戏主菜单界面，在**DesktopWindow**中进行切换到游戏主界面、设置界面、关闭游戏的功能，才用*QPushButton*实现。

### 2. 蛇的显示和移动

  游戏主界面，利用计时器以固定帧率(30FPS)刷新地图。刷新地图由*WindowMap::Draw*负责实现，Draw从*WindowMap*成员中读取蛇对应的坐标，转化为地图网格坐标，从Resource中读取游戏素材中的像素图绘制蛇。蛇的每次刷新时间，触发蛇的移动函数，向当前*direction *方向移动一格。

### 3.墙（砖块）和⻝物的显示与判定

  Draw从*WindowMap*成员中读取地图元素对应的坐标，转化为地图网格坐标，从Resource中读取游戏素材中的像素图绘制地图元素。

### 4.⽀持游戏的暂停、存档与读档、重新开始。

  在游戏界面刷新函数中，按键触发开关变脸是否暂停。如果暂停，暂停所有计时器。存档和读档由*Saver*类实现，需要额外开辟空间实现Game中所有元素的拷贝，涉及到复制构造函数的重构。重新开始涉及到*Game::ReInit*中对于游戏元素的刷新。

### 5.单机多⼈游戏
  首先，我们将Game存储Snake的方式更改为链表，并在*SettingWindow*中提供了滑块条给用户修改蛇的数量的接口。第一条蛇绑定按键WASD，第二条蛇绑定按键IJKL，其余蛇绑定按键上下左右。
### 6.具有三种特殊效果的⻝物
  #### (1)加⼀条命

  死亡后，为了防止同一地点出现反复触发死亡判定的情况。我们选择蛇自动移动到左下角地图外，向地图中移动。
  #### (2)加速/减速

  我们通过修改*snake.refreshTime*，并在每次计时器超时的时候，更新计时器刷新时间。针对扩展到n条蛇的情况，我们选择在绑定timeout信号的槽函数中判断*sender()*得到调用此计时器的对象蛇的信息。避免了由于timeout信号的槽函数无法传递形参导致需要设置n个对应函数的情况。
  #### (3)镜像翻转

  吃到此食物后，按键镜像翻转，通过一个布尔型判断。
### 7. 地图编辑

  在游戏开始前、以及游戏暂停时，可以编辑地图上的砖块和各种⻝物。由于游戏网格的绘制由**WindowMap**实现，在其中提供了*LeftClicked*函数来提供鼠标点击点到网格坐标的映射。映射后修改*background.ground*的对应状态即可。
### 8. AI 蛇

  游戏提供了寻路方法BFS，供蛇在场景中寻路。当蛇并不能直接找到一条路径时，将采用启发性算法，尝试找到一条可能路径。
具体实现：

1. Target Generater，每个Snake中维护了两个参数id和currentLength，在产生目标阶段，对id和currentLength进行两次哈希运算，得到簇中心点。然后在中心点周围搜寻食物，得到target食物点。
2. BFS effector，得到target食物点后，调用BFS得到最短路径。
3. Movement Decision-maker，如果最短路径可达到，则前往最短路径；否则，在邻域内所有可达到的点中，寻找一个使蛇的身体的方差最大的点，以尽可能避免进入身体中自杀的情况。


### 9.用户友好设计

  游戏内含大量的资源文件的封装，包含各种场景元素的立绘、场景触发的音效、不同界面的背景音乐，提供给用户良好的视觉和听觉盛宴。设置界面提供给了用户以修改网格格数的滑动条，方便用户进行不同难度的游戏。

##  四、代码风格

  本项目中，变量均满足小骆驼式命名法（变量名中的每一个逻辑断点都有一个大写字母来标记），函数均满足大骆驼式命名法。
  关键代码均已添加注释，方便阅览。
