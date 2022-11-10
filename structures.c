#include "structures.h"
#include "functions.h"
#include <stdlib.h>

p_node create_node(char letter)
{
    p_node mynode;
    mynode->letter = letter;
    mynode->next = NULL;
    mynode->fflechies.head = NULL;
    mynode->fflechies.tail = NULL;
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


    if(f_base[index+1] != '\0'){

        if (siblings.head == NULL)
        {
            p_node new_node = create_node(f_base[index]);        
            siblings.head = new_node;
            siblings.tail = new_node;
            siblings.size = 1;
            return add_letter(new_node->next_letters, new_node, f_base, index+1);
        }
        else
        {
            p_node temp = search_letter(siblings, f_base[index]);
            if(temp == NULL)
            {
                p_node new_node = create_node(f_base[index]);
                siblings.tail->next = new_node;
                siblings.tail = new_node;
                siblings.size ++;
                return add_letter(new_node->next_letters, new_node, f_base, index+1);
            }else{
                return add_letter(temp->next_letters, temp, f_base, index+1);
            }

        }
    }else{
        return my_node;
    }
}

void add_word(t_tree mytree, char *fbase, char *fflechie, char *subtype)
{
    p_node my_node = add_letter(mytree.roots, NULL, fbase, 0);
    
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
                add_word(noun_tree, f_base, f_flechie, subtype);
                break;
            case 1:
                //Functions to add the line in the adj_tree
                add_word(adj_tree, f_base, f_flechie, subtype);
                break;
            case 2:
                //Functions to add the line in the adv_tree
                add_word(adv_tree, f_base, f_flechie, subtype);
                break;
            case 3:
                //Functions to add the line in the ver_tree
                add_word(verb_tree, f_base, f_flechie, subtype);
                break;
            default :
                break;
        }
        
        
    }
}

//We add to f_flechie its correct content which is until the first space
        /*
        while(line[index_line]!='\t')
        {
            f_flechie[index_string] = line[index_line];
            index_line++;
            index_string++;
        }
        f_flechie[index_string] = '\0';
        puts(f_flechie);
        index_line++;// to move over from the character "tab" or "\t", for it to not be taken it consideration in the next while loop
    

        //We add to f_base its correct content which is from the precedent index to the second space
        index_string = 0;
        while(line[index_line]!='\t')
        {
            f_base[index_string] = line[index_line]; //The string f_base gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        f_base[index_string] = '\0';
        puts(f_base);
        index_line++;

        //We add to category its correct content which is from the precedent index to the colon
        index_string = 0;
        while(line[index_line]!=':')
        {
            category[index_string] = line[index_line]; //The string category gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        category[index_string] = '\0';
        puts(category);
        index_line++;

        //We add to f_base its correct content which is from the precedent index to the end
        index_string = 0;
        while(line[index_line]!='\0')
        {
            subtype[index_string] = line[index_line]; //The string subtype gets completed with an independant counter to the while loop
            index_line ++;
            index_string ++;
        }
        subtype[index_string] = '\0';
        puts(subtype);
        */