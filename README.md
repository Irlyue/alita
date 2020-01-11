# alita
这是自己的一个闲暇游戏项目，用C++复现《仿传奇单机版》这个游戏，使用了原游戏的资源文件。

## 依赖
这个项目使用了如下依赖库：
- SDL2.0, SDL2-ttf, SDL2-image：主要用于windows窗口界面的编程，图像加载和渲染，文字渲染等。这些库都可以在https://www.libsdl.org/ 网站上下载，
下面也附上了具体的下载链接。由于该项目主要的开发环境为windows，所以下面的依赖库都选择VC版本的。
  - [SDL2.0](https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip)
  - [SDL2-image](https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-VC.zip)
  - [SDL2-ttf](https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.0.15-VC.zip)
- tinyxml2：主要用于读取xml配置文件。tinyxml2的源码已经直接添加到该项目并且添加为一个vs项目的依赖，所以不需要额外下载什么，有问题可以
访问tinyxml2的官方[repo](https://github.com/leethomason/tinyxml2)

## 编译
首先clone这个repo到本地，然后下载上面列出的依赖库，解压放到`dev/`目录下。接着用Visual Studio打开根目录下的解决方案文件`alita.sln`，然后便可以
按照普通的vs项目正常编译。

## 运行
由于本repo只提供了代码文件，游戏的资源文件需要下载《仿传奇单机版》得到对应的下载环境，后面会提供一个网盘链接。
