// В таблице Т1 хранится информация об изделиях: {номер изделия;
// наименование}. В таблице Т2 хранится информация о том, какое изделие
// из каких изделий состоит: {номер изделия1; номер изделия, входящего в состав изделия1}.
// Написать функцию consist(состав
// изделия, наименование изделия), формирующую список изделий,
// входящих в состав изделия наименование_изделия.

#include <stdio.h>
#include <string.h>
#define SIZE 4

typedef struct
{
  int id;
  char name[16];
} product;

typedef struct
{
  int id;
  int id_parent;
} detail;

product product_list[SIZE];

void consist(detail* detail_list, int detail_list_size, const char* product_name)
{
  int product_id = 0;

  for (int i = 0; i < SIZE; ++i)
  {
    if (!strcmp(product_list[i].name, product_name))
    {
      product_id = product_list[i].id;
    }
  }

  int found = 0;

  for (int i = 0; i < SIZE; ++i)
  {
    if (product_id == detail_list[i].id_parent)
    {
      for (int j = 0; j < SIZE; ++j)
      {
        if (detail_list[i].id == product_list[j].id)
        {
          if (found == 0)
          {
            printf("%s состоит из: ", product_name);
            found = 1;
          }

          printf("%s ", product_list[j].name);
        }
      }
    }
  }

  if (!found)
  {
    printf("Детали не найдены");
  }

  printf("\n");
}

int main()
{
// Заполняем поля
  for (int i = 0; i < SIZE; ++i)
  {
    product_list[i].id = i;
  }
  strcpy(product_list[0].name, "toyota");
  strcpy(product_list[1].name, "kia");
  strcpy(product_list[2].name, "wheel");
  strcpy(product_list[3].name, "door");

  detail detail_list[SIZE];

  detail_list[0].id = 2;
  detail_list[0].id_parent = 0;
  detail_list[1].id = 2;
  detail_list[1].id_parent = 1;
  detail_list[2].id = 3;
  detail_list[2].id_parent = 0;
  detail_list[3].id = 3;
  detail_list[3].id_parent = 1;

// Вызываем функцию для каждой детали
  consist(detail_list, SIZE, "toyota");
  consist(detail_list, SIZE, "kia");
  consist(detail_list, SIZE, "wheel");
  consist(detail_list, SIZE, "door");
}
