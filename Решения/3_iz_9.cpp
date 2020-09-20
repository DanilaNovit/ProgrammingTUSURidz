// Дан файл, содержащий сведения о сотрудниках в виде:
// {Фамилия; И.О.; НомерТелефона}. Найти телефон сотрудника
// по значениям фамилии и инициалов. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Каждый контакт будем записывать в файл как структуру person
typedef struct
{
  char last_name[16];
  char initials[16];
  char phone_number[16];
} person;

// Функ-я заполняет файл с контактами на основе пользовательского ввода
void fill_in_the_phone_book(FILE* phone_book_file)
{
  if (phone_book_file == NULL)
  {
    fprintf(stderr, "The file does not exist");
    return;
  }

  char buf[20];

  printf("Enter the number of contacts: ");
  size_t number_of_contacts = atoi(fgets(buf, 8, stdin));

  printf("\n");

  person contact;

  for (size_t i = 0; i < number_of_contacts; ++i)
  {
    printf("Contact data %ld\n", i + 1);

    printf(" Last Name: ");
    strcpy(contact.last_name, fgets(buf, 16, stdin));

    printf(" Iinitials: ");
    strcpy(contact.initials, fgets(buf, 16, stdin));

    printf(" Phone number: ");
    fgets(contact.phone_number, 16, stdin);

    fwrite(&contact, sizeof(person), 1, phone_book_file);

    printf("\n");
  }

  fseek(phone_book_file, 0, SEEK_SET);
}

// Функ-я ищет номер телефона на основе введенных фамилии и инициалов
void find_phone_number(FILE* phone_book_file)
{
  if (phone_book_file == NULL)
  {
    fprintf(stderr, "The file does not exist");
    return;
  }

  person сontact_sought;
  char buf[16];

  printf("Who needs to be found?\n");

  printf(" Last Name: ");
  strcpy(сontact_sought.last_name, fgets(buf, 16, stdin));

  printf(" Iinitials: ");
  strcpy(сontact_sought.initials, fgets(buf, 16, stdin));

  person сontact_read;

  while (feof(phone_book_file) != EOF)
  {
    fread(&сontact_read, sizeof(person), 1, phone_book_file);

    if (!strcmp(сontact_read.last_name, сontact_sought.last_name) &&
        !strcmp(сontact_read.initials, сontact_sought.initials))
      {
        printf("\nPhone number: %s", сontact_read.phone_number);
        return;
      }
  }

  printf("Contact not found");

  fseek(phone_book_file, 0, SEEK_SET);
}

int main()
{
// Открываем/Создаем файл
  char phone_book_file_name[] = "phone_book.bin";
  FILE* phone_book_file = fopen(phone_book_file_name, "w+b");
  
// Заполняем файл
  fill_in_the_phone_book(phone_book_file);

// Ищем номер контакта
  find_phone_number(phone_book_file);

// Закрываем файл
  fclose(phone_book_file);
}
