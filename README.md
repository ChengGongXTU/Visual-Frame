VISUAL-FRAME: a simple c++ render.

Hi，I have created a render framework base on c++ and Window GDI, not need any other graph API and Lib!

This render consist of geometry part, camera part and render part, including som math operators and camera transforms.

The gemetry part is build basing on "pbrt"(writing by MATT PHARR and GREG HUMPHREYS). I will add more function into render part.

Update：

2017.3.17：

Light and surafce shading is coming!

1,Add a "light" class: a point light source, include diffuse, ambient and Phong shading.

2,Add a render pipelin function in "render" part: you can bind each triangle with the specified camera and light source.


