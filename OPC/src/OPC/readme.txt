Shared Cross Platform C++ Project : For more information see http://www.visualmicro.com

Arduino shared projects allow code to be shared in multiple projects.

It is important to 'Add' a 'Reference' to 'Shared Projects' from the master Arduino project that uses the shared code.

It is also important to understand that, during compilation, shared project code is combined in a temporary folder along with the main project code. Arduino compile usually involves a temp folder therefore this way of working ensures that code could be manually combined and developed with a different IDE in the future.

Visual Studio will display intellisense based on the platform/board that has been specified for the currently active "Startup Project" of the current solution.

The Arduino IDE will also compile code that exists in an \src sub folder and it's children. Adhering to the same structure will also protect your code and allow compilation outside Visual Micro or a custom build tool. 

Suggestion: Because code is combined during compilation duplicate source code file names can be avoided with by structuring code as follows:-

SharedProject1
	src\SharedProject1\SharedProject1.cpp
	src\SharedProject1\SharedProject1.h
	
SharedProject2
	src\SharedProject2\SharedProject2.cpp
	src\SharedProject2\SharedProject2.h
	
When combined into the temp build folder during build of MainArduinoProject1, the structure will end like this:-

MainArduinoProject1
MainArduinoProject1.ino
	src\SharedProject1\SharedProject1.cpp
	src\SharedProject1\SharedProject1.h
	src\SharedProject2\SharedProject2.cpp
	src\SharedProject2\SharedProject2.h

	
blog: http://www.visualmicro.com/post/2016/05/27/Sharing-code-between-projects.aspx