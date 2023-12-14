#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "forca.h"

// Vars
char secret_word[WORD_LENGTH];
char guesses[26];
int guesses_gone = 0;

void opening(){
    printf("***********************\n");
    printf("*    Jogo da forca    *\n");
    printf("***********************\n\n");
}

void add_word(){

    char user_ans;
    char new_word[WORD_LENGTH];

    printf("Voce deseja adicionar uma nova palavra no jogo? (S/N)\n");
    scanf(" %c", &user_ans);

    if(user_ans == 'S'){
        printf("Qual a nova palavra?");
        scanf("%s", &new_word);

        FILE* f;

        f = fopen("secret_words.txt", "r+");

        if(f == 0){
            printf("Desculpe, banco de dados indisponivel\n\n");
            exit(1);
        }

        int qnt;
        fscanf(f, "%d", &qnt);
        qnt++;

        // Go to first line
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", qnt);
        // Go to last line
        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", new_word);

        fclose(f);

    }
}

void choose_word(){
    FILE* f;

    f = fopen("secret_words.txt", "r");
    if(f == 0){
        printf("Desculpe, banco de dados indisponivel\n\n");
        exit(1);
    }

    int word_qnt;
    fscanf(f, "%d", &word_qnt);


    srand(time(0));
    int randomic = rand() % word_qnt;

    for(int i = 0; i <= randomic; i++){
        fscanf(f, "%s", &secret_word);
    }

    fclose(f);
}

void user_guessing(){

    char user_guess;
    printf("Chute uma letra: ");
    scanf(" %c", &user_guess);

    guesses[guesses_gone] = user_guess;
    guesses_gone++;
}

// Check if user got one letter
int already_guessed(char letter){

    int found = 0;

    for(int j = 0; j < guesses_gone; j++){
        if(guesses[j] == letter){
            found = 1;
            break;
        }
    }

    return found;
}

// Win
int win(){
    for(int i = 0; i < strlen(secret_word); i++){
        if(!already_guessed(secret_word[i])){
            return 0;
        }
    }

    return 1;
}

void draw_hang(){

    int errors = missed_guesses();

    printf("  _______      \n");
    printf(" |/      |     \n");
    printf(" |      %c%c%c \n", (errors >= 1 ? '(' : ' '),
                                (errors >= 1 ? '_' : ' '),
                                (errors >= 1 ? ')' : ' '));
    printf(" |      %c%c%c \n", (errors >= 3 ? '\\' : ' '),
                                (errors >= 2 ? '|' : ' '),
                                (errors >= 3 ? '/' : ' '));
    printf(" |       %c    \n", (errors >= 2 ? '|' : ' '));
    printf(" |      %c %c  \n", (errors >= 4 ? '/' : ' '),
                                (errors >= 4 ? '\\' : ' '));
    printf(" |             \n");
    printf("_|___          \n");
    printf("\n\n");

    // Logic
    for(int i = 0; i < strlen(secret_word); i++){

        // Iterate through guesses and check if it's equal to the letter in word
        int found = already_guessed(secret_word[i]);

        // Print underline if nothin find or letter if is correct
        if(found){
            printf("%c ", secret_word[i]);
        }else{
            printf("_ ");
        }
    }
    printf("\n\n");
}

int missed_guesses(){
    int errors = 0;

    for(int i = 0; i < guesses_gone; i++){

        int exists = 0;
        for(int j = 0; j < strlen(secret_word); j++){
            if(guesses[i] == secret_word[j]){
                exists = 1;
                break;
            }
        }
        if(!exists){
            errors++;
        }
    }

    return errors;
}

int hanged(){

    return missed_guesses() >= 5;

}

int main()
{

    choose_word();
    opening();

    do{

        // Draw hangman game
        draw_hang();

        // Gettin user input
        user_guessing();

    }while(!win() && !hanged());

    if(win()){
        printf("Parabens voce ganhou!!!\n");
    }else{
        printf("Voce morreu! FOI ENFORCADO\n");
    }

    add_word();

    return 0;
}
