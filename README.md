## OpenGL_兔子模型

2022年秋季学期计算机图形学大作业  
实现对bunny_iH.ply模型的导入显示，以及通过鼠标左键点击选取模型上的点的拾取功能

---

### 编译方式
使用XMake编译即可

### 文件结构

|——include 包含目录  
|——resources 模型数据    
|——src 源代码  
&emsp;&emsp;|——main.cpp  模型文件  
&emsp;&emsp;|——learnopengl   自定义的类    
&emsp;&emsp;|——shader  着色器

---

### 功能描述
- 实现了bunny_iH.ply模型的读入与显示
  - 材质采用亮铜
- 实现了混合光源的光照
  - 定向光
  - 一个点光源（以及显示光源位置的方块）
  - 跟随摄像机镜头的聚光
- 键盘鼠标操作
  - WSAD控制前后左右
  - 空格键和左shift控制上下
  - tab键切换模型的显示方式（线框模式或填充模式）
  - 鼠标右键拖动控制视角
  - 鼠标左键点击拾取
- 拾取功能
  - 鼠标左键点击，可选取模型上的一个点
  - 选取的点以蓝色高亮显示
  - 选取点的世界空间坐标在命令行显示输出

---

### 参考教程
https://learnopengl-cn.github.io/