#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string {
  char* chars;
  size_t length;
};

struct string_arr {
  struct string* strings;
  size_t length;
};

typedef int (*CmpFunc)(struct string a, struct string b);

void merge_combine(struct string_arr* out, struct string_arr left, struct string_arr right, CmpFunc func) {
  int i, left_indx, right_indx;
  for (i = 0, left_indx = 0, right_indx = 0; left_indx < left.length && right_indx < right.length; i++) {
    if (func(left.strings[left_indx], right.strings[right_indx])) {
      out->strings[i] = left.strings[left_indx++];
    } else {
      out->strings[i] = right.strings[right_indx++];
    }
  }

  if (left_indx < left.length) {
    memcpy(out->strings + i, left.strings + left_indx, sizeof(struct string) * left.length - left_indx);
  } else if (right_indx < right.length) {
    memcpy(out->strings + i, right.strings + right_indx, sizeof(struct string) * right.length - right_indx);
  }
}

void merge_sort(struct string_arr* base, CmpFunc func) {
  if (base->length <= 1) {
    return;
  }

  struct string_arr left, right;

  left.length = base->length / 2;
  right.length = base->length - left.length;
  printf("Base Length: %ld\n", base->length);
  printf("Left Length: %ld\n", left.length);
  printf("Right Length: %ld\n", right.length);

  fflush(stdout);

  if (left.length > 0) {
    left.strings = malloc(sizeof(struct string) * left.length);
  } else {
    left.strings = NULL;
  }

  if (right.length > 0) {
    right.strings = malloc(sizeof(struct string) * right.length);
  } else {
    right.strings = NULL;
  }

  memcpy(left.strings, base->strings, sizeof(struct string) * left.length);
  memcpy(right.strings, base->strings + left.length, sizeof(struct string) * right.length);

  merge_sort(&left, func);
  merge_sort(&right, func);

  merge_combine(base, left, right, func);

  if (left.strings) {
    free(left.strings);
  }

  if (right.strings) {
    free(right.strings);
  }
}

int lexicographic_sort(struct string a, struct string b) {
  return strcmp(a.chars, b.chars);
}

int lexicographic_sort_reverse(struct string a, struct string b) {
  return strcmp(b.chars, a.chars);
}

int check_distinct(const char* str, char c) {
  char* a = strchr(str, c);
  char* b = strrchr(str, c);
  return a == b && a != NULL && b != NULL;
}

int sort_by_number_of_distinct_characters(struct string a, struct string b) {
  int i, a_count = 0, b_count = 0;
  for (i = 0; i < a.length; i++) {
    if (check_distinct(a.chars, a.chars[i])) {
      a_count++;
    }
  }

  for (i = 0; i < b.length; i++) {
    if (check_distinct(b.chars, b.chars[i])) {
      b_count++;
    }
  }

  return a_count > b_count;
}

int sort_by_length(struct string a, struct string b) {
  return a.length > b.length;
}

void string_sort(char** arr, const int len, CmpFunc func){
    struct string_arr strings;
    strings.strings = malloc(sizeof(struct string) * len);
    strings.length = len;

    int i;
    for (i = 0; i < len; i++) {
        struct string* str = &strings.strings[i];
        str->chars = arr[i];
        str->length = strlen(str->chars);
    }

    merge_sort(&strings, func);

    for (i = 0; i < len; i++) {
      arr[i] = strings.strings[i].chars;
    }

    free(strings.strings);
}

int main()
{
    int n;
    scanf("%d", &n);

    char** arr;
	arr = (char**)malloc(n * sizeof(char*));

    for(int i = 0; i < n; i++){
        *(arr + i) = malloc(1024 * sizeof(char));
        scanf("%s", *(arr + i));
        *(arr + i) = realloc(*(arr + i), strlen(*(arr + i)) + 1);
    }

    string_sort(arr, n, lexicographic_sort);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, lexicographic_sort_reverse);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, sort_by_length);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, sort_by_number_of_distinct_characters);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");
}
