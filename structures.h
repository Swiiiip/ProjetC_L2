#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Using forward declaration to avoid circular dependencies
struct s_node; 
struct s_ht_list_flechie;


typedef struct s_cell
{
    int number_type;// The number that determines the type of the word (Mas, Fem, Sg, Pl...)
    char* forme_flechie; // The forme flechie of the word
    struct s_cell *next; // Pointer to the next forme flechie of the list
}t_cell;

typedef t_cell *p_cell;

//The list used for les formes flechies
typedef struct s_ht_list_flechie
{
    int size;
    p_cell *head, *tail; // First and Last cell of the list

} t_ht_list_flechie;

// The list used for the general strcuture of the tree
typedef struct s_ht_list_node
{
    int size;
    struct s_node *head, *tail; //First and Last node of the list
} t_ht_list_node;

typedef struct s_node
{
    char letter; // The letter on the node
    t_ht_list_node next_letters; // The next level of letters of the word
    struct s_node  *next; // Pointer to the next and previous nodes of the list, the letters of the same level
    t_ht_list_flechie fflechies; // The structure that stores the "formes flechies" and the integer that determines their type (Mas, Fem, Sg, Pl...)
}t_node;

typedef t_node *p_node;

//The structure t_node is what forms the t_ht_list (s) structures, 
//and each node points its self to another t_ht_list to make a tree 
//and cover all possible words of the dictionnary

//The structure that contains the tree of words for a given word type
typedef struct s_tree 
{
    t_ht_list_node roots; //The roots are the first letters of the words
}t_tree;

//void add_word(t_tree mytree, char *fbase, char *fflechie, char *subtype);



t_tree create_empty_tree();
void fill_trees();
p_node add_letter(t_ht_list_node siblings, p_node my_node,char * f_base, int index);
void add_word(t_tree *mytree, char *fbase, char *fflechie, char *subtype);
p_node search_letter(t_ht_list_node mylist, char letter);

void display_tree(p_node,int);
p_node create_node(char letter);

