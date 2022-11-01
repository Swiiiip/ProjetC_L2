#include <stdio.h>
#include <stdlib.h>
#include "list.h"


//The structure t_node is what forms the t_ht_list (s) strcutures, 
//and each node points its self to another t_ht_list to make a tree 
//and cover all possible words of the dictionnary

typedef struct s_node
{
    char letter; // The letter on the node
    t_ht_list next_letters; // Pointer to the next level of letters of the word
    struct s_node  *next, *prev; // Pointer to the next and previous nodes of the list, of the letters of the same level
    t_ht_list fflechies; // Pointer to the structure that stores the "formes flechies" and the integer that determines their type (Mas, Fem, Sg, Pl...)
   
  
}t_node;

typedef t_node *p_node;

typedef struct s_cell{

    int number_type;
    char* forme_flechie;
    struct s_cell *next;

}t_cell;

typedef t_cell *p_cell;
