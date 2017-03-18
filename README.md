VISUAL-FRAME: a simple c++ render.

Hi，I have created a render framework base on c++ and Window GDI, not need any other graph API and Lib!

This render consist of geometry part, camera part and render part, including som math operators and camera transforms.

The gemetry part is build basing on "pbrt"(writing by MATT PHARR and GREG HUMPHREYS). I will add more function into render part.

Update：

2017.3.19:

I put some parameters about radiance into shape part, such as Vertex class. So the vertex can reflect some material feature easily.

1,Add diffuse, Phong reflection and emission parameter into vertex class, the vertex can be a material surface point or point light source.

2,Chane the function of light model. The light class can only control luminous intensity, phong reflection pow and ambient light now.


2017.3.17：

Light and surafce shading is coming!

1,Add a "light" class: a point light source, include diffuse, ambient and Phong shading.

2,Add a render pipelin function in "render" part: you can bind each triangle with the specified camera and light source.


