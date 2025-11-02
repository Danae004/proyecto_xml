#include "XmlDoc.hpp"
#include <iostream>
#include <cstdio>

void testBasicOperations() {
    printf("=== TESTING BASIC OPERATIONS ===\n");
    
    XmlDoc doc("catalogo");
    
    // Agregar libros
    int libro1 = doc.addChild(0, "libro");
    doc.addAttribute(libro1, "isbn", "978-0142437230");
    doc.addAttribute(libro1, "categoria", "novela");
    
    int titulo1 = doc.addChild(libro1, "titulo");
    doc.editText(titulo1, "Don Quijote de la Mancha");
    
    int autor1 = doc.addChild(libro1, "autor");
    int nombre1 = doc.addChild(autor1, "nombre");
    doc.editText(nombre1, "Miguel de Cervantes");
    int pais1 = doc.addChild(autor1, "pais");
    doc.editText(pais1, "España");
    
    // Segundo libro
    int libro2 = doc.addChild(0, "libro");
    doc.addAttribute(libro2, "isbn", "978-0061120084");
    doc.addAttribute(libro2, "categoria", "ciencia ficcion");
    
    int titulo2 = doc.addChild(libro2, "titulo");
    doc.editText(titulo2, "1984");
    
    int autor2 = doc.addChild(libro2, "autor");
    doc.editText(autor2, "George Orwell");
    
    // Mostrar resultados
    printf("=== showNode(libro1) ===\n");
    char* nodeXml = doc.showNode(libro1);
    if (nodeXml) {
        printf("%s\n", nodeXml);
        free(nodeXml);
    }
    
    printf("=== showAll() ===\n");
    char* allXml = doc.showAll();
    if (allXml) {
        printf("%s\n", allXml);
        free(allXml);
    }
    
    printf("=== showFrom(autor1) ===\n");
    char* fromXml = doc.showFrom(autor1);
    if (fromXml) {
        printf("%s\n", fromXml);
        free(fromXml);
    }
    
    // Probar búsquedas
    printf("=== Testing find operations ===\n");
    int foundChild = doc.findChild(libro1, "autor");
    if (foundChild > 0) {
        printf("Encontrado hijo 'autor' con ID: %d\n", foundChild);
    }
    
    char* attrValue = doc.findAttribute(libro1, "isbn");
    if (attrValue) {
        printf("Atributo 'isbn': %s\n", attrValue);
        free(attrValue);
    }
}

void testAdvancedOperations() {
    printf("\n=== TESTING ADVANCED OPERATIONS ===\n");
    
    XmlDoc doc("universidad");
    
    // Estructura compleja de 3 niveles
    int facultad = doc.addChild(0, "facultad");
    doc.addAttribute(facultad, "nombre", "Ingenieria");
    
    int carrera = doc.addChild(facultad, "carrera");
    doc.addAttribute(carrera, "codigo", "ISC");
    doc.addAttribute(carrera, "creditos", "250");
    
    int materias = doc.addChild(carrera, "materias");
    
    int materia1 = doc.addChild(materias, "materia");
    doc.addAttribute(materia1, "codigo", "PROG3");
    doc.editText(materia1, "Programacion III");
    
    int materia2 = doc.addChild(materias, "materia");
    doc.addAttribute(materia2, "codigo", "EDD");
    doc.editText(materia2, "Estructuras de Datos");
    
    int materia3 = doc.addChild(materias, "materia");
    doc.addAttribute(materia3, "codigo", "BD1");
    doc.editText(materia3, "Bases de Datos I");
    
    printf("=== showFrom(carrera) ===\n");
    char* fromXml = doc.showFrom(carrera);
    if (fromXml) {
        printf("%s\n", fromXml);
        free(fromXml);
    }
    
    // Probar edición
    printf("=== After editing ===\n");
    doc.editAttribute(carrera, "creditos", "280");
    doc.editText(materia3, "Bases de Datos Avanzadas");
    
    char* editedXml = doc.showFrom(carrera);
    if (editedXml) {
        printf("%s\n", editedXml);
        free(editedXml);
    }
    
    // Probar eliminación
    printf("=== After removing materia2 ===\n");
    doc.removeNode(materia2);
    
    char* afterRemove = doc.showFrom(carrera);
    if (afterRemove) {
        printf("%s\n", afterRemove);
        free(afterRemove);
    }
    
    // Guardar archivos
    doc.save("test_output.xml", false);
    doc.save("test_output_min.xml", true);
    printf("Archivos guardados: test_output.xml y test_output_min.xml\n");
}

void testErrorHandling() {
    printf("\n=== TESTING ERROR HANDLING ===\n");
    
    XmlDoc doc("test");
    
    // Probar errores
    int result = doc.addChild(999, "nodo"); // ID inexistente
    if (result == ERROR_NODE_NOT_FOUND) {
        printf("✓ Correctamente detectado: Nodo padre no existe\n");
    }
    
    result = doc.addChild(0, ""); // Nombre inválido
    if (result == ERROR_INVALID_NAME) {
        printf("✓ Correctamente detectado: Nombre inválido\n");
    }
    
    // Probar búsqueda de hijo inexistente
    result = doc.findChild(0, "inexistente");
    if (result == ERROR_CHILD_NOT_FOUND) {
        printf("✓ Correctamente detectado: Hijo no encontrado\n");
    }
    
    // Probar atributo inexistente
    char* attr = doc.findAttribute(0, "atributo_inexistente");
    if (!attr) {
        printf("✓ Correctamente detectado: Atributo no existe\n");
    }
}

int main() {
    printf("🚀 INICIANDO PRUEBAS DE LIBRERÍA XML\n\n");
    
    testBasicOperations();
    testAdvancedOperations();
    testErrorHandling();
    
    printf("\n🎉 TODAS LAS PRUEBAS COMPLETADAS\n");
    printf("Revisa los archivos generados:\n");
    printf("  - test_output.xml (formato legible)\n");
    printf("  - test_output_min.xml (formato minificado)\n");
    
    return 0;
}