#include "structures.h"
#include "functions.h"
#include <math.h>
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

int conversion_type(char* type){
    int type_int = 0;
    char * temp = (char*)malloc((strlen(type)+1) * sizeof(char));
    strcpy(temp, type);
    char *subtype, *token = strtok(temp, "+");
    
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
        else if(!strcmp(subtype, "Mas"))
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

    return type_int;
}

p_cell add_fflechie(char* fflechie, char* type){

    int type_int = conversion_type(type);
    
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
    
<<<<<<< Updated upstream
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

    t_tree t_trees[4] = {noun_tree, adj_tree, verb_tree, adv_tree};
    generate_modele1(t_trees);
=======
    
>>>>>>> Stashed changes

    
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
    //search_fbase(noun_tree.roots, "fqdsfq",0);

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

<<<<<<< Updated upstream
p_node random_path(p_node current)
=======
p_node random_path(p_node current, int print_path)
{
    //printf("I eneter the function");
    if(current->fflechies.head != NULL || current->next_letters.head == NULL)
    {
        //printf("I am a leaf");
       
        if(rand() % number_paths(current) == 0 || current->next_letters.head == NULL){
            //printf("test1 last letter");
            return current;
        }
    }        
    
    //printf("test2 ");
    int random = rand() % current->next_letters.size;
    p_node temp = current->next_letters.head;
    for(int i = 0 ; i < random ; i++)
        temp = temp->next;
        //printf("In the for");
    if(print_path)
    {
        printf("%c",temp->letter);
        return random_path(temp,1);
    }
    else 
        return random_path(temp,0);            

}


int number_paths(p_node current)
>>>>>>> Stashed changes
{
    if(current->next_letters.head == NULL)
        return current;
    else
    {
        int random = rand() % current->next_letters.size;
        p_node temp = current->next_letters.head;
        for(int i = 0 ; i < random ; i++)
            temp = temp->next;
        return random_path(temp);
    }
}

char * generate_random_type(int mode){

    char * type = malloc(18*sizeof(char));
 
    char random_subtype1 [][7]= {"Mas","Fem","InvGen"};
    int index1 = rand() % 3;
    char random_subtype2[][6] = {"SG","PL","InvPL"};
    int index2 = rand() % 3;

    strcpy(type, random_subtype1[index1]);
    strcat(type, "+");
    strcat(type,random_subtype2[index2]);
    
   
    if(mode == 1){
    char random_subtype3[][3] = {"P1","P2", "P3"};
    int index3 = rand() % 3;
    char random_subtype4[][5] = {"Inf","IImp", "IPre", "SPre"};
    int index4 = rand() % 4;
    strcat(type, "+");
    strcat(type,random_subtype3[index3]);
    strcat(type, "+");
    strcat(type,random_subtype4[index4]);
    }
    return type;
    
}

char * generate_modele1(t_tree * trees)
{
    char * noun_type = generate_random_type(0); // Example "Mas+SG"
    char * sub_verb_type = (char*)malloc(5*sizeof(char)); 
    int founds = 0, cpt_noun =0, cpt_verb =0;
    while(cpt_noun < strlen(noun_type)){

        if (founds == 1){
            sub_verb_type[cpt_verb] = noun_type[cpt_noun];
            cpt_verb++;
        }

        if('+' == noun_type[cpt_noun]){
            founds++;
        }
        if(founds == 2){
            break;
        }
        cpt_noun++;
    }

    if(!strcmp(sub_verb_type, "InvPL")){
        strcpy(sub_verb_type, "SG");
    }
    
    int noun_type_1 = conversion_type(noun_type);
    int adj_type = noun_type_1;
    strcat(sub_verb_type, "+P3");
    int verb_type = conversion_type(sub_verb_type);
    //int verb_type = conversion_type(random_type);

    char * noun1_flechie = malloc(50*sizeof(char));
    char * adj_flechie = malloc(50*sizeof(char));
    char * verb_flechie = malloc(50*sizeof(char));
    char * noun2_flechie = malloc(50*sizeof(char));

    // Finding a noun that correcsponds to the type
    noun1_flechie = finding_fflechie_corresponding_to_type(trees[0], noun_type_1);
    // Finding an adjective that correcsponds to the type
    adj_flechie = finding_fflechie_corresponding_to_type(trees[1], adj_type);
    // Finding a verb that correcsponds to the type
    verb_flechie = finding_fflechie_corresponding_to_type(trees[2], verb_type);
    //Finding a random second noun
    p_node tmp = trees[0].roots.head;
        int random = rand() % trees[0].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
    p_node noun2 = random_path(tmp);

    int i = rand() % (noun2->fflechies.size);
    p_cell temp = noun2->fflechies.head;
    for(int j = 0; j < i; j++)
        temp = temp->next;

    noun2_flechie = temp->forme_flechie;
    int noun_type_2 = temp->number_type; // Used for the determinant of the second noun

    char * determinant_noun1 = determinant_generator(noun_type_1);
    char * determinant_noun2 = determinant_generator(noun_type_2);

    char * sentence = (char*)malloc(100*sizeof(char));
    strcpy(sentence, determinant_noun1);
    strcat(sentence, " ");
    strcat(sentence, noun1_flechie);
    strcat(sentence, " ");
    strcat(sentence, adj_flechie);
    strcat(sentence, " ");
    strcat(sentence, verb_flechie);
    strcat(sentence, " ");
    strcat(sentence, determinant_noun2);
    strcat(sentence, " ");
    strcat(sentence, noun2_flechie);
<<<<<<< Updated upstream
    printf("%s", sentence);
=======
    printf("%s.\n", sentence);



    }
    else{
        p_node tmp = trees[NOM].roots.head;
        //printf("SIZE =%d", trees[NOM].roots.size);
        int random = rand() % trees[NOM].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        //printf("SIZE HELLO = %d", tmp->next_letters.size);
        random_path(tmp, 1);

        printf(" ");

        tmp = trees[ADJ].roots.head;
        random = rand() % trees[ADJ].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(" ");

        tmp = trees[VER].roots.head;
        random = rand() % trees[VER].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(" ");

        tmp = trees[NOM].roots.head;
        random = rand() % trees[NOM].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);
        printf(".\n");
    
    }
    
}

void generate_modele2(t_tree * trees, int mode_fflechie){

    if(mode_fflechie){
        
    char * noun_type = generate_random_type(0); // Example "Mas+SG"
    char * noun_type2 = generate_random_type(0); // Example "Fem+SG"

    char * sub_verb_type = (char*)malloc(5*sizeof(char)); 
    int founds = 0, cpt_noun =0, cpt_verb =0;
    while(cpt_noun < strlen(noun_type)){

        if (founds == 1){
            sub_verb_type[cpt_verb] = noun_type[cpt_noun];
            cpt_verb++;
        }

        if('+' == noun_type[cpt_noun]){
            founds++;
        }
        if(founds == 2){
            break;
        }
        cpt_noun++;
    }

    if(!strcmp(sub_verb_type, "InvPL")){
        strcpy(sub_verb_type, "SG");
    }
    strcat(sub_verb_type, "+P3");
    
    int noun_type_1 = conversion_type(noun_type);
    int noun_type_2 = conversion_type(noun_type2);
    int adj_type = noun_type_2;
    int verb_type = conversion_type(sub_verb_type);
    //int verb_type = conversion_type(random_type);

    char * noun1_flechie = malloc(50*sizeof(char));
    char * verb1_flechie = malloc(50*sizeof(char));
    char * verb2_flechie = malloc(50*sizeof(char));
    char * noun2_flechie = malloc(50*sizeof(char));
    char * adj_flechie = malloc(50*sizeof(char));
    

    // Finding a noun that correcsponds to the type
    noun1_flechie = finding_fflechie_corresponding_to_type(trees[NOM], noun_type_1);
    // Finding a verb that correcsponds to the type
    verb1_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type);
    // Finding a second verb that correcsponds to the type
    verb2_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type);
    // Finding a second noun that correcsponds to another ranodm type
    noun2_flechie = finding_fflechie_corresponding_to_type(trees[NOM], noun_type_2);
    // Finding an adjective that correcsponds to the type
    adj_flechie = finding_fflechie_corresponding_to_type(trees[ADJ], adj_type);

    char * determinant_noun1 = determinant_generator(noun_type_1);
    char * determinant_noun2 = determinant_generator(noun_type_2);

    printf("%s %s qui %s %s %s %s %s", determinant_noun1, noun1_flechie, verb1_flechie, verb2_flechie, determinant_noun2, noun2_flechie, adj_flechie);



    }
    else{

        p_node tmp = trees[NOM].roots.head;
        int random = rand() % trees[NOM].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);
>>>>>>> Stashed changes

    return sentence;
    
}

void generate_modele3(t_tree * trees, int mode_fflechie){

    //THE SENTENCE IS OF TYPE "pronom + verb + adverbe + et + determinat + noun + adjective + verb."

    if(mode_fflechie){
    
    //Declarations
    char * verbal_tenses[6] = {"SPre", "IPre", "IImp"};
    char * pronom_S[5] = {"je", "tu", "il", "elle"};
    char * pronom_P[17] = {"nous", "vous", "ils", "elles"};

    char * pronom = malloc(10*sizeof(char));
    char * verb_type_1 = malloc(20*sizeof(char)); 

    //Random integers for the random selection of the words
    int random_Pronom = rand() % 3;
    int random_SP = rand() % 2;
    int random_tense = rand() % 3;

    char * tense = {verbal_tenses[random_tense]}; //Random tense, tense is now either "SPre", "IPre", "IImp" or "Inf"

    if(random_SP){//if random_SP == 1, the pronoun is in plural
        strcpy(pronom, pronom_P[random_Pronom]);//pronom is now either "nous", "vous", "ils" or "elles"
        strcpy(verb_type_1, "PL+");
    }
    else{
        strcpy(pronom, pronom_S[random_Pronom]);//pronom is now either "je", "tu", "il" or "elle"
        strcpy(verb_type_1, "SG+");
        
    }
    strcat(verb_type_1, "P");//verb_type_1 would now be "SG+P" for example (depending on the random values, it could be "PL+P" )
    random_Pronom++;
    char str[5];
    sprintf(str, "%d", random_Pronom); // integer to string
    strcat(verb_type_1, str);//verb_type_1 would now be "SG+P1" for example (also depending on the random values)
    strcat(verb_type_1, "+");
    strcat(verb_type_1, tense);//finally verb_type_1 would now be "SG+P1+SPre" for example (also depending on the random values)

    //types
    int verb_type = conversion_type(verb_type_1);
    char * noun_type = generate_random_type(0); // Example "Mas+SG"
    int noun_type_1 = conversion_type(noun_type);
    int adj_type = noun_type_1;


    // Declarations of the formes flechies
    char * verb1_flechie = malloc(30*sizeof(char));
    char * adverbe_flechie = malloc(30*sizeof(char));
    char * noun_flechie = malloc(30*sizeof(char));
    char * adj_flechie = malloc(30*sizeof(char));
    char * determinant_noun = malloc(7*sizeof(char));
    char * verb2_flechie = malloc(30*sizeof(char));
    char * verb_type_2 = malloc(5*sizeof(char));
    
    //Getting the first verb
    verb1_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type);

    //Getting the first noun
    noun_flechie = finding_fflechie_corresponding_to_type(trees[NOM], noun_type_1);

    //Getting the determinant
    determinant_noun = determinant_generator(noun_type_1);

    //Getting the adjective
    adj_flechie = finding_fflechie_corresponding_to_type(trees[ADJ], adj_type);

    //Getting the second verb (since it has to agree with the first noun, we need to do the following)
 
    int founds = 0, cpt_noun =0, cpt_verb =0;
    while(cpt_noun < strlen(noun_type)){

        if (founds == 1){// start adding to the verb_type_2 only when we found the first "+", because verb_type_2 should not store "Mas" or "Fem" from the noun_type
            verb_type_2[cpt_verb] = noun_type[cpt_noun];
            cpt_verb++;
        }

        if('+' == noun_type[cpt_noun]){
            founds++;
        }

        if(founds == 2){// stop when we fninsh adding the "SG" or "PL" to the verb_type_2
            break;
        }
        cpt_noun++;
    }

    if(!strcmp(verb_type_2, "InvPL")){
        strcpy(verb_type_2, "SG");
    }

    strcat(verb_type_2, "+P3+");//In our sentence, the second verb is always in the third person
    strcat(verb_type_2, tense);//The tense should be the same as the first verb, to make the sentence make more sense

    int verb_type_2_int = conversion_type(verb_type_2);

    verb2_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type_2_int);
    
    //############## PRINTING THE SENTENCE####################

    //For the case when we have the verbs in the subjunctive mood (we implement a que or qu' in 2 places in the sentence)
    
    if(!strcmp(tense, "SPre")){
        if(!(strcmp(pronom, "il")) || !(strcmp(pronom, "ils")) || !(strcmp(pronom, "elle")) || !(strcmp(pronom, "elles")))
            printf("Qu'");
        else
            printf("Que ");
    }

    printf("%s %s ", pronom , verb1_flechie);

    //Printing the adverbe
    p_node tmp = trees[ADV].roots.head;
    int random = rand() % trees[ADV].roots.size;
    for (int i = 0;i < random; i++)//Access to the root chosen randomly
        tmp = tmp->next;
    printf("%c", tmp->letter);
    random_path(tmp, 1);
    //Printing the adverbe

    printf(" et ");

    if(!strcmp(tense, "SPre")){
        if(!(strcmp(determinant_noun, "un") || !(strcmp(determinant_noun, "une"))))
            printf("qu'");
        else
            printf("que ");
    }

    printf("%s %s %s %s.\n", determinant_noun, noun_flechie, adj_flechie, verb2_flechie);


    }
    /*else{

        p_node tmp = trees[NOM].roots.head;
        int random = rand() % trees[NOM].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(" qui ");

        tmp = trees[VER].roots.head;
        random = rand() % trees[VER].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(" ");

        tmp = trees[VER].roots.head;
        random = rand() % trees[VER].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(" ");

        tmp = trees[NOM].roots.head;
        random = rand() % trees[NOM].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);
        
        printf(" ");

        tmp = trees[ADJ].roots.head;
        random = rand() % trees[ADJ].roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        printf("%c", tmp->letter);
        random_path(tmp, 1);

        printf(".\n");
    
    }
    */
    

}


char * finding_fflechie_corresponding_to_type(t_tree my_tree, int type){
    char * fflechie = malloc(50*sizeof(char));
    int found = 0;
    while(!found)
    {
        p_node tmp = my_tree.roots.head;
        int random = rand() % my_tree.roots.size;
        for (int i = 0;i < random; i++)//Access to the root chosen randomly
            tmp = tmp->next;
        
        p_node categorie = random_path(tmp);
        p_cell temp = categorie->fflechies.head;
        for(int i = 0; i< categorie->fflechies.size; i++){
    
            if(is_type_in(type, temp->number_type)){

                found = 1;
                fflechie = temp->forme_flechie;
                break;
            }
            temp = temp->next;
        }
    }
    return fflechie;

}

char * determinant_generator(int type){

    char  dets_M_S[][7] = {"le", "un", "notre", "votre", "leur"};
    char  dets_F_S[][7] = {"la", "une","notre", "votre", "leur"};
    char  dets_M_F_P[][7] = {"les", "des", "mes", "tes", "ses", "nos", "vos", "leurs"};
    char * determinant_noun= malloc(7*sizeof(char));
    int random_integer_1 = rand() % 5;
    int random_integer_2 = rand() % 8;

   if(type == 6 || type == 7){
        strcpy(determinant_noun, dets_F_S[random_integer_1]);
    }
    else if(type == 9 || type == 10 || type == 11){
        strcpy(determinant_noun, dets_M_F_P[random_integer_2]);

    }else{
        strcpy(determinant_noun, dets_M_S[random_integer_1]);
    }
    return determinant_noun;


}

int is_type_in( int desired_type, int full_type){

    //convert decimal full_type into binary
    int i, binary_desired[11], binary_full[11];

    for(i = 0; i < 11; i++){
        binary_full[i] = full_type % 2;
        full_type /= 2;
    }

    //convert decimal desired_type into binary
    for(i = 0; i < 11; i++){
        binary_desired[i] = desired_type % 2;
        desired_type /= 2;
    }

    //check if desired type is in full type
    for(i = 11; i > 0; i--)
        if(binary_desired[i] == 1 && binary_full[i] == 0)
            return 0;
    
    return 1;
}



    

    


 


    
