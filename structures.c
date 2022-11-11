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

p_node add_letter(t_ht_list_node siblings, p_node my_node, char * f_base, int index)
{
    if (siblings.head != NULL)
    {
        printf("coucou");
        //printf("first letter \n%c ",siblings->head->letter);
    }

    if(f_base[index] != '\0'){
        if (siblings.head == NULL)
        {            
            printf("case 1\n");
            p_node new_node = create_node(f_base[index]);
            siblings.head = new_node;
            siblings.tail = new_node;
            siblings.size = 1;
            printf("%c \n",new_node->letter);
            printf("in add letter%u\n",siblings.head);
            return add_letter(siblings.head->next_letters, new_node, f_base, index+1);
        }
        else
        {
            printf("coucou1234341");
            p_node found = search_letter(siblings, f_base[index]);
            printf("coucou2\n");
            if(found == NULL)
            {
                printf("case 2\n");
                p_node new_node = create_node(f_base[index]);
                siblings.tail->next = new_node;
                siblings.tail = new_node;
                siblings.size ++;
                return add_letter(new_node->next_letters, new_node, f_base, index+1);
            }else{
                printf("case 3\n");
                return add_letter(found->next_letters, found, f_base, index+1);
            }
        }
    }else{
        return my_node;
    }
}

void add_word(t_tree *mytree, char *fbase, char *fflechie, char *subtype)
{
    //mytree.roots.head = NULL;
    printf("before add letter%u\n",mytree->roots.head);
    p_node my_node = add_letter(mytree->roots, NULL, fbase, 0);
    printf("after add letter%u\n",mytree->roots.head);
    //Add the forme flechie to the list of the last node
    my_node->next_letters.head = NULL;
    my_node->next_letters.tail = NULL;
    my_node->next_letters.size = 0;
    
}

t_tree create_empty_tree()
{
    t_tree mytree;
    mytree.roots.head = NULL;
    mytree.roots.tail = NULL;
    mytree.roots.size = 0;
        printf("%u\n",mytree.roots.head);

}

void fill_trees()
{
    //Creation of the empty trees and initialisation of their fields
    /*
    t_tree noun_tree = create_empty_tree();
    t_tree verb_tree = create_empty_tree();
    t_tree adj_tree = create_empty_tree();
    t_tree adv_tree = create_empty_tree();
    */
    t_tree noun_tree, verb_tree, adj_tree,adv_tree;
    verb_tree.roots.head = noun_tree.roots.head = adj_tree.roots.head = adv_tree.roots.head = NULL;
    verb_tree.roots.tail = noun_tree.roots.tail = adj_tree.roots.tail = adv_tree.roots.tail = NULL;
    verb_tree.roots.size = noun_tree.roots.size = adj_tree.roots.size = adv_tree.roots.size = 0;
    printf("initialisation adress :%u \n",verb_tree.roots.head);


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
                add_word(&noun_tree, f_base, f_flechie, subtype);
                break;
            case 1:
                //Functions to add the line in the adj_tree
                add_word(&adj_tree, f_base, f_flechie, subtype);
                break;
            case 2:
                //Functions to add the line in the adv_tree
                add_word(&adv_tree, f_base, f_flechie, subtype);
                break;
            case 3:
                //Functions to add the line in the ver_tree
                add_word(&verb_tree, f_base, f_flechie, subtype);
                break;
            default :
                break;
        }
        //display_tree(verb_tree.roots.head,0);
        
        
    }
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