#include <string.h>
#include <stdio.h>

int main () {
   char str[255] = "15921.706055,102941.101562,45707.289062,105810.851562,35320.957031,72766.773438,11592.797852,49696.765625,110488.132812,78619.351562,5018.962402";
   const char s[2] = ",";
   char *token;
   int count = 0;
   
   /* get the first token */
   token = strtok(str, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      printf( " %s\n", token );
    
      token = strtok(NULL, s);
      count++;
   }
   printf("Found %d tokens", count);
   return(0);
}