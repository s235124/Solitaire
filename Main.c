#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
//#include <SDL.h>
//#include <ddraw.h>
//#include <SDL_image.h>
#include <string.h>

#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)

typedef struct CardStruct {
    int number;
    char suit;
    int column;
    bool isColumn;
    char displayedChars[2];
    struct CardStruct* next;
    bool facingDown;
} Card;

typedef Card column;



// Subroutines in the program
//void sdlExample();
void show (Card* head, column columns[]);
void startshow ();
void load();
void printFileLines(const char *filename, Card* cards);
Card* linkedv(const char *filename);
void printLinkedList(Card* head);
Card* iteratelist(int iteration, Card* head);
bool isCardPointingToColumn(Card* card);
int lengthoflist(Card* head);
void createcolumns (Card* finalCard, column columns[]);
Card* unappend(Card* head);
int longestcolumn (column columns[]);
void flipcard (column columns[]);
void parseInput(const char* input, char* filename);
Card* split(const char *filename, int split);
void insertCard(Card* head, int position, Card* newCard);
Card* randomShuffle(const char *filename);
void writeLinkedListToFile(const char *filename, Card* head);
void copyFile(const char *sourcePath, const char *destinationPath);
void createcolumns2 (Card* finalCard, column columns[]);
void flipcard2 (column columns[]);
void move(Card* from, Card* to, column columns[]);
Card* findCard(char*card, column columns[]);
void parseInput2(const char* input, char* card1, char* card2);
Card* findColumn(int iteration, Card* head);
int main(int argc, char* argv[]) {
    //sdlExample();
    Card cards[52];
    Card *head = NULL;
    char *relativePath = "../file.txt";
    char absolutePath[1024];

    column columns[7];
    char card1[3];
    char card2[3];
    if (realpath(relativePath, absolutePath) == NULL) {
        return 1;
    }

    head = linkedv(absolutePath);
    //head = split(absolutePath, 22);

    //createcolumns(head, columns);
    /*for(int i = 0;i < 7 ;i++){

        printLinkedList(columns[i].next);
    }*/
    //flipcard(columns);
    //show(head, columns);

    startshow();

    char input[100];
    char prevInput[100];
    char message[100];
    char filename[100];

    while (1) {
        strcpy(message, "OK");
        printf("INPUT: ");
        fflush(stdout);
        fgets(input, sizeof(input), stdin);
        strcpy(prevInput, input);
        // Check if input matches trigger string
        if (strstr(input, "SW") != NULL) {
            flipcard(columns);
        } else if (strstr(input, "LD") != NULL) {

            parseInput(input, filename);
            //printf("Filename: %s\n", filename);
            head = linkedv(filename);
            if (head != NULL) {
                writeLinkedListToFile("currentd.txt", head);

                createcolumns(head, columns);
            } else {
                strcpy(message, "Failed to load file.");
            }

        } else if (strstr(input, "SI") != NULL) {
            int splitNumber;
            if (sscanf(input, "SI %d", &splitNumber) == 1) {
                head = split("currentd.txt", splitNumber);
                if (head != NULL) {
                    createcolumns(head, columns);
                } else {
                    strcpy(message, "Failed to split the file.");
                }
            } else {
                strcpy(message, "Invalid input format for SI.");
            }
        } else if (strstr(input, "SR") != NULL) {
            //printf("length of the top pile %d \n", lengthoflist(head));
            head = randomShuffle("currentd.txt");
            writeLinkedListToFile("currentd.txt", head);
            createcolumns(head, columns);
            flipcard(columns);
        }else if (strstr(input, "SD") != NULL) {
            parseInput(input , filename);
            copyFile("currentd.txt",filename);

        }
            else if (strstr(input, "QQ") != NULL) {
                break;
            }else if (strstr(input, "P") != NULL) {
            head = linkedv("currentd.txt");
            createcolumns2(head, columns);
            flipcard2(columns);

        }else if (strstr(input, "->") != NULL) {
            parseInput2(input, card1, card2);
            //printf(" first char of card1 %c second char of card1 %c  ", card1[0], card1[1]);
            Card* cardc2 = findCard(card2, columns);
            Card* cardc1 = findCard(card1, columns);

            move(cardc2, cardc1, columns);
            printf(" card %c  %c  has moved ", card2[0], card2[1]);
        } else {
                printf("v.");
            }

            //show(head, columns);
            //move(columns[1].next->next, columns[3].next, columns);
            show(head,columns);
            printf("LAST COMMAND: %s", prevInput);
            printf("Message: %s\n", message);
        }

        return 0;

    }


void printFileLines(const char *filename, Card* cards) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        if (i == 0) {
            cards[i].displayedChars[0] = line[0];
            cards[i].displayedChars[1] = line[1];

            cards[i].next = 0;
        }
        else {
            cards[i].displayedChars[0] = line[0];
            cards[i].displayedChars[1] = line[1];

            cards[i].next = &(cards[i-1]);
            printf("%c  %c\n", cards[i].next -> displayedChars[0], cards[i].next ->displayedChars[1]);
        }

        printf("%c  %c\n", cards[i].displayedChars[0], cards[i].displayedChars[1]);

        i++;
    }

    fclose(file);
}


Card* linkedv(const char *filename){
    Card* head = NULL;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
//        printf("Could not open file %s\n", filename);
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Card* recentCard = (Card*)malloc(sizeof(Card));
        if (recentCard == NULL) {
            fclose(file);
            return NULL;
        }

        recentCard->displayedChars[0] = line[0];
        recentCard->displayedChars[1] = line[1];
        recentCard->facingDown = true;
        recentCard->isColumn = false;
        recentCard->next = NULL; //

        if (head == NULL) {
            // If the list is empty, set the new card as the head
            head = recentCard;
        } else {
            // Find the last card in the list and append the new card
            Card* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = recentCard;
        }
    }

    fclose(file);
    return head;
}

void printLinkedList(Card* head) {
    Card* current = head;



    while (current != NULL) {
        if (current->isColumn) {
            printf(" C%d", current->number);
            break;
        }

        printf("%c%c ", current->displayedChars[0], current->displayedChars[1]);
        current = current->next;
    }

    if (current == NULL) {
        printf("Reached the end of the list.\n");
    }
    printf("\n");
}

Card* iteratelist(int iteration, Card* head){
    if(head == NULL){
        return NULL;
    }
    Card* current = head;

    for(int i = 0; i<iteration; i++){
        //printf("%c  %c\n", current-> displayedChars[0], current->displayedChars[1]);
        if (current->isColumn) {
            return NULL;
        }
        current = current->next;

    }
    return current;

}
Card* findColumn(int iteration, Card* head){
    if(head == NULL){
        return NULL;
    }
    Card* current = head;

    for(int i = 0; i<iteration; i++){
        //printf("%c  %c\n", current-> displayedChars[0], current->displayedChars[1]);
        if (current->isColumn) {
            return current;
        }
        current = current->next;

    }
    return current;

}
int lengthoflist(Card* head){

    int length = 0;
    Card* current = head;

    while (current != NULL && !current->isColumn) {
        length++;
        current = current->next;
    }

    return length;

}

void createcolumns (Card* finalCard, column columns[]) {
    //for loop for creating the 7 columns
    for(int i = 0; i<7; i++){

        columns[i].column = i+1;
        columns[i].isColumn = true;
        columns[i].next = &columns[i];
    }
    int totalcards = lengthoflist(finalCard);
    //printf("%d", totalcards);
    int assignedcards = 0;
    Card* current = finalCard;
    Card* newHead= finalCard;
    //printf("%c  %c\n", current-> displayedChars[0], current->displayedChars[1]);
    while(assignedcards< 52){
        for(int i = 0;i < 7 ;i++){
            current = iteratelist(0, newHead);//fetch first elememt in list

            if(current == NULL){
                //printf("Current is null v");
                break;
            }
            //printf(" %c  %c\n", newHead-> displayedChars[0], newHead->displayedChars[1]);

            newHead = current->next;


            current->next = columns[i].next;//assign current cards pointer to column i
            current->column = i;
            printf("%c%c is in column %d", current->displayedChars[0],current->displayedChars[1], current->column);
            columns[i].next = current;//assign column i's pointer to current, such that column i points to the end of the head of the list
            //printf("columns %d %c  %c\n",i, columns[i].next-> displayedChars[0], columns[i].next->displayedChars[1]);
            assignedcards++;
        }
    }
}
void createcolumns2 (Card* finalCard, column columns[]) {
    //for loop for creating the 7 columns
    for(int i = 0; i<7; i++){

        columns[i].number = i+1;
        columns[i].isColumn = true;
        columns[i].next = &columns[i];
    }
    int totalcards = lengthoflist(finalCard);
    //printf("%d", totalcards);
    int assignedcards = 0;
    Card* current = finalCard;
    Card* newHead= finalCard;
    //printf("%c  %c\n", current-> displayedChars[0], current->displayedChars[1]);
    while(assignedcards< 52){
        for(int i = 0;i < 7 ;i++){
            current = iteratelist(0, newHead);//fetch first elememt in list

            if(current == NULL){
                //printf("Current is null v");
                break;
            }
            if(i == 0 && assignedcards>1){
                //printf("Current is null v");
                i++;
            }
            if(i == 1 && assignedcards>36){
                //printf("Current is null v");
                i++;
            }
            if(i == 2 && assignedcards>37){
                //printf("Current is null v");
                i++;
            }
            if(i == 3 && assignedcards>42){
                //printf("Current is null v");
                i++;
            }
            if(i == 4 && assignedcards>47){
                //printf("Current is null v");
                i++;
            }
            if(i == 5 && assignedcards>50){
                //printf("Current is null v");
                i++;
            }
            //printf(" %c  %c\n", newHead-> displayedChars[0], newHead->displayedChars[1]);

            newHead = current->next;


            current->next = columns[i].next;//assign current cards pointer to column i
            columns[i].next = current;//assign column i's pointer to current, such that column i points to the end of the head of the list
            //printf("columns %d %c  %c\n",i, columns[i].next-> displayedChars[0], columns[i].next->displayedChars[1]);
            assignedcards++;
        }
    }
}
bool isCardPointingToColumn(Card* card) {
    return (card->next != NULL && card->next->isColumn == true);
}

void show (Card* head, column columns[]) {
    if (head == NULL) startshow();

    Card* current = head;
    Card lastcurrent[8];
    int t = 0;

    printf("\n");
    printf("C1  C2  C3  C4  C5  C6  C7\n");
    printf("\n");

    for(t = 1;t < longestcolumn(columns) ;t++){
        for(int i = 0;i < 7 ;i++){

            current = iteratelist(lengthoflist(columns[i].next)-t,columns[i].next);
            if(!isCardPointingToColumn(current)){
            }
            if(current != NULL ){
                if(!(lastcurrent[i].displayedChars[0] == current->displayedChars[0] && lastcurrent[i].displayedChars[1] == current->displayedChars[1])){
                    if(!current->facingDown){
                        printf("%c%c  ", current->displayedChars[0], current->displayedChars[1]);
                    }else{
                        printf("[]  ");
                    }

                } else{
                    printf("    ");
                }

                lastcurrent[i].displayedChars[0] = current->displayedChars[0];
                lastcurrent[i].displayedChars[1] = current->displayedChars[1];
            }

        }
        if(t == 1){
            printf("    []  F1");
        } else if(t == 3){
            printf("    []  F2");
        }else if(t == 5){
            printf("    []  F3");
        }else if(t == 7){
            printf("    []  F4");
        }

        printf("\n");

    }
    printf("\n");
}



Card* unappend(Card* head){
    Card* newHead;
    newHead = iteratelist(lengthoflist(head) - lengthoflist(head), head);
    return newHead;
}

int longestcolumn (column columns[]){
    int v = 0;
    for (int i = 0;i < 7 ;i++){
        if(lengthoflist(columns[i].next)+1> v){
            v = lengthoflist(columns[i].next)+1;
        }
    }
    return v;
}

void flipcard (column columns[]){
    for(int t = 0;t < longestcolumn(columns) ;t++){
        for(int i = 0;i < 7 ;i++){
            Card* current = iteratelist(lengthoflist(columns[i].next)-t,columns[i].next);
            current->facingDown = false;
        }

    }
}
void flipcard2 (column columns[]){
    for (int i = 0; i < 7; i++) {
        int numberOfCardsToFlip = 5;
        int cardsFlipped = 0;
        Card* current = columns[i].next;

        // Flip the specified number of cards in the column
        while (current != NULL && cardsFlipped < numberOfCardsToFlip) {
            current->facingDown = false;
            current = current->next;
            cardsFlipped++;
        }
    }
}
void move(Card* from, Card* to, column columns[]){
if(from == NULL || to == NULL){
    printf("Cards doesnt exist");
    return;
}

    Card* fromColumn = findColumn(lengthoflist(from),from);

    printf("initiate move card %c%c in column %d \n", from->displayedChars[0], from->displayedChars[1], fromColumn->column);

    Card* lastCardinFrom = fromColumn->next;
    printf("the last card in this column is %c%c ", lastCardinFrom->displayedChars[0], lastCardinFrom->displayedChars[1]);
    printf("\n");
    fromColumn->next = from->next;
    printf("we move from columns pointer to point at %c%c", from->next->displayedChars[0], from->next->displayedChars[1]);
    printf("\n");
    from->next = to;
    printf("we now set the card %c%c to point at %c%c", from->displayedChars[0], from->displayedChars[1], to->displayedChars[0], to->displayedChars[1]);
    printf("\n");
    from->column = to->column;

    columns[to->column].next = lastCardinFrom;


}

void startshow () {


    printf("C1  C2  C3  C4  C5  C6  C7\n");
    printf("\n");
    printf("                        ");
    printf("        []  F1");
    printf("\n");
    printf("\n");
    printf("                        ");
    printf("        []  F2");
    printf("\n");
    printf("\n");
    printf("                        ");
    printf("        []  F3");
    printf("\n");
    printf("\n");
    printf("                        ");
    printf("        []  F4");
    printf("\n");
    printf("LAST COMMAND: ");
    printf("\n");
}

void parseInput(const char* input, char* filename) {
    // Find the position of '<' and '>'
    const char* start = strchr(input, '<');
    const char* end = strchr(input, '>');

    // Copy the filename between '<' and '>'
    if (start != NULL && end != NULL && end > start) {
        strncpy(filename, start + 1, end - start - 1);
        filename[end - start - 1] = '\0'; // Null-terminate the string
    } else {
        // No filename found
        filename[0] = '\0';
    }
}
void parseInput2(const char* input, char* card1, char* card2) {
    // Find the position of '->'
    const char* arrow = strstr(input, "->");
    if (arrow != NULL) {
        // Calculate the length of the substring before '->'
        strncpy(card2, input, arrow - input);
        card2[arrow - input] = '\0';
        //card1[2] = '\0'; // Null-terminate the first card
        //printf("first char of card1 %c second char of card 1 %c  ", card1[0], card1[1]);

        // Extract the second card after '->'
        strcpy(card1, arrow + strlen("->"));
    } else {
        // If '->' is not found, set both cards to empty strings
        card1[0] = '\0';
        card2[0] = '\0';
    }
}

Card* split(const char *filename, int split){
    Card* head = linkedv(filename);

    if(split>51 || split<2){
        return NULL;
    }
    Card* pile1 = head;

    int counter = 0;
    printf("%d", lengthoflist(head));
    Card* pile2 = iteratelist(split-1, head)->next;
    iteratelist(split-1, head)->next = NULL;


    printf("\n");
    printf("length of the top pile %d \n", lengthoflist(pile1));
    printf("top piles first card %c%c  \n", pile1->displayedChars[0], pile1->displayedChars[1]);

    printf("\n");
    printf("length of the bottom pile %d \n", lengthoflist(pile2));
    printf("bottom piles first card %c%c  \n", pile2->displayedChars[0], pile2->displayedChars[1]);


    Card* currentnewpile;

    Card* newpile;
    newpile = pile1;
    currentnewpile = newpile;
    pile1 = pile1->next;
    printf("length of the top pile %d \n", lengthoflist(pile1));

    if(lengthoflist(pile1)> lengthoflist(pile2)){
        while(lengthoflist(pile1)>1){
            currentnewpile->next = pile2;
            currentnewpile = pile2;
            pile2 = pile2->next;
            currentnewpile->next = pile1;
            currentnewpile = pile1;
            pile1 = pile1->next;
            currentnewpile->next = NULL;
            if(lengthoflist(pile2)<1){
                currentnewpile->next = pile1;
                break;

            }
            printf("length of the top pile %d \n", lengthoflist(pile1));
            printLinkedList(newpile);
        }
    }else{
        while(lengthoflist(pile2)>1){
            currentnewpile->next = pile2;
            currentnewpile = pile2;
            pile2 = pile2->next;
            currentnewpile->next = pile1;
            currentnewpile = pile1;
            pile1 = pile1->next;
            currentnewpile->next = NULL;
            if(lengthoflist(pile1)<1){
                currentnewpile->next = pile2;
                break;

            }
            printf("length of the top pile %d \n", lengthoflist(pile1));
            printLinkedList(newpile);
        }
    }
    printf("length of the new pile %d \n", lengthoflist(newpile));
    printLinkedList(newpile);
    return newpile;
}


Card* randomShuffle(const char *filename) {
    Card* head = linkedv(filename);
    if (head == NULL) {
        printf("The list is empty.\n");
        return NULL;
    }

    Card* current = head;
    Card* current2 = head;
    Card* newpile = current;
    Card* currentnewpile = current;
    current = current->next;
    newpile->next = NULL;
    printf("length of the top pile %d \n", lengthoflist(current));
    while (lengthoflist(current)>0) {
        //printf("length of the top pile %d \n", lengthoflist(current));
        int min = 0;
        int max = lengthoflist(newpile);
        int randomNumber = rand() % (max - min + 1) + min;
        printf("random number %d \n", randomNumber);
        current2 = current->next;
        //insertCard(newpile, randomNumber, current);
        if(randomNumber == 0){
            current->next = newpile;
            newpile = current;
        } else{
        currentnewpile = iteratelist(randomNumber-1, newpile);

        current->next = currentnewpile->next;

        currentnewpile->next = current;
    }
        current = current2;
        //printf("length of the new pile %d \n", lengthoflist(newpile));
        //printLinkedList(newpile);
    }

    return newpile;
}

Card* findCard(char*card, column columns[]) {

    for(int t = 0;t < longestcolumn(columns) ;t++){
        for(int i = 0;i < 7 ;i++){
            Card* current = iteratelist(lengthoflist(columns[i].next)-t,columns[i].next);
            //printf("%c%c  ", current->displayedChars[0], current->displayedChars[1]);
            //printf("\n");
            //printf("card1 %c card2 %c  ", card[0], card[1]);
            if(current->displayedChars[0] == card[0] && current->displayedChars[1] == card[1]){
                return current;
            }
        }

    }
    return NULL;
}
void writeLinkedListToFile(const char *filename, Card* head) {
    // Open the file for writing
    FILE *file = fopen(filename, "w+");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    // Traverse the linked list and write each element's content to the file
    Card* current = head;
    while (current != NULL) {
        fprintf(file, "%c%c\n", current->displayedChars[0], current->displayedChars[1]);
        current = current->next;
    }

    // Close the file
    fclose(file);
}
void copyFile(const char *sourcePath, const char *destinationPath) {
    FILE *sourceFile, *destinationFile;
    char ch;

    // Open source file in read mode
    sourceFile = fopen(sourcePath, "r");
    if (sourceFile == NULL) {
        printf("Error: Unable to open source file.\n");
        return;
    }

    // Open destination file in write mode
    destinationFile = fopen(destinationPath, "w+");
    if (destinationFile == NULL) {
        printf("Error: Unable to create destination file.\n");
        fclose(sourceFile); // Close source file before returning
        return;
    }

    // Copy contents from source file to destination file
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destinationFile);
    }

    // Close files
    fclose(sourceFile);
    fclose(destinationFile);
}



void insertCard(Card* head, int position, Card* newCard) {
    // If the position is less than 0, exit the function
    if (position < 0) {
        printf("Error: Invalid position for insertion.\n");
        return;
    }

    // If the position is 0, insert the new card at the beginning of the list
    if (position == 0) {
        newCard->next = head;
        return;
    }

    // Find the node at the position just before the insertion point
    Card* current2 = head;
    current2 = iteratelist(position-1, head);

    // Insert the new card between the current node and the next node
    newCard->next = current2->next;
    current2->next = newCard;
}

/*
void sdlExample () {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL image: %s", SDL_GetError());
        return;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Error renderer creation: %s", SDL_GetError());
        return;
    }

    SDL_Surface* lettuce_sur = IMG_Load("/Users/mohammedismail/CLionProjects/Solitaire/BarackOsama.png");
    if (lettuce_sur == NULL) {
        SDL_Log("Error loading image: ", IMG_GetError());
        return;
    }

    SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, lettuce_sur);
    if (lettuce_tex == NULL) {
        SDL_Log("Error creating texture");
        return;
    }

    SDL_FreeSurface(lettuce_sur);

    while (true) {
        SDL_Event e;
        if (SDL_WaitEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(lettuce_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}*/
