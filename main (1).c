#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int strNumMods(char* str1, char* str2) {
    int ans = 0;
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int l = 0;
    while(l < len1 || l < len2){
        if(l < len1 && l < len2){
            if(str1[l] != str2[l]){
                ans++;
            }
        }
        else{
            ans++;
        }
        l++;
    }
    return ans;
}

int strDiffInd(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int l = 0;
    while(l < len1 || l < len2){
        if(l < len1 && l < len2){
            if(str1[l] != str2[l]){
                break;
            }
        }
        else{
            break;
        }
        l++;
    }
    return l; //modify this
}

void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    if (*numWords >= *maxWords) {
        // Double the capacity
        *maxWords *= 2;
        char** newWords = malloc((*maxWords) * sizeof(char*));
        if (newWords == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);  // or handle the error in an appropriate way
        }
        
        // Copy the existing words to the new array
        for (int i = 0; i < *numWords; i++) {
            newWords[i] = (*words)[i];
        }
        
        // Free the old array
        free(*words);
        
        // Update the array pointer
        *words = newWords;
    }

    // Allocate memory for the new word
    (*words)[*numWords] = malloc((strlen(newWord) + 1) * sizeof(char));
    if ((*words)[*numWords] == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);  // or handle the error in an appropriate way
    }

    // Copy the new word to the allocated memory
    strcpy((*words)[*numWords], newWord);

    // Increment the word count
    (*numWords)++;
}

int findWord(char** words, int numWords, char* aWord) {
    for (int i = 0; i < numWords; i++) {
        if (strcmp(words[i], aWord) == 0) {
            return i;  // Match found, return the index
        }
    }
    return -1;  // No match found
}


bool validWordCheck(char** words, int numWords, char** ladder, int ladderHeight, char* aWord) {
    // Check if the user wants to stop
    if (strcmp(aWord, "DONE") == 0) {
        printf("Stopping with an incomplete word ladder...\n");
        return true;
    }

    // Check if the word is in the dictionary or has the wrong length
    int wordIndex = findWord(words, numWords, aWord);
    if (wordIndex == -1) {
        printf("Entered word NOT in dictionary. Try again...\n");
        return false;
    }

    // Check if the word is a one-character change from the previous word
    if (ladderHeight > 0) {
        char* prevWord = ladder[ladderHeight - 1];
        int numMods = strNumMods(prevWord, aWord);
        if (numMods != 1) {
            printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
            return false;
        }
    }

    printf("Entered word is valid and will be added to the word ladder.\n");
    return true;
}


bool ladderComplete(char** ladderWords, int n, char* finalWord) {
    if (n <= 0)
        return false;

    char* lastWord = ladderWords[n - 1];
    return strcmp(lastWord, finalWord) == 0;
}

void displayIncompleteLadder(char** ladderWords, int n) {
    // Display three lines of "..." at the top of the ladder
    for (int i = 0; i < 3; i++) {
        printf("  ...\n");
    }

    // Display the ladder words in reverse order
    for (int i = n - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]);
    }
}


void displayCompleteLadder(char** ladderWords, int n) {
    int wordLen = strlen(ladderWords[0]);
    // Display the ladder words in reverse order
    for (int i = n - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]);

        // Display the symbol '^' for changing character between rungs
        if (i > 0) {
            printf("  ");
            int pos = strDiffInd(ladderWords[i], ladderWords[i-1]);
            for (int j = 0; j < pos; j++) {
                printf(" ");
            }
            printf("^");
            for (int j = 0; j < wordLen - pos - 1; j++) {
                printf(" ");
            }
            printf("\n");
        }
    }
}


int main(int argc, char* argv[]) {

    printf("\nProcessing command-line arguments...\n");

    int wordLen = 0;
    int maxLadder = 1;
    char dict[100] = "notAfile";     
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid"; 


    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) {
            // Set word length
            if (i + 1 < argc) {
                wordLen = atoi(argv[i + 1]);
                i++;  // Skip the next argument
            }
        } 
        else if (strcmp(argv[i], "-m") == 0) {
            // Set maximum ladder height
            if (i + 1 < argc) {
                maxLadder = atoi(argv[i + 1]);
                i++;  // Skip the next argument
            }
        } 
        else if (strcmp(argv[i], "-d") == 0) {
            // Set dictionary file
            if (i + 1 < argc) {
                strncpy(dict, argv[i + 1], sizeof(dict) - 1);
                dict[sizeof(dict) - 1] = '\0';
                i++;  // Skip the next argument
            }
        } 
        else if (strcmp(argv[i], "-s") == 0) {
            // Set starting word
            if (i + 1 < argc) {
                strncpy(startWord, argv[i + 1], sizeof(startWord) - 1);
                startWord[sizeof(startWord) - 1] = '\0';
                i++;  // Skip the next argument
            }
        } 
        else if (strcmp(argv[i], "-f") == 0) {
            // Set final word
            if (i + 1 < argc) {
                strncpy(finalWord, argv[i + 1], sizeof(finalWord) - 1);
                finalWord[sizeof(finalWord) - 1] = '\0';
                i++;  // Skip the next argument
            }
        }
    }


    printf("Interactively (re)setting invalid parameters:\n");

    // set word length using interactive user-input if necessary
    while (wordLen < 2 || wordLen > 20) {
        printf("Invalid word length = %d\n", wordLen);
        printf("Enter a valid word length (must be 2-20): ");
        scanf("%d",&wordLen);
        printf("\n");
    }

    // set max ladder height using interactive user-input if necessary
    if (maxLadder < 2) {
        printf("Invalid max ladder height = %d\n", maxLadder);
        printf("Enter a valid max ladder height (must be >1): ");
        scanf("%d",&maxLadder);
        printf("\n");
    }

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    FILE* fileTry = fopen(dict,"r");
    while (fileTry == NULL) {
        printf("Dictionary file %s not found...\n", dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        fileTry = fopen(dict,"r");
    }
    fclose(fileTry);
    
    // build the [words] array, a heap-allocated array of C-strings
    // where the words are a read-in from the dictionary file
    // and only those words of the desired length [wordLen] 

    int numWords = 0; // initially, no words in the array
    int maxWords = 4; // initially, capacity = 4


    FILE* file = fopen(dict, "r");
    char buffer[100];  // Assuming no word in the dictionary exceeds 100 characters
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove the trailing newline character, if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        if (len == wordLen) {
            addWord(&words, &numWords, &maxWords, buffer);
        }
    }
    fclose(file);
    

    printf("The dictionary contains %d words of length %d.\n",numWords,wordLen);
    printf("Max size of the dynamic words array is %d.\n",maxWords);
    printf("\n");

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("Dictionary %s contains insufficient %d-letter words...\n",dict,wordLen);
        printf("Terminating program...\n");
        return -1;
    }

    // resetting start word using interactive user-input if necessary
    while (findWord(words,numWords,startWord) == -1) {
        printf("Start word %s is not valid...\n", startWord);
        printf("Enter a valid start word: ");
        scanf("%s", startWord);
        printf("\n");
    }

    // resetting final word using interactive user-input if necessary
    while (findWord(words,numWords,finalWord) == -1) {
        printf("Final word %s is not valid...\n", finalWord);
        printf("Enter a valid final word: ");
        scanf("%s", finalWord);
        printf("\n");
    }


    printf("\nWelcome to the CS 211 Word Ladder Builder!\n");
    printf("\n"); 

    printf("Your goal is to make a word ladder between two ");
    printf("%d-letter words: \n  %s -> %s\n\n",wordLen, startWord,finalWord);  
    
    // Allocating the word ladder for the maximum allowed height
    char** ladder = (char**)malloc(maxLadder*sizeof(char*));

    int ladderHeight = 0; // initially, the ladder is empty
    
    // add the start word to the ladder, i.e. the "bottom rung"
    addWord(&ladder,&ladderHeight,&maxLadder,startWord);
    
    char aWord[30] = "XYZ";
    printf("\n");


    while (ladderHeight < maxLadder && strcmp(aWord, "DONE") != 0 && !ladderComplete(ladder, ladderHeight, finalWord)) {   // modify this line 
        printf("The goal is to reach the final word: %s\n",finalWord);
        printf("The ladder is currently: \n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Current ladder height: %d\n",ladderHeight);
        printf("Maximum ladder height: %d\n",maxLadder);
        printf("Enter the next word (or DONE to stop): ");
        scanf("%s",aWord);
        printf("\n");

        // Make sure the entered word is valid for the next ladder rung;
        // if not, repeatedly allow user to enter another word until one is valid
        while (!validWordCheck(words, numWords, ladder, ladderHeight, aWord)) {
            printf("Enter another word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
        }

        // add the entered word to the ladder (unless it is "DONE")
        if (strcmp(aWord,"DONE") != 0) {
            addWord(&ladder,&ladderHeight,&maxLadder,aWord);
        }
        printf("\n");

    }
    

    // Check if the built word ladder is complete and 
    // display the word ladder appropriately. 
    if (ladderComplete(ladder, ladderHeight, finalWord)) {
        printf("Word Ladder complete!\n");
        displayCompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you find a shorter Word Ladder next time??? \n");
    } else {
        if (ladderHeight == maxLadder) printf("The Word Ladder reached the maximum height.\n");
        printf("The final Word Ladder is incomplete:\n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you complete the Word Ladder next time??? \n");
    }
    


    // words
    for(int i = 0; i < numWords; i++){
        free(words[i]);
    }
    free(words);

    // ladder
    for(int i = 0; i < ladderHeight; i++){
        free(ladder[i]);
    }
    free(ladder);

    return 0;
}
