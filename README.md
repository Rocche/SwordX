# SwordX
Authors: Liscio Alessandro, Rocchetti Giacomo

This is a project for the Operating Systems exam at UNICAM (University of Camerino, Italy).

SwordX is a command line tool that will count occurencies of words contained in files specified by the user.

## Installation
After cloning SwordX folder, go into it and, from command line:
```shell
  sudo make
  sudo make install
  ```
  Now you should be able to run swordx command from anywhere in your filesystem. Make sure you run it as administrator (it avoids permissions problems).
  
## Uninstall
To uninstall the program, go to SwordX directory and type:
```shell
  sudo make uninstall
```
  
## Examples
  ```shell
  swordx test.txt
  ```
Count occurrencies in test.txt and print the result in swordx.out.
  
  ```shell
  swordx -m 7 -o output.txt -r TestDirectory/
  ```
Count occurrencies of words with at least 7 characters in all files contained in TestDirectory and its subdirectories, then prints the result in output.txt.

  ```shell
  swordx -a -i ignoredwords.txt -r TestDirectory/ -l logfile.txt
  ```
  Count occurrencies of words, which are not contained in ignoredwords.txt, in all files contained in TestDirectory and its subdirectories, prints the result in swordx.out and prints stats in logfile.txt (file name, counted words, ignored words and process time).
