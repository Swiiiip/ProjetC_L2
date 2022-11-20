#include "menu.h"
#include <time.h>


//Main menu where the major functionalities are proposed to the user
void main_menu(t_tree * my_trees)
{
    title("MAIN MENU");
    printf("\nHello and welcome to the random sentences generator !\n\n");
    int choice;
    char *user_input= malloc(sizeof(char)*30);

    do{
    printf("\nPlease choose an option:\n"
           "1 - Generate a random french sentence\n"
           "2 - Obtain a random word from our dictionary\n"
           "3 - Check if a word of your choice is in our dictionary, and add it if it isn't\n"
           "0 - Exit\n>>> ");
    gets(user_input);
    choice = intVerify(0, 3, user_input);//This is used in every input to verify it
    }while(choice == -1);

    switch (choice)
    {
        case 1:
            generate_sentence(my_trees);
            break;
        case 2:
            extract_random_word(my_trees);
            break;
        case 3:
            search_and_add_word(my_trees);
            break;
        case 0:
            return;
            break;
        default:
            printf("\nPlease choose a valid option!\n");
            break;
    }
    main_menu(my_trees);//This is used to loop the menu
}

//Sub menu for the sentence generation
void generate_sentence(t_tree * my_trees)
{
    title("GENERATE RANDOM SENTENCE");
    int sentence_model;
    int choice_form;
    char *user_input = malloc(sizeof(char)*30);

    do{
    printf("\nYou can generate a sentence from three different models :\n"
           "1 - Noun - Adjective - Verb - Noun\n"
           "2 - Noun - 'Qui' - Verb - Verb - Noun - Adjective\n"
           "3 - Pronoun - Verb - Adverb - 'et' - Noun - Adjectif - Verb\n\n");
    printf("Please choose a model :\n>>> ");
    gets(user_input);
    sentence_model = intVerify(1, 3, user_input);
    }while(sentence_model == -1);

    do{
    printf("\nPlease choose whether you want it in base form or contracted form:\n"
           "1 - Base form\n"
           "2 - Contracted form\n"
           "0 - To return to the main menu\n>>> ");
    gets(user_input);
    choice_form = intVerify(0, 2, user_input);
    }while(choice_form == -1);

    printf("\n");

    switch (sentence_model)
    {
        case 1://For the first model, which is "noun + adjective + verb + noun"
            
            if(choice_form == 1)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele1(my_trees,0);//For the sentence with the base forms
            }
            else if(choice_form == 2)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele1(my_trees,1);//For the sentence with the contracted forms
            }
            else
                return;
            break;

        case 2://For the second model, which is :"noun + 'qui' + verb + verb + noun + adjective"

            if(choice_form == 1)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele2(my_trees,0);//For the sentence with the base forms
            }
            else if(choice_form == 2)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele2(my_trees,1);//For the sentence with the contracted forms
            }
            else
                return;
            break;

        case 3://For the third model, which is : "pronoun + verb + adverbe + 'et' + noun + adjective + verb."

            if(choice_form == 1)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele3(my_trees,0);//For the sentence with the base forms
            }
            else if(choice_form == 2)
            {
                printf("\nThe sentence generated is :\n\n");
                generate_modele3(my_trees,1);//For the sentence with the contracted forms

            }
            else
                return;
            break;

        default:
            return;
            break;
    }
    printf("\n\n[PRESS ENTER TO CONTINUE]\n\n");
    getchar();
}

//Sub menu for the random word extraction
void extract_random_word(t_tree * my_trees)
{
    char *user_input = malloc(sizeof(char)*30);
    int word_type;
    title("EXTRACT RANDOM WORD");

    do{
        printf("\nHere, you can get a random word from our dictionary.\nPlease choose the type of word you want :\n"
               "1 - Noun\n"
               "2 - Adjective\n"
               "3 - Adverb\n"
               "4 - Verb\n"
               "0 - To return to the main menu\n>>> ");
        gets(user_input);
        word_type = intVerify(0, 4, user_input);
    }while(word_type == -1);

    switch (word_type)//Select the right tree to extract from
    {
        case 1:
            extract_random_fbase(my_trees[NOM]);
            break;
        case 2:
            extract_random_fbase(my_trees[ADJ]);
            break;
        case 3:
            extract_random_fbase(my_trees[ADV]);
            break;
        case 4:
            extract_random_fbase(my_trees[VER]);
            break;
        default:
            return;
            break;
    }
    printf("\n\n[PRESS ENTER TO CONTINUE]\n\n");
    getchar();
}    

//Sub menu for the word search call to the addition of words in the tree
void search_and_add_word(t_tree *my_trees)
{
    int choice_tree, choice_type, result;
    char word[30];
    char *user_input = malloc(sizeof(char)*30);
    title("SEARCH FOR A WORD");
    printf("\nHere, you can search for a word in our dictionary.\nIf it isn't in it, you can add it.\n\n");

    do{
        printf("\nPlease choose the type of word you want to search for :\n"
               "1 - Noun\n"
               "2 - Adjective\n"
               "3 - Adverb\n"
               "4 - Verb\n>>> ");
        gets(user_input);
        choice_tree = intVerify(1, 4, user_input);
    }while(choice_tree == -1);

    do{
        printf("\nPlease choose the type of word you want to search for :\n"
               "1 - Base form\n"
               "2 - Contracted form\n>>> ");
        gets(user_input);
        choice_type = intVerify(1, 2, user_input);
    }while(choice_type == -1);

    printf("\nPlease enter the word you want to check :\n>>> ");
    scanf("%s",word);

    if(choice_type == 1)//Looking for a base form
    {
        switch (choice_tree)
        {
            case 1:
            {
                result = search_fbase(my_trees[NOM].roots, word, 0);//Search for the word in the correct tree 
                if (!result)//If the word isn't in the tree
                    add_word_if_not_found(my_trees[NOM].roots, word, 1);//Add it to the tree with the 1 indicating it's a base form
                break;
            }
            case 2:
            {
                result = search_fbase(my_trees[ADJ].roots, word, 0);//Search for the word in the correct tree
                if (!result)//If the word isn't in the tree
                    add_word_if_not_found(my_trees[ADJ].roots, word, 1);//Add it to the tree with the 1 indicating it's a base form
                break;
            }
            case 3:
            {
                result = search_fbase(my_trees[ADV].roots, word, 0);//Search for the word in the correct tree
                if (!result)//If the word isn't in the tree
                    add_word_if_not_found(my_trees[ADV].roots, word, 1);//Add it to the tree with the 1 indicating it's a base form
                break;
            }
            case 4:
            {
                result = search_fbase(my_trees[VER].roots, word, 0);//Search for the word in the correct tree
                if(!result)//If the word isn't in the tree
                    add_word_if_not_found(my_trees[VER].roots, word, 1);//Add it to the tree with the 1 indicating it's a base form
                break;
            }
            default:
                printf("Invalid choice, please try again\n");
                break;
        }
    }
    else if(choice_type == 2)//Looking for a contracted form
    {
        int found = 0;
        switch (choice_tree)
        {
            case 1:
            {
                search_fflechie(my_trees[NOM].roots, word, &found);//Search for the word in the correct tree
                if (!found)//If the word isn't in the tree
                    add_word_if_not_found(my_trees[NOM].roots, word, 0);//Add it to the tree with the 0 indicating it's a contracted form
                break;
            }
            case 2:
            {
                search_fflechie(my_trees[ADJ].roots, word, &found);
                if (!found)
                    add_word_if_not_found(my_trees[ADJ].roots, word, 0);
                break;
            }
            case 3:
            {
                search_fflechie(my_trees[ADV].roots, word, &found);
                if (!found)
                    add_word_if_not_found(my_trees[ADV].roots, word, 0);
                break;
            }
            case 4:
            {
                search_fflechie(my_trees[VER].roots, word, &found);
                if(!found)
                    add_word_if_not_found(my_trees[VER].roots, word, 0);
                break;
            }
            default:
                printf("\nInvalid choice, please try again\n");
                break;
        }
    }
    else
    {
        printf("\nInvalid choice, please try again\n");
    }
    printf("\n\n[PRESS ENTER TO CONTINUE]\n\n");
    getchar();
}

//Sub menu to add a word to the tree
void add_word_if_not_found(t_ht_list_node root, char *given, int is_fbase)
{
    int add;
    char *user_input = malloc(sizeof(char)*30);

    do{
        printf("\nThe word '%s' was not found in our dictionary.\nWould you like to add it ?\n1 - Yes\n2 - No\n>>> ",given);
        gets(user_input);
        add = intVerify(1, 2, user_input);
    }while(add == -1);

    if(add == 1)//We want to add the word to the tree
    {
        int type_tree, type_fflechie;
        if(is_fbase)//The given is a base form
        {
            char* fflechie;

            do{
                printf("\nPlease choose the type of word you want to add :\n");
                printf("1 - Noun\n2 - Adjective\n3 - Adverb\n4 - Verb\n>>> ");
                gets(user_input);
                type_tree = intVerify(1, 4, user_input);
            }while(type_tree == -1);
            
            printf("\nPlease enter a valid 'forme flechie' for your word :\n>>> ");
            scanf("%s", fflechie);//We ask the user to enter a valid contracted form for the word he wants to add

            switch (type_tree)
            {
                case 1||2://Nouns and adjectives have the save type so we can regroup them 
                {

                    do{
                        printf("\nPlease choose the type of the 'forme flechie' you just entered :\n"
                               "1 - Masculine singular\n"
                               "2 - Feminine singular\n"
                               "3 - Masculine plural\n"
                               "4 - Feminine plural\n"
                               "0 - Return to the main menu>>> ");
                        gets(user_input);
                        type_fflechie = intVerify(0, 4, user_input);
                    }while(type_fflechie == -1);
                    
                    switch(type_fflechie)//For each possible type of noun/adjective, we add the total word to the correct tree
                    {
                        case 1:
                            add_word(&root, given, fflechie, "Mas+SG");
                            break;
                        case 2:
                            add_word(&root, given, fflechie, "Fem+SG");
                            break;
                        case 3:
                            add_word(&root, given, fflechie, "Mas+PL");
                            break;
                        case 4:
                            add_word(&root, given, fflechie, "Fem+PL");
                            break;
                        default:
                            return;
                            break;
                    }
                    break;
                }
                case 3://For the adverb, the base form and contracted form are the same
                {
                    add_word(&root, given, given, NULL);//We add it with NULL as the type since adverbs don't have a type
                    break;
                }
                case 4://For the verbs, we need many informations : tense, person and singular/plural
                {
                    int tense;
                    char *type = malloc(sizeof(char)*12);
                    strcpy(type,"");

                    do{
                        printf("\nSince it is a verb, let's start by adding its tense:\n"
                               "1 - Infinitive\n"
                               "2 - Present\n"
                               "3 - Imperfect of the indicative\n"
                               "4 - Subjunctive present\n>>> ");
                        gets(user_input);
                        tense = intVerify(1, 4, user_input);
                    }while(tense == -1);

                    if (tense == 1)//For the infinitive, we don't need more informations
                    {
                        add_word(&root, given, fflechie, "Inf");
                        return;
                    }
                        
                    do{
                        printf("\nPlease select the type of the 'forme flechie' you just entered:\n"
                               "1 - First person of the singular\n"
                               "2 - Second person of the singular\n"
                               "3 - Third person of the singular\n"
                               "4 - First person of the plural\n"
                               "5 - Second person of the plural\n"
                               "6 - Third person of the plural\n>>> ");
                        gets(user_input);
                        type_fflechie = intVerify(1, 6, user_input);
                    }while(type_fflechie == -1);

                    //We add the tense to the type
                    if (tense == 2)
                        strcat(type, "IPre");
                    else if (tense == 3)
                        strcat(type, "IImp");
                    else if (tense == 4)
                        strcat(type, "SPre");
                    else
                    {
                        printf("Invalid choice, please try again\n");
                        return;
                    }

                    switch(type_fflechie)//Then for each possiblitity, we complete the type
                    {
                        case 1:
                            strcat(type, "+SG+P1");
                            break;
                        case 2:
                            strcat(type, "+SG+P2");
                            break;
                        case 3:
                            strcat(type, "+SG+P3");
                            break;
                        case 4:
                            strcat(type, "+PL+P1");
                            break;
                        case 5:
                            strcat(type, "+PL+P2");
                            break;
                        case 6:
                            strcat(type, "+PL+P3");
                            break;
                        default:
                        {
                            printf("Invalid choice, please try again\n");
                            return;
                            break;
                        }
                    }

                    add_word(&root, given, fflechie, type);//We add the word to the tree with everyting we just found
                    break;
                }
                default:
                    printf("Invalid choice, please try again\n");
                    break;
            }
        }

        else//Same as before but now the given is the contracted form

        {

            char* fbase = malloc(sizeof(char)*30);

            do{
                printf("\nPlease choose the type of word you want to add :\n");
                printf("1 - Noun\n2 - Adjective\n3 - Adverb\n4 - Verb\n>>> ");
                gets(user_input);
                type_tree = intVerify(1, 4, user_input);
            }while(type_tree == -1);

            printf("\nPlease enter the valid base form of your word :\n>>> ");
            scanf("%s", fbase);//We now want the user to enter the base form of the contracted form he just entered

            switch (type_tree)
            {
                case 1||2:
                {
                    do{
                        printf("\nPlease choose the type of the 'forme flechie' you searched for :\n"
                               "1 - Masculine singular\n"
                               "2 - Feminine singular\n"
                               "3 - Masculine plural\n"
                               "4 - Feminine plural\n"
                               "0 - Return to the main menu>>> ");
                        gets(user_input);
                        type_fflechie = intVerify(0, 4, user_input);
                    }while(type_fflechie == -1);

                    switch(type_fflechie)
                    {
                        case 1:
                            add_word(&root, fbase, given, "Mas+SG");
                            break;
                        case 2:
                            add_word(&root, fbase, given, "Fem+SG");
                            break;
                        case 3:
                            add_word(&root, fbase, given, "Mas+PL");
                            break;
                        case 4:
                            add_word(&root, fbase, given, "Fem+PL");
                            break;
                        default:
                            return;
                            break;
                    }
                    break;
                }
                case 3:
                {
                    add_word(&root, given, given, NULL);
                    return;
                    break;
                }
                case 4:
                {
                    int tense;
                    char *type = malloc(sizeof(char)*12);
                    strcpy(type,"");

                    do{
                        printf("\nSince it is a verb, let's start by adding its tense:\n"
                               "1 - Infinitive\n"
                               "2 - Present\n"
                               "3 - Imperfect of the indicative\n"
                               "4 - Subjunctive present\n>>> ");
                        gets(user_input);
                        tense = intVerify(1, 4, user_input);
                    }while(tense == -1);

                    if (tense == 1)
                    {
                        add_word(&root, fbase, given, "Inf");
                        return;
                    }
                    
                    do{
                        printf("\nPlease select the type of the 'forme flechie' you just entered:\n"
                               "1 - First person of the singular\n"
                               "2 - Second person of the singular\n"
                               "3 - Third person of the singular\n"
                               "4 - First person of the plural\n"
                               "5 - Second person of the plural\n"
                               "6 - Third person of the plural\n>>> ");
                        gets(user_input);
                        type_fflechie = intVerify(1, 6, user_input);
                    }while(type_fflechie == -1);

                    if (tense == 2)
                        strcat(type, "IPre");
                    else if (tense == 3)
                        strcat(type, "IImp");
                    else if (tense == 4)
                        strcat(type, "SPre");
                    else
                    {
                        printf("Invalid choice, please try again\n");
                        return;
                    }
                    
                    switch(type_fflechie)
                    {
                        case 1:
                            strcat(type, "SG+P1");
                            break;
                        case 2:
                            strcat(type, "SG+P2");
                            break;
                        case 3:
                            strcat(type, "SG+P3");
                            break;
                        case 4:
                            strcat(type, "PL+P1");
                            break;
                        case 5:
                            strcat(type, "PL+P2");
                            break;
                        case 6:
                            strcat(type, "PL+P3");
                            break;
                        default:
                        {
                            printf("Invalid choice, please try again\n");
                            return;
                            break;
                        }
                    }

                    add_word(&root, fbase, given, type);
                    break;
                }

                default:
                    printf("Invalid choice, please try again\n");
                    break;
            }
        }
        printf("\nWord added successfully! Try searching it again\n");
        printf("\n\n[PRESS ENTER TO CONTINUE]\n\n");
        getchar();
    }
}

//Print the title in the correct way
void title(char *title)
{
    printf("\n\n================================ %s ================================\n\n", title);
}
