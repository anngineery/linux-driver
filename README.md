# Summary
A loadable linux module that creates a file in the proc file system upon loading and allows read and write operation on the file

# How to run
1. In the project root directory and build the module: `make`
2. Once compiled, load the module: `sudo insmod ldd.ko`
3. Check if the module has been loaded (`lsmod`) and the file has been created in /proc (`ls /proc`)
4. Write something to the file: `echo hello > /proc/<file_name>`
5. Read the file and see if what you wrote in the previous step comes back as a result: `cat /proc/<file_name>`

# Limitation
Currently, each new write overrides the previous written text. For example:
```
$ echo first > /proc/my_file_node
$ cat /proc/my_file_node
first
$ echo second > /proc/my_file_node
$ cat /proc/my_file_node
second
```
The desired behaviour would be the new text keeps getting appended as long as we do not run out of the buffer. For example, if the file is read after the second write, the desired output would be
```
first second
```

# Areas of Improvments 
- [ ] Extend the functionality as discussed in Limitation section
- [ ] Wrtie a small bash script to automate build, log cleanup and insertion of module
- [ ] Write a python test script
- [ ] Improve the Makefile to optionally choose to compile in debug mode
