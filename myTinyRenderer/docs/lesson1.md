# 第一课：布雷森汉姆的线段绘制算法

## 首次尝试
第一课的目标是渲染线框网。为此，我们需要学会如何绘制线段。我们可以简单地阅读布雷森汉姆线算法是什么，但让我们自己编写代码。绘制线段的最简单的代码看起来像这样：

```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    for (float t=0.; t<1.; t+=.01) { 
        int x = x0 + (x1-x0)*t; 
        int y = y0 + (y1-y0)*t; 
        image.set(x, y, color); 
    } 
}
```

代码的快照可以在这里找到。

## 第二次尝试
这段代码的问题（除了效率低下）是常数的选择，我选择的是0.01。如果我们选择0.1，我们的线段看起来会是这样：

我们可以很容易地找到必要的步长：它只是需要绘制的像素数量。最简单的（但错误的）代码看起来像这样：

```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    for (int x=x0; x<=x1; x++) { 
        float t = (x-x0)/(float)(x1-x0); 
        int y = y0*(1.-t) + y1*t; 
        image.set(x, y, color); 
    } 
}
```
注意！我的学生们在这种代码中的第一个错误来源是整数除法，就像`(x-x0)/(x1-x0)`。然后，如果我们用这段代码尝试绘制以下线段：

```c
line(13, 20, 80, 40, image, white); 
line(20, 13, 40, 80, image, red); 
line(80, 40, 13, 20, image, red);
```

结果发现一条线很好，第二条有空洞，第三条线根本不存在。注意，第一和第三条线（在代码中）用不同的颜色绘制了同一条线，并且方向不同（源点和目标点翻转）。我们已经看到了白色的，它画得很好。我本想将白线的颜色改为红色，但做不到。这是对称性的测试：绘制线段的结果不应依赖于点的顺序：(a,b)线段应该与(b,a)线段完全相同。

## 第三次尝试
我们通过交换点来修复缺失的红线，使得x0总是小于x1。

由于线段的高度大于宽度，其中一个线段有空洞。我的学生们经常建议以下的修复方法：

```c
if (dx>dy) {for (int x)} else {for (int y)}
```
天哪！

```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { // 如果线很陡，我们转置图像 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { // 使其从左到右 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    for (int x=x0; x<=x1; x++) { 
        float t = (x-x0)/(float)(x1-x0); 
        int y = y0*(1.-t) + y1*t; 
        if (steep) { 
            image.set(y, x, color); // 如果转置了，就还原 
        } else { 
            image.set(x, y, color); 
        } 
    } 
}
```

## 时间：第四次尝试
警告：编译器的优化器（g++ -O3）通常比您（和我）在创建快速代码方面做得更好。这部分在这里是为了历史/文化原因。

这段代码工作得很好。这正是我想在最终版本的渲染器中看到的那种复杂性。它肯定效率不高（多次除法等），但它简短且易于阅读。注意，它没有断言，也没有检查是否超出边界，这很不好。在这些文章中，我尽量不使这段特定的代码过于复杂，因为它被频繁阅读。同时，我系统地提醒进行检查的必要性。

所以，之前的代码工作得很好，但我们可以优化它。优化是一件危险的事情。我们应该清楚代码将在哪个平台上运行。为图形卡或仅为CPU优化代码——是完全不同的事情。在任何优化之前和优化期间，必须对代码进行分析。试着猜测，这里哪个操作是最耗费资源的操作？

为了测试，我100万次绘制了我们之前画过的3条线段。我的CPU是Intel® Core(TM) i5-3450 CPU @ 3.10GHz。对于每个像素，这段代码调用了TGAColor复制构造函数。大约是1000000 * 3条线段 * 每条线段大约50个像素。相当多的调用，不是吗？从哪里开始优化？分析器会告诉我们。

我用g++ -ggdb -g -pg -O0 keys编译了代码，然后运行了gprof：

```
%   cumulative   self              self     total 
 time   seconds   seconds    calls  ms/call  ms/call  name 
 69.16      2.95     2.95  3000000     0.00     0.00  line(int, int, int, int, TGAImage&, TGAColor) 
 19.46      3.78     0.83 204000000     0.00     0.00  TGAImage::set(int, int, TGAColor) 
  8.91      4.16     0.38 207000000     0.00     0.00  TGAColor::TGAColor(TGAColor const&) 
  1.64      4.23     0.07        2    35.04    35.04  TGAColor::TGAColor(unsigned char, unsigned char, unsigned char, unsigned char) 
  0.94      4.27     0.04                             TGAImage::get(int, int)
```
10%的时间花在了复制颜色上。但接着70%的时间花在调用line()上！那就是我们将要优化的地方。

## 第四次尝试继续
我们应该注意到每次除法都有相同的除数。让我们将其从循环中取出。误差变量给出了我们当前(x, y)像素到最佳直线的距离。每次误差大于一个像素时，我们就增加（或减少）y一个单位，并将误差也减少一个。

代码可以在这里找到。

```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    float derror = std::abs(dy/float(dx)); 
    float error = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error += derror; 
        if (error>.5) { 
            y += (y1>y0?1:-1); 
            error -= 1.; 
        } 
    } 
} 
```
这是gprof的输出：

```
%   cumulative   self              self     total 
 time   seconds   seconds    calls  ms/call  ms/call  name 
 38.79      0.93     0.93  3000000     0.00     0.00  line(int, int, int, int, TGAImage&, TGAColor) 
 37.54      1.83     0.90 204000000     0.00     0.00  TGAImage::set(int, int, TGAColor) 
 19.60      2.30     0.47 204000000
    0.00     0.00  TGAColor::TGAColor(int, int) 
  2.09      2.35     0.05        2    25.03    25.03  TGAColor::TGAColor(unsigned char, unsigned char, unsigned char, unsigned char) 
  1.25      2.38     0.03                             TGAImage::get(int, int) 
```
## 时间：第五次也是最后的尝试
我们为什么需要浮点数？唯一的原因是dx的除法和循环体中的0.5比较。我们可以通过用另一个变量替换误差变量来摆脱浮点数。让我们称它为error2，并假设它等于`error * dx * 2`。这是等效的代码：

```c
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
} 
```
```
%   cumulative   self              self     total 
 time   seconds   seconds    calls  ms/call  ms/call  name 
 42.77      0.91     0.91 204000000     0.00     0.00  TGAImage::set(int, int, TGAColor) 
 30.08      1.55     0.64  3000000     0.00     0.00  line(int, int, int, int, TGAImage&, TGAColor) 
 21.62      2.01     0.46 204000000     0.00     0.00  TGAColor::TGAColor(int, int) 
  1.88      2.05     0.04        2    20.02    20.02  TGAColor::TGAColor(unsigned char, unsigned char, unsigned char, unsigned char) 
```
现在，通过按引用传递颜色来移除不必要的复制（或者只需启用编译标志-O3），就完成了。代码中没有乘法或除法。执行时间从2.95秒减少到0.64秒。

我还建议检查这个问题。优化是棘手的！

## 线框渲染
现在我们准备好创建线框渲染了。你可以在这里找到代码的快照和测试模型。我使用了文件的wavefront obj格式来存储模型。我们渲染所需的一切是从文件中读取顶点数组，类型如下：

```
v 0.608654 -0.568839 -0.416318
```
是x,y,z坐标，每个文件行一个顶点和面

```
f 1193/1240/1193 1180/1227/1180 1179/1226/1179
```
我们感兴趣的是每个空格后的数字。这是我们之前读取的顶点数组中的顶点编号。因此，这行表示1193、1180和1179顶点形成一个三角形。注意，在obj文件中索引从1开始，意味着你应该分别查找1192、1179和1178顶点。model.cpp文件包含一个简单的解析器。将以下循环写入我们的main.cpp，瞧，我们的线框渲染器准备好了。

```c
for (int i=0; i<model->nfaces(); i++) { 
    std::vector<int> face = model->face(i); 
    for (int j=0; j<3; j++) { 
        Vec3f v0 = model->vert(face[j]); 
        Vec3f v1 = model->vert(face[(j+1)%3]); 
        int x0 = (v0.x+1.)*width/2.; 
        int y0 = (v0.y+1.)*height/2.; 
        int x1 = (v1.x+1.)*width/2.; 
        int y1 = (v1.y+1.)*height/2.; 
        line(x0, y0, x1, y1, image, white); 
    } 
}
```
下次我们将绘制2D三角形并改进我们的渲染器。
