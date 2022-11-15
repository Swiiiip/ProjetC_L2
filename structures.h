#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    NOM= 0,
    ADJ = 1,
    ADV = 2,
    VER = 3
}CATEGORY;




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
    t_cell *head, *tail; // First and Last cell of the list

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


//     FUNCTIONS FOR TREE MANIPULATION    //
t_tree create_empty_tree();
t_tree* fill_trees();

void print_tree_paths(t_ht_list_node roots);
void print_node_paths(p_node node, char path[], int pathLen);

//    FUNCTIONS FOR WORDS MANIPULATION    //
p_cell add_fflechie(char* fflechie, char* types);
p_node add_fbase(t_ht_list_node * siblings, p_node my_node, char * f_base, int index);
void add_word(t_ht_list_node * the_root, char *fbase, char *fflechie, char *type);
void search_fbase(t_ht_list_node roots, char *fbase, int index);
void extract_random_fbase(t_tree);
p_node random_path(p_node, int);

//    USEFUL FUNCTIONS    //
int conversion_type(char* type);
p_node search_letter(t_ht_list_node mylist, char letter);
void print_fflechies(p_node leaf);
int number_paths(p_node current);

//    FUNCTIONS FOR NODE MANIPULATION    //
p_node create_node(char letter);



char * generate_random_type(int mode);
void generate_modele1(t_tree * trees, int mode_fflechie);

void print_fflechies(p_node leaf);

char * finding_fflechie_corresponding_to_type(t_tree my_tree, int type);
char * determinant_generator(int type);
int is_type_in( int desired_type, int full_type);


