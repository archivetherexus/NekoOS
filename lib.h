//=======================================
// LIB
//=======================================

/// Define size_t if not defined!
#ifndef size_t
#define size_t long
#endif

/// Define NULL
#ifndef NULL
#define NULL 0
#endif

/// Define booleans.
#define true 1
#define false 0
#define bool int

/// Define basic IO functions.
int puts(const char *str);
int putchar(int character);
int getchar();
#ifndef EOF
#define EOF -1
#endif

/// String functions.
size_t strlen(const char *str);
int strcmp (const char *str1, const char *str2);
long atol(const char *str);
bool isalpha(const int c);
bool isdigit(const int c);

/// Memmory managment.
void free(void *ptr);
void * malloc(size_t size);
void * calloc(size_t num, size_t size);


