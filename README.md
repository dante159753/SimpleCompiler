# SimpleCompiler

* `globals.h`: 定义用到的结构体和枚举值
* `driver.c`:			程序入口，接受命令行参数并调用相应的分析函数
* `lexical_processor.L`:	lex文件，用于生成词法分析器代码
* `lex.yy.c`:			生成的词法分析器代码
* `parser.c`:			语法分析器代码
* `util.h`:				工具函数头文件
* `util.c`:				工具函数实现
* `symtable.h`:			符号表头文件
* `symtable.c`:			符号表实现
* `interpreter.h`:		语义分析及解释器头文件
* `interpreter.c`:		语义分析及解释器实现

## 编译
编译过程在windows下的 gcc4.6.3 以及linux下的 gcc4.8.4 中测试成功

编译命令：

`gcc driver.c parser.c util.c symtable.c interpreter.c lex.yy.c -o executor`


## 使用简介

注：下例中的prog.txt为需要分析的程序文件

`executor.exe -scanning < prog.txt` 只进行词法分析，有错误则输出类型和位置

`executor.exe -parsing < prog.txt` 进行词法和语法分析，有错误则输出类型和位置

`executor.exe -analyze < prog.txt` 进行词法、语法和语义分析，有错误则输出类型和位置

`executor.exe < prog.txt` 进行所有分析并执行程序
