#include "structures.h"
#include "tree.h"


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

    dictionary = fopen("dictionnaire.txt","r");
    
    //We get each line one by one as a string
    while(fgets(line,100,dictionary))
    {
        index_line = index_string = 0;

        //We add to f_flechie its correct content which is until the first space
        while(line[index_line]!='   ')
        {
            f_flechie[index_string] = line[index_line]; //The string f_flechie gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        f_flechie[index_string+1] = '\0';

        //We add to f_base its correct content which is from the precedent index to the second space
        index_string = 0;
        while(line[index_line]!='   ')
        {
            f_base[index_string] = line[index_line]; //The string f_base gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        f_base[index_string+1] = '\0';

        //We add to type its correct content which is from the precedent index to the colon
        index_string = 0;
        while(line[index_line]!=':')
        {
            type[index_string] = line[index_line]; //The string type gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        type[index_string+1] = '\0';

        //We add to f_base its correct content which is from the precedent index to the end
        index_string = 0;
        while(line[index_line]!='\0')
        {
            personality[index_string] = line[index_line]; //The string personality gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        personality[index_string+1] = '\0';


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