#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

int stringLength(char s[])
{
	int i = 0;
	while(s[i] != '\0')
	{
		i++;
	}
	return i;
}

int minimum(int a, int b, int c)
{
    if (a < b && a < c)
    {
        return a;
    }
    else if(b < a && b < c)
    {
        return b;
    }
    
    else return c;
}
 

int LevenshteinDistance(const char *s, int len_s, const char *t, int len_t)
{ 
    int cost;

    if (len_s == 0) return len_t;
    if (len_t == 0) return len_s;
 
    if (s[len_s-1] == t[len_t-1])
        cost = 0;
    else
        cost = 1;
 
    return minimum(LevenshteinDistance(s, len_s - 1, t, len_t    ) + 1,
    LevenshteinDistance(s, len_s    , t, len_t - 1) + 1,
    LevenshteinDistance(s, len_s - 1, t, len_t - 1) + cost);
}
 
 
/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
    char* word = nextWord(file);
    while (word) 
    {
        hashMapPut(map, word, 1);
        free(word);
        word = nextWord(file);
    }

    free(word);
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    HashMap* map = hashMapNew(1000);   
    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (quit == 0)
    {   

        printf("Enter a word or \"quitprogram\" to quit: ");
        scanf("%s", inputBuffer);
        int c = 0;
        while(inputBuffer[c] != '\0')
        {
        	if(inputBuffer[c] >= 'A' && inputBuffer[c] <= 'Z')
        	{
        	    inputBuffer[c] = inputBuffer[c] + 32;
        	}	
            c++;
        }

        if (strcmp(inputBuffer, "quitprogram") == 0) 
        {
            quit = 1;
        } 

        else if (hashMapContainsKey(map, inputBuffer) == 1) 
        {
            printf("The inputted word %s is spelled correctly!\n", inputBuffer);
        } 
        
        else if (hashMapContainsKey(map, inputBuffer) == 0) 
        {
	    printf("The inputted word %s is spelled incorrectly!\n", inputBuffer);
	    printf("Please choose the search parameter!\n");
	    printf("1) Dictionary File! (This option is more accurate, but may will take a longer time than the other option)\n");
	    printf("2) Linked List From the Hashed Index!\n");
	    
	    char choice[5];
	    scanf("%s", choice);       
                 
            if(choice[0] == '1')
    	    {     
                printf("I will now run through the dictionary to find possible suggestions! Hold one min please while the program runs through the entire file!\n");
                int wordCounter = 1;
                int count = 5;
                int looping = 0;
                printf("We will begin looking for words with a distance of 1.\n");
                while (looping == 0)
                {
                    for (int i = 0; i < map->capacity; i++)
                    {
                        HashLink* link = map->table[i];
                        if(link != NULL)
                        {
                            while(link!= NULL)
                            {
                                int result = LevenshteinDistance(inputBuffer, stringLength(inputBuffer), link->key, stringLength(link->key));
                                if(result == wordCounter)
                                {	
//				    printf("Word Counter: %d", wordCounter);
                                    count--;
                                    printf("Did you mean %s?\n", link->key);
                                }
    
        
                                if(count < 1)
                                {
                                    looping = 1;
                                    break;
                                }
    
                                link = link->next;
                                if(count < 1)
                                {
                                    break;
                                }
	
				if(count < 1)
				{
					break;
				}
                            }
                        }
   
//			printf("Compring Word: %s", link->key);
//			printf("Index: %d\n", i);
                        if ((i == (map->capacity - 1)) && (count > 1))
                        {
			    printf("INCREMENTED!");
                            i = 0;
                            wordCounter++;
                            printf("Looking for words with a distance of: %d\n", wordCounter);
                        }
		
			if(count < 1)
			{
			break;
			}
    		    }
                }          
            }
            
            if(choice[0] == '2')
            {
                int resultTwo;
                int idx = HASH_FUNCTION(inputBuffer) % hashMapCapacity(map);
                HashLink* cur = map->table[idx];
                int loopingTwo = 0;
                int wordCounterTwo = 1;
                int countTwo = 5;	
                HashLink* startPoint = cur;
    
                while (loopingTwo == 0)
                {
                    resultTwo = LevenshteinDistance(inputBuffer, stringLength(inputBuffer), cur->key, stringLength(cur->key));
    
                    if(resultTwo == wordCounterTwo)
                    {
                        countTwo--;
                        printf("Did you mean %s?\n", cur->key);
                    }
    
                    if(countTwo < 1)
                    {
    	                loopingTwo = 1;
                    }
    
                    if(cur->next != NULL)
                    {
    	                cur= cur->next;
                    }
    
                    else if(cur->next == NULL)
                    {
    	                cur = startPoint;
    	                wordCounterTwo++;
                    }
                }
            } 
        }
    }
    hashMapDelete(map);
    return 0;
}
