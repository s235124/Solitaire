#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <ddraw.h>
#include <SDL_image.h>
#include <string.h>

#define realpath(N,R) _fullpath((R),(N),_MAX_PATH)
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define GRID_SIZE 10
#define PADDING 10

typedef struct CardStruct {
    int number;
    char suit;
    int column;
    bool isColumn;
    char displayedChars[2];
    struct CardStruct* next;
    bool facingDown;
    bool movedFromFoundation;
} Card;

typedef Card column;
typedef Card Foundation;

// Subroutines in the program
void sdlExample(column columns[]);
bool checkIfWon (column columns[]);
void show (Card* head, column columns[], Foundation foundation[]);
void startshow ();
void load();
void printFileLines(const char *filename, Card* cards);
Card* linkedv(const char *filename);
void printLinkedList(Card* head);
Card* iteratelist(int iteration, Card* head);
bool isCardPointingToColumn(Card* card);
int lengthoflist(Card* head);
bool toFoundation (column columns[], Foundation foundation[]);
void createcolumns (Card* finalCard, column columns[]);
Card* unappend(Card* head);
int longestcolumn (column columns[]);
void flipcard (column columns[]);
bool parseInput(const char* input, char* filename, size_t size);
Card* split(const char *filename, int split);
void insertCard(Card* head, int position, Card* newCard);
Card* randomShuffle(const char *filename);
void writeLinkedListToFile(const char *filename, Card* head);
void copyFile(const char *sourcePath, const char *destinationPath);
void createFoundation (Foundation foundation[]);
void createcolumns2 (Card* finalCard, column columns[]);
void flipcard2 (column columns[]);
bool move(Card* from, Card* to, column columns[]);
Card *findCard(char *card, column columns[], Foundation foundation[]);
void parseInput2(const char* input, char* card1, char* card2);
Card* findColumn(int iteration, Card* head);
void flipcard3 (column columns[]);

int main(int argc, char* argv[]) {
    Card *head = NULL;

    column columns[7];
    head = linkedv("../file.txt");
    createcolumns(head, columns);

    sdlExample(columns);

    Foundation foundation[4];
    char card1[3];
    char card2[3];

    bool p = false;

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
        if (!p && strstr(input, "SW") != NULL) {
            flipcard(columns);
        }
        else if (!p && strstr(input, "LD") != NULL) {
            if(!parseInput(input, filename, sizeof(filename)))
                strcpy(message, "File not found!");
            else {
                //printf("Filename: %s\n", filename);
                head = linkedv(filename);
                if (head != NULL) {
                    writeLinkedListToFile("currentd.txt", head);

                    createFoundation(foundation);
                    createcolumns(head, columns);
                } else {
                    strcpy(message, "Failed to load file.");
                }
            }
        }
        else if (!p && strstr(input, "SI") != NULL) {
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
        }
        else if (!p && strstr(input, "SR") != NULL) {
            //printf("length of the top pile %d \n", lengthoflist(head));
            head = randomShuffle("currentd.txt");
            writeLinkedListToFile("currentd.txt", head);
            createcolumns(head, columns);
            flipcard(columns);
        }
        else if (strstr(input, "SD") != NULL) {
            parseInput(input , filename, sizeof(filename));
            copyFile("currentd.txt",filename);
        }
        else if (p && strstr(input, "->") != NULL) {
            parseInput2(input, card1, card2);

            Card* cardc1 = findCard(card1, columns, foundation);
            Card* cardc2 = findCard(card2, columns, foundation);

            if (card1[0] == 'F') {
                cardc2->movedFromFoundation = false;
            }
            else {
                if (!move(cardc2, cardc1, columns))
                    strcpy(message, "Invalid move!");
                flipcard3(columns);
                printf(" card %c  %c  has moved ", card2[0], card2[1]);
            }
        }
        else if (strstr(input, "QQ") != NULL) {
            break;
        }
        else if (p && strstr(input, "Q") != NULL) {
            p = false;
        }
        else if (!p && strstr(input, "P") != NULL) {
            p = true;
            head = linkedv("currentd.txt");
            createcolumns2(head, columns);
            createFoundation(foundation);
            flipcard2(columns);
        }
        else if (p && strstr(input, "->") != NULL) {
            parseInput2(input, card1, card2);
            //printf(" first char of card1 %c second char of card1 %c  ", card1[0], card1[1]);
            Card* cardc1 = findCard(card1, columns, foundation);
            Card* cardc2 = findCard(card2, columns, foundation);

            if(!move(cardc2, cardc1, columns))
                strcpy(message, "Invalid move!");
            flipcard3(columns);
            printf(" card %c  %c  has moved ", card2[0], card2[1]);
        }
        else {
            strcpy(message, "Invalid Command!");
        }
        if (p) while(toFoundation(columns, foundation));

        if (checkIfWon(columns))
            strcpy(message, "Game is won!");

        show(head, columns, foundation);
        printf("LAST COMMAND: %s", prevInput);
        printf("Message: %s\n", message);
    }

    return 0;
}

bool checkIfWon (column columns[]) {
    for (int i = 0; i < 7; i++) {
        if (lengthoflist(columns[i].next) > 1)
            return false;
    }
    return true;
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

        switch (line[0]) {
            case 'A':
                recentCard->number = 1;
                break;
            case 'T':
                recentCard->number = 10;
                break;
            case 'J':
                recentCard->number = 11;
                break;
            case 'Q':
                recentCard->number = 12;
                break;
            case 'K':
                recentCard->number = 13;
                break;
            default:
                recentCard->number = line[0] - '0';
                break;
        }
        //printf("%d\n", recentCard->number);
        recentCard->displayedChars[0] = line[0];
        recentCard->displayedChars[1] = line[1];
        recentCard->facingDown = true;
        recentCard->isColumn = false;
        recentCard->next = NULL;

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

bool toFoundation (column columns[], Foundation foundation[]) {
    flipcard3(columns);
    for (int i = 0; i < 7; i++) {
        Card *current = columns[i].next;

        if (current->movedFromFoundation) {
            continue;
        }

        if (current->number == 1) {
            columns[i].next = current->next;
            for (int j = 0; j < 4; j++) {
                if (foundation[j].suit == current->displayedChars[1]) {
                    current->next = foundation[j].next;
                    foundation[j].next = current;

                    if (columns[i].next == foundation[j].next)
                        columns[i].next = &columns[i];

                    return true;
                }
            }
        }
        else {
            for (int j = 0; j < 4; j++) {
                if (foundation[j].suit == current->displayedChars[1] &&
                    current->number - 1 == foundation[j].next->number
                    && foundation[j].next != &foundation[j]) {
                    columns[i].next = current->next;
                    current->next = foundation[j].next;
                    foundation[j].next = current;

                    if (columns[i].next == foundation[j].next)
                        columns[i].next = &columns[i];

                    return true;
                }
            }
        }
    }
    return false;
}

void createcolumns (Card* finalCard, column columns[]) {
    //for loop for creating the 7 columns
    for(int i = 0; i<7; i++){
        columns[i].column = i+1;
        columns[i].displayedChars[0] = 'C';
        columns[i].displayedChars[1] = i + '1';
        columns[i].isColumn = true;
        columns[i].next = &columns[i];
        //printf("%c\n", columns[i].displayedChars[1]);
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
            //printf("%c%c is in column %d\n", current->displayedChars[0],current->displayedChars[1], current->column);
            columns[i].next = current;//assign column i's pointer to current, such that column i points to the end of the head of the list
            //printf("columns %d %c  %c\n",i, columns[i].next-> displayedChars[0], columns[i].next->displayedChars[1]);
            assignedcards++;
        }
    }
}

void createFoundation (Foundation foundation[]) {
    //for loop for creating the 4 foundations
    for(int i = 0; i < 4; i++){
        foundation[i].column = i+1;
        foundation[i].displayedChars[0] = '[';
        foundation[i].displayedChars[1] = ']';
        foundation[i].isColumn = true;
        foundation[i].next = &foundation[i];
        switch (i) {
            case 0:
                foundation[i].suit = 'S';
                break;
            case 1:
                foundation[i].suit = 'D';
                break;
            case 2:
                foundation[i].suit = 'C';
                break;
            case 3:
                foundation[i].suit = 'H';
                break;
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
            current->column = i;
            columns[i].next = current;//assign column i's pointer to current, such that column i points to the end of the head of the list
            //printf("columns %d %c  %c\n",i, columns[i].next-> displayedChars[0], columns[i].next->displayedChars[1]);
            assignedcards++;
        }
    }
}
bool isCardPointingToColumn(Card* card) {
    return (card->next != NULL && card->next->isColumn == true);
}

void show (Card* head, column columns[], Foundation foundation[]) {
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
                if (current->displayedChars[0] == 'C')
                    printf("    ");
                else if(!(lastcurrent[i].displayedChars[0] == current->displayedChars[0] && lastcurrent[i].displayedChars[1] == current->displayedChars[1])){
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
            printf("    %c%c  F1", foundation[0].next->displayedChars[0], foundation[0].next->displayedChars[1]);
        } else if(t == 3){
            printf("    %c%c  F2", foundation[1].next->displayedChars[0], foundation[1].next->displayedChars[1]);
        }else if(t == 5){
            printf("    %c%c  F3", foundation[2].next->displayedChars[0], foundation[2].next->displayedChars[1]);
        }else if(t == 7){
            printf("    %c%c  F4", foundation[3].next->displayedChars[0], foundation[3].next->displayedChars[1]);
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

void flipcard3 (column columns[]) {
    for (int i = 0; i < 7; i++) {
        if (columns[i].next->facingDown) {
            columns[i].next->facingDown = false;
        }
    }
}

bool move(Card* from, Card* to, column columns[]){
    printf("initiate move card %c%c in column \n", from->displayedChars[0], from->displayedChars[1]);

    if(from == NULL || to == NULL){
        printf("Cards doesnt exist");
        return false;
    }

    Card* fromColumn = findColumn(lengthoflist(from),from);
    Card* toColumn = findColumn(lengthoflist(to),to);

    if (from->isColumn || fromColumn == toColumn) {
        printf("Cards doesnt exist2");
        return false;
    }

    if ((from->number != to->number - 1 && !to->isColumn) || from->displayedChars[1] == to->displayedChars[1] || (to->isColumn && from->displayedChars[0] != 'K')) {
        printf("Cards doesnt exist 3");
        return false;
    }



    Card* lastCardinFrom = fromColumn->next;
    printf("the last card in this column is %c%c ", lastCardinFrom->displayedChars[0], lastCardinFrom->displayedChars[1]);
    printf("\n");
    fromColumn->next = from->next;
    printf("we move from columns pointer to point at %c%c", from->next->displayedChars[0], from->next->displayedChars[1]);
    printf("\n");
    from->next = to;
    printf("we now set the card %c%c to point at %c%c", from->displayedChars[0], from->displayedChars[1], to->displayedChars[0], to->displayedChars[1]);
    printf("\n");

    printf("to's column is: %d", to->column);
    printf("\n");
    from->column = to->column;

    toColumn->next = lastCardinFrom;

    return true;
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

bool parseInput(const char* input, char* filename, size_t size) {
    // Find the position of '<' and '>'
    const char* start = strchr(input, '<');
    const char* end = strchr(input, '>');

    // Check if '<' and '>' are found in the input
    if (start == NULL || end == NULL || *start != '<' || *end != '>') {
        strncpy(filename, "../file.txt", size - 1);
        filename[size - 1] = '\0';
    } else if (start != NULL && end != NULL && end > start + 1) {
        // Copy the filename between '<' and '>'
        size_t length = end - start < size ? end - start - 1 : size - 1;
        strncpy(filename, start + 1, length);
        filename[length] = '\0'; // Null-terminate the string
    } else {
        // No filename found or brackets are empty
        filename[0] = '\0';
        return false;
    }
    return true;
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

Card *findCard(char *card, column columns[], Foundation foundation[]) {
    // Moving from foundation
    for (int i = 0; i < 4; i++) {
        if (foundation[i].next->displayedChars[0] == card[0] && foundation[i].next->displayedChars[1] == card[1]) {
            foundation[i].next->movedFromFoundation = true;
            return foundation[i].next;
        }
    }
    for(int t = 0;t < longestcolumn(columns) ;t++){
        for(int i = 0;i < 7 ;i++){
            Card* current = iteratelist(lengthoflist(columns[i].next)-t,columns[i].next);
            //printf("%c%c  ", current->displayedChars[0], current->displayedChars[1]);
            //printf("\n");
            //printf("card1 %c card2 %c  ", card[0], card[1]);
            if(current->displayedChars[0] == card[0] && current->displayedChars[1] == card[1]){
                current->movedFromFoundation = false;
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


void sdlExample (column columns[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Grid Layout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("../arial.ttf", 24);
    if (!font)
        printf("nope");


    SDL_Color color = {255, 255, 255, 255};

    SDL_Rect grid[GRID_SIZE][GRID_SIZE];
    int cell_width = (800 - PADDING * (GRID_SIZE + 1)) / GRID_SIZE - 15;
    int cell_height = (600 - PADDING * (GRID_SIZE + 1)) / GRID_SIZE + 30;

    int total_width = GRID_SIZE * (cell_width + PADDING) - PADDING;
    int start_x = (WINDOW_WIDTH - total_width) / 2;

    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_RenderClear(renderer);

    Card lastcurrent[8];
    flipcard(columns);

    for (int i = 1; i < longestcolumn(columns); ++i) {
        for (int j = 0; j < 7; ++j) {
            Card* current = iteratelist(lengthoflist(columns[j].next)-i,columns[j].next);

            grid[i][j].x = start_x + j * (cell_width + PADDING);
            grid[i][j].y = PADDING + i * (cell_height + PADDING);
            grid[i][j].w = cell_width;
            grid[i][j].h = cell_height;

            char text[8];

            if(current != NULL ){
                if (current->displayedChars[0] == 'C')
                    sprintf(text, "    ");
                else if(!(lastcurrent[j].displayedChars[0] == current->displayedChars[0] && lastcurrent[j].displayedChars[1] == current->displayedChars[1])){
                    if(!current->facingDown){
                        sprintf(text, "%c%c  ", current->displayedChars[0], current->displayedChars[1]);
                    }else{
                        sprintf(text, "[]  ");
                    }

                } else{
                    sprintf(text, "    ");
                }

                lastcurrent[j].displayedChars[0] = current->displayedChars[0];
                lastcurrent[j].displayedChars[1] = current->displayedChars[1];
            }

            SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            if (!surface)
                printf("nope2");
            if (!texture)
                printf("nope3");

            SDL_Rect dest;
            dest.x = grid[i][j].x + (cell_width * 0.2);
            dest.y = grid[i][j].y + (cell_height * 0.1);
            SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

            SDL_RenderCopy(renderer, texture, NULL, &dest);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                SDL_RenderDrawRect(renderer, &grid[i][j]);
            }
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
