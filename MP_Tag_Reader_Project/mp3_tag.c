#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mp3_tag.h"

// Case-insensitive compare
int my_strcasecmp(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        char c1 = (*s1 >= 'A' && *s1 <= 'Z') ? *s1 + 32 : *s1;
        char c2 = (*s2 >= 'A' && *s2 <= 'Z') ? *s2 + 32 : *s2;

        if (c1 != c2)
            return c1 - c2;

        s1++; s2++;
    }
    return *s1 - *s2;
}

// ID3v2.4 size
int read_size(unsigned char size_bytes[4])
{
    return (size_bytes[0] << 21) |
           (size_bytes[1] << 14) |
           (size_bytes[2] << 7)  |
           (size_bytes[3]);
}

// ID3v2.3 size
int read_size_v23(unsigned char size_bytes[4])
{
    return (size_bytes[0] << 24) |
           (size_bytes[1] << 16) |
           (size_bytes[2] << 8)  |
           (size_bytes[3]);
}

// Validate MP3
int validate_mp3(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;

    char header[3];
    fread(header, 1, 3, fp);
    fclose(fp);

    return strncmp(header, "ID3", 3) == 0;
}
//=================== VIEW ====================
void view_tags(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Error opening file\n");
        return;
    }

    unsigned char header[10];
    fread(header, 1, 10, fp);

    printf("------------------------------------------------------------\n");
    printf("           MP3 Tag Reader and Editor \n");
    printf("------------------------------------------------------------\n");

    while (1)
    {
        char frame_id[5] = {0};
        unsigned char size_bytes[4];
        unsigned short flags;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;

        frame_id[4] = '\0';

        if (frame_id[0] == 0)
            break;

        fread(size_bytes, 1, 4, fp);
        fread(&flags, 1, 2, fp);

        int size = (size_bytes[0] << 24) |
                   (size_bytes[1] << 16) |
                   (size_bytes[2] << 8)  |
                   (size_bytes[3]);

        char *data = malloc(size + 1);
        fread(data, 1, size, fp);
        data[size] = '\0';

        int offset = (data[0] == 1) ? 3 : 1;

        if (strcmp(frame_id, "TIT2") == 0)
            printf("Title     : %s\n", data + offset);
        else if (strcmp(frame_id, "TPE1") == 0)
            printf("Artist    : %s\n", data + offset);
        else if (strcmp(frame_id, "TALB") == 0)
            printf("Album     : %s\n", data + offset);
        else if (strcmp(frame_id, "TYER") == 0 || strcmp(frame_id, "TDRC") == 0)
            printf("Year      : %s\n", data + offset);
        else if (strcmp(frame_id, "TCON") == 0)
            printf("Music     : %s\n", data + offset);
        else if (strcmp(frame_id, "COMM") == 0)
        {
            int i = 4;
            while (i < size && data[i] != '\0')
                i++;
            if (i + 1 < size)
                printf("Comment   : %s\n", data + i + 1);
        }

        free(data);
    }

    printf("------------------------------------------------------------\n");

    fclose(fp);
}
// ================= EDIT TAGS =================

void edit_tags(const char *filename, const char *tag, const char *new_value)
{
    FILE *fp = fopen(filename, "r+b");
    if (!fp)
    {
        printf("Error opening file\n");
        return;
    }

    unsigned char header[10];
    fread(header, 1, 10, fp);

    int version = header[3];   // 🔥 detect version

    while (1)
    {
        char frame_id[5] = {0};
        unsigned char size_bytes[4];
        unsigned short flags;

        if (fread(frame_id, 1, 4, fp) != 4)
            break;

        frame_id[4] = '\0';

        if (frame_id[0] == 0)
            break;

        fread(size_bytes, 1, 4, fp);
        fread(&flags, 1, 2, fp);

        int size;
        if (version == 3)
            size = read_size_v23(size_bytes);
        else
            size = read_size(size_bytes);

        long pos = ftell(fp);

        // ✅ YEAR FIX
        int is_year = (strcmp(tag, "YEAR") == 0 &&
                      (strcmp(frame_id, "TDRC") == 0 || strcmp(frame_id, "TYER") == 0));

        if (is_year || strcmp(frame_id, tag) == 0)
        {
            unsigned char *buffer = malloc(size);
            fread(buffer, 1, size, fp);

            int new_len = strlen(new_value);

            // ================= COMM =================
            if (strcmp(frame_id, "COMM") == 0)
            {
                unsigned char encoding = 0; // force ASCII

                char language[3] = {'e','n','g'};
                int desc_end = 4;

                while (desc_end < size && buffer[desc_end] != '\0')
                    desc_end++;

                int desc_len = desc_end - 4;

                int required = 1 + 3 + (desc_len + 1) + new_len;

                if (required > size)
                {
                    printf("Comment too long!\n");
                    free(buffer);
                    fclose(fp);
                    return;
                }

                fseek(fp, pos, SEEK_SET);

                fputc(encoding, fp);
                fwrite(language, 1, 3, fp);

                fwrite(buffer + 4, 1, desc_len + 1, fp); // preserve desc
                fwrite(new_value, 1, new_len, fp);

                for (int i = required; i < size; i++)
                    fputc('\0', fp);

                printf("COMM updated successfully!\n");

                free(buffer);
                fclose(fp);
                return;
            }

            // ================= NORMAL TEXT =================
            if (new_len + 1 > size)
            {
                printf("New value too long!\n");
                free(buffer);
                fclose(fp);
                return;
            }

            fseek(fp, pos, SEEK_SET);

            fputc(0, fp); // encoding ASCII
            fwrite(new_value, 1, new_len, fp);

            for (int i = new_len + 1; i < size; i++)
                fputc('\0', fp);

            printf("%s updated successfully!\n", frame_id);

            free(buffer);
            fclose(fp);
            return;
        }

        fseek(fp, size, SEEK_CUR);
    }

    printf("⚠️ Tag not found!\n");
    fclose(fp);
}