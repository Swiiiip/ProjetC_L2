#include "structures.h"
#include "functions.h"
#include <stdlib.h>

p_node create_node(char character)
{
    p_node mynode;
    mynode = malloc(sizeof(t_node));
    mynode->next = NULL;
    mynode->letter = character;
    mynode->fflechies.head = NULL;
    mynode->fflechies.tail = NULL;
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
            printf("case 1\n");
            siblings->head = temp;
            siblings->tail = temp;
            siblings->size = 1;
        }
        else
        {
            printf("case 2\n");
            siblings->tail->next = temp;
            siblings->tail = temp;
            siblings->size ++;
        }
    }
    printf("%c\n", f_base[index]);
    add_fbase(&(temp->next_letters), temp, f_base, index + 1);
}

void add_word(t_ht_list_node * the_root, char *fbase, char *fflechie, char *subtype)
{
    //mytree.roots.head = NULL;
    printf("before add letter%p\n",the_root->head);
    p_node my_node = add_fbase(the_root, NULL, fbase, 0);
    printf("after add letter%p\n", the_root->head);
    //Add the forme flechie to the list of the last node

    
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
    char* f_flechie, *f_base, *category, *subtype;
    FILE *dictionary;
    int index_line;

    //Opening of the file
    dictionary = fopen("minidictionnary.txt","r");
    
    //The while returns each line one by one as a string, line 
    while(fgets(line,110,dictionary))
    {

        index_line = 0;
        //By using the create_sub_lines function, we cut up the different parts of the line into strings(f_flechie, f_base, category, subtype)
        f_flechie = create_sub_lines(line, '\t', &index_line);
        f_base = create_sub_lines(line, '\t', &index_line);
        category = create_sub_lines(line, ':', &index_line);
        subtype = create_sub_lines(line, '\0', &index_line);


        //Now that we have each information of a line into the correct variables, we add it to the correct tree
        char categories[4][4] = {"Nom\0","Adj\0","Adv\0","Ver\0"};
        int found = -1;
        for(int i = 0 ; i < 4 ; i++)
        {
            if (!strcmp(categories[i],category))//Looks if the category we have is among the four we consider
            {
                found = i;
    
            }
        }
    
        switch(found)
        {
            case 0:
                //Functions to add the line in the noun_tree
                add_word(&(noun_tree.roots), f_base, f_flechie, subtype);
                break;
            case 1:
                //Functions to add the line in the adj_tree
                add_word(&(adj_tree.roots), f_base, f_flechie, subtype);
                break;
            case 2:
                //Functions to add the line in the adv_tree
                add_word(&(adv_tree.roots), f_base, f_flechie, subtype);
                break;
            case 3:
                //Functions to add the line in the ver_tree
                add_word(&(verb_tree.roots), f_base, f_flechie, subtype);
                break;
            default :
                break;
        }
        
        
        
    }

    //display_tree(verb_tree.roots.head,0);
}

void display_tree(p_node root, int index)
{
    p_node temp = root;

    if(temp->next_letters.head == NULL) return;

    for(int i = 0; i<index;i++)
        {
            temp = temp->next;
        }
    while(temp != NULL)
    {
        printf("%c",temp->letter);
        display_tree(temp->next_letters.head,index);
        temp = temp->next;
        index+=1;
    }
    return;
}