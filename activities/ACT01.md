# Actividad 01

## Fecha Entrega, Conv1 = 15/12/19, Conv2 = 21/02/20

## EJ02

Implementar 6 ejercicios complementarios a AG02, basándose en la implementación de dicha AG02 vista en clase realizar:

- EJ02.01 - Pintar dos triángulos, uno al lado del otro (sin formar un quad, de esta forma " /\/\ "), usando glDrawArrays, añadiendo más vértices a los datos (usando un solo VBO).
- EJ02.02 - Pintar los dos mismos triángulos usando dos VAOs y VBOs diferentes para sus datos.
- EJ02.03 - Pintar los dos mismos triángulos usando un solo VAO, VBO y un EBO.
- EJ02.04 - Crear dos programs que pinten los triángulos del ejercicio anterior, que usen el mismo shader de vértices, pero usen un shader de fragmentos distinto para pintar cada triángulo, uno de cada color.
- EJ02.05 - Pintar un hexágono centrado en la pantalla, usando un VAO, VBO y EBO, generando los vértices de la manera habitual a mano.
- EJ02.06 - Pintar el mismo hexágono, pero generando los vértices de manera procedural (usando algún tipo de algoritmo), mediante coordenadas polares.

## EJ03

Implementar 4 ejercicios complementarios a AG03, basándose en la implementación de dicha AG03 vista en clase realizar:

- EJ03.01 - Dibujar el triángulo boca abajo, usando los vértices de los ejemplos anteriores, tocando solo el shader de vértices.
- EJ03.02 - Cambiar el color del triángulo con el paso del tiempo, usando un uniform para ello y la función glfwGetTime(), que mide el tiempo que ha transcurrido desde que se inició GLFW.
- EJ03.03 - Mover el triángulo a lo largo de la pantalla con el paso del tiempo, usando un uniform para ello.
- EJ03.04 - Pasar la posición del vértice al shader de fragmentos y pintar ese valor. Entender el significado y como se interpola, y escribir una breve descripción detallándolo.

Cada uno de los proyectos en ejecución mostrará una ventana pintando lo solicitado en cada uno.

Es necesario crear un Tag en el repositorio de Git con fecha anterior a la de expiración de la actividad, para justificar su ejecución, y enlazar dicho Tag a la actividad para evaluarla.