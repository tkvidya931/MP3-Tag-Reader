/*      Name:VIDYA T K
Project Name:MP3 Tag Reader and Editor
        Date:26/04/2026
Description:The MP3 Tag Reader and Editor is a C-based application developed to read, display, and 
            modify metadata stored in MP3 files using the ID3v2 tag format.
           The project provides an efficient command-line interface for viewing and editing
           common MP3 tags such as title, artist, album, year, genre, and comments.

          The application directly accesses binary MP3 files and processes ID3 frames using low-level file 
          handling techniques. It supports both ID3v2.3 and ID3v2.4 tag versions by properly decoding frame 
          sizes and managing metadata structures.
             */


#include <stdio.h>
#include "mp3_tag.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("View : %s -v <file.mp3>\n", argv[0]);
        printf("Edit : %s -e -t|-a|-l|-y|-c|-g <new_value> <file.mp3>\n", argv[0]);
        return 1;
    }

    if (my_strcasecmp(argv[1], "-v") == 0)
    {
        if (!validate_mp3(argv[2]))
            return 1;

        view_tags(argv[2]);
    }
    else if (my_strcasecmp(argv[1], "-e") == 0)
    {
        if (argc < 5)
        {
            printf("Error: Missing arguments\n");
            return 1;
        }

        char *tag = NULL;

        if (my_strcasecmp(argv[2], "-t") == 0) tag = "TIT2";
        else if (my_strcasecmp(argv[2], "-a") == 0) tag = "TPE1";
        else if (my_strcasecmp(argv[2], "-l") == 0) tag = "TALB";
        else if (my_strcasecmp(argv[2], "-y") == 0) tag = "YEAR";   // ✅ FIX
        else if (my_strcasecmp(argv[2], "-c") == 0) tag = "COMM";
        else if (my_strcasecmp(argv[2], "-g") == 0) tag = "TCON";
        else
        {
            printf("Invalid tag option\n");
            return 1;
        }

        if (!validate_mp3(argv[4]))
            return 1;

        edit_tags(argv[4], tag, argv[3]);

        printf("\nUpdated Details:\n");
        view_tags(argv[4]);
    }
    else
    {
        printf("Invalid option\n");
    }

    return 0;
}