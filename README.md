# f00l_llvm_learn
- 记录一下llvm优化学习的过程
- 课程是cscd70
<details>
<summary>lec1</summary>
  
## lec1
### basic block
- basic block的标志
  - 第一条指令
  - 一个jmp跳转的target
  - 一个紧跟着jmp的指令
### local optimization
- 本地优化，是在basic block里进行的
- 减少相同表达式的计算次数
  - build DAG
    - 表达式都有一个语法树
    - 建立DAG就是合并相同的结点，然后再根据树合并得到的图自底向上来计算表达式
    - 缺点：
      - 依赖于表达式值的计算，中间有表达式计算慢了的话就会拖慢后面指令运行的时间
  - value-number
    - 为每一个value都分配一个number，然后让var(变量)去对应那个value
    - 算法
```
Data structure:
VALUES = Table of
expression //[OP, valnum1, valnum2}
var //name of variable currently holding expression
For each instruction (dst = src1 OP src2) in execution order
valnum1 = var2value(src1); valnum2 = var2value(src2);
IF [OP, valnum1, valnum2] is in VALUES
v = the index of expression
Replace instruction with CPY dst = VALUES[v].var
ELSE Add
        expression = [OP, valnum1, valnum2]
        var        = dst
     to VALUES
v = index of new entry; tv is new temporary for v
Replace instruction with: tv = VALUES[valnum1].var OP VALUES[valnum2].var
dst = tv;
set_var2value (dst, v)
```
     - 例子
```
Assign: a->r1,b->r2,c->r3,d->r4
a = b+c;  ADD t1 = r2,r3 CPY r1 = t1 
b = a-d;  SUB t2 = r1,r4 CPY r2 = t2 
c = b+c;  ADD t3 = r2,r3 CPY r3 = t3 
d = a-d;  SUB t4 = r1,r4 CPY r4 = t4
```
- 在编译时可以计算出数值的时候，用常量替换之
### global optimization
- loop optimization
  - 减少每个循环内执行的指令次数
- global version of local optimization
  - 减少全局相同表达式的计算次数
    - 减少变量的使用次数，计算过一次的变量就不再计算
    - eg:
```
优化前
B1: i := n-1
B2: if i<1 goto out 
B3: j := 1
B4: if j>i goto B5 
B6: t1 := j-1
    t2 := 4*t1
    t3 := A[t2]     ;A[j]
    t6 := 4*j
    t7 := A[t6]     ;A[j+1]
    if t3<=t7 goto B8
|B7: t8 :=j-1 
|    t9 := 4*t8
|    temp := A[t9] ;temp:=A[j] 
|    t12 := 4*j
|    t13 := A[t12] ;A[j+1] 
|    A[t9]:= t13 ;A[j]:=A[j+1] 
|    A[t12]:=temp ;A[j+1]:=temp
B8: j := j+1 goto B4
B5: i := i-1 goto B2
out: 
```
```
优化后
B1: i := n-1
B2: if i<1 goto out
B3: j := 1
B4: if j>i goto B5
B6: t1 := j-1
|B7: A[t2] := t7
|    A[t6] := t3
B8: j := j+1
    goto B4
    t2 := 4*t1
    t3 := A[t2]   ;A[j]
    t6 := 4*j
    t7 := A[t6]   ;A[j+1]
    if t3<=t7 goto B8
B5: i := i-1 goto B2
out:
```
    - 可以看到优化后，利用了之前汇编就计算出的临时变量值来使用，无需再次计算
    - 优化的部分我用|标记出来了
- loop optimization
  - 尽量用加法替换乘法
  - 对循环的下标，尽量用其他已经有的变量来代替，这样就无需再次计算
```
优化前
B1: i := n-1
B2: if i<1 goto out
|B3: j := 1
|B4: if j>i goto B5
|B6: t1 := j-1
|    t2 := 4*t1
|    t3 := A[t2]     ;A[j]
|    t6 := 4*j
|    t7 := A[t6]     ;A[j+1]
|    if t3<=t7 goto B8
B7: A[t2] := t7
    A[t6] := t3
|B8: j := j+1
|    goto B4
B5: i := i-1 goto B2
out:
```
```
优化后
B1: i := n-1
B2: if i<1 goto out 
|B3: t2 := 0
|    t6 := 4
|B4: t19 := 4*I
|    if t6>t19 goto B5
|B6: t3 := A[t2]
|    t7 := A[t6] ;A[j+1] 
|    if t3<=t7 goto B8
B7: A[t2] := t7
    A[t6] := t3
|B8: t2 := t2+4
|    t6 := t6+4
|    goto B4 
B5: i := i-1
    goto B2 
out:
```
  - 可以看到优化后，乘法由两次变成一次，然后用统一的变量来代替数组的index和循环的变量
</details>
<details>
<summary>LLVM的坑</summary>

## 源码外编译llvm pass

- 首先要新建一个<project>文件夹，然后在里面建一个写Pass的文件夹，<project>里的CMakeLists.txt要这么写
  
```c
  
cmake_minimum_required(VERSION 3.16) #说明cmake的最小版本
project(assignment1)  #项目名称
set(CMAKE_CXX_STANDARD 11) #设置c++版本
set(LLVM_HOME ~/llvm/build) #设置llvm的位置
set(LLVM_DIR ${LLVM_HOME}/lib/cmake/llvm) #设置cmake的位置
find_package(LLVM REQUIRED CONFIG) #找到llvm-config
add_definitions(${LLVM_DEFINITIONS}) 
include_directories(${LLVM_INCLUDE_DIRS}) #将llvm的include文件夹包含进来
link_directories(${LLVM_LIBRARY_DIRS}) #将llvm的library文件夹包含进来

MESSAGE(${LLVM_LIBRARY_DIRS})
MESSAGE(${LLVM_INCLUDE_DIRS})

add_subdirectory(FunctionInfo) #将pass的文件夹加入

```
- 然后是pass文件夹里的

```

set(CMAKE_CXX_STANDARD 11)
add_library(FunctionPass MODULE
        FunctionInfo.cpp
        )

set_target_properties(FunctionPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )

if(APPLE)
    set_target_properties(FunctionPass PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)

```

- 这样就能源码外编译pass了
- 最开始的时候我没加if(APPLE)那个，所以每次build都会出错

- 还有一个是自行编译出来的pass用opt加载会出现Symbol Not Found
- 这是因为我的电脑用的opt是自带的，版本比较低，从而导致符号和我自行编译的llvm不一样所导致的
- 在环境变量里加一行包含llvm/bin路径的语句就可以了

## 在项目中include llvm的库
- 在CMakeLists里这么写

```

cmake_minimum_required(VERSION 3.16)
project(kaledoscope_front)

set(CMAKE_CXX_STANDARD 14)
set(LLVM_HOME ~/llvm/llvm-project/build)
set(LLVM_DIR ${LLVM_HOME}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_executable(kaledoscope_front main.cpp)

llvm_map_components_to_libnames(llvm-config --cxxflags --ldflags --system-libs --libs core mcjit native)
target_link_libraries(kaledoscope_front ${llvm_libs})
link_directories(${LLVM_LIBRARY_DIRS})

set_target_properties(kaledoscope_front PROPERTIES
        COMPILE_FLAGS "-fno-rtti "
        )
if(APPLE)
    set_target_properties(kaledoscope_front PROPERTIES
            LINK_FLAGS "-undefined dynamic_lookup"
            )
endif(APPLE)

```

</details>
