#include "file_node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//metodo per creare un nodo
FileNode* create_FileNode(char* file_path, FileNode* next){
  FileNode* new_file_node = malloc(sizeof(FileNode));
  new_file_node->file_path = file_path;
  new_file_node->next = next;
  printf("Nodo creato con path: %s\n", file_path);
  return new_file_node;
}

FileNode* init_FileNode(){
  return create_FileNode("a", NULL);
}

FileNode* add_FileNode(char* file_path, FileNode* previous){
  FileNode* new_file_node = create_FileNode(file_path, NULL);
  previous->next = new_file_node;
  return new_file_node;
}