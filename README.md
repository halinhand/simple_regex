# simple_regex
simple regex engine in C.
Extended based on the code from Rob Pike (http://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html)

Supported
=========

c    matches any literal character c
.    matches any single character
[]   matches any one character listed between the brackets
[^]  matches any one character not listed between the brackets
?    matches any character one time if it exists
*    matches zero or more occurrences of the previous character
+    matches one or more occurrences of the previous character
^    matches the beginning of the input string
$    matches the end of the input string'
\\   escape character to interpret metasymbol as a literal
