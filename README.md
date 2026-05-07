# MP3-Tag-Reader
The MP3 Tag Reader and Editor is a C-based application developed to read, display, and modify metadata stored in MP3 files using the ID3v2 tag format. The project provides an efficient command-line interface for viewing and editing common MP3 tags such as title, artist, album, year, genre, and comments.

The application directly accesses binary MP3 files and processes ID3 frames using low-level file handling techniques. It supports both ID3v2.3 and ID3v2.4 tag versions by properly decoding frame sizes and managing metadata structures.

Features:


      View MP3 metadata tags
   
      Edit MP3 tag information
   
      Supports ID3v2.3 and ID3v2.4 formats
   
      Modify:
    
        Title
      
        Artist
      
        Album
      
        Year
      
        Genre
       
        Comments
       
        Command-line based interface
       
         Binary file processing
       
         Dynamic memory management
      
Supported Commands:

    View Tags
    
           ./a.out -v song.mp3
           
Edit Tags

      ./a.out -e -t "New Title" song.mp3
      
      ./a.out -e -a "Artist Name" song.mp3
      
      ./a.out -e -l "Album Name" song.mp3
       
      ./a.out -e -y "2026" song.mp3
        
      ./a.out -e -g "Pop" song.mp3
        
      ./a.out -e -c "New Comment" song.mp3
      
        
Working Principle:


   MP3 Validation

    The program verifies whether the input file is a valid MP3 file by checking the "ID3" header.

   Tag Reading

     Metadata frames are parsed individually to extract information such as:

         TIT2 → Title
         
         TPE1 → Artist
         
         TALB → Album
         
         TYER/TDRC → Year
         
         TCON → Genre
         
         COMM → Comment
         
    Tag Editing

       The program updates frame data directly inside the MP3 file while preserving the existing file structure.

Technologies Used:->


    C Programming Language
    
    File Handling
     
    Dynamic Memory Allocation
   
    String Manipulation
   
    Structures and Pointers
    
    Binary File Processing
    
    
Concepts Demonstrated:

This project demonstrates practical implementation of:

   Binary file parsing

   Metadata processing

   Dynamic memory handling
  
   Command-line argument processing

   String operations in C

   Modular programming


Applications:->

     Music Library Management

     Audio Metadata Processing

     MP3 File Editing Tools

      Multimedia Software Development

Conclusion:

The MP3 Tag Reader and Editor project provides hands-on experience with binary file manipulation and metadata management in C. It strengthens understanding of file structures, dynamic memory allocation, and low-level programming concepts while building a practical multimedia utility.
