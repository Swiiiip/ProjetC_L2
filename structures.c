#include "structures.h"
#include "functions.h"
#include <stdlib.h>

p_node create_node(char character)
{
    p_node mynode = malloc(sizeof(t_node));
    
    mynode->next = NULL;
    mynode->letter = character;

    mynode->fflechies.head = NULL;
    mynode->fflechies.tail = NULL;
    mynode->fflechies.size = 0;

    mynode->next_letters.head = NULL;
    mynode->next_letters.tail = NULL;
    mynode->next_letters.size = 0;
    
    return mynode;
}

p_node search_letter(t_ht_list_node siblings, char letter)
{
    p_node temp = siblings.head;
    while(temp != NULL)
    {
        if(temp->letter == letter)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

p_node add_fbase(t_ht_list_node * siblings, p_node my_node,char * f_base, int index)
{
    if(f_base[index] == '\0'){
        my_node->next_letters.head = NULL;
        my_node->next_letters.tail = NULL;
        my_node->next_letters.size = 0;
        return my_node;
    }

    p_node temp = search_letter(*siblings, f_base[index]);
    if(temp == NULL)
    {
        temp = create_node(f_base[index]);
        if(siblings->head == NULL)
        {
            siblings->head = temp;
            siblings->tail = temp;
            siblings->size = 1;
        }
        else
        {
            siblings->tail->next = temp;
            siblings->tail = temp;
            siblings->size ++;
        }
    }
    add_fbase(&(temp->next_letters), temp, f_base, index + 1);
}

p_cell add_fflechie(char* fflechie, char* type){

    int type_int = 0;
    char *subtype, *token = strtok(type, "+");
    
    while(token != NULL){
        subtype = token;
        token = strtok(NULL, "+");
            
        // Binary table for type conversion : 
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
        // |   10   |   9   |   8   |   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
        // |  Inf   | IImp  | IPre  | SPre  |  P1   |  P2   |  P3   |  PL   |  SG   |  Fem  |  Mas  |
        // +--------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+

        if(!strcmp(subtype, "InvGen"))
            type_int += 2 + 1;
        else{ if(!strcmp(subtype, "Mas"))
            type_int += 1;
        else if(!strcmp(subtype, "Fem"))
            type_int += 2;

        else if(!strcmp(subtype, "InvPL"))
            type_int += 4 + 8;
        else if(!strcmp(subtype, "SG"))
            type_int += 4;
        else if(!strcmp(subtype, "PL"))
            type_int += 8;

        else if(!strcmp(subtype, "P3"))
            type_int += 16;
        else if(!strcmp(subtype, "P2"))
            type_int += 32;
        else if(!strcmp(subtype, "P1"))
            type_int += 64;

        else if(!strcmp(subtype, "SPre"))
            type_int += 128;
        else if(!strcmp(subtype, "IPre"))
            type_int += 256;
        else if(!strcmp(subtype, "IImp"))
            type_int += 512;
        else if(!strcmp(subtype, "Inf")){
            type_int += 1024;
            break;
        }

        else{ // for errors, unknown types, or excluded types
            type_int = -1;
            //printf("%s : Unknown subtype : %s\n", fflechie, subtype);
            break;
        } 
        }
    }
    
    if(type_int == -1) //exclude forbidden typed p_cell
        return NULL;

    else{
        //printf("%s : subtype : %s\n", fflechie, subtype);
        //create cell for fflechie
        p_cell my_cell = (p_cell)malloc(sizeof(t_cell));

        my_cell->forme_flechie = (char*)malloc(sizeof(char) * (strlen(fflechie) + 1));
        strcpy(my_cell->forme_flechie, fflechie);

        my_cell->number_type = type_int;
        my_cell->next = NULL;

        return my_cell;
    }
}



void add_word(t_ht_list_node * the_root, char *fbase, char *fflechie, char *types)
{
    p_node my_node = add_fbase(the_root, NULL, fbase, 0);
    if(types != NULL){
        //Add the forme flechie to the list of the last node
        char *type, *token = strtok(types, ":");
        while(token != NULL){
            type = token;
            token = strtok(NULL, ":");
            
            p_cell my_cell = add_fflechie(fflechie, type);
            
            

            if(my_cell!=NULL){

                if(my_node->fflechies.head == NULL){ // checks if node has no fflechies yet
                    my_node->fflechies.head = my_cell; 
                    my_node->fflechies.tail = my_cell;
                }
                else{
                    my_node->fflechies.tail->next = my_cell;
                    my_node->fflechies.tail = my_cell;
                }
                my_node->fflechies.size ++;
            }
        }
    }
}

t_tree create_empty_tree()
{
    t_tree mytree;
    mytree.roots.head = NULL;
    mytree.roots.tail = NULL;
    mytree.roots.size = 0;
    return mytree;

}

void fill_trees()
{
    //Creation of the empty trees and initialisation of their fields
    t_tree noun_tree = create_empty_tree();
    t_tree verb_tree = create_empty_tree();
    t_tree adj_tree = create_empty_tree();
    t_tree adv_tree = create_empty_tree();


    //Reading of the file line by line
    char line[110];
    char* f_flechie, *f_base, *category, *type;
    FILE *dictionary;

    //Opening of the file
    //dictionary = fopen("dictionnaire.txt","r");
    //dictionary = fopen("minidictionnary.txt","r");
    dictionary = fopen("dictionnaire_non_accentue.txt","r");

    
    //The while returns each line one by one as a string, line 
    while(fgets(line,110,dictionary))
    {   

        split_line(line, &f_flechie, &f_base, &category, &type);
        
        //Now that we have each information of a line into the correct variables, we add it to the correct tree
        char categories[4][4] = {"Nom\0","Adj\0","Adv\0","Ver\0"};
        int found = -1;
        for(int i = 0 ; i < 4 ; i++){
            if (!strcmp(categories[i],category))//Looks if the category we have is among the four we consider
                found = i;
        }
    
        switch(found)
        {
            case 0:
                //Functions to add the line in the noun_tree
                add_word(&(noun_tree.roots), f_base, f_flechie, type);
                break;
            case 1:
                //Functions to add the line in the adj_tree
                add_word(&(adj_tree.roots), f_base, f_flechie, type);
                break;
            case 2:
                //Functions to add the line in the adv_tree
                add_word(&(adv_tree.roots), f_base, f_flechie, type);
                break;
            case 3:
                //Functions to add the line in the ver_tree
                add_word(&(verb_tree.roots), f_base, f_flechie, type);
                break;
            default :
                break;
        }
    }
    
    fclose(dictionary);


    //Displaying all trees :
    /*
    printf("=============\n Noun tree :\n=============\n\n");
    print_tree_paths(noun_tree.roots);
    printf("=============\n Adj tree :\n=============\n\n");
    print_tree_paths(adj_tree.roots);
    printf("=============\n Adv tree :\n=============\n\n");
    print_tree_paths(adv_tree.roots);
    printf("=============\n Verb tree :\n=============\n\n");
    print_tree_paths(verb_tree.roots);*/

    //Example for the search_fbase function
    search_fbase(noun_tree.roots, "fqdsfq",0);

    //Example for the random fbase function
    //extract_random_fbase(verb_tree);

}


void print_tree_paths(t_ht_list_node roots)
{
    char* path = malloc(roots.size*sizeof(char));
    p_node tmp = roots.head;
    for(int i = 0 ; i < roots.size ; i++){
        printf("+---+\n| %c |\n+---+  ", tmp->letter);
        print_node_paths(tmp, path, 0);
        tmp = tmp->next;
        printf("\n\n\n[ENTER] \n\n\n");
        //getchar();
    }
}

void print_node_paths(p_node node, char path[], int pathLen)
{
  if (node==NULL)
    return;

  /* append this node to the path array */
  path[pathLen] = node->letter;
  pathLen++;

  /* it's a leaf, so print the path that led to here  */
  if (node->next_letters.head == NULL){
    
    for (int i=0; i<pathLen; i++)
        printf("%c", path[i]);
    printf(" ");
    //COMMENT THIS PART IF YOU DON'T WANT TO SEE THE FORMES FLECHIES :

    /*
    p_cell tmp = node->fflechies.head;
    printf(" %d fflechies :\n", node->fflechies.size);
    while(tmp != NULL){
        if(node->fflechies.size > 0)
            printf("%s %d -> ", tmp->forme_flechie, tmp->number_type);
        tmp = tmp->next;
    }
    printf("\n\n");
    */
    
  }
  
  else
  {
    /* otherwise try both subtrees */
    p_node temp = node->next_letters.head;
    while(temp != NULL)
    {
        print_node_paths(temp, path, pathLen);
        temp = temp->next;
    }
  }
}


void search_fbase(t_ht_list_node roots, char *fbase, int index)
{
    p_node tmp = roots.head;
    int i = 0;
    while(tmp != NULL && i < roots.size){
        if(tmp->letter == fbase[index]){
            
            if(index == strlen(fbase)-1)
            {
                printf("\nThe word %s has been found.\n", fbase);
                print_fflechies(tmp);
                return;
            }
            else
                return search_fbase(tmp->next_letters, fbase, index+1);
        }
        tmp = tmp->next;
        i++;
    }
    printf("\nThe word %s has not been found.\n", fbase);
    return;
}


void extract_random_fbase(t_tree mytree)
{
    int tree_roots_size = 0;
    p_node tmp = mytree.roots.head;
    while(tmp != NULL)//Count the number of letter in the root of the tree
    {
        tree_roots_size ++;
        tmp = tmp->next;
    }
    tmp = mytree.roots.head;
    int random = rand() % tree_roots_size;
    for (int i = 0;i < random; i++)//Access to the root chosen randomly
        tmp = tmp->next;


    printf("\nThe word '%c",tmp->letter);
    p_node last_node_fbase = random_path(tmp);
    printf("' is a base form. "); 
    print_fflechies(last_node_fbase);
}


//Used to print all fflechies of a fbase
void print_fflechies(p_node leaf)
{
    if(leaf->fflechies.head == NULL) //Case of the adverb tree
        printf("Since it is an adverb, it doesn't have any contracted forms.\n");
    else
    {
        if(leaf->fflechies.size == 1)//If there is only one fflechie, we use the singular in the sentence
            //Need to modify to put into words the number type -> need a new function
            printf("Its contracted form is :\n\t- '%s' which is of number %d (temporary display).\n\n",leaf->fflechies.head->forme_flechie, leaf->fflechies.head->number_type);
        else 
        {
            printf("Its contracted forms are :\n");
            p_cell temp = leaf->fflechies.head;
            while(temp != NULL)
            {
                printf("\t- '%s' which is of number %d (temporary display).\n",temp->forme_flechie,temp->number_type);
                temp = temp->next;
            }
            printf("\n");
        }
    }   
}


//Need to modify later so that it prints the path outside of the function so that this can be used in the sentence generator 

p_node random_path(p_node current)
{
    if(current->next_letters.head == NULL)
        return current;
    else
    {
        int random = rand() % current->next_letters.size;
        p_node temp = current->next_letters.head;
        for(int i = 0 ; i < random ; i++)
            temp = temp->next;
        printf("%c",temp->letter);
        return random_path(temp);
    }
}