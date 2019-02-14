# postfix-lisp-interpreter
Interpreter for a post-fix style basic Lisp

> ((2 3 *) ((5 7 -) 1 +) *)


#### Installation
Use make to compile with g++ on linux machine.
```
make
```
#### Usage
Call postlisp from command line with -i flag for repl mode. -h flag for help.
```
./postlisp -i
```

##### Commands
Basic arithmetic commands can be used on numbers. Can take any number of inputs two or greater.
```
((2 3 4 8 9 +) ((2 4 /) 1 -) *)
=> -26
```

eq? - Checks if given inputs are equal. Can take any number of inputs two or greater.
```
((2 2 +) (2 8 /) (1 4 *) eq?)
=> True
```

atom? - Checks if the given input is an atom. Kindof clunky with my current typing, as numbers aren't considered atoms. Can take any number of inputs two or greater.
```
("Pie" atom?)
=> True
```

quote - Prints to screen a given input. This works a bit different than normal lisp, and is not analogous to a single quote (not implemented). Can take any number of inputs two or greater.
```
("Pie" (2 2 +) quote)
=> 2 2 + "Pie" 
```

cons - Takes two parts and returns a list of them concatenated.
```
(Pie Apple cons)
=> Apple Pie
```

car - Takes a list and returns the first item.
```
((Pie Apple cons) car)
=> Apple
```

cdr - Takes a list and returns a list with the first item removed.
```
(((Pie Apple cons) Big cons) cdr)
=> Apple Pie
```

define - Symbols can be defined to have a value, or a function (see lambda), and be called later.
```
(4 a *) (2 a define)
=> 8
```

lambda - Functions can be defined in place, or saved using define. [Currently defining lambdas works, but calling them does not].
```
(((x x *) (x) lambda) square define)
```

#### Credits
The Lean Mean C++ Option Parser - optionparser.h