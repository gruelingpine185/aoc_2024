#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define ARR_SIZE(_arr) (sizeof(_arr) / sizeof(*_arr))
#define MAX(_l, _r) (((_l) >= (_r))? (_l): (_r))
#define MIN(_l, _r) (((_l) <= (_r))? (_l): (_r))

static int read_file(const char* _pathname, char** _data, long* _size);
static int is_digit(int _c);
static int is_whitespace(int _c);
static void skip_whitespace(const char* _str, size_t _size, size_t* _idx);
static void collect_number(const char* _str, size_t _size, size_t* _idx, int* _value);
static size_t partition(int* _arr, size_t _low, size_t _high);
static void quick_sort(int* _arr, int _low, int _high);
static void extract_values(const char* _input, long _size, int* _arr_l, int* _arr_r);
static void part_1(const int* _arr_l, const int* _arr_r, size_t _size);
static void part_2(const int* _arr_l, const int* _arr_r, size_t _size);


int main(int _argc, char** _argv) {
    const char* input_pathname = NULL;
    if(_argc == 2) {
        input_pathname = _argv[1];
    } else {
        printf("Expected 1 arguments. <input file>\n");
        return 1;
    }

    char* input = NULL;
    long input_size = 0;
    if(!read_file(input_pathname, &input, &input_size)) return 1;

    int list_left[1000] = {0};
    int list_right[1000] = {0};
    extract_values(input, input_size, list_left, list_right);
    part_1(list_left, list_right, ARR_SIZE(list_left));
    part_2(list_left, list_right, ARR_SIZE(list_left));
    free(input);
    return 0;
}

static int read_file(const char* _pathname, char** _data, long* _size) {
    if((!_pathname) || (!_data) || (!_size)) return 0;

    FILE* file = fopen(_pathname, "r");
    if(!file) return 0;

    if(fseek(file, 0, SEEK_END) == -1) {
        fclose(file);
        return 0;
    }

    long size = ftell(file);
    if(size == -1) {
        fclose(file);
        return 0;
    }
    
    if(fseek(file, 0, SEEK_SET) == -1) {
        fclose(file);
        return 0;
    }

    char* data = (char*) malloc(sizeof(char) * (size + 1));
    if(!data) {
        fclose(file);
        return 0;
    }

    if(!fread(data, size, 1, file)) {
        free(data);
        fclose(file);
        return 0;
    }

    data[size] = '\0';
    *_data = data;
    *_size = size;
    fclose(file);
    return 1;
}

static int is_digit(int _c) {
    return ((_c >= '0') && (_c <= '9'));    
}

static int is_whitespace(int _c) {
    return ((_c == '\r') || (_c == '\n') || (_c == ' '));
}

static void skip_whitespace(const char* _str, size_t _size, size_t* _idx) {
    size_t i = *_idx;
    do {
        if(!is_whitespace(_str[i])) {
            *_idx = i;
            return;
        }

        i++;
    } while(i < _size);
}

static void collect_number(const char* _str, size_t _size, size_t* _idx, int* _value) {
    if((!_str) || (!_idx) || (!_value)) return;

    size_t i = *_idx;
    int value = 0;
    do {
        if(!is_digit(_str[i])) {
            *_value = value;
            *_idx = i;
            return;
        }

        value *= 10;
        value += (_str[i] - '0');
        i++;
    } while(i < _size);
}

void swap(int *_l, int *_r) {
  int tmp = *_l;
  *_l = *_r;
  *_r = tmp;
}

static size_t partition(int* _arr, size_t _low, size_t _high) {
    int pivot_val = _arr[_high];
    size_t i = (_low - 1);
    for(size_t j = _low; j < _high; j++) {
        if(_arr[j] <= pivot_val) {
            i++;
            swap(&_arr[i], &_arr[j]);
        }
    }

    swap(&_arr[i + 1], &_arr[_high]);
    return (i + 1);
}

static void quick_sort(int* _arr, int _low, int _high) {
    if(_low < _high) {
        size_t pivot_val = partition(_arr, _low, _high);    
        quick_sort(_arr, _low, pivot_val - 1);    
        quick_sort(_arr, pivot_val + 1, _high);
    }
}

static void extract_values(const char* _input, long _size, int* _arr_l, int* _arr_r) {
    int list_selector = 0;
    int list_index = 0;
    int value = 0;
    for(size_t i = 0; i < _size - 1;) {
        if(is_digit(_input[i])) {
            collect_number(_input, (size_t) _size, &i, &value);
            if(list_selector & 1) {
                _arr_r[list_index] = value;
                list_index++;
            } else {
                _arr_l[list_index] = value;
            }

            list_selector++;
        }

        if(is_whitespace(_input[i])) {
            skip_whitespace(_input, (size_t) _size, &i);
        }
    }

    quick_sort(_arr_l, 0, 999);
    quick_sort(_arr_r, 0, 999);
}

static void part_1(const int* _arr_l, const int* _arr_r, size_t _size) {
    if((!_arr_l) || (!_arr_r) || (_size <= 0)) return;

    int sum = 0;
    int delta = 0;
    for(size_t i = 0; i < _size; i++) {
        delta =
            MAX(_arr_l[i], _arr_r[i]) -
            MIN(_arr_l[i], _arr_r[i]);
        sum += delta;
    }

    printf("part 1: %d\n", sum);
}

static void part_2(const int* _arr_l, const int* _arr_r, size_t _size) {
    int sum = 0;
    int n = 0;
    for(size_t i = 0; i < _size; i++) {
        n = 0;
        for(size_t j = 0; j < _size; j++) {
            if(_arr_l[i] == _arr_r[j]) {
                n++;
            }
        }
        
        sum += n * _arr_l[i];
    }

    printf("part 2: %d\n", sum);
}
