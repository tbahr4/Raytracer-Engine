<h1>Raytracer-Engine</h1>
<p>
  A simple CPU-based ray tracing engine using specular reflection, refraction, and diffuse lighting
  <p align=center>
    <img width="764" height="422" alt="main" src="https://github.com/user-attachments/assets/0f4e5712-651e-4b7f-addf-a8ab03292d28" />
  </p>
</p>



<h2>Overview</h2>
<p>
  This project implements a simple algorithm that demonstrates the basic fundamentals of CPU-based raytracing.
  The code uses SDL minimally for writing a pixel buffer to a window and stb_image.h for loading images. There are no other dependencies.
</p>



<h2>Features</h2>
<ol>
  <li>
    <p>
      Resolution downscaling - For improved framerates
    </p>
    <table>
    <tr>
      <td>
        <img width="1764" height="970" alt="downscale" src="https://github.com/user-attachments/assets/d8c4aad0-906e-4b2d-a390-4046de4f29bd" />
        <p align="center">Sample 2x Downscaling</p>
      </td>
      <td>
        <img width="1248" height="933" alt="downscale2" src="https://github.com/user-attachments/assets/6e288e02-edf8-4a60-b25e-74dc72351148" />
        <p align="center">Aggressive 8x downscaling</p>
      </td>
    </tr>
    </table>
  </li>
  <li>Multithreading - Uses a thread pool with specialized worker threads</li>
  <li>Custom material definitions - Color, reflectivity, transparency, refractivity</li>
  <li>
    Additional configurables
    <ul>
      <li>Field of view</li>
      <li>Screen size</li>
      <li>Input</li>
    </ul>
  </li> 
</ol>



<h2>Algorithm Details</h2>
<p>
  The algorithm implements a traditional ray tracing model, not path tracing. Path tracing methods (used in Blender and CGI) are not real-time and typically use Monte Carlo sampling to simulate realistic lighting behavior. With ray tracing, lighting is computed using much fewer explict rays typically used in real-time environments. Ray tracing deterministically computes each pixel color by casting rays from the camera to the world, whereas path tracing casts rays from each light, simulates bounces, and reaches the camera.

  The algorithm begins with a generation of all of the primary rays that are cast from the camera. A ray is spawned for each pixel to be rendered to the screen when downscaling is disabled.
  
  Once the rays are generated, each ray must be cast to determine the color of the pixel. Upon cast, the primary ray may collide with an object and will split into the 3 components: Diffuse, reflection, and refraction. In the case where there is no collision, the skybox is rendered. Each ray component is created to determine the total light that reaches the ray collision location. The final light contribution is then combined with the object's color and any additional material properties to produce the final rendered color of the pixel. Each of the 3 components are rays themselves and may further split into components recursively until a maximum ray depth is reached. 

  The contribution weighting of each of the 3 ray components is dependent on the properties of the material that the ray collides with. These properties mainly are reflectivity, transparency, and the material's refractive index.
  <i>Details about each ray component are below.</i>
</p>

<h3>Diffuse lighting</h3>
<p>
  Diffuse rays simulate light scattering that occurs after hitting a rough surface. Upon collision with an object, a diffuse ray is created for each contributing light source in the scene. Given a single light source, the diffuse ray emanates from the object toward the light. If the ray can reach the light without collision, the light contributes. If the ray collides with an object, the original object is in shadow. The light is completely shadowed if the collided object(s) opacity allows no light through itself. 
  
  Given a non-shadowed object, the incoming light intensity is determined based on the angle between the normal direction of the surface of the original object and the diffuse (light) ray. Based on the Lambert's cosine law, a surface's observed brightness decreases as the angle of the surface increases. This intensity is combined with the object's material color to determine the total diffuse contribution.

  The weight of the diffuse contribution is 100% minus the contribution of reflectivity and transparency. Reflectivity and transparency cannot sum to be over 100%.
  
  <p align=center>
    <img width="625" height="436" alt="diffuse" src="https://github.com/user-attachments/assets/16464edf-b141-4445-9c55-f34957cfb6e9" />
    <p align="center">Diffuse shadowing on an opaque object</p>
  </p>
</p>


<h3>Reflection</h3>
<p>
  Reflection rays simulates the light that bounces off a surface. Upon collision with an object, a reflection ray is created following the Law of Reflection: with origin at the collision and direction mirroring the normal of the surface and the original direction. From here, the reflection is cast as a new primary ray to determine the final color. The object that is collided with the new ray is essentially projected to the pixel's final color like a mirror.

  The weight of the reflection contribution is based on the reflectivity of the surface.
  
  <p align=center>
    <img width="795" height="442" alt="reflection" src="https://github.com/user-attachments/assets/70a15b4f-8ebb-4c69-9e53-dfd8760af49a" />
    <p align="center">Mirroring on a reflective surface</p>
  </p>
</p>

<h3>Refraction</h3>
<p>
  Refraction rays simulates light passing through transparent surfaces. Upon collision with an object, a refraction ray is created if the material has any amount of transparency. Following Snell's law, as a light passes through a medium, the path it takes changes dependent on the refractive index of the material. The direction of this ray changes both on entry and exit from the material. The initial ray may also experience total internal reflection (TIR). TIR occurs when a light's angle of incidence exceeds a critical angle, causing the light to be completely absorbed by the material as the refractive index of the material bends the light back into the object upon collision. This is typically seen on the edges of transparent materials.

  As the ray exits the material and changes direction once more, the ray becomes a new primary ray to determine the final color of the pixel.

  The weight of the refraction contribution is based on the transparency of the surface.
  
  <p align=center>
    <img width="570" height="428" alt="refraction" src="https://github.com/user-attachments/assets/1ee8513e-7780-4685-8bc8-0f9b5ebb1524" />
    <p align="center">Refraction in a semi-transparent glass-like surface</p>
  </p>
</p>











<h2>Potential Improvements</h2>
<ul>
  <li>GPU Acceleration</li>
  <li>Bounding volume hierarchy</li>
  <li>Triangle rendering for polygons</li>
  <li>Normal maps and textures</li>
  <li>Global illumination</li>
</ul>
