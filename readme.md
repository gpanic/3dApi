# 3dApi

3dApi is an application that benchmarks the performance of Direct3D and OpenGL. It includes a real time rendering engine that uses the phong lighting model, directional lights and shadowmapping.

## Test3D

The project Test3D performs the benchmarking by running several analogous Direct3D and OpenGL applications that each test a different use case. It records the delta time, the time it took to execute the update and render functions and how long the swap buffer operation took. The results are output to a text file for further analysis. Test3D also saves the first and last rendered image for later comparison.

![Test3D](http://i.imgur.com/1DaGeR8.png)

[Compiled Test3D application](https://github.com/gpanic/3dApi/releases/download/v1.0.0/Test3D.zip)

## Suzanne

The project Suzanne is used to demonstrate the rendering engine. Arrow keys can be used to view the model from different angles.

![Suzanne](http://i.imgur.com/RkbX3O3.png)

[Compiled Suzanne demo](https://github.com/gpanic/3dApi/releases/download/v1.0.0/Suzanne.zip)