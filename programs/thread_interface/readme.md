# c skeleton

taken shamelessly from [here](http://c.learncodethehardway.org/book/ex28.html).

## Description
    In this assignment, your task is to implement a simple  user-level thread
    library in C. The initial implementation will be done individually and is
    simplified to afford you the time to gain/regain familiarity with the C
    programming environment and the challenges of writing and debugging low-level
    code. In later assignments, you will work in groups of increasing size to
    enhance the implementation with additional functionality, and to conduct
    performance measurement and optimization of a provided application.

    Your library must implement the C interface in the attached file unmthread.h.
    This file, which you may not modify, includes the file unmthread_priv.h, in
    which you should define any additional data structures necessary for the
    compiling and linking against your library. Running "make all" in your
    assignment should build the library libunmthread.a, which test programs may
    link against to use the interface supplied by your library.

    For this assignment, you may make the following assumptions:

    Only a single processor/kernel-thread will be running in the library at a time.
    Threads are non-preemptive, and are only switched when unmthr_yield() is called
    or the currently running thread exits.  Threads are scheduled round-robin,
    scheduler manipulation routines will be defined later in a different include
    file, and all scheduling information arguments should be NULL System-level I/O
    routines such as read, write, accept, open, and connect may block indefinitely
    All user-level threads share signal contexts and thread-local storage for that
    kernel thread All threads will be created with 8KB stacks, and you do not have
    to detect or respond to stack overflow Note that your library should implement
    all of the routines in the described interface files, even though some of them
    (both those explicitly listed above and others) will be no-ops.

    You should use the various Linux ucontext_t routines such as makecontext and
    swapcontext as described in the Linux man pages to implement your library. 

    As described in the syllabus, submit your assignment as a single gzip-ed
    tarfile containing in a directory names with your UNM CS user name, a Makefile
    that builds your library without warnings while using the gcc '-Wall' flag, and
    a README file that provides any additional information that the instructor
    should know prior to grading your assignment. Grading will be 85% based on
    correctness of implementation and 15% based on good programming style,
    including consistent indentation, appropriate commenting, understandable code,
    and division of code into reasonably-sized functions and compilation modules. 

    Finally, as mentioned above, this is an individual assignment, and all work you
    submit for it should be your own original work. You may use only the following
    code from elsewhere:

    Simple data structure definitions that have been explicitly approved by Prof.
    Bridges and are cited in your README file Public test cases posted by students
    to the class forums. You must cite any test cases you used that were written by
    others in your README file.

