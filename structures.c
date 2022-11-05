#include "structures.h"

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


void create_trees()
{

    //Creation of the empty trees and initialisation of their fields
    t_tree noun_tree, adj_tree, adv_tree, verb_tree;
    noun_tree.roots.head = noun_tree.roots.tail = adj_tree.roots.head = adj_tree.roots.tail = NULL;
    adv_tree.roots.head = adv_tree.roots.tail = verb_tree.roots.head = verb_tree.roots.tail = NULL;
    noun_tree.roots.size = adj_tree.roots.size = adv_tree.roots.size = verb_tree.roots.size = 0;
    

    //Reading of the file line by line
    char line[110], f_flechie[20], f_base[20], type[3], personality[60];
    FILE *dictionary;
    int index_line, index_string;

    dictionary = fopen("minidictionnary.txt","r");
    
    //We get each line one by one as a string
    while(fgets(line,110,dictionary))
    {
        puts(line);
        index_line = index_string = 0;

        //We add to f_flechie its correct content which is until the first space
        while(line[index_line]!='\t')
        {
            f_flechie[index_string] = line[index_line];
            index_line++;
            index_string++;
        }
        f_flechie[index_string] = '\0';
        puts(f_flechie);
    

        //We add to f_base its correct content which is from the precedent index to the second space
        index_string = 0;
        while(line[index_line]!='\t')
        {
            f_base[index_string] = line[index_line+1]; //The string f_base gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        f_base[index_string] = '\0';
        puts(f_base);

        //We add to type its correct content which is from the precedent index to the colon
        index_string = 0;
        while(line[index_line]!=':')
        {
            type[index_string] = line[index_line+1]; //The string type gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        type[index_string] = '\0';
        puts(type);

        //We add to f_base its correct content which is from the precedent index to the end
        index_string = 0;
        while(line[index_line]!='\0')
        {
            personality[index_string] = line[index_line+1]; //The string personality gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        personality[index_string] = '\0';
        puts(personality);

        //printf("\n%s %s %s %s\n", f_flechie, f_base, type, personality);
        //Now that we have each information of a line into the correct variables, we add it to the correct tree
        char types[4][4] = {"Nom\0","Adj\0","Adv\0","Ver\0"};
        int found = -1;
        for(int i = 0 ; i < 4 ; i++)
        {
            if (!strcmp(types[i],type))//Looks if the type we have is among the four we consider
            {
                found = i;
            }
        }
         
        switch(found)
        {
            case 0:
                //Functions to add the line in the noun_tree
                add_word(noun_tree, f_base, f_flechie, personality);
                break;
            case 1:
                //Functions to add the line in the adj_tree
                add_word(adj_tree, f_base, f_flechie, personality);
                break;
            case 2:
                //Functions to add the line in the adv_tree
                add_word(adv_tree, f_base, f_flechie, personality);
                break;
            case 3:
                //Functions to add the line in the ver_tree
                add_word(verb_tree, f_base, f_flechie, personality);
                break;
            default :
                break;
        }
    }
}