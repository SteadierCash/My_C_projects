#include <stdio.h>
#include <string.h>

int copy_file_to_temp(FILE* user_file, FILE* temp_file, char* file_name);
int encrypt_data(FILE* user_file, FILE* temp_file, char* file_name);
int decrypt_data(FILE* user_file, FILE* temp_file, char* file_name);

int main(int argc, char* argv[])
{
   if (argc != 2 || (strcmp(argv[1], "encrypt") != 0 && strcmp(argv[1], "decrypt") != 0))
   {
      printf("Function usage: ./encrypt [encrypt] or [decrypt]\n");
      return 1;
   }

   FILE *user_file;
   FILE *temp_file;
   char line[100];
   char file_name[100];

   if (copy_file_to_temp(user_file, temp_file, file_name) == 1)
   {
      return 1;
   };

   if (strcmp(argv[1], "encrypt") == 0)
   { 
      if (encrypt_data(user_file, temp_file, file_name) == 1)
      {
         return 1;
      };

      printf("Data encrypted successfully.\n");
   }
   else
   {
      if (decrypt_data(user_file, temp_file, file_name) == 1)
      {
         return 1;
      };

      printf("Data encrypted successfully.\n");
   }
   return 0;
}


int copy_file_to_temp(FILE* user_file, FILE* temp_file, char* file_name)
{
   char line[100];
   temp_file = fopen("temp.txt", "w");
   
   if (temp_file == NULL)
   {
      printf("Encryption is not possible\n");
      return 1;
   }

   printf("What txt file do you want to encrypt?: ");
   scanf("%s", file_name);

   // opening user file which will be encrypted
   user_file = fopen(file_name, "r");

   if (user_file == NULL)
   {
      printf("There is no such file\n");
      return 1;
   }

   while (fgets(line, sizeof(line), user_file) != NULL)
   {
      fputs(line, temp_file);
   }

   //closing user file 
   fclose(user_file);

   //closing temp file
   fclose(temp_file);

   return 0;
}

int encrypt_data(FILE* user_file, FILE* temp_file, char* file_name)
{
   char line[100];
   char ch;

   temp_file = fopen("temp.txt", "r");

   if (temp_file == NULL)
   {  
      printf("Encryption is not possible\n");
      return 1;
   }

   user_file = fopen(file_name, "w");
   if (user_file == NULL)
   {  
      printf("Encryption is not possible\n");
      return 1;
   }

   ch = fgetc(temp_file);
    while(ch != EOF)
    {
        ch = ch+100;
        fputc(ch, user_file);
        ch = fgetc(temp_file);
    }

   
   fclose(user_file);
   fclose(temp_file);

   remove("temp.txt");

   return 0;
}

int decrypt_data(FILE* user_file, FILE* temp_file, char* file_name)
{
   char line[100];
   char ch;

   temp_file = fopen("temp.txt", "r");

   if (temp_file == NULL)
   {  
      printf("Encryption is not possible\n");
      return 1;
   }

   user_file = fopen(file_name, "w");
   if (user_file == NULL)
   {  
      printf("Encryption is not possible\n");
      return 1;
   }

   ch = fgetc(temp_file);
    while(ch != EOF)
    {
        ch = ch-100;
        fputc(ch, user_file);
        ch = fgetc(temp_file);
    }

   
   fclose(user_file);
   fclose(temp_file);

   remove("temp.txt");
   return 0;
}

