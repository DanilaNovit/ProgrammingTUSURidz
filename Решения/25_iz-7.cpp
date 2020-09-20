// Багаж пассажира характеризуется количеством вещей и общим весом.
// Создать таблицу, содержащую информацию о багаже нескольких пассажиров.
// Найти багаж, число вещей в котором не меньше, чем в любом другом, а вес,
// не больше, чем в любом другом с тем же числом вещей. 

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int number_of_items;
  int weight;
} baggage;

// Заполняет массив baggages данными из консоли
void read_baggages(baggage* baggages, int baggages_length)
{
  char buf[9];

  for (int i = 0; i < baggages_length; ++i)
  {
    printf("Baggage %d\n", i + 1);

    printf(" Number of items: ");
    baggages[i].number_of_items = atoi(fgets(buf, 8, stdin));

    printf(" Weight: ");
    baggages[i].weight = atoi(fgets(buf, 8, stdin));

    printf("\n");
  }
}

// Выводит в консоль данные структуры baggage
void print_baggage(baggage baggage)
{
  printf(" Number of items: %d\n", baggage.number_of_items);
  printf(" Weight: %d\n", baggage.weight);

  printf("\n");
}

// Ишет и выводит все структуры, которые подходят по условию задания
void find_and_print_baggages(baggage* baggages, int baggages_length)
{
  int max_number_of_items = baggages[0].number_of_items;
  int min_weight = baggages[0].weight;

  for (int i = 1; i < baggages_length; ++i)
  {
    if (baggages[i].number_of_items > max_number_of_items)
    {
      max_number_of_items = baggages[i].number_of_items;
      min_weight = baggages[i].weight;
    } else if (baggages[i].number_of_items == max_number_of_items &&
               min_weight > baggages[i].weight)
    {
      min_weight = baggages[i].weight;
    }
  }

  for (int i = 0; i < baggages_length; ++i)
  {
    if (baggages[i].number_of_items == max_number_of_items &&
        baggages[i].weight == min_weight)
    {
      printf("Baggage %d\n", i + 1);
      print_baggage(baggages[i]);
    }
  }
}

int main()
{
  char buf[9];

  printf("Number of baggages: ");
  int baggages_length = atoi(fgets(buf, 8, stdin));
  printf("\n");
  
  baggage* baggages = (baggage*) malloc(sizeof(baggage) * baggages_length);

  read_baggages(baggages, baggages_length);

  printf("----------\n\n");

  find_and_print_baggages(baggages, baggages_length);

  free(baggages);
}
