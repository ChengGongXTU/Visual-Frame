# VISUAL-FRAME: a simple c++ render.  
  
The mission of this project is complete!  
--------------------------------  
  
I have learned much of things from building this project. Now, I have more knowledge and experience about computer graphic, it's time to end this project and begin new project to learn some recent CG technology.  
-------------------  

Most functions of this project will move into "SeaAsia" project, it's a DX11 + CPU Ray-Trace render software, and it has a convenient UI to controll the render pipeline and ray trace function.
-----------------


Hi，I have created a render framework base on c++ and Window GDI, not need any other graph API and Lib!


This render consist of geometry part, camera part and render part, including som math operators and camera transforms.

The gemetry part is build basing on "pbrt"(writing by MATT PHARR and GREG HUMPHREYS). I will add more function into render part.


HOW TO USE IT? 

Put it into visual studio 2015，and write function in "window.cpp"'s WndMain function, and run it.

Maybe I will create a menu and some button in main window, but I need to write more fundamental function before.


Update：

2017.4.7:

In the last week, I studied some non-photorealistic rendering literature about silhouette edges extraction, and I choose the paper: "Fast High-Quality Line Visibility" (by Forrester Cole and Adam Finkelstein) to implement. The result looks good.
(see "Cartoon+Silhouette.jpg").


1, Add "Contour" part, including some function: Line-Draw, Front-Face Detection, Silhouette Detection, and Silhouette-Draw(Cole and Finkelstein 2009), I implement these function by c++, not using OpenGL or DX.




2017.3.31：

Now, you can set texture position for each vertex, and load bmp image, to binding with Triangle mesh. You can also use some software to build obj model with bmp-texture, and input them into Visual-Frame to render.

1,Texture mapping can be implemented by this Process: create texture Instance -> load texture-> binding it with triangle mesh->render it.

2,Set a parameter named "flag" in TriangleMesh class, flag == 0 is vertex-color style, flag == 1 is texture mapping style, 
and flage == 2 is a cartoon style witch is proposed by Jeff Lander on 'game developer magazine'  in March, 2000.


2017.3.29:

Recently, I find the ray trace efficiency of my code is tooooooooooo low!!!!!!!!!!! I will optimize my data structure and GDI aprt.

I also think that the parallel computing of GPU is a attractive method to improve the  ray trace and real-time render's efficiency.

Now. it's time to pick up DirectX, and extend new functions!


1，Add a LoadObj function to load ".obj" model, and return a triangle mesh. Now, ray trace and renderpipeline can  handle multiple triangle meshes.

2, Add OpenMP to compute path trace algorithm.

3, I discard GDI's setpixel, and combine backbuffer with DIB to show the image.
 

2017.3.23

I create a radiance part and a pathtracing part to  implement ray trace algorithm. To solve the image noise, I read Kevin Beason's "smallpt: Global Illumination in 99 lines of C++" (http://www.kevinbeason.com/smallpt/), and add Russian roulette method into recursion.

1, Creat a "Cornell Box" in "Window.cpp"

2,Add a radiance part, including interpolation equation to compute the property of each point, such as emission and diffuse parameters.

3,Add a path trace part. The tracing depth, sample number, triangle array, pixel number, subpixel number and so on, can be imported to
this pathtrace function.

4,Change Vertex class, it has reflection type now.

5,Add a 400x400 pixels "Cornell Box" image by seting 5 depth, 2x2 subpixel per pixel, and 40 samples per subpixel. 



2017.3.19:

I put some parameters about radiance into shape part, such as Vertex class. So the vertex can reflect some material feature easily.

1,Add diffuse, Phong reflection and emission parameter into vertex class, the vertex can be a material surface point or point light source.

2,Chane the function of light model. The light class can only control luminous intensity, phong reflection pow and ambient light now.



2017.3.17：

Light and surafce shading is coming!

1,Add a "light" class: a point light source, include diffuse, ambient and Phong shading.

2,Add a render pipelin function in "render" part: you can bind each triangle with the specified camera and light source.


