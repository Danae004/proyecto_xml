#ifndef XMLDOC_HPP
#define XMLDOC_HPP

class XmlDoc {
private:
    XmlNode* root;
    int nextId;
    int nodeCount;
    
    XmlNode* findNodeById(int nodeId, XmlNode* currentNode);
    void generateXmlString(XmlNode* node, char*& buffer, int& bufferSize, int& currentPos, int indentLevel, bool minify);
    void addToBuffer(char*& buffer, int& bufferSize, int& currentPos, const char* text);
    void destroySubtree(XmlNode* node);
    XmlNode* findParentNode(XmlNode* target, XmlNode* currentNode);

public:
    XmlDoc(const char* rootName);
    ~XmlDoc();
    
    int addChild(int nodeId, const char* nodeName);
    bool addAttribute(int nodeId, const char* key, const char* value);
    bool editName(int nodeId, const char* newName);
    bool editText(int nodeId, const char* newText);
    bool editAttribute(int nodeId, const char* key, const char* newValue);
    int findChild(int nodeId, const char* name);
    char* findAttribute(int nodeId, const char* key);
    void removeNode(int nodeId);
    void removeAttribute(int nodeId, const char* key);
    char* showNode(int nodeId);
    char* showAll();
    char* showFrom(int nodeId);
    void save(const char* filename, bool minify);
    
    int getNodeCount() const { return nodeCount; }
    bool nodeExists(int nodeId);
    
    XmlDoc(const XmlDoc&) = delete;
    XmlDoc& operator=(const XmlDoc&) = delete;
};

#endif