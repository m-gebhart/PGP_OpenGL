# PGP_OpenGL

## Description

This <b>Prodecural Generation Project</b> is a graphics programming project made with C++ and OpenGL. It is a solo project created as part of my seventh semester. The user is able to set configurations to generate Minecraft-inspired landscapes containing multiple settings that differ in vegetation and geographical aesthetics.
The application reads in noise visuals to then generate and render various cube-based entities each with different textures in accordance to the given vegetation. PGP was a great opportunity to dive deeper into more low-level concepts such as rendering, performance optimisation, memory management and multithreading.

Project page on my <a href="https://michael-gebhart.com/projects/PGP.html">portfolio website</a>.
Full executable project (.zip) and documentation (.pdf) on my <a href="https://drive.google.com/drive/folders/1D6GbXNsoCSvGSosJ7Im4Ap7GzGTJWdXM">Google Drive</a>.

### Features

<ul>
  <li>Procedural Generation</li>
  <ul>
  <li>Landscape can be generated, reset and regenerated on simple button presses</li>
  <li>Generation settings can be defined in configuration file (e.g., world size, background color, enabled snow, animation settings)</li>
  <li>Geographical systems dependent on surrounding cubes (e.g., sand close by water, rocks at steep descents, snow with higher position)</li>
  </ul>
  <li>Rendering</li>
  <ul>
  <li>Efficient rendering of landscape by using multithreading and hollow cube groups</li>
  <li>Camera can freely navigate through the scene</li>
  <li>Tiles support alpha channel and can be replaced in Ressources folder</li>
  </ul>
</ul>

## Instructions

<ol>
<li>Download the .zip-file from my <a href="https://drive.google.com/drive/folders/1D6GbXNsoCSvGSosJ7Im4Ap7GzGTJWdXM">Google Drive</a>.</li>
<li>Unzip the file and open the project via the .sln-file with a supported IDE (e.g., MS Visual Studio).</li>
<li>Execute the project - be sure to be on platform x86!</li>
<li>In the console application, type in the landscape size (e.g., 55) to open the render window.</li>
<li>In the render window, Press G to generate the landscape.</li>
<li>Use the following <b>Controls</b> to navigate through the scene:</li>
<ul>
  <li><b>G</b> to generate a new landscape</li>
  <li><b>R</b> to remove the currently displayed landscape</li>
  <li><b>Mouse</b> to control camera view</li>
  <li><b>1</b> and <b>2</b> to change camera speed</li>
</ul>
<li>Additionally: Project Settings can be defined in PGP_Config.h.</li>
</ol>

Pls note: after starting the scene and triggering the first generation, the landscape can be loaded right behind "you".

## Media

#### Example of subsequent generations (with animation - can also be turned off):

![pgp_anim](https://user-images.githubusercontent.com/45672199/198697520-18d20623-cddd-4916-8bb9-0985701497c3.gif)

#### Other results:
![grafik](https://user-images.githubusercontent.com/45672199/200533831-f03625b8-94a9-4ec2-9ba4-3e20f9163a88.png)
![grafik](https://user-images.githubusercontent.com/45672199/200553897-7d9b8ced-753c-4171-84ad-2248f43111b1.png)
![grafik](https://user-images.githubusercontent.com/45672199/200553925-d78f3f8f-9035-4f64-9033-e51f0676ee1b.png)


