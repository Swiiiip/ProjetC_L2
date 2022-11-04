#include "functions.h"


p_node create_node(char letter)
{
    p_node mynode;
    mynode->letter = letter;
    //Missing 
    return mynode;
}

void add_word(t_tree mytree, char *fbase, char *fflechie, char *personality)
{
    
    p_node current_node, temp_node;
    
    mytree.roots.size +=1;

    if(mytree.roots.head == NULL)
    {
        current_node = create_node(fbase[0]);
        mytree.roots.head = current_node;
        current_node->next = NULL;
    }
    else
    {
        temp_node = mytree.roots.head;
        while(temp_node->next != NULL || temp_node->next->letter != fbase[0]) //not good condition a retravailler
        {
            temp_node = temp_node->next;
        }
        current_node->next = temp_node->next;
        temp_node->next = current_node;
    }

            //NEED RECURSION BAD EVERYTHING
    int char_index = 1;
    while(fbase[char_index] != '\0')
    {
        current_node = create_node(fbase[char_index]);

    }
}


