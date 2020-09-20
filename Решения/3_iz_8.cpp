// Массив записей с именем ZNAK, содержит сведения о знакомых:
// {Фамилия Имя; Знак Зодиака; День рождения (массив из трёх чисел)}.
// Написать программу, обеспечивающую ввод с клавиатуры данных в
// массив ZNAK и вывод на экран информации о человеке, чья фамилия
// введена с клавиатуры. Если такого нет, выдать на дисплей
// соответствующее сообщение.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>

typedef struct person_s
{
  char last_name[8];
  char first_name[8];
  char zodiac_sign[8];
  size_t date_of_birth[3];
} person;

person* read_person(size_t length_person_list)
{
  person* person_list = (person*) malloc(sizeof(person) * length_person_list);
  char buf[12];

  for (size_t i = 0; i < length_person_list; ++i)
  {
    printf("Person %ld\n", i + 1);

    printf(" First name: ");
    strcpy(person_list[i].first_name, fgets(buf, 8, stdin));

    printf(" Last name: ");
    strcpy(person_list[i].last_name, fgets(buf, 8, stdin));

    printf(" Zodiac sign: ");
    strcpy(person_list[i].zodiac_sign, fgets(buf, 8, stdin));

    printf(" Date of birth (D.M.Y): ");
    fgets(buf, 12, stdin);

    char* start_of_number = buf;

    for (size_t j = 0; j < 2; ++j)
    {
      char* dot = strchr(start_of_number, '.');
      *dot = 0;

      person_list[i].date_of_birth[j] = atoi(start_of_number);
      start_of_number = dot + 1;
    }

    person_list[i].date_of_birth[2] = atoi(start_of_number);

    printf("\n");
  }

  return person_list;
}

void print_person(char* last_name_find, person* person_list,
                                size_t person_list_lenght)
{
  for (size_t i = 0; i < person_list_lenght; ++i)
  {
    if (!strcmp(person_list[i].last_name, last_name_find))
    {
      printf("First name: %s", person_list[i].first_name);
      printf("Last name: %s", person_list[i].last_name);
      printf("Zodiac sign: %s", person_list[i].zodiac_sign);

      printf("Date of birth: %ld", person_list[i].date_of_birth[0]);

      for (size_t j = 1; j < 3; ++j)
      {
        printf(".%ld", person_list[i].date_of_birth[i]);
      }

      printf("\n");

      return;
    }
  }

  printf("Person is not found\n");
}

int main()
{
  char buf[8];
  
  printf("Person count: ");
  size_t ZNAK_length = atoi(fgets(buf, 8, stdin));
  printf("\n");

  person* ZNAK = read_person(ZNAK_length);

  printf("Last name of the person sought: ");

  char last_name_find[8];
  strcpy(last_name_find, fgets(buf, 8, stdin));

  printf("\n");

  print_person(last_name_find, ZNAK, ZNAK_length);
}
