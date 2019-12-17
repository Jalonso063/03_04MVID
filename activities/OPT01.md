# Optativa 01, Prueba técnica Electronic Arts

## Fecha Entrega, Conv1 = 09/02/20

## 01.Eliminar Elementos de un Vector STL

Hay al menos un bug en el siguiente código, por favor, arréglalo.

```cpp
// Modify the timers array in-place to remove all expired timers
void RemoveExpiredTimers(std::vector<Timer> timers)
{
	for (auto it = timers.begin(), end = timers.end();
		it != end;
		++it)
	{
		if (it->IsExpired())
			timers.erase(it);
	}
}
```

## 02.Leer de un Stream
Implementa el método Stream::ReadAll en la clase siguiente.

```cpp
class Stream
{
public:
	// Implemented by subclasses; reads up to `bytes` bytes into `buf`, returns 
// number of bytes actually read.  If the return value is less than requested 
// `bytes`, the end of the stream has been encountered.
	virtual size_t Read(char* buf, size_t bytes) = 0;

	// Candidate to implement: return a vector with all bytes in the file.  Note 
// that the size of the stream is unknown.
	std::vector<char> ReadAll();
};
```

## 03.Animación de una Puerta
Según el fichero adjunto 03.Door.cpp , implementa el método Door::Update, para que la puerta se abra cuando los personajes se acerquen, y se cierre detrás de ellos. La puerta nunca debería dar un portazo entre animaciones (nunca empezar una animación mientras otra está en marcha).
Implementa cualquier función, método, constructores o miembros de clases adicionales que sean necesarios.

## 04.Mover Gente
En el fichero adjunto 04.MovePeople.cpp, el método AI::MovePeople() se llama en cada frame y se ha determinado que se ejecuta muy lentamente. La lista de personas de la AI no añade ni elimina gente durante la ejecución normal del juego.
Enumera las optimizaciones que considerarías en la función AI::MovePeople() y describe cada optimización junto con el objetivo perseguido. Se debería optimizar para maximizar el rendimiento de la CPU así como minimizar el uso de memoria. Si hay que elegir entre ambos, el rendimiento de la CPU debería ser considerado más importante. Asume que el programa se ejecutará en un Pc Intel en un entorno mono-hilado.

## 05.Memory Problems
Adjunto el fichero 05.MemoryProblems.cpp. Se adjunta una captura de pantalla de la memoria en el heap en algún punto de la ejecución del juego, con cada fila correspondiente a un bloque de memoria.

- size es el tamaño requerido de la asignación en bytes (si Kind está USED) o de los bytes libres (si kind es FREE).
- kind es el tipo de bloque.
- timestamp es el momento en el cual se hizo la asignación de memoria
- label es una cadena describiendo la asignación

### Información Adicional:
- Las asignaciones de memoria están alineadas a 16 bytes (cada petición de asignación esta alineada al siguiente limite de 16 bytes en memoria).
- Hay un gasto extra de 16 bytes (overhead) por cada asignación.
- Sizeof() reporta los siguientes tamaños:
  - Size of MonsterT = 120
  - Size of TreasureT = 72
- Uint8_t, char8_t y Bool tienen longitud de 1 byte
- Float es un valor en coma flotante de 32 bits, Uint32_t es un entero sin signo de 32 bits.
- Las asignaciones de los objetos deben ser dinámicas.
- Las asignaciones de “MONSTER_MODEL” y “ENVIRONMENT” no se pueden reducir en tamaño.

### Pregunta A
El diseñador del juego ha pedido un incremento en el número máximo de monstruos de 3. La librería de memoria está reportando que, en algún momento del tiempo, el número de bytes disponibles es inferior a 128. Asumiendo que no puedes incrementar el tamaño de la memoria del heap, ¿qué se puede cambiar sobre la estrategia de asignación de memoria del juego para cumplir esa petición que no requiera modificar los tamaños de las estructuras de datos? También existe una preocupación por la fragmentación de la heap de memoria. ¿Cómo modificarías el código para hacer un uso más eficiente de la heap?

### Pregunta B
¿Hay alguna optimización de memoria que se pueda realizar para reducir el coste (tamaño) del objeto monstruo?


