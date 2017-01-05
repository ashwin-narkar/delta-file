# delta-file

This is my project from CS 32 at UCLA involving creating and applying a delta file to an old and new file. The delta file encodes the changes between the old and new files. The objective of this project was to create the smallest possible delta file to correctly highlight the exact changes from the old file to the new file.

The delta file is to be formatted with two commands, add and copy. The add command takes the amount of characters to copy and the characters to copy. The copy command takes the amount of characters to copy and the index from where to begin copying. 

The applyDelta fuction takes the old file and the delta file. It decodes the metadata in the file to correctly apply changes to the old file resulting in the new file. If the delta file is in any way incorrectly formatted, the function returns false and ends right away.

The createDelta function was the main challenge of this project. To create the delta file efficiently, I implemented a hash table with an array of linked list pointers. The program then takes sequences of characters from the old file and stores them in the hash table. Then it looks at the new file and sees if any of the character sequences matches. Then it keeps checking until there is no match and makes that entire sequence into a copy instruction. If a match is not met, the characters in the file are appended to a string toAdd. Once the next match is met, this toAdd string is made into an add instruction for the delta file. At the end, anything in toAdd is added to the delta file.
