# Actividad 03

## Fecha Entrega, Conv1 = 12/01/20, Conv2 = 21/02/20

## EJ05

Implementar 6 ejercicios complementarios a AG05, basándose en la implementación de dicha AG05 vista en clase realizar:

- EJ05.01 - Generar un cubo de manera procedural, a partir de un centro y un radio.
- EJ05.02 - Cambiar las UVs del cubo, para soportar el unwrapping completo, y pintar el cubo con la textura adjunta.
- EJ05.03 - A partir de la actividad AG_05, en la escena con el cubo rotando, invertir el orden de la aplicación de las transformaciones (traslación y rotación) y verificar que el orden de aplicación es relevante.
- EJ05.04 - Crear una escena con un suelo, y varios cubos apoyados en él, a diversas alturas, distancias y tamaños.

## EJ06

Implementar 2 ejercicios complementarios a AG06, basándose en la implementación de dicha AG06 vista en clase realizar:

- EJ06.01 - Sobre la escena del ejercicio 05.04, hacer que la cámara no permita volar y se comporte como una cámara FPS en la cual el personaje siempre estuviera andando sobre el suelo.
- EJ06.02 - Implementar la función LookAt, y no usar la de GLM.

## EJ07

Implementar 5 ejercicios complementarios a AG07, basándose en la implementación de dicha AG07 vista en clase realizar:

- EJ07.01 - Mover la luz alrededor de la esfera describiendo un circulo sobre el eje Y.
- EJ07.02 - Cambiar valores de ambient, diffuse, specular y shininess y ver como se comporta el modelo de iluminación.
- EJ07.03 - Hacer el modelo de iluminación Phong en espacio de view, en lugar de en espacio de mundo.
- EJ07.04 - Implementar el modelo de iluminación Gouraud (Phong en el shader de vertices).
- EJ07.05 - Montar una escena con tres esferas y una luz, la primera esfera tiene un modelo de iluminación faceteado (solo difuso), la segunda tiene un modelo Gouraud y la tercera un modelo Phong (que acabe viendose algo parecido a la imagen adjunta).

## EJ08

Implementar 5 ejercicios complementarios a AG08, basándose en la implementación de dicha AG08 vista en clase realizar:

- EJ08.01 - Simular varios objetos reales con sus materiales según la tabla en http://devernay.free.fr/cours/opengl/materials.html (tener en cuenta que esos valores están calculados para luces blancas vec3(1.0) 
- EJ08.02 - Cambiar valores de ambient, diffuse, specular de la luz y ver como afecta a como se ve la esfera.
- EJ08.03 - Invertir el efecto del mapa specular, para que se vean más brillantes las zonas oscuras y viceversa.
- EJ08.04 - Añadir un mapa emisivo, es una textura que almacena valores de color emisivo por fragmento. Son colores que un objeto emite como si tuviera una luz interna, de esta manera un objeto puede emitir luz independientemente del origen de luz. Usar como textura emisiva la imagen adjunta. Usarla para tener un tercer mapa en el modelo de iluminación y pintar sobre la esfera iluminada.
- EJ08.05 - Intentar crear un mapa specular (con algún editor de imagenes Photoshop, Gimp, ...) a partir de un mapa difuso que en lugar de blanco y negro use colores reales, y usar ese mapa en el Phong en lugar del specular habitual y verificar el cambio de comportamiento.

Cada uno de los proyectos en ejecución mostrará una ventana pintando lo solicitado en cada uno.

Es necesario crear un Tag en el repositorio de Git con fecha anterior a la de expiración de la actividad, para justificar su ejecución, y enlazar dicho Tag a la actividad para evaluarla.