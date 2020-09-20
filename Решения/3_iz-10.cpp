//  Для хранения данных о планшетных сканерах описать структуру вида: 
// struct scan_info
// {
//   char model[25]; // наименование модели
//   int price;  // цена double
//   x_size; // горизонтальный размер области сканирования
//   double y_size; // вертикальный размер области сканирования 
//   int optr;  // оптическое разрешение
//   int grey;  // число градаций серого 
// };
// Написать функцию, которая записывает в бинарный файл данные о сканере из приведенной структуры. Структура файла: в первых четырёх байтах размещается значение типа long, определяющее количество сделанных в файл записей; далее без пропусков размещаются записи о сканерах.
// Написать функцию, которая сортирует записи в описанном выше бинарном файле по наименованию модели сканера.
// Привести пример программы, создающей файл с данными о сканерах (данные вводятся с клавиатуры) из не менее восьми записей и осуществляющей его сортировку.
// Все необходимые данные для функций должны передаваться им в качестве параметров. Использование глобальных переменных в функциях не допускается. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct scan_info_s
{
  char name[25];
  int price;
  double x_size, y_size;
  int optr;
  int grey;
} scan_info;

int create_record(scan_info* scan_list, long scan_list_length, FILE* file)
{
  if (!file)
  {
    return -1;
  }

  fwrite(&scan_list_length, sizeof(long), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);
  return 0;
}

void read_scan_info(scan_info* scan)
{
  char buf[25];

  printf("Name: ");
  strcpy(scan->name, fgets(buf, 25, stdin));

  printf("Price: ");
  scan->price = atoi(fgets(buf, 8, stdin));

  printf("X Size: ");
  scan->x_size = atof(fgets(buf, 8, stdin));

  printf("Y Size: ");
  scan->y_size = atof(fgets(buf, 8, stdin));

  printf("Optical Resolution: ");
  scan->optr = atoi(fgets(buf, 8, stdin));

  printf("Number gradations of gray: ");
  scan->grey = atoi(fgets(buf, 8, stdin));

  printf("\n");
}

int scan_cmp_name(const void * a, const void * b)
{
  return strcmp(((scan_info*)a)->name, ((scan_info*)b)->name);
}

int sort_file_scan_name(FILE* file)
{
  if (!file)
  {
    return -1;
  }

  long scan_list_length = 0;

  fread(&scan_list_length, sizeof(long), 1, file);
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  qsort(scan_list, scan_list_length, sizeof(scan_info), scan_cmp_name);

  fseek(file, 0, SEEK_SET);
	
  fwrite(&scan_list_length, sizeof(long), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);
  return 0;
}

int print_records(FILE* file)
{
  if (!file)
  {
    return -1;
  }

  fseek(file, 0, SEEK_SET);
  long scan_list_length = 0;

  fread(&scan_list_length, sizeof(long), 1, file);
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  printf("Number of records: %ld\n\n", scan_list_length);

  for (size_t i = 0; i < scan_list_length; ++i)
  {
    printf("Name: %s", scan_list[i].name);
    printf("Price: %d\n", scan_list[i].price);
    printf("X Size: %.2f\n", scan_list[i].x_size);
    printf("Y Size: %.2f\n", scan_list[i].y_size);
    printf("Optical Resolution: %d\n", scan_list[i].optr);
    printf("Number gradations of gray: %d\n\n", scan_list[i].grey);
  }

  fseek(file, 0, SEEK_SET);
  return 0;
}

int main()
{
  char filename[] = "data.bin";
  FILE* file = fopen(filename, "w+b");

  if (!file)
  {
    return -1;
  }
  
  char buf[16];
  long scan_info_length = 0;

  printf("Number of records: ");
  scan_info_length = atoi(fgets(buf, 16, stdin));
  printf("\n");

  scan_info* scan_list = (scan_info*) malloc(sizeof(scan_info) * scan_info_length);

  for (size_t i = 0; i < scan_info_length; ++i)
  {
  	read_scan_info(&scan_list[i]);
  }

  create_record(scan_list, scan_info_length, file);
  sort_file_scan_name(file);
  print_records(file);

  fclose(file);
}
