## **C-Shell**
### **Assignment 3**
### **Vyom Goyal | 2021101099**
---
  ### Requirements
  - GCC compiler
  - Linux System
  
  ### Instructions to execute
  Run `make all` command followed by `./shell`

  ### Files and Functions
    1) main.c - This contains the main function for the code and has certain functionalities like tokenising the command and sending them to further functions.

    2) com_selector.c - This gets the command from main and then passes to various further functions depending on it's type.

    3) ls.c - This is used to execute ls command wherein we display contents of a directory

    4) cd.c - This is used to navigate to different directories

    5) pwd.c - Used to display path of current directory

    6) echo.c - Used to print certain text on terminal

    7) job.c - Used to execute files on foreground or background.

    8) discover.c - Used to explore all paths inside a folder and if a file is given it can be searched for.

    9) pinfo.c - Used to display details of a certain process

    10) prompt.c - Used to display the prompt after every command execution

    11) history.c - Used to display the last 10 commands(if present or else all present)

    12) redir.c - Used for pipelining and input output redirection.

    13) tabfunc.c - Used to handle the case of autocompletion

    14) user_define.c - Used to define user commands like sig, fg, bg, jobs.

  ### Assumptions

    1) Certain assumptions have been made reagrding the size of various strings allocated for the paths and command lengths 

    2) History is stored in a hidden .history file which must not be changed else it can cause errors in history command 

    3) The history command displays even the invalid commands

    4) The terminal shows terminated normally or abnormally only after prompt is given the input or the foreground process going on is terminated.

    5) The OS used to run the shell has /proc file system present as it is used in pinfo command.
