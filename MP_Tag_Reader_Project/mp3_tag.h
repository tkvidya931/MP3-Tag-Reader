#ifndef MP3_TAG_H
#define MP3_TAG_H

#include <stdio.h>

// Structure to store tag information
typedef struct
{
    char title[100];
    char artist[100];
    char album[100];
    char year[10];
    char comment[200];
    char genre[50];
} TagInfo;

// Function prototypes
int read_size(unsigned char size_bytes[4]);
int validate_mp3(const char *filename);
void view_tags(const char *filename);
void edit_tags(const char *filename, const char *tag, const char *new_value);

// Custom case-insensitive comparison
int my_strcasecmp(const char *s1, const char *s2);

#endif