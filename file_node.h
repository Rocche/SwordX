#ifndef FILE_NODE_H_INCLUDED
#define FILE_NODE_H_INCLUDED

typedef struct FileNode {
  char * file_path;
  struct FileNode *next;
} FileNode;

FileNode* init_FileNode();

FileNode* add_FileNode(char*, FileNode*);

#endif //FILE_NODE_H_INCLUDED