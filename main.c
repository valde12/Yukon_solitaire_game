#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <conio.h>
#include <string.h>
#include <time.h>

#define NUMBER_OF_LISTS 11
char message[50];
char lastCommand[20];
////////////////////////////////////////////////////////////////
//Card linkedLists

//Every card has a char array with two slots, one for the color and one for the type
//so queen of hearts char[]{'Q','H'}
struct cardNode {
    char card[2];
    bool turned;
    struct cardNode* next;
};

struct cardNode* startOfColumns[NUMBER_OF_LISTS] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

//list index 0-6 equals C1-C7, list index 7-10 equals F1-F4
//Inserts new card in the end of the linkedlist
void insertCardLast(int list_index, const char* new_card, bool turned) {
    struct cardNode* new_cardNode = (struct cardNode*) malloc(sizeof(struct cardNode));
    new_cardNode->card[0] = new_card[0];
    new_cardNode->card[1] = new_card[1];
    new_cardNode->turned = turned;
    new_cardNode->next = NULL;

    if (startOfColumns[list_index] == NULL) {
        startOfColumns[list_index] = new_cardNode;
    } else {
        struct cardNode* current = startOfColumns[list_index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_cardNode;
    }
}

//list index 0-6 equals C1-C7, list index 7-10 equals F1-F4
//Inserts new card in the start of the linkedlist
void insertCardFirst(int listIndex, const char* new_card, bool turned) {
    struct cardNode* new_cardNode = (struct cardNode*) malloc(sizeof(struct cardNode));
    new_cardNode->card[0] = new_card[0];
    new_cardNode->card[1] = new_card[1];
    new_cardNode->turned = turned;
    new_cardNode->next = startOfColumns[listIndex];
    startOfColumns[listIndex] = new_cardNode;
}

void removeCardFromList(int listIndex, const char* card) {
    struct cardNode* current = startOfColumns[listIndex];
    struct cardNode* previous = NULL;

    // Traverse the list until the input card is found
    while (current != NULL && (current->card[0] != card[0] || current->card[1] != card[1])) {
        previous = current;
        current = current->next;
    }
    // removes card from list
    if (current != NULL) {
        if (previous == NULL) {
            startOfColumns[listIndex] = current->next;
        } else {
            previous->next = current->next;
        }
        free(current);
    }
}

//removes all cards from the card inserted and down in the oldlistindex and adds them to the newlistindex
void removeFromColumnAndAddToNewColumn(int oldListIndex, int newListIndex, const char* card) {
    struct cardNode* currentCard = startOfColumns[oldListIndex];
    struct cardNode* tempCard;

    // Traverse the list until the input card is found
    while (currentCard != NULL && (currentCard->card[0] != card[0] || currentCard->card[1] != card[1])) {
        currentCard = currentCard->next;
    }
    // If the input card was found create a new linked list starting from it
    if (currentCard != NULL) {
        insertCardLast(newListIndex, currentCard->card, currentCard->turned);
        tempCard = currentCard->next;
        removeCardFromList(oldListIndex, currentCard->card);

        // Copy all the cards after the given card
        currentCard = tempCard;
        while (currentCard != NULL) {
            insertCardLast(newListIndex, currentCard->card, currentCard->turned);
            tempCard = currentCard->next;
            removeCardFromList(oldListIndex, currentCard->card);
            currentCard = tempCard;
        }
    }
}

struct cardNode* returnLastInCardStack(struct cardNode* card){
    struct cardNode* currentCard = card;
    if (currentCard == NULL) {
        return currentCard;
    }
    while (currentCard->next != NULL) {
        currentCard = currentCard->next;
    }
    return currentCard;
}

void clearAllLists() {
    for (int i = 0; i < NUMBER_OF_LISTS; i++) {
        struct cardNode* currentCard = startOfColumns[i];
        while (currentCard != NULL) {
            struct cardNode* nextCard = currentCard->next;
            free(currentCard);
            currentCard = nextCard;
        }
        startOfColumns[i] = NULL;
    }
}

//takes card value char as input and converts to int for comparisons
int cardValueConverter(char card_value) {
    if (card_value == 'J') {
        return 11;
    } else if (card_value == 'Q') {
        return 12;
    } else if (card_value == 'K') {
        return 13;
    } else if (card_value == 'A') {
        return 1;
    } else if (card_value == 'T') {
        return 10;
    } else {
        return ((int)card_value)-48;
    }
}
/////////////////////////////////////////////
// commands

void LD(char filename[], bool turned) {
    //Load a deck of cards from a file
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        strncpy(message, "Error: file points to null", 50);
    }
    int columnCounter = 0;
    int spades[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int clubs[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int hearts[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int diamonds[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    bool trueDeck = true;
    int cardValue = 0;
    char content[104];
    int cardCounter = 0;

    while(fgets(content, 52, fptr) && cardCounter < 52) {
        insertCardLast(columnCounter, content, turned);
        columnCounter = columnCounter + 1;
        if(columnCounter > 6) {
            columnCounter = 0;
        }
        cardValue = cardValueConverter(content[0]);
        if(content[1] == 'D'){
            diamonds[cardValue-1]++;
        }
        if(content[1] == 'H'){
            hearts[cardValue-1]++;
        }
        if(content[1] == 'C'){
            clubs[cardValue-1]++;
        }
        if(content[1] == 'S'){
            spades[cardValue-1]++;
        }
        cardCounter++;
    }
    for (int i = 0; i <= 12; ++i) {
        if(diamonds[i] == 0 || spades[i] == 0 || hearts[i] == 0 || clubs[i] == 0) {
            trueDeck = false;
            strncpy(message, "Error: False deck", 50);
        }
    }
    if(trueDeck) {
        strncpy(message, "OK", 50);
    }
    fclose(fptr);
}



void SD(char filename[]) {
    //Saves current deck in a file with the filename
    FILE *fwrtr;
    fwrtr = fopen(filename, "w");
    int columnCounter = 0;
    struct cardNode* current0 = startOfColumns[0];
    struct cardNode* current1 = startOfColumns[1];
    struct cardNode* current2 = startOfColumns[2];
    struct cardNode* current3 = startOfColumns[3];
    struct cardNode* current4 = startOfColumns[4];
    struct cardNode* current5 = startOfColumns[5];
    struct cardNode* current6 = startOfColumns[6];
    //card = current0->card;

    while (current0 != NULL || current1 != NULL || current2 != NULL || current3 != NULL || current4 != NULL || current5 != NULL || current6 != NULL) {

        if (current0 != NULL) {
            fprintf(fwrtr, "%c%c\n", current0->card[0], current0->card[1]);
            current0 = current0->next;
        }
        if (current1 != NULL) {
            fprintf(fwrtr, "%c%c\n", current1->card[0], current1->card[1]);
            current1 = current1->next;
        }
        if (current2 != NULL) {
            fprintf(fwrtr, "%c%c\n", current2->card[0], current2->card[1]);
            current2 = current2->next;
        }
        if (current3 != NULL) {
            fprintf(fwrtr, "%c%c\n", current3->card[0], current3->card[1]);
            current3 = current3->next;
        }
        if (current4 != NULL) {
            fprintf(fwrtr, "%c%c\n", current4->card[0], current4->card[1]);
            current4 = current4->next;
        }
        if (current5 != NULL) {
            fprintf(fwrtr, "%c%c\n", current5->card[0], current5->card[1]);
            current5 = current5->next;
        }
        if (current6 != NULL) {
            fprintf(fwrtr, "%c%c\n", current6->card[0], current6->card[1]);
            current6 = current6->next;
        }
    }
    fclose(fwrtr);
}

void SW() {
    //Show the loaded deck of cards if no loaded cards, show standard deck
    SD("tempCards.txt");
    clearAllLists();
    LD("tempCards.txt", true);
}

void SI(int split) {
    //Move alle cards to one pile
    struct cardNode* current1 = startOfColumns[1];
    struct cardNode* current2 = startOfColumns[2];
    struct cardNode* current3 = startOfColumns[3];
    struct cardNode* current4 = startOfColumns[4];
    struct cardNode* current5 = startOfColumns[5];
    struct cardNode* current6 = startOfColumns[6];

    removeFromColumnAndAddToNewColumn(6, 0, current6->card);
    removeFromColumnAndAddToNewColumn(5, 0, current5->card);
    removeFromColumnAndAddToNewColumn(4, 0, current4->card);
    removeFromColumnAndAddToNewColumn(3, 0, current3->card);
    removeFromColumnAndAddToNewColumn(2, 0, current2->card);
    removeFromColumnAndAddToNewColumn(1, 0, current1->card);

    //Splitting the deck according to int split
    struct cardNode* splitCard = startOfColumns[0];

    for(int i = 0; i < split; ++i) {
        if(splitCard->next != NULL) {
            splitCard = splitCard->next;
        }
    }
    removeFromColumnAndAddToNewColumn(0, 1, splitCard->card);

    struct cardNode* deck1 = startOfColumns[0];
    struct cardNode* deck2 = startOfColumns[1];
    struct cardNode* tempCard = NULL;
    int intSwitch = 1;

    bool moveCard = true;
    while(moveCard) {
        if(intSwitch == 1) {
            tempCard = deck1;
            insertCardFirst(2, tempCard->card, true);
            deck1 = deck1->next;
            removeCardFromList(0, tempCard->card);
        } else {
            tempCard = deck2;
            insertCardFirst(2, tempCard->card, true);
            deck2 = deck2->next;
            removeCardFromList(1, tempCard->card);
        }
        if(deck1 == NULL || deck2 == NULL) {
            moveCard = false;
        }
        intSwitch = intSwitch ^ 1;
    }
    if(deck1 == NULL) {
        removeFromColumnAndAddToNewColumn(1, 2, deck2->card);
    } else {
        removeFromColumnAndAddToNewColumn(0, 2, deck1->card);
    }
    SD("tempCards.txt");
    clearAllLists();
    LD("tempCards.txt", false);
}

void SR() {
    struct cardNode* current0 = startOfColumns[0];
    struct cardNode* current1 = startOfColumns[1];
    struct cardNode* current2 = startOfColumns[2];
    struct cardNode* current3 = startOfColumns[3];
    struct cardNode* current4 = startOfColumns[4];
    struct cardNode* current5 = startOfColumns[5];
    struct cardNode* current6 = startOfColumns[6];

    removeFromColumnAndAddToNewColumn(1, 0, current1->card);
    removeFromColumnAndAddToNewColumn(2, 0, current2->card);
    removeFromColumnAndAddToNewColumn(3, 0, current3->card);
    removeFromColumnAndAddToNewColumn(4, 0, current4->card);
    removeFromColumnAndAddToNewColumn(5, 0, current5->card);
    removeFromColumnAndAddToNewColumn(6, 0, current6->card);

    time_t t;
    srand((unsigned) time(&t));     //Initializes a random number generator

    struct cardNode* pTemp = NULL;

    int i;
    for (i = 1; i < 53; i++) {
        int r = rand() % 7 + 1;    // Returns an almost-random integer between 1 and 7 which represents the different columns

        switch(r) {
            case 1:
                pTemp = current0;
                insertCardFirst(0, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 2:
                pTemp = current0;
                insertCardFirst(1, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 3:
                pTemp = current0;
                insertCardFirst(2, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 4:
                pTemp = current0;
                insertCardFirst(3, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 5:
                pTemp = current0;
                insertCardFirst(4, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 6:
                pTemp = current0;
                insertCardFirst(5, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
            case 7:
                pTemp = current0;
                insertCardFirst(6, pTemp->card, true);
                current0 = current0->next;
                removeCardFromList(0, pTemp->card);
                break;
        }
    }
    SD("tempCards.txt");
    clearAllLists();
    LD("tempCards.txt", false);
    remove("tempCards.txt");
}

int QQ() {
    return 0;
    //Quits the game
}

void P() {
    //Starts a game using the current deck of cards
    clearAllLists();
    FILE *fptr;
    fptr = fopen("currentDeck.txt", "r");
    if (fptr == NULL) {
        strncpy(message, "Error: There is no deck loaded", 50);
    }

    int rowCounter = 0;
    int columnCounter = 0;
    int columnTurnedCards = 1;
    int startOfColumnCards = 0;
    char content[104];

    //while loop that keeps printing
    while(rowCounter <= 11) {
        if(rowCounter == 0) {
            if (columnCounter == 0) {
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, true);
            } else {
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, false);
            }
        }
        if(rowCounter > 0 && rowCounter < 6) {
            if (columnCounter < columnTurnedCards && columnCounter >= startOfColumnCards) {
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, true);
            } else if (columnCounter >= columnTurnedCards && columnCounter >= startOfColumnCards){
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, false);
            }
        }
        if (rowCounter >= 6) {
            if (columnCounter < columnTurnedCards && columnCounter >= startOfColumnCards) {
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, true);
            } else if (columnCounter >= columnTurnedCards && columnCounter >= startOfColumnCards){
                fgets(content, 104, fptr);
                insertCardLast(columnCounter, content, false);
            }
        }
        if (columnCounter < 6) {
            columnCounter++;
        } else {
            columnCounter = 0;
            if (rowCounter == 0) {
                startOfColumnCards++;
            }
            if(rowCounter >= 5) {
                startOfColumnCards++;
            }
            rowCounter++;
            columnTurnedCards++;
        }
    }
    fclose(fptr);

}

/////////////////////////////////////////////////
//play screen printer
void printCard(struct cardNode* card){
    if (card == NULL) {
        printf("%c%c\t", ' ', ' ');
    }
    if (card != NULL && !card->turned) {
        printf("[]\t");
    }
    if (card != NULL && card->turned) {
        printf("%c%c\t", card->card[0], card->card[1]);
    }
}

void printPlayScreen(struct cardNode* cardLinkedLists[]) {
    //clears terminal before printing the playscreen again
    system("cls");
    //Initializes all C columns to be the first in the stack of cards
    struct cardNode* C_columns[7] = {
            cardLinkedLists[0], cardLinkedLists[1], cardLinkedLists[2], cardLinkedLists[3],
            cardLinkedLists[4], cardLinkedLists[5], cardLinkedLists[6]};

    //initializes all F columns to be the last in the stack of cards
    struct cardNode* F_columns[4] = {
            returnLastInCardStack(cardLinkedLists[7]), returnLastInCardStack(cardLinkedLists[8]),
            returnLastInCardStack(cardLinkedLists[9]), returnLastInCardStack(cardLinkedLists[10])};

    int counterForF = 0;
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    printf("\n");
    while ((C_columns[0] != NULL || C_columns[1] != NULL || C_columns[2] != NULL || C_columns[3] != NULL
            || C_columns[4] != NULL || C_columns[5] != NULL || C_columns[6] != NULL) || counterForF < 7) {

        for (int column = 0; column < 7; ++column) {
            printCard(C_columns[column]);
        }

        printf("\t");
        if (counterForF == 0) {
            printCard(F_columns[0]);
            printf("F1");
        } else if (counterForF == 2) {
            printCard(F_columns[1]);
            printf("F2");
        } else if (counterForF == 4) {
            printCard(F_columns[2]);
            printf("F3");
        } else if (counterForF == 6) {
            printCard(F_columns[3]);
            printf("F4");
        }
        counterForF++;

        for (int column1 = 0; column1 < 7; ++column1) {
            if (C_columns[column1] != NULL) {
                C_columns[column1] = C_columns[column1]->next;
            }
        }
        printf("\n");
    }
    printf("LAST command:%s\n", lastCommand);
    printf("Message:%s\n", message);
    printf("INPUT > ");
}
void startUpPhase(bool isQUsed);

void playPhase() {
    char command[20];
    char fromColumn[2];
    char toColumn[2];
    char fromCard[2];
    char toCard[2];
    struct cardNode* tempToCard;
    struct cardNode* tempFromCard;
    strncpy(command, "", 20);

    while ((startOfColumns[0] != NULL || startOfColumns[1] != NULL || startOfColumns[2] != NULL || startOfColumns[3] != NULL || startOfColumns[4] != NULL || startOfColumns[5] != NULL || startOfColumns[6] != NULL) && command[0] != 'Q') {
        strncpy(command, "", 20);
        strncpy(fromColumn, "", 2);
        strncpy(toColumn, "", 2);
        strncpy(fromCard, "", 2);
        strncpy(toCard, "", 2);
        do {
            printPlayScreen(startOfColumns);
            scanf(" %[^\n]", command);
            strncpy(lastCommand, command, 20);
            if (((command[2] == ':' && command[5] == '-' && command[6] == '>') || (command[2] == ':' && command[5] != ' ')) && command[0] != 'Q') {
                if (command[2] == ':' && command[5] == '-' && command[6] == '>') {
                    fromColumn[0] = command[0];
                    fromColumn[1] = command[1];
                    fromCard[0] = command[3];
                    fromCard[1] = command[4];
                    toColumn[0] = command[7];
                    toColumn[1] = command[8];
                    tempToCard = returnLastInCardStack(startOfColumns[((int) toColumn[1])-49]);
                    if (tempToCard != NULL) {
                        toCard[0] = tempToCard->card[0];
                        toCard[1] = tempToCard->card[1];
                    }
                } else if (command[2] == ':' && command[5] == '\000') {
                    fromColumn[0] = command[0];
                    fromColumn[1] = command[1];
                    toColumn[0] = command[3];
                    toColumn[1] = command[4];
                    if (toColumn[0] == 'F') {
                        tempToCard = returnLastInCardStack(startOfColumns[(((int) toColumn[1])-49)+7]);
                    } else  {
                        tempToCard = returnLastInCardStack(startOfColumns[(((int) toColumn[1])-49)]);
                    }
                    if (tempToCard != NULL) {
                        toCard[0] = tempToCard->card[0];
                        toCard[1] = tempToCard->card[1];
                    }
                    if (fromColumn[0] == 'F') {
                        tempFromCard = returnLastInCardStack(startOfColumns[(((int) fromColumn[1])-49)+7]);
                    } else  {
                        tempFromCard = returnLastInCardStack(startOfColumns[((int) fromColumn[1])-49]);
                    }
                    if (tempFromCard != NULL) {
                        fromCard[0] = tempFromCard->card[0];
                        fromCard[1] = tempFromCard->card[1];
                    }
                }
                int fromCardValue = cardValueConverter(fromCard[0]);
                int toCardValue = cardValueConverter(toCard[0]);
                bool rightColorForMove = true;

                if (toColumn[0] == 'C') {
                    if ((toCard[1] != fromCard[1]) && ((int)toColumn[1]-48) < 8) {
                        rightColorForMove = true;
                        strncpy(message, "Ok", 50);
                    } else {
                        rightColorForMove = false;
                        strncpy(message, "Error: Move is not valid", 50);
                    }
                    if (((toCardValue == fromCardValue+1) || tempToCard == NULL) && rightColorForMove) {
                        if (fromColumn[0] == 'F') {
                            removeFromColumnAndAddToNewColumn(((int)fromColumn[1]-49)+7, (int)toColumn[1]-49, fromCard);
                        } else {
                            removeFromColumnAndAddToNewColumn((int)fromColumn[1]-49, (int)toColumn[1]-49, fromCard);
                        }
                    }
                } else if (toColumn[0] == 'F') {
                    if (toColumn[0] == 'F' && fromColumn[0] == 'C') {
                        if (tempToCard == NULL && fromCardValue == 1) {
                            insertCardFirst(((int)toColumn[1]-49)+7, fromCard, true);
                            removeCardFromList(((int)fromColumn[1]-49), fromCard);
                            strncpy(message, "Ok", 50);
                        } else if (tempToCard != NULL && fromCardValue == toCardValue+1 && fromCard[1] == toCard[1]) {
                            removeFromColumnAndAddToNewColumn(((int)fromColumn[1]-49), ((int)toColumn[1]-49)+7, fromCard);
                            strncpy(message, "Ok", 50);
                        } else {
                            strncpy(message, "Error: Move is not valid", 50);
                        }
                    }
                }
            }
        } while (((command[2] != ':' && command[5] != '-' && command[6] != '>') || (command[2] != ':' && command[5] != ' ')) && command[0] != 'Q');
        struct cardNode* tempLastCard;
        for (int i = 0; i < 7; ++i ) {
            tempLastCard = returnLastInCardStack(startOfColumns[i]);
            if (tempLastCard != NULL) {
                tempLastCard->turned = true;
            }
        }
    }
    if (command[0] == 'Q' && command[1] == '\000') {
        startUpPhase(true);
    }
    if (command[0] == 'Q' && command[1] == 'Q') {
        QQ();
    }
    if ((startOfColumns[0] != NULL || startOfColumns[1] != NULL || startOfColumns[2] != NULL || startOfColumns[3] != NULL || startOfColumns[4] != NULL || startOfColumns[5] != NULL || startOfColumns[6] != NULL)) {
        strncpy(message, "Winner", 50);
    }
}

void startUpPhase(bool isQUsed){
    clearAllLists();
    char command[20];
    char fileName[16];
    char shuffleIndex[2];
    bool breakLoop = false;
    strncpy(command, "", 20);
    time_t t;
    srand((unsigned) time(&t));
    int r;

    // Get input for loading
    while (((command[0] != 'L' && command[1] != 'D') || breakLoop == false) && (!isQUsed || !breakLoop)) {
        printPlayScreen(startOfColumns);
        scanf(" %[^\n]", command);
        strncpy(lastCommand, command, 20);
        if (command[0] == 'L' && command[1] == 'D') {
            if (strlen(command)>2) {
                for (int i = 0; i < strlen(command)-3; ++i) {
                    // if an input is "LD <cards.txt>" we want the cards.txt
                    // \0 is added at the end, so we always get the right output
                    fileName[i] = command[i+3];
                    if (i == strlen(command)-4) {
                        fileName[i+1] = '\0';
                    }
                }
            }
            if (strlen(command) < 3) {
                LD("cards.txt", false);
            } else {
                LD(fileName, false);
            }
            if (strcmp(message, "OK") == 0) {
                breakLoop = true;
            }
        } else {
            strncpy(message, "Error: Command is not available right now", 50);
        }
        if (isQUsed && command[0] == 'P'){
            breakLoop = true;
            LD("currentDeck.txt", false);
            strncpy(message, "Ok: Game is started with previous deck", 50);
        }
    }

    char fileName1[16];

    while (command[0] != 'P') {
        printPlayScreen(startOfColumns);
        scanf(" %[^\n]", command);
        strncpy(lastCommand, command, 20);
        if (command[0] == 'S' && command[1] == 'I') {
            for (int i = 0; i < 2; ++i) {
                shuffleIndex[i] = command[i+3];
            }
            int index = atoi(shuffleIndex);
            if (strlen(shuffleIndex) != 0) {
                if (index > 0 && index < 52) {
                    SI(index);
                }
            } else {
                r = rand() % 51 + 1;
                SI(r);
            }
        }
        if (command[0] == 'S' && command[1] == 'R') {
            SR();
        }
        if (command[0] == 'S' && command[1] == 'D') {
            if (strlen(command)>2) {
                for (int i = 0; i < strlen(command)-3; ++i) {
                    fileName1[i] = command[i+3];
                    if (i == strlen(command)-4) {
                        fileName1[i+1] = '\0';
                    }
                }
            }
            if (strlen(command) < 3) {
                SD("cards2.txt");
            } else {
                SD(fileName1);
            }
        }
        if (command[0] == 'S' && command[1] == 'W') {
            SW();
        }
        if ((command[0] != 'S' && command[1] != 'I') || (command[0] != 'S' && command[1] != 'R')) {
            strncpy(message, "Error: Command is not available in START phase", 50);
        }
    }
    //Saves deck to be loaded into the playPhase
    SD("currentDeck.txt");
    strncpy(message, "Ok", 50);
    P();
    playPhase();
    printPlayScreen(startOfColumns);
}

int main() {
    startUpPhase(false);
}
