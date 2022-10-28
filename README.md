# PGP_OpenGL

## Description

This <b>Prodecural Generation Project</b> is a graphics programming project made with C++ and OpenGL. The user is able to configure settings to generate Minecraft-inspired landscapes containing multiple settings that differ in vegetation and geographical aesthetics.
The application reads in noise visuals to then generate and render various cube-based entities each with different textures in accordance to the given vegetation. PGP was a great opportunity to dive deeper into more low-level concepts such as rendering, performance optimisation, memory management and multithreading.

Project Description on my <a href="https://michael-gebhart.com/projects/PGP.html">portfolio website</a>.
Project and Documentation on my <a href="https://drive.google.com/drive/folders/1D6GbXNsoCSvGSosJ7Im4Ap7GzGTJWdXM">Google Drive</a>.

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

Pls note: on the first time generation, the landscape can be loaded right behind "you".

## Media

![pgp_anim](https://user-images.githubusercontent.com/45672199/198697520-18d20623-cddd-4916-8bb9-0985701497c3.gif)
