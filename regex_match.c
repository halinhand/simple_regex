/* 
 * supported metasymbol
 *
============ 
c    matches any literal character c
.    matches any single character
[]   matches any one character listed between the brackets
[^]  matches any one character not listed between the brackets
?    matches any character one time if it exists
*    matches zero or more occurrences of the previous character
+    matches one or more occurrences of the previous character
^    matches the beginning of the input string
$    matches the end of the input string
===========
*/


// TODO:
//  1. handling metasymbols after group symbol[]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char* match(const char* regex, const char* text);



const char* matchhere(const char *regex, const char* text, const char* textEnd, bool escaped)
{
  if (!escaped) {
    switch (*regex)
    {
    case 0: 
      return text;
    case '\\': 
      return matchhere(regex + 1, text, textEnd, true);
    }
  }


  // check for group []
  if (*regex == '[' && !escaped) {
      regex++;

      bool negated = false;
      if (*regex == '^') { // NEGATE in GROUP
          regex++;
          negated = true;
      }

      bool matched = false;
      for (; *regex && *regex != ']'; regex++) {
          if (*regex == *text) {
              matched = true;
              // keep going until ']'
          }
      }
      if (matched != negated) {
          return matchhere(regex+1, ++text, textEnd, false);
      }

      return NULL;
  }

  switch (regex[1])
  {
  case '+': 
    while (*regex == *text || (*regex == '.' && !escaped && text != textEnd && *text != 0))
    {
      const char* matched = matchhere(regex + 2, ++text, textEnd, false);
      if (matched != NULL)
        return matched;
    }

    break; 

  case '*': 
    do {
      const char* matched = matchhere(regex + 2, text, textEnd, false);
      if (matched != NULL)
        return matched;
    } while (*regex == *text++ || (*regex == '.' && !escaped && text != textEnd && *text != 0));

    break; 

  case '?':
    {
      const char* matched = matchhere(regex + 2, text, textEnd, false);
      if (matched != NULL)
        return matched;
    }

    if (*regex == *text || (*regex == '.' && !escaped && *text != 0))
      return matchhere(regex + 2, text + 1, textEnd, false);

    break;

  case 0:
    if (*regex == '$' && !escaped)
    {
      if (text == textEnd || *text == 0)
        return text;
      break; 
    }

    if (text == textEnd || *text == 0)
      break;

    return (*regex == *text || (*regex == '.' && !escaped)) ? text + 1 : NULL;

  default: 
    if (text == textEnd || *text == 0)
      break;

    if (*regex == '$' && !escaped)
      break; 

    if (*regex == *text || (*regex == '.' && !escaped))
      return matchhere(regex + 1, text + 1,textEnd, false);

    break; 
  }

  return NULL;
}


const char* match(const char* regex, const char* text)
{
  if (!text || !regex)
    return NULL;

  if (*regex == '^')
    return matchhere(regex + 1, text, NULL, false) ? text : NULL;

  do
  {
    if (matchhere(regex, text, NULL, false))
      return text;
  } while (*text++ != 0);

  return NULL;
}


/* 
 * test driver: 
 *  usage: $0 test.csv
 */

#if 0
int main() {
    printf("%s\n", match("abc\\[d\\]ef","abc[d]efg") == NULL ? "FALSE" : "TRUE");
}
#else
int main(int argc, char **argv) {
    if (argc != 2) {
        printf("need csv file\n");
        exit(0);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == (FILE *)NULL) {
        perror(argv[1]);
        exit(0);
    }
    int i=1;
	int success=0;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *pattern = strtok(line, ","); // FIXME: consecutive comma parse issue
        char *text = strtok(NULL, ",");
		if (text == NULL) {
			printf("NULL at %d\n", i);
		}
        char *expected = strtok(NULL, "\n\r");  

	bool bExpected = strcmp(expected, "TRUE") == 0;
	bool matched = match(pattern, text) == NULL ? false : true;

        if (matched != bExpected) {
            printf("%d: '%s'\t~\t'%s'\t\t=>(expected:%s) %s\n", i, pattern, text, expected,
                matched ? "TRUE" : "FALSE");
        } else {
		success++;
        }
        // line counter
        i++;
    }
	printf("total: %d, success: %d\n", i-1, success);
}
#endif
