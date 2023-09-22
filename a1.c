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
#include <stdio.h>

void generate_selections_util(int a[], int n,int k, int b[], int index, int start, void *data, void(*process_selection)(int *b, int k, void *data)) 
{
    if (index==k) 
    {
        process_selection(b,k, data);         //// Call for the printing function
        return;
    }

    for (int m= start; m<n;m++) 
    {
        b[index]= a[m];
        generate_selections_util(a,n, k, b,index+1, m+1,data,process_selection);        //// Call for recursion
    }
}

void generate_selections(int a[], int n, int k,int b[],void *data, void(*process_selection)(int *b,int k, void *data)) 
{
    if (k>n ||n<=0||k<=0)       //// For invalid input
    {
        printf("Invalid Input");
        return;
    }

    generate_selections_util(a, n, k, b, 0,0, data, process_selection);     //// Call the selecting function
}

void process_selection(int *b, int k, void *data) 
{
    for (int i=0; i<k;i++) 
    {
        printf("%d ", b[i]);
    }
    printf("\n");
}

//// Call the main function
int main() {
    
    int a[] = {5,6,2,9,1,3};
    int n = sizeof(a)/sizeof(a[0]);
    int k = 4;
    int b[k];

    generate_selections(a, n, k, b, NULL, process_selection);

    return 0;
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

/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */

void reverseArray(int a[], int i, int n)        //// to reverse the array from the provided index
{
    while (i < n) 
    {
        int temp = a[i];
        a[i] = a[n];
        a[n] = temp;
        i++;
        n--;
    }
}

void swap(int *a, int *b)            //// to process swap between two elements
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void previous_permutation(int a[], int n)
{
    int i = n - 1;                      //// We are looking for a point where the array is decreasing from the end of the array
    while (i > 0 && a[i - 1] <= a[i])
    {
        i -= 1;
    }   

    if (i == 0)             //// If the entire array is decreasing from the end, then there will be no previous permutation possible
    {
        return;            //// returning the given array as it is 
    }

    int j = n - 1;
    while (a[j] >= a[i - 1])
    {
        j -= 1;
    }

    swap(&a[i-1], &a[j]);         //// swapping the elements where the decrease happened in the array from the end

    reverseArray(a, i,n-1);       //// reversing the array after the (i-1)th element
}

/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
    int first;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

void count_selections(int b[], int k, void *data)
{
    int *d = (int*)data;
    ++*d;
}

typedef struct {
    int b[100];
} selection_t;

void last_selection(int b[], int k, void *data)
{
    selection_t *s = (selection_t*)data;
    for (int i = 0; i < k; ++i) {
        s->b[i] = b[i];
    }
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[10];
    state_t s2165 = { .index = 0, .err = 1, .first = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
    int c = 0;
    int aa[] = { 1, 5, 3, 0, 1, 12, 4, 3, 6, 6 };
    generate_selections(aa, 10, 5, b, &c, count_selections);
    ASSERT_EQ(c, 252, "Failed on 10C5.");

    selection_t s;
    generate_selections(aa, 10, 5, b, &s, last_selection);
    ASSERT_ARRAY_VALUES_EQ(s.b, 5, "Failed on last of 10C5.", 12, 4, 3, 6, 6);
} END_TEST

void test_splits_art(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    if (s->first) {
        s->err = 0;
        s->first = 0;
    }
    switch (s->index) {
    case 0:
        if (strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1, .first = 1 };
    char buf[256];
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
    int aa[] = { 1, 2, 3, 5, 4, 6 };
    previous_permutation(aa, 3); // 3 is correct.
    ASSERT_ARRAY_VALUES_EQ(aa, 3, "Failed on 1 2 3.", 1, 2, 3);
} END_TEST

int main()
{
    run_tests((test_t[]) {
        TEST(generate_selections),
        TEST(generate_splits),
        TEST(previous_permutation),
        0
    });
    return 0;
}
