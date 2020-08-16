#include "csapp.h"
void my_reverse(char *s);
int string_length(char *pointer);
int main(void){
	char * str = "aabbccddeeff1122";
	printf("%s\n", str);
	my_reverse(str);
	printf("%s\n", str);
	
}

void my_reverse(char *s)
{
   int length, c;
   char *begin, *end, temp;
 
   length = strlen(s);
   begin  = s;
   end    = s;
 
   for (c = 0; c < length - 1; c++)
      end++;
 
   for (c = 0; c < length/2; c++)
   {        
      temp   = *end;
      *end   = *begin;
      *begin = temp;
 
      begin++;
      end--;
   }
}