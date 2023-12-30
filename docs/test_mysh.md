test of mysh

```bash
/Users/xuzy/Code/EDU-1S-C-project-myshell -> myls 
drwxr-xr-x 9 	xuzy 	staff 	288 	Dec 26 10:19	obj
drwxr-xr-x 3 	xuzy 	staff 	96 	Dec 26 10:19	bin
-rw-r--r-- 1 	xuzy 	staff 	740 	Nov 19 12:43	Makefile
drwxr-xr-x 8 	xuzy 	staff 	256 	Dec 26 08:51	include
drwxr-xr-x 5 	xuzy 	staff 	160 	Dec 01 18:18	output
-rw-r--r-- 1 	xuzy 	staff 	10133 	Dec 24 20:30	1S-C-project-myshell-readme.md
drwxr-xr-x 5 	xuzy 	staff 	160 	Dec 26 10:19	docs
-rwxr-xr-x 1 	xuzy 	staff 	33 	Nov 25 08:47	mysh
-rw-r--r-- 1 	xuzy 	staff 	149 	Dec 26 10:31	test.txt
drwxr-xr-x 9 	xuzy 	staff 	288 	Dec 26 10:19	src
/Users/xuzy/Code/EDU-1S-C-project-myshell -> set a=3
/Users/xuzy/Code/EDU-1S-C-project-myshell -> setenv b=4
/Users/xuzy/Code/EDU-1S-C-project-myshell -> echo $a+$b
3+4
/Users/xuzy/Code/EDU-1S-C-project-myshell -> ./mysh
--- another mysh
/Users/xuzy/Code/EDU-1S-C-project-myshell -> echo $d
Error: Variable 'd' not set.
/Users/xuzy/Code/EDU-1S-C-project-myshell -> echo $b
4
/Users/xuzy/Code/EDU-1S-C-project-myshell -> exit
--- fin another mysh
/Users/xuzy/Code/EDU-1S-C-project-myshell -> echo $b
4
/Users/xuzy/Code/EDU-1S-C-project-myshell -> unsetenv $b
/Users/xuzy/Code/EDU-1S-C-project-myshell -> echo $b
Error: Variable 'b' not set.
/Users/xuzy/Code/EDU-1S-C-project-myshell -> ls && myls 
1S-C-project-myshell-readme.md	output
Makefile			mysh
bin				src
docs				t.txt
include				test.txt
obj
drwxr-xr-x 9 	xuzy 	staff 	288 	Dec 27 22:51	obj
drwxr-xr-x 3 	xuzy 	staff 	96 	Dec 27 22:51	bin
-rw-r--r-- 1 	xuzy 	staff 	740 	Nov 19 12:43	Makefile
drwxr-xr-x 8 	xuzy 	staff 	256 	Dec 27 22:48	include
drwxr-xr-x 5 	xuzy 	staff 	160 	Dec 01 18:18	output
-rw-r--r-- 1 	xuzy 	staff 	10133 	Dec 24 20:30	1S-C-project-myshell-readme.md
drwxr-xr-x 5 	xuzy 	staff 	160 	Dec 26 10:19	docs
-rwxr-xr-x 1 	xuzy 	staff 	33 	Nov 25 08:47	mysh
-rw-r--r-- 1 	xuzy 	staff 	61 	Dec 27 22:52	t.txt
-rw-r--r-- 1 	xuzy 	staff 	149 	Dec 26 10:31	test.txt
drwxr-xr-x 9 	xuzy 	staff 	288 	Dec 27 22:51	src
/Users/xuzy/Code/EDU-1S-C-project-myshell -> ls || myls 
1S-C-project-myshell-readme.md	output
Makefile			mysh
bin				src
docs				t.txt
include				test.txt
obj
/Users/xuzy/Code/EDU-1S-C-project-myshell -> nl < ./mysh
     1	#!/bin/bash
     2	clear
     3	make
     4	./bin/exec
/Users/xuzy/Code/EDU-1S-C-project-myshell -> nl < mysh > t.txt 
/Users/xuzy/Code/EDU-1S-C-project-myshell -> cat t.txt
     1	#!/bin/bash
     2	clear
     3	make
     4	./bin/exec
/Users/xuzy/Code/EDU-1S-C-project-myshell -> ^C
/Users/xuzy/Code/EDU-1S-C-project-myshell -> ^C
/Users/xuzy/Code/EDU-1S-C-project-myshell -> exit
➜  EDU-1S-C-project-myshell git:(2312) ✗ 
```
