## Week Of OpenGL.

My week of learning OpenGL using tutorials from FreeCodeCamp and official documentation from the OpenGL website.

## Current File Structure.

![file](C:\Users\omanb\Downloads\file.PNG)

## Fragment Shaders and Vertex  Shaders.

Here are the contents of the fragment shader;

##### Fragment Shaders are in-charge of color and texture of an object.

```c++
#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;


void main()
{
	FragColor = vec4(color, 5.0f);
}
```

Here are the contents of the vertex shader;

##### Vertex Shaders are in-charge of  shape and orientation of an object.

```c++
#version 330 core

//Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;


// Outputs the color for the Fragment Shader
out vec3 color;
// Controls the scale of the vertices
uniform float scale;


void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	color = aColor;
}
```

