typedef double stack_element;
typedef long long int canary_t;



#define void_arr(size)                                      \
        if (size <= 0) {                                    \
        printf("Error: массив пустой! Size = %zu\n" , size);    \
        assert (0);                                         \
    }


#define STACK_ASSERT(stk)                                   \
        if (Stack_Error (stk) != COMPLETE_VALUE) {          \
            Stack_Dump (stk);                               \
            assert (0);                                     \
        }


struct Stack {
    canary_t canary_1;
    stack_element* data; // poison
    unsigned int length_bytes_aligment;
    size_t size;
    size_t capacity;
    unsigned long checksum_stack;
    unsigned long checksum_arr;
    canary_t canary_2;
};

enum Stack_Error {
    BAD_HASH_ARR = 52 ,
    BAD_HASH_STACK = 69 ,
    BAD_SIZE = 111 , // size > capacity
    BAD_CANARY_1 = 1 ,
    BAD_CANARY_2 = 2 ,
    BAD_CANARY_3 = 3 ,
    BAD_CANARY_4 = 4 ,
    COMPLETE_VALUE = 0 ,

};

enum canary_values {
    val_canary_1 = 777 ,
    val_canary_2 = 666 ,
    val_canary_3 = 334 ,
    val_canary_4 = 333
};

const double FACTOR_INCR = 2;
const double FACTOR_DECR = 0.5;

int Stack_Error (Stack* stk);
int Stack_Dump (Stack* stk);
FILE* Open_Log_File (const char* filename_write);
int Stack_Dtor (Stack* stk);
int Stack_Ctor (Stack* stk , int capacity);
int Stack_Push (Stack* stk , stack_element value);
stack_element Stack_Pop (Stack* stk);
unsigned long Calculate_stack_checksum (Stack* stk);
unsigned long Calculate_checksum (void* stk , int size_in_bytes);

/*
data[0] = 1
data[1] = -7
data[2] = 0
data[3] = FREE
data[4] = FREE
data[5] = FREE
data[6] = 8
data[7] = FREE
data[8] = FREE
*/
