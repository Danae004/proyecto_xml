#ifndef XMLNODE_HPP
#define XMLNODE_HPP

struct XmlAttribute {
    char* key;
    char* value;
    XmlAttribute* next;
    
    XmlAttribute(const char* k, const char* v);
    ~XmlAttribute();
};

struct XmlNode {
    int id;
    char* name;
    char* text;
    XmlAttribute* attributes;
    XmlNode* firstChild;
    XmlNode* nextSibling;
    XmlNode* parent;
    
    XmlNode(int nodeId, const char* nodeName);
    ~XmlNode();
    
    void addAttribute(const char* key, const char* value);
    void setText(const char* newText);
    XmlNode* findChildByName(const char* childName);
    const char* getAttributeValue(const char* key);
    bool removeAttribute(const char* key);
    void removeAllChildren();
};

#endif