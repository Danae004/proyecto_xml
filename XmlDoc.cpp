#include "XmlDoc.hpp"
#include "XmlNode.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ERROR_NODE_NOT_FOUND -1
#define ERROR_MEMORY_ALLOCATION -2
#define ERROR_INVALID_NAME -3
#define ERROR_CHILD_NOT_FOUND -4

XmlDoc::XmlDoc(const char* rootName) {
    root = new XmlNode(0, rootName);
    nextId = 1;
    nodeCount = 1;
}

XmlDoc::~XmlDoc() {
    delete root;
}

XmlNode* XmlDoc::findNodeById(int nodeId, XmlNode* currentNode) {
    if (!currentNode) return nullptr;
    if (currentNode->id == nodeId) return currentNode;
    
    XmlNode* result = findNodeById(nodeId, currentNode->firstChild);
    if (result) return result;
    
    return findNodeById(nodeId, currentNode->nextSibling);
}

int XmlDoc::addChild(int nodeId, const char* nodeName) {
    if (!nodeName || strlen(nodeName) == 0) {
        return ERROR_INVALID_NAME;
    }
    
    XmlNode* parent = findNodeById(nodeId, root);
    if (!parent) {
        return ERROR_NODE_NOT_FOUND;
    }
    
    XmlNode* newChild = new XmlNode(nextId, nodeName);
    if (!newChild) {
        return ERROR_MEMORY_ALLOCATION;
    }
    
    if (!parent->firstChild) {
        parent->firstChild = newChild;
    } else {
        XmlNode* lastChild = parent->firstChild;
        while (lastChild->nextSibling) {
            lastChild = lastChild->nextSibling;
        }
        lastChild->nextSibling = newChild;
    }
    
    newChild->parent = parent;
    nextId++;
    nodeCount++;
    
    return newChild->id;
}

bool XmlDoc::addAttribute(int nodeId, const char* key, const char* value) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node || !key || !value) return false;
    
    node->addAttribute(key, value);
    return true;
}

bool XmlDoc::editName(int nodeId, const char* newName) {
    if (!newName || strlen(newName) == 0) return false;
    
    XmlNode* node = findNodeById(nodeId, root);
    if (!node) return false;
    
    free(node->name);
    node->name = strdup(newName);
    return true;
}

bool XmlDoc::editText(int nodeId, const char* newText) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node) return false;
    
    node->setText(newText);
    return true;
}

bool XmlDoc::editAttribute(int nodeId, const char* key, const char* newValue) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node || !key || !newValue) return false;
    
    XmlAttribute* attr = node->attributes;
    while (attr) {
        if (strcmp(attr->key, key) == 0) {
            free(attr->value);
            attr->value = strdup(newValue);
            return true;
        }
        attr = attr->next;
    }
    
    return addAttribute(nodeId, key, newValue);
}

int XmlDoc::findChild(int nodeId, const char* name) {
    XmlNode* parent = findNodeById(nodeId, root);
    if (!parent || !name) return ERROR_NODE_NOT_FOUND;
    
    XmlNode* child = parent->findChildByName(name);
    if (!child) return ERROR_CHILD_NOT_FOUND;
    
    return child->id;
}

char* XmlDoc::findAttribute(int nodeId, const char* key) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node || !key) return nullptr;
    
    const char* value = node->getAttributeValue(key);
    if (!value) return nullptr;
    
    return strdup(value);
}

XmlNode* XmlDoc::findParentNode(XmlNode* target, XmlNode* currentNode) {
    if (!currentNode || !target) return nullptr;
    
    XmlNode* child = currentNode->firstChild;
    while (child) {
        if (child == target) return currentNode;
        if (child->firstChild) {
            XmlNode* result = findParentNode(target, child);
            if (result) return result;
        }
        child = child->nextSibling;
    }
    return nullptr;
}

void XmlDoc::removeNode(int nodeId) {
    if (nodeId == 0) return; // No se puede eliminar la raíz
    
    XmlNode* nodeToRemove = findNodeById(nodeId, root);
    if (!nodeToRemove) return;
    
    XmlNode* parent = findParentNode(nodeToRemove, root);
    if (!parent) return;
    
    // Desconectar de la lista de hijos del padre
    if (parent->firstChild == nodeToRemove) {
        parent->firstChild = nodeToRemove->nextSibling;
    } else {
        XmlNode* sibling = parent->firstChild;
        while (sibling && sibling->nextSibling != nodeToRemove) {
            sibling = sibling->nextSibling;
        }
        if (sibling) {
            sibling->nextSibling = nodeToRemove->nextSibling;
        }
    }
    
    // Eliminar el nodo y todos sus descendientes
    nodeCount--; // La eliminación real ocurre en el destructor
    delete nodeToRemove;
}

void XmlDoc::removeAttribute(int nodeId, const char* key) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node || !key) return;
    
    node->removeAttribute(key);
}

void XmlDoc::addToBuffer(char*& buffer, int& bufferSize, int& currentPos, const char* text) {
    if (!text) return;
    
    int textLen = strlen(text);
    int neededSize = currentPos + textLen + 1;
    
    if (neededSize > bufferSize) {
        int newSize = bufferSize * 2;
        if (newSize < neededSize) newSize = neededSize;
        
        char* newBuffer = (char*)realloc(buffer, newSize);
        if (!newBuffer) return;
        
        buffer = newBuffer;
        bufferSize = newSize;
    }
    
    strcpy(buffer + currentPos, text);
    currentPos += textLen;
}

void XmlDoc::generateXmlString(XmlNode* node, char*& buffer, int& bufferSize, int& currentPos, int indentLevel, bool minify) {
    if (!node) return;
    
    char indent[256] = "";
    if (!minify) {
        for (int i = 0; i < indentLevel; i++) {
            strcat(indent, "  ");
        }
    }
    
    // Tag de apertura
    char openTag[512];
    if (node->attributes) {
        sprintf(openTag, "%s<%s", indent, node->name);
        addToBuffer(buffer, bufferSize, currentPos, openTag);
        
        XmlAttribute* attr = node->attributes;
        while (attr) {
            char attrStr[256];
            sprintf(attrStr, " %s=\"%s\"", attr->key, attr->value);
            addToBuffer(buffer, bufferSize, currentPos, attrStr);
            attr = attr->next;
        }
        addToBuffer(buffer, bufferSize, currentPos, ">");
    } else {
        sprintf(openTag, "%s<%s>", indent, node->name);
        addToBuffer(buffer, bufferSize, currentPos, openTag);
    }
    
    if (!minify && (node->firstChild || node->text)) {
        addToBuffer(buffer, bufferSize, currentPos, "\n");
    }
    
    // Contenido de texto
    if (node->text) {
        char textLine[512];
        if (minify) {
            sprintf(textLine, "%s", node->text);
        } else {
            sprintf(textLine, "%s  %s", indent, node->text);
        }
        addToBuffer(buffer, bufferSize, currentPos, textLine);
        if (!minify) addToBuffer(buffer, bufferSize, currentPos, "\n");
    }
    
    // Hijos recursivamente
    XmlNode* child = node->firstChild;
    while (child) {
        generateXmlString(child, buffer, bufferSize, currentPos, indentLevel + 1, minify);
        child = child->nextSibling;
    }
    
    // Tag de cierre
    if (!minify && (node->firstChild || node->text)) {
        addToBuffer(buffer, bufferSize, currentPos, indent);
    }
    
    char closeTag[256];
    sprintf(closeTag, "</%s>", node->name);
    addToBuffer(buffer, bufferSize, currentPos, closeTag);
    
    if (!minify) {
        addToBuffer(buffer, bufferSize, currentPos, "\n");
    }
}

char* XmlDoc::showNode(int nodeId) {
    XmlNode* node = findNodeById(nodeId, root);
    if (!node) return nullptr;
    
    int bufferSize = 1024;
    int currentPos = 0;
    char* buffer = (char*)malloc(bufferSize);
    if (!buffer) return nullptr;
    
    buffer[0] = '\0';
    generateXmlString(node, buffer, bufferSize, currentPos, 0, false);
    
    return buffer;
}

char* XmlDoc::showAll() {
    return showNode(0);
}

char* XmlDoc::showFrom(int nodeId) {
    return showNode(nodeId);
}

void XmlDoc::save(const char* filename, bool minify) {
    char* xmlContent = showAll();
    if (!xmlContent) return;
    
    FILE* file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s", xmlContent);
        fclose(file);
    }
    
    free(xmlContent);
}

bool XmlDoc::nodeExists(int nodeId) {
    return findNodeById(nodeId, root) != nullptr;
}