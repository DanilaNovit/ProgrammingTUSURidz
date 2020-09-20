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

// Возвращает размер переданного ей файла
int get_file_size(FILE* file)
{
  int file_size = 0;

  while (fgetc(file) != EOF)
  {
    ++file_size;
  }

  fseek(file, 0, SEEK_SET);

  return file_size;
}

// Выделяет текущее значение в строке и возвращает указатель на следующее
char* get_next_value(char* begin_value)
{
  char* end_value = strchr(begin_value, ';');

  if (end_value == NULL)
  {
    return NULL;
  }

  *end_value = 0;

  return end_value + 1;
}

// Перносит данные из data.txt в data.bin
int create_records(FILE* file)
{
  if (!file)
  {
    return -1;
  }

  char filename[] = "data.txt";
  FILE* source_file = fopen(filename, "r");

  if (!source_file)
  {
    return -1;
  }

  int source_file_size = get_file_size(source_file);
  char* source_text = (char*) malloc(source_file_size + 1);

  fseek(file, 0, SEEK_SET);
  strcpy(source_text, fgets(source_text, source_file_size + 1, source_file));

  char* end_value = get_next_value(source_text);
  char* begin_value = end_value;

  int scan_list_length = atoi(source_text);
  scan_info scan_list[scan_list_length];

  for (int i = 0; i < scan_list_length; ++i)
  {
    // name
    end_value = get_next_value(begin_value);
    strcpy(scan_list[i].name, begin_value);
    begin_value = end_value;

    // price
    end_value = get_next_value(begin_value);
    scan_list[i].price = atoi(begin_value);
    begin_value = end_value;

    // x_size
    end_value = get_next_value(begin_value);
    scan_list[i].x_size = atof(begin_value);
    begin_value = end_value;

    // y_size
    end_value = get_next_value(begin_value);
    scan_list[i].y_size = atof(begin_value);
    begin_value = end_value;

    // optr
    end_value = get_next_value(begin_value);
    scan_list[i].optr = atoi(begin_value);
    begin_value = end_value;

    // grey
    end_value = get_next_value(begin_value);
    scan_list[i].grey = atoi(begin_value);
    begin_value = end_value;
  }

  fseek(file, 0, SEEK_SET);

  fwrite(&scan_list_length, sizeof(int), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);
  
  return 0;
}

// Добавляет запись в начало файла
int add_record_to_file(scan_info* scan, FILE* file)
{
  if (!file)
  {
    return -1;
  }

  fseek(file, 0, SEEK_SET);

  int scan_list_length = 0;
  fread(&scan_list_length, sizeof(int), 1, file);
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  ++scan_list_length;
  fseek(file, 0, SEEK_SET);

  fwrite(&scan_list_length, sizeof(int), 1, file);
  fwrite(scan, sizeof(scan_info), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length - 1, file);

  fseek(file, 0, SEEK_SET);

  return 0;
}

// Создаёт запись и добавляет её в начало файла
int add_record(FILE* file)
{
  char buf[25];
  scan_info scan;

  printf("Name: ");
  strcpy(scan.name, fgets(buf, 25, stdin));

  printf("Price: ");
  scan.price = atoi(fgets(buf, 8, stdin));

  printf("X Size: ");
  scan.x_size = atof(fgets(buf, 8, stdin));

  printf("Y Size: ");
  scan.y_size = atof(fgets(buf, 8, stdin));

  printf("Optical Resolution: ");
  scan.optr = atoi(fgets(buf, 8, stdin));

  printf("Number gradations of gray: ");
  scan.grey = atoi(fgets(buf, 8, stdin));

  printf("\n");

  return add_record_to_file(&scan, file);
}

// Выводит все данные из data.bin
int print_records(FILE* file)
{
  if (!file)
  {
    return -1;
  }

  fseek(file, 0, SEEK_SET);

  int scan_list_length = 0;
  fread(&scan_list_length, sizeof(int), 1, file);
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  printf("Number of records: %d\n\n", scan_list_length);

  for (size_t i = 0; i < scan_list_length; ++i)
  {
    printf("Name: %s\n", scan_list[i].name);
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

  // Выводит данные, которые перенёс из data.txt
  create_records(file);
  print_records(file);

  // Создаёт новую запись, добавляет её в начало data.bin и выводит data.bin
  add_record(file);
  print_records(file);

  fclose(file);
}
