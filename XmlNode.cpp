#include "XmlNode.hpp"
#include <cstdlib>
#include <cstring>

// Implementación de XmlAttribute
XmlAttribute::XmlAttribute(const char* k, const char* v) {
    key = strdup(k);
    value = strdup(v);
    next = nullptr;
}

XmlAttribute::~XmlAttribute() {
    free(key);
    free(value);
}

// Implementación de XmlNode
XmlNode::XmlNode(int nodeId, const char* nodeName) {
    id = nodeId;
    name = strdup(nodeName);
    text = nullptr;
    attributes = nullptr;
    firstChild = nullptr;
    nextSibling = nullptr;
    parent = nullptr;
}

XmlNode::~XmlNode() {
    free(name);
    free(text);
    
    // Liberar atributos
    XmlAttribute* currentAttr = attributes;
    while (currentAttr) {
        XmlAttribute* nextAttr = currentAttr->next;
        delete currentAttr;
        currentAttr = nextAttr;
    }
    
    // Liberar hijos recursivamente
    removeAllChildren();
}

void XmlNode::addAttribute(const char* key, const char* value) {
    XmlAttribute* newAttr = new XmlAttribute(key, value);
    newAttr->next = attributes;
    attributes = newAttr;
}

void XmlNode::setText(const char* newText) {
    if (text) {
        free(text);
    }
    text = (newText && strlen(newText) > 0) ? strdup(newText) : nullptr;
}

XmlNode* XmlNode::findChildByName(const char* childName) {
    XmlNode* child = firstChild;
    while (child) {
        if (strcmp(child->name, childName) == 0) {
            return child;
        }
        child = child->nextSibling;
    }
    return nullptr;
}

const char* XmlNode::getAttributeValue(const char* key) {
    XmlAttribute* attr = attributes;
    while (attr) {
        if (strcmp(attr->key, key) == 0) {
            return attr->value;
        }
        attr = attr->next;
    }
    return nullptr;
}

bool XmlNode::removeAttribute(const char* key) {
    XmlAttribute* current = attributes;
    XmlAttribute* prev = nullptr;
    
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                attributes = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void XmlNode::removeAllChildren() {
    XmlNode* currentChild = firstChild;
    while (currentChild) {
        XmlNode* nextChild = currentChild->nextSibling;
        delete currentChild;
        currentChild = nextChild;
    }
    firstChild = nullptr;
}