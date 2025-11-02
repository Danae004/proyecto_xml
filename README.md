# 🗂️ Proyecto Librería XML

Una librería simple para crear y manipular documentos XML en C++.

## 📖 ¿Qué es esto?

Esta librería te permite **crear archivos XML** de forma programática. Piensa en ella como una **caja de herramientas** para organizar información de manera estructurada.

### 🔍 Ejemplo de XML
```xml
<contactos>
  <persona id="1">
    <nombre>María García</nombre>
    <telefono>555-1234</telefono>
    <edad>25</edad>
  </persona>
</contactos>

## Archivos del Proyecto
|--|--|
|Archivo	|¿Qué hace?|
|XmlNode.hpp	|Define las piezas básicas (elementos y atributos)|
|XmlNode.cpp	|Lógica interna de los elementos XML|
|XmlDoc.hpp	|Controlador principal del documento|
|XmlDoc.cpp	|Implementa todas las funciones|
|main.cpp	|Ejemplos y pruebas de uso|

## Probar

# Compilar
g++ -std=c++11 XmlNode.cpp XmlDoc.cpp main.cpp -o xml_programa

# Ejecutar (Linux/Mac)
./xml_programa

# Ejecutar (Windows)
xml_programa.exe

### Crear un documento XML
#include "XmlDoc.hpp"

int main() {
    // Crear documento con nodo raíz
    XmlDoc doc("biblioteca");
    
    // Agregar un libro
    int libroId = doc.addChild(0, "libro");
    doc.addAttribute(libroId, "año", "2023");
    
    // Agregar título
    int tituloId = doc.addChild(libroId, "titulo");
    doc.editText(tituloId, "Cien Años de Soledad");
    
    // Agregar autor
    int autorId = doc.addChild(libroId, "autor");
    doc.editText(autorId, "Gabriel García Márquez");
    
    // Mostrar resultado
    char* xml = doc.showAll();
    printf("%s", xml);
    free(xml);
    
    return 0;
}

### resultado
<biblioteca>
  <libro año="2023">
    <titulo>
      Cien Años de Soledad
    </titulo>
    <autor>
      Gabriel García Márquez
    </autor>
  </libro>
</biblioteca>

## Crear Estructura

// Agregar elemento hijo
int id = doc.addChild(parentId, "nombre_elemento");

// Agregar atributo
doc.addAttribute(nodeId, "clave", "valor");

## Modificar

// Cambiar nombre de elemento
doc.editName(nodeId, "nuevo_nombre");

// Cambiar texto interno
doc.editText(nodeId, "nuevo texto");

// Modificar atributo
doc.editAttribute(nodeId, "clave", "nuevo_valor");

## Buscar

// Buscar hijo por nombre
int hijoId = doc.findChild(parentId, "nombre_hijo");

// Obtener valor de atributo
char* valor = doc.findAttribute(nodeId, "clave");

## Eliminar
// Eliminar nodo y todo su contenido
doc.removeNode(nodeId);

// Eliminar solo un atributo
doc.removeAttribute(nodeId, "clave");

## Visualizar
// Ver todo el documento
char* todo = doc.showAll();

// Ver solo un elemento
char* parte = doc.showNode(nodeId);

// Ver desde un punto específico
char* desde = doc.showFrom(nodeId);

// IMPORTANTE: Liberar memoria después
free(todo);
free(parte);
free(desde);

## Guardar
// Guardar formato legible
doc.save("archivo.xml", false);

// Guardar formato compacto
doc.save("archivo.xml", true);

# Manejo de Errores
La librería retorna códigos de error cuando algo sale mal:

|--|--|

|Código|	Significado|
|-1	|Nodo no encontrado|
|-2	|Error de memoria|
|-3	|Nombre inválido|
|-4	|Hijo no encontrado|

## Consejos Importantes
1. Siempre libera la memoria después de usar show...()
2. Verifica los IDs retornados por addChild()
3. Usa los ejemplos como referencia
4. Prueba con datos pequeños primero

3# Con esta librería puedes:
✅ Crear documentos XML estructurados
✅ Guardar configuraciones
✅ Exportar datos de programas
✅ Aprender sobre formatos XML
