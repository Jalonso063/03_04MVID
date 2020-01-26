# Actividad Final

## Fecha Entrega, Conv1 = 09/02/20, Conv2 = 21/02/20

Se debe usar el motor desarrollado durante las clases, y será necesario ampliarlo como se ha mostrado en la segunda parte de la asignatura (grafo de escena, main loop, ...).

El gameplay es libre (se proveen dos propuestas), pero en cualquier caso debe ser un juego sencillo que permita centrarse en implementar y demostrar que el motor desarrollado durante la asignatura es competente. Asimismo debe ser lo suficientemente amplio para dar la opción a mostrar todas las técnicas gráficas aprendidas. El juego deberá ser jugable un mínimo de 5 minutos (evitar memory leaks, corrupción de memoria, ....). Los modelos 3D se pueden conseguir fácilmente de las webs de recursos vistas durante el curso.

Es necesario crear un Tag en el repositorio de Git con fecha anterior a la de expiración de la actividad, para justificar su ejecución, y enlazar dicho Tag a la actividad para evaluarla. Se adjuntará también un pequeño documento con la arquitectura de motor empleada con su diagrama, y un postmortem del desarrollo (https://blog.codinghorror.com/game-development-postmortems). Se recomienda adjuntar un pequeño video del juego en ejecución por si fuera imposible recompilar el proyecto por alguna causa.

## Requisitos

- Modelo de iluminación Blinn-Phong
- Uso de mapas difusos y speculares
- Correcto uso de luces, direccionales, point o spot lights
- Usar algún modelo complejo cargandolo con Assimp
- Usar normal mapping
- Implementar sombras

## Propuestas

## 01.3D Arkanoid

https://es.wikipedia.org/wiki/Arkanoid

El juego mínimo consistirá en tres paredes (dos verticales a los lados, y una superior) que delimitan la zona de juego, una pelota que se mueve de manera continua con una velocidad, una paleta controlada por el usuario mediante el teclado (se mueve solo de izquierda a derecha) y una serie de ladrillos dispuestos en la parte superior. El objetivo del juego es destruir todos los ladrillos golpeandolos con la pelota, haciendola rebotar con la paleta. Si la pelota rebasa la paleta en la parte inferior el jugador pierde una vida.El jugador tiene un máximo de 3 vidas y cuando se queda sin vidas el juego acaba. Cuando la pelota rebota en la paleta está cambia de ángulo en función del lado de la paleta donde golpea. Cuando la pelota impacta en un ladrillo este desaparece y el jugador gana cuando no queda ningún ladrillo en la escena. Los rebotes contra las paredes y los ladrillos deben ser realistas.

Posibles ampliaciones:
- La velocidad de la pelota se incrementa con el paso del tiempo aumentando la dificultad
- Enriquecer el escenario 3D (fondos, ladrillos con texturas, ....)
- Hacer que la pelota sea la luz
- Powerups que caen de los ladrillos al destruirlos y que hacen que el jugador obtenga alguna ventaja (paleta más grande, más de una pelota en pantalla, ....)
- Implementar un sistema de puntuación
- Sistema de vidas para el jugador
- Mostrar textos 2D en pantalla con la puntuación o vidas restantes
- Ladrillos de distintos tipos según el color (necesitan varios golpes para ser destruidos, se mueven,...)
- Distintos niveles con distintas distribuciones de ladrillos
- Freestyle!!! Be creative!!!

## 02.Shoot'em Up

Implementar un juego de naves con gráficos en 3D y mecánicas en 2D con scroll vertical/horizontal, con mecánicas, sistema de juego y perspectiva similares a Terra Cresta 3D https://www.youtube.com/watch?v=kmcxF_OIoew

La mecánica del juego permitirá al jugador manejar una nave que sobrevuela un terreno, y donde aparecen naves enemigas que le disparan. El jugador podrá disparar y si sus disparos alcanzan las naves enemigas estas se destruirán. También aparecerán algunos meteoritos que no podrán ser destruidos y el jugador deberá esquivarlos. Todos los elementos de juego (jugador, enemigos, meteoritos) están en el mismo plano sobre el suelo. Si el jugador es alcanzado por un proyectil enemigo o un meteorito entonces su nave es destruida y pierde una vida. El juego deberá mostrar una nave 3D para el jugador, al menos dos tipos de enemigos distintos (con naves distintas), al menos un tipo de meteorito, y un suelo. Las naves deberán mostrar algo de deriva cuando se muevan lateralmente (roll), pero siempre se moverán en el mismo plano 2D. Los meteoritos deberán rotar sobre sí mismos para dar la impresión de movimiento al acercarse al jugador. Los proyectiles del jugador y de los enemigos pueden ser formas simples o modelos complejos. La nave del jugador no podrá salir de la pantalla visible en el eje X, y solo un poco de libertad en el eje Y.

Posibles ampliaciones:
- Los movimientos de la nave del jugador pueden tener cierta inercia, para simular el uso de físicas
- Al explotar los enemigos o la nave del jugador se puede implementar un sistema de partículas para simular una explosión o la desintegración de las naves
- Implementar un sistema de puntuación por naves destruidas
- Sistema de vidas para el jugador
- Mostrar textos 2D en pantalla con la puntuación o vidas restantes
- Sistema de powerups, al eliminar ciertas naves especiales (o un conjunto de ellas), pueden dejar algún item, que al cogerlo el jugador mejora su velocidad o capacidad de disparo
- Implementar oleadas de enemigos, que sigan ciertas trayectorias simples o incluso con curvas beizer o splines
- Montar un enemigo final más grande y poderoso
- Varios tipos de disparo, balas, rayos, ...
- Hacer que algunos enemigos al ser alcanzados en lugar de explotar caigan al suelo y exploten allí
- Freestyle!!! Be creative!!!