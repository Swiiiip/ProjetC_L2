#include "structures.h"
#include <math.h>
#include <stdlib.h>

#define CATEGORY_SIZE 4
const char* categories[CATEGORY_SIZE]={"Nom","Adj","Adv","Ver"};

//Initialisation of a node and of its fields
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

//Looks in a t_ht_list for a specific letter and returns the p_node containing it
p_node search_letter(t_ht_list_node siblings, char letter)
{
    p_node temp = siblings.head;
    while(temp != NULL)
    {
        if(temp->letter == letter)
            return temp;
        temp = temp->next;
    }
    return NULL;//returns NULL if the letter is not found
}

//Add a base form to the tree and returns the node containing the last letter of the word
p_node add_fbase(t_ht_list_node * siblings, p_node my_node,char * f_base, int index)
{
    if(f_base[index] == '\0'){//If the end of the word is reached, we set its fields at NULL
        my_node->next_letters.head = NULL;
        my_node->next_letters.tail = NULL;
        my_node->next_letters.size = 0;
        return my_node;
    }

    p_node temp = search_letter(*siblings, f_base[index]);//We look for the letter in the list of siblings
    if(temp == NULL)//If the letter was not found
    {
        temp = create_node(f_base[index]);//Create the corresponding node to add
        if(siblings->head == NULL)//Add it at the head if the list was empty 
        {
            siblings->head = temp;
            siblings->tail = temp;
            siblings->size = 1;
        }
        else//Or at the tail if not
        {
            siblings->tail->next = temp;
            siblings->tail = temp;
            siblings->size ++;
        }
    }
    add_fbase(&(temp->next_letters), temp, f_base, index + 1);//Recursive call to add the next letter of the word
}

//create a cell containing all the contracted forms
p_cell add_fflechie(char* fflechie, char* type){

    int type_int = conversion_type(type,1);//Encoding of the type
    
    if(type_int == -1) //exclude forbidden typed p_cell
        return NULL;

    else{
        //create cell for fflechie
        p_cell my_cell = (p_cell)malloc(sizeof(t_cell));

        my_cell->forme_flechie = (char*)malloc(sizeof(char) * (strlen(fflechie) + 1));
        strcpy(my_cell->forme_flechie, fflechie);

        my_cell->number_type = type_int;
        my_cell->next = NULL;

        return my_cell;
    }
}

//Add a word to the tree with its base form and its contracted forms
void add_word(t_ht_list_node *the_root, char *fbase, char *fflechie, char *types)
{
    p_node my_node = add_fbase(the_root, NULL, fbase, 0);//We first add the base form
    if(types != NULL){
        //Add the forme flechie to the list of the last node
        char *type, *token = strtok(types, ":");
        while(token != NULL){
            type = token;
            token = strtok(NULL, ":");
            
            p_cell my_cell = add_fflechie(fflechie, type);//Create the cell of the contracted forms 
            
            if(my_cell!=NULL){//And connect that cell to the head of the list of contracted forms of the last node of the fbase

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

//Initialize an empty tree with all its fields
t_tree create_empty_tree()
{
    t_tree mytree;
    mytree.roots.head = NULL;
    mytree.roots.tail = NULL;
    mytree.roots.size = 0;
    return mytree;

}

//Calls all functions necessary to fully extract the dictionary into a tree
t_tree* fill_trees()
{   
    //Creation of the array of trees
    t_tree* trees = malloc(CATEGORY_SIZE*sizeof(t_tree));
    
    for(int i=0; i<CATEGORY_SIZE ; i++)
        trees[i] = create_empty_tree();

    //Reading of the file line by line
    char line[110];
    char* f_flechie, *f_base, *category, *type;
    FILE *dictionary;

    //Opening of the file
    //dictionary = fopen("dictionnaire.txt","r");
    //dictionary = fopen("minidictionnary.txt","r");
    dictionary = fopen("dictionnaire_non_accentue.txt","r");

    //The while returns each line one by one as a string, line 
    while(fgets(line,110,dictionary)){   

        split_line(line, &f_flechie, &f_base, &category, &type);
    
        //Now that we have each information of a line into the correct variables, we add it to the correct tree
        for(int i = 0 ; i < CATEGORY_SIZE ; i++)
            if (!strcmp(categories[i],category))//Looks if the category we have is among the $CATEGORY_SIZE we consider
                add_word(&(trees[i].roots), f_base, f_flechie, type);

    }
    fclose(dictionary);
    return trees;//We return the array of trees to be able to use them everywhere
}

//Developper-destined function to display all words in the tree
void print_tree_paths(t_ht_list_node roots)
{
    char* path = malloc(roots.size*sizeof(char));
    p_node tmp = roots.head;
    for(int i = 0 ; i < roots.size ; i++){//For each letter of the root
        printf("+---+\n| %c |\n+---+  ", tmp->letter);
        print_node_paths(tmp, path, 0);//Print all paths of the subtree
        tmp = tmp->next;
        printf("\n\n\n [ENTER] \n\n\n");
        getchar();
    }
}

//Prints all the paths of a node
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

//Search a if a word exists in the tree from its base form
int search_fbase(t_ht_list_node roots, char *fbase, int index)
{
    p_node tmp = roots.head;//The variable that will go through the list of letters for each layer
    int i = 0;
    while(tmp != NULL && i < roots.size){//We loop through the list of letters of the layer
        if(tmp->letter == fbase[index]){//If we find the letter we are looking for
            
            if(index == strlen(fbase)-1)//If we are at the end of the word
            {
                printf("\nThe word '%s' has been found.\n", fbase);
                print_fflechies(tmp);//Print all of its contracted forms and its type
                return 1;//Return true, the word is found
            }
            else
                return search_fbase(tmp->next_letters, fbase, index+1);//Recursive call while we're not at the end of the word
        }
        tmp = tmp->next;
        i++;
    }
    return 0;//If we reach this point, the word is not found so return false
}

//Search a if a word exists in the tree from its contracted form
void search_fflechie(t_ht_list_node roots, char *fflechie, int*found, char *fbase_array)
{
    p_node tmp = roots.head;//The variable that will go through the list of letters for each layer
    int i = 0;
    while(tmp != NULL && i < roots.size && !*found)//We loop while we're not done with the layer and we haven't found the word
    {
        if(tmp->fflechies.head != NULL)//Check whenever a word has at least one contracted form
        {
            p_cell tmp_cell = tmp->fflechies.head;
            while(tmp_cell != NULL)//Run through each of its contracted forms to check if it's here
            {
                if(!strcmp(tmp_cell->forme_flechie, fflechie))
                {
                    printf("\nThe word '%s' has been found.\n", fflechie);
                    *found = 1;//Change found to true
                    return;
                }
                tmp_cell = tmp_cell->next;
            }
        }
        //strcat(fbase_array, tmp->letter);
        search_fflechie(tmp->next_letters, fflechie, found, fbase_array);//Recursive call to the next layer
        tmp = tmp->next;
        i++;
    }
}

//Allows to pick randomly a base form from the tree
void extract_random_fbase(t_tree mytree)
{
    //We get the last node of the word we're randomly extracting, the parameter 1 is to indicate that we print the word
    p_node last_node_fbase = random_path(mytree.roots,mytree.roots.head,1);
    printf("' is a base form. "); 
    print_fflechies(last_node_fbase);//Print all of its contracted forms and its type
}

//Used to print all fflechies of a fbase
void print_fflechies(p_node leaf)
{

    if(leaf->fflechies.size == 1)//If there is only one fflechie, we use the singular in the sentence
        printf("Its contracted form is :\n\t- '%s' which is of type %s.\n\n",leaf->fflechies.head->forme_flechie, conversion_string(leaf->fflechies.head->number_type, 1));
    else 
    {
        printf("Its contracted forms are :\n");
        p_cell temp = leaf->fflechies.head;
        while(temp != NULL)
        {
            printf("\t- '%s' which is of type %s\n",temp->forme_flechie,conversion_string(temp->number_type, 1));
            temp = temp->next;
        }
        printf("\n");

    }
}

//Used to pick randomly a path in the tree
p_node random_path(t_ht_list_node current_list, p_node current_node, int print_path)
{
    if(current_node->fflechies.head != NULL || current_list.head == NULL)//If we get to a leaf or if we find a p_node that has contracted forms
    {
        if(rand() % number_paths(current_node) == 0 || current_list.head == NULL){//1 chance out of the number of paths to stop the path 
            return current_node;//And we return the last node of the word in that case
        }
    }        
    
    //Main recursive body
    int random = rand() % current_list.size;//Pick a random number between 0 and the number of letters in the current layer
    p_node temp = current_list.head;
    for(int i = 0 ; i < random ; i++)//We get to the random letter
        temp = temp->next;
    if(print_path)//Mode to print each letter as we go through the path
    {
        printf("%c",temp->letter);
        return random_path(temp->next_letters,temp,1);//Recursive call to the next layer, with display 
    }
    return random_path(temp->next_letters,temp,0);//Recursive call to the next layer, without display        
}

//Used to get the number of paths from a node, which is usefull to have equal probability in random_path
int number_paths(p_node current)
{
    if(current->next_letters.head == NULL)//Stop condition if we're at a leaf
        return 1;
    else
    {
        int sum = 0;
        p_node temp = current->next_letters.head;
        while(temp != NULL)
        {
            sum += number_paths(temp);
            temp = temp->next;
        }
        return sum;//Return the sum of all the paths from the current node
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


void generate_modele1(t_tree * trees, int mode_fflechie)
{
    if(mode_fflechie){
 
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
   
    int noun_type_1 = conversion_type(noun_type, 0);
    int adj_type = noun_type_1;
    strcat(sub_verb_type, "+P3");
    int verb_type = conversion_type(sub_verb_type, 0);
    //int verb_type = conversion_type(random_type);
 
    char * noun1_flechie = malloc(50*sizeof(char));
    char * adj_flechie = malloc(50*sizeof(char));
    char * verb_flechie = malloc(50*sizeof(char));
    char * noun2_flechie = malloc(50*sizeof(char));
 
    // Finding a noun that correcsponds to the type
    noun1_flechie = finding_fflechie_corresponding_to_type(trees[NOM], noun_type_1);
    // Finding an adjective that correcsponds to the type
    adj_flechie = finding_fflechie_corresponding_to_type(trees[ADJ], adj_type);
    // Finding a verb that correcsponds to the type
    verb_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type);
    //Finding a random second noun
   
    p_node noun2 = random_path(trees[0].roots,trees[0].roots.head ,0);
 
    int i = rand() % (noun2->fflechies.size);
    p_cell temp = noun2->fflechies.head;
    for(int j = 0; j < i; j++)
        temp = temp->next;
 
    noun2_flechie = temp->forme_flechie;
    int noun_type_2 = temp->number_type; // Used for the determinant of the second noun
 
    char * determinant_noun1 = determinant_generator(noun_type_1);
    char * determinant_noun2 = determinant_generator(noun_type_2);
 
    printf("%s %s %s %s %s %s.", determinant_noun1, noun1_flechie, adj_flechie, verb_flechie, determinant_noun2, noun2_flechie);
   
    }
 
    else{
        random_path(trees[NOM].roots, trees[NOM].roots.head, 1);
        printf(" ");
        random_path(trees[ADJ].roots, trees[ADJ].roots.head, 1);
        printf(" ");
        random_path(trees[VER].roots, trees[VER].roots.head, 1);
        printf(" ");
        random_path(trees[NOM].roots, trees[NOM].roots.head, 1);
        printf(".\n");
   
    }
   
}


void generate_modele2(t_tree * trees, int mode_fflechie){
 
    if(mode_fflechie){
 
    char * noun_type = generate_random_type(0); // Example "Mas+SG"
    char * noun_type2 = generate_random_type(0); // Example "Fem+SG"
 
    char * sub_verb_type = (char*)malloc(5*sizeof(char));
 
    //Getting the  verb (since it has to agree with the first noun, we need to do the following)
    int founds = 0, cpt_noun =0, cpt_verb =0;
    while(cpt_noun < strlen(noun_type)){
 
        if (founds == 1){// start adding to the verb_type_2 only when we found the first "+", because verb_type_2 should not store "Mas" or "Fem" from the noun_type
            sub_verb_type[cpt_verb] = noun_type[cpt_noun];
            cpt_verb++;
        }
 
        if('+' == noun_type[cpt_noun]){
            founds++;
        }
        if(founds == 2){//Stop adding
            break;
        }
        cpt_noun++;
    }
 
    if(!strcmp(sub_verb_type, "InvPL")){
        strcpy(sub_verb_type, "SG");
    }
 
    strcat(sub_verb_type, "+P3");// Example "SG+P3"
   
    //Declaration of the types
    int noun_type_1 = conversion_type(noun_type, 0);
    int noun_type_2 = conversion_type(noun_type2, 0);
    int adj_type = noun_type_2;
    int verb_type = conversion_type(sub_verb_type, 0);
 
    //Declaration of the fflechies
    char * noun1_flechie = malloc(50*sizeof(char));
    char * verb1_flechie = malloc(50*sizeof(char));
    char * verb2_flechie = malloc(50*sizeof(char));
    char * noun2_flechie = malloc(50*sizeof(char));
    char * adj_flechie = malloc(50*sizeof(char));
    //Declaration of the determinants
    char * determinant_noun1 = determinant_generator(noun_type_1);
    char * determinant_noun2 = determinant_generator(noun_type_2);
   
 
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
 
    printf("%s %s qui %s %s %s %s %s.", determinant_noun1, noun1_flechie, verb1_flechie, verb2_flechie, determinant_noun2, noun2_flechie, adj_flechie);
 
 
    }
 
    else
    {
        random_path(trees[NOM].roots,trees[NOM].roots.head, 1);
        printf(" qui ");      
        random_path(trees[VER].roots,trees[VER].roots.head, 1);
        printf(" ");
        random_path(trees[VER].roots,trees[VER].roots.head, 1);
        printf(" ");
        random_path(trees[NOM].roots,trees[NOM].roots.head, 1);
        printf(" ");
        random_path(trees[ADJ].roots,trees[ADJ].roots.head, 1);
        printf(".\n");
    }
 
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
 
 
    char * noun_flechie = malloc(30*sizeof(char));
    //types
    int verb_type = conversion_type(verb_type_1, 1);
    char * noun_type = generate_random_type(0); // Example "Fem+PL"
    int noun_type_1 = conversion_type(noun_type, 1); //integer corresponding to "Mas+InvPL"
    noun_flechie = finding_fflechie_corresponding_to_type(trees[NOM], noun_type_1);
    noun_type_1 = conversion_type(noun_type, 0);
    strcpy(noun_type, conversion_string(noun_type_1, 0));
   
 
    // Declarations of the formes flechies
    char * verb1_flechie = malloc(30*sizeof(char));
    char * adverbe_flechie = malloc(30*sizeof(char));
   
    char * adj_flechie = malloc(30*sizeof(char));
    char * determinant_noun = malloc(7*sizeof(char));
    char * verb2_flechie = malloc(30*sizeof(char));
    char * verb_type_2 = malloc(5*sizeof(char));
   
    //Getting the first verb
    verb1_flechie = finding_fflechie_corresponding_to_type(trees[VER], verb_type);
 
    //Getting the first noun
   
 
    //Getting the determinant
    determinant_noun = determinant_generator(noun_type_1);
 
    //Getting the adjective
    adj_flechie = finding_fflechie_corresponding_to_type(trees[ADJ], noun_type_1);
 
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
 
    int verb_type_2_int = conversion_type(verb_type_2, 0);
 
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
    random_path(trees[ADV].roots, trees[ADV].roots.head, 1);
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
    else
    {
       
        random_path(trees[VER].roots, trees[VER].roots.head, 1);
        printf(" ");
        random_path(trees[ADV].roots, trees[ADV].roots.head, 1);
        printf(" et ");
        random_path(trees[NOM].roots, trees[NOM].roots.head, 1);
        printf(" ");
        random_path(trees[ADJ].roots, trees[ADJ].roots.head, 1);
        printf(" ");
        random_path(trees[VER].roots, trees[VER].roots.head, 1);
        printf(".\n");
 
    }
 
}


char * finding_fflechie_corresponding_to_type(t_tree my_tree, int type){
    char * fflechie = malloc(50*sizeof(char));
    int found = 0;
    while(!found)
    {
        p_node categorie = random_path(my_tree.roots, my_tree.roots.head, 0);
        p_cell temp = categorie->fflechies.head;
        for(int i = 0; i< categorie->fflechies.size; i++){
    
            if(is_type_in(type, temp->number_type))
            {
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

