# How to generate doc with json

> program errorMaker

## Generate error to json
run
```
./errorMaker create
```
then fill in all prompts

### Fill in file instead of prompts
To skip filling in the prompts one could fill the prompts in a
file and echo its content to the program

input.txt
```
MAINCODE
SUBCODE
DESC
ARG_1
...
ARG_N
DONE
NUM OF CODE SNIPPETS
SNIPPET_1_LINE_1
SNIPPET_1_...
SNIPPET_1_LINE_N
DONE
...
SNIPPET_N_LINE_1
SNIPPET_N_...
SNIPPET_N_LINE_N
DONE
NOTE_1
...
NOTE_2
DONE
FOOTER
```
then run
```
cat input.txt | ./errorMaker create
```


## Output to index.html
run
```
./errorMaker output
```

## Both
run
```
./errorMaker create output
```
this will run the commands above at after one another

## TODO
- [x] Add codeblocks



