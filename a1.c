#include "test.h"

#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */
void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
    
}

/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.hh
 */
#include <stdio.h>
#include <string.h>

void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data)) {
    int source_len = strlen(source);
    
    void generate(int start, int buf_index) {
        if (start == source_len) {
            process_split(buf, data);
            return;
        }
        
        for (int i = 0; i < nwords; i++) {
            const char *word = dictionary[i];
            int word_len = strlen(word);
            
            if (strncmp(source + start, word, word_len) == 0) {
                if (buf_index != 0) {
                    buf[buf_index++] = ' ';
                }
                strncpy(buf + buf_index, source + start, word_len);
                generate(start + word_len, buf_index + word_len);
            }
        }
    }
    
    generate(0, 0);
}


void process_split(char buf[], void *data) {
    printf("%s\n", buf);
}

int main() {
    const char *source = "applebanana";
    const char *dictionary[] = {"apple", "banana"};
    int nwords = 2;
    char buf[100];
    
    generate_splits(source, dictionary, nwords, buf, NULL, process_split);
    
    return 0;
}

/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
void previous_permutation(int a[], int n)
{
    
}

/* Write your tests here. Use the previous assignment for reference. */



int main()
{
    
    return 0;
}
