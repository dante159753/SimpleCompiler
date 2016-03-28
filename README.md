# SimpleCompiler

本编译器主程序使用c编写，用户界面和自动生成代码的程序使用java编写，生成语法树的程序使用python编写，运行之前请确保环境配置完毕

## 运行环境配置

### 1.编译主程序
编译过程在windows下的 gcc4.6.3 以及linux下的 gcc4.8.4 中测试成功

编译命令：

`gcc driver.c parser.c util.c symtable.c interpreter.c lex.yy.c -o executor`

### 2.UI界面
在java1.8.0_66中测试成功

需要将上一步编译得到的executor.exe复制到与SimpleIDE.jar相同目录中，
才可以正常进行分析与执行(提交的文件夹中已经包含了一个编译好的executor.exe)

要使用可视化语法树的功能，需要配置好第3步的python环境
jre配置正确后，双击SimpleIDE.jar即可运行
源代码在 java-source/CompilerUI/中


### 3.生成语法树
需要两个部分：graphviz库 和 graphviz库的python接口

graphviz官网:[http://www.graphviz.org/](http://www.graphviz.org/) 可以在此下载安装

python需要的包已导出至 `requirements.txt`

python安装完毕后在命令行中执行 `pip install -r requirements.txt` 安装必备包

## 使用简介

### 使用UI界面
本程序实现了一个简单的IDE，可以一边编写代码一边处理高亮，左下角显示当前光标位置

进入程序后，默认处于词法分析模式，上方一排按钮的前三个用于模式切换
* 词法分析：只进行词法分析
* 词、语法分析：进行词法分析和语法分析
* 所有分析：词法、语法、语义分析
分析中如果发现程序有错误，会将错误部分涂成红色，并在下方输出栏中输出错误类型和位置

点击`执行`会进行所有分析并执行程序，如果分析中发现错误则不会执行代码。

　　执行中会检查类型，若有将double赋给int的语句，则将此变量涂成黄色，在下方输出警告，并继续执行

点击`可视化语法树`会进行一次语法分析并将生成的语法树以图形方式展示出来(需要配置好graphviz和python环境)

点击`生成随机代码`会将当前代码编辑区中的代码更换为程序生成的随机代码

### 使用命令行调用编译器

注：下例中的prog.txt为需要分析的程序文件

`executor.exe -scanning < prog.txt` 只进行词法分析，有错误则输出类型和位置

`executor.exe -parsing < prog.txt` 进行词法和语法分析，有错误则输出类型和位置

`executor.exe -analyze < prog.txt` 进行词法、语法和语义分析，有错误则输出类型和位置

`executor.exe < prog.txt` 进行所有分析并执行程序
