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

struct string_arr string_arr_new(size_t size);
void  string_arr_delete(struct string_arr);
void string_arr_copy(struct string_arr src, size_t src_offset, struct string_arr dest, size_t dest_offset, size_t length);

typedef int (*CmpFunc)(struct string a, struct string b);

void merge_combine(struct string_arr out, struct string_arr left, struct string_arr right, CmpFunc func) {

  int i, left_indx = 0, right_indx = 0;
  for (i = 0; left_indx < left.length && right_indx < right.length; i++) {
    if (func(left.strings[left_indx], right.strings[right_indx])) {
      out.strings[i] = left.strings[left_indx++];
    } else {
      out.strings[i] = right.strings[right_indx++];
    }
  }

  if (left_indx < left.length) {
    string_arr_copy(left, left_indx, out, i, left.length - left_indx);
  } else if (right_indx < right.length) {
    string_arr_copy(right, right_indx, out, i, right.length - right_indx);
  }
}

void merge_sort(struct string_arr base, CmpFunc func) {
  if (base.length <= 1) {
    return;
  }

  struct string_arr left, right;

  size_t left_length = base.length / 2;
  size_t right_length = base.length - left_length;

  left = string_arr_new(left_length);
  right = string_arr_new(right_length);

  string_arr_copy(base, 0, left, 0, left_length);
  string_arr_copy(base, left.length, right, 0, right_length);

  merge_sort(left, func);
  merge_sort(right, func);

  merge_combine(base, left, right, func);

  string_arr_delete(left);
  string_arr_delete(right);
}

int lexicographic_sort(struct string a, struct string b) {
  return strcmp(a.chars, b.chars) < 0;
}

int lexicographic_sort_reverse(struct string a, struct string b) {
  return strcmp(a.chars, b.chars) >= 0;
}

int check_distinct(struct string str, char c) {
  return strchr(str.chars, c) == strrchr(str.chars, c);
}

int get_distinct(struct string str) {
  int i, count = 0;
  for (i = 0; i < str.length; i++) {
    if (check_distinct(str, str.chars[i])) {
      count++;
    }
  }
  return count;
}

int sort_by_number_of_distinct_characters(struct string a, struct string b) {
  int a_count = get_distinct(a);
  int b_count = get_distinct(b);

  return a_count == b_count ? lexicographic_sort(a, b) : a_count <= b_count;
}

int sort_by_length(struct string a, struct string b) {
  return a.length == b.length ? lexicographic_sort(a, b) : a.length <= b.length;
}

void charptr_to_string(struct string* str, char* chars) {
  str->chars = chars;
  str->length = strlen(chars);
}

void string_sort(char** arr, const int len, CmpFunc func){
  struct string_arr strings = string_arr_new(len);

  int i;
  for (i = 0; i < len; i++) {
    charptr_to_string(&strings.strings[i], arr[i]);
  }

  merge_sort(strings, func);

  for (i = 0; i < len; i++) {
    arr[i] = strings.strings[i].chars;
  }

  string_arr_delete(strings);
}

struct string_arr string_arr_new(size_t size) {
  struct string_arr arr;
  arr.strings = size ? malloc(sizeof(struct string) * size) : NULL;
  arr.length = size;
  return arr;
}

void string_arr_delete(struct string_arr arr) {
  if (arr.strings) {
    free(arr.strings);
  }
}

void string_arr_copy(struct string_arr src, size_t src_offset, struct string_arr dest, size_t dest_offset, size_t length) {
  if (src.strings && dest.strings) {
    memcpy(
      dest.strings + dest_offset,
      src.strings + src_offset,
      sizeof(struct string) * length
    );
  }
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
