Data Comm Assignment 2
	Author: Ramzi Chennafi and Ian Davidson

Includes a project planning pdf, which has pseudocode and a diagram for the program. Also includes this readme and
the program + code + SkyeTek API in the visual studio package for easy importing.

Program for reading data tags from a Skyetek Reader in Asynchronous mode.

In order for program to work, the directory for the stapi.lib must be changed to wherever you put the project. 
Right click porperties in visual studio on skyetekReader, go to configuration properties, then linker, then click input.
Double click additional dependencies and input the correct directory for stapi.lib. It should be located within the API folder
of the project.

stapi.dll must also be placed into your windows/system32 and windows/syswow64. stapi.dll is also located in the API folder.

One more note, when hitting "Connect" in the program - the program will freeze for about 5 seocnds before reading.

Working with the tags:

	The smaller the reflective surface of the tags was, the shorter the range of scanning.
The smallest tags required direct contact with the reader to be reader. While the largest
tags could be read from a foot away. However certain designs of tag such as the dogbone 
had a directional type of reading and could be read from very far depending on how they were aimed.
Some were also able to be read regardless of the direction they faced, such as the frog 3d, while others
had to be aimed a certain way. Some were also a bit more powerful than others and allowed the 
reader to read through the box.