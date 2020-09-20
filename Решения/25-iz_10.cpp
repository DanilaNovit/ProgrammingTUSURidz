//  Для хранения данных о планшетных сканерах описать структуру вида:

//  struct scan_info
//  {
//    char model[25]; // наименование модели
//    int price;  // цена
//    double x_size; // горизонтальный размер области сканирования
//    double y_size; // вертикальный размер области сканирования
//    int optr;  // оптическое разрешение
//    int grey;  // число градаций серого
//    };
   
// Написать функцию, которая записывает данные о сканере из приведённой
// структуры в требуемую позицию в бинарном файле. Структура файла:
// в первых двух байтах размещается значение типа int, определяющее
// количество сделанных в файл записей; далее без пропусков размещаются записи о сканерах.

// Запись может осуществляться в любую позицию, причём, если между вводимой записью и последней (или началом файла) имеются пропуски, они заполняются нулями.
// Написать функцию, которая «уплотняет» описанный выше бинарный файл путем удаления из него записей, содержащих все нули. Привести пример программы, создающей файл с данными о сканерах (данные вводятся с клавиатуры) из не менее шести записей и осуществляющей его уплотнение.

// Все необходимые данные для функций должны передаваться им в качестве параметров. Использование глобальных переменных в функциях не допускается

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

// Записывает в data.bin массив сткуктур scan_info
int create_record(scan_info* scan_list, int scan_list_length, FILE* file)
{
  if (!file) return -1;

  fwrite(&scan_list_length, sizeof(scan_list_length), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);
  return 0;
}

// Заполняет структуру scan_info
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

// Выводит информацию из data.bin
int print_records(FILE* file)
{
  if (!file) return -1;

  fseek(file, 0, SEEK_SET);
  int scan_list_length = 0;

  fread(&scan_list_length, sizeof(scan_list_length), 1, file);
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  printf("Number of records: %d\n\n", scan_list_length);

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


// Добавляет запись в файл на место
int add_record(scan_info* new_scan, int new_place, FILE* file)
{
  if (!file) return -1;

  fseek(file, 0, SEEK_SET);
  int scan_list_length = 0;

  fread(&scan_list_length, sizeof(scan_list_length), 1, file);
  
  scan_info* scan_list = (scan_info*) malloc(sizeof(scan_info) * scan_list_length);

  for (int i = 0; i < scan_list_length; ++i)
  {
    fread(&scan_list[i], sizeof(scan_info), 1, file);
  }

  if (new_place > scan_list_length)
  {
    scan_list = (scan_info*) realloc(scan_list, sizeof(scan_info) * new_place);

    for (int i = scan_list_length; i < new_place; ++i)
    {
      scan_list[i] = {0};
    }

    scan_list_length = new_place;
  } else if (new_place == scan_list_length)
  {
    scan_list_length = new_place + 1;
    scan_list = (scan_info*) realloc(scan_list, sizeof(scan_info) * scan_list_length);

    scan_list[scan_list_length - 1] = scan_list[scan_list_length - 2];
  } else
  {
    ++scan_list_length;
    scan_list = (scan_info*) realloc(scan_list, sizeof(scan_info) * scan_list_length);

    for (int i = scan_list_length; i >= new_place; --i)
    {
      scan_list[i - 1] = scan_list[i - 2];
    }
  }

  scan_list[new_place - 1] = *new_scan;

  fseek(file, 0, SEEK_SET);

  fwrite(&scan_list_length, sizeof(scan_list_length), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);

  free(scan_list);

  return 0;
}


// Удаляет все пустые записи из файла
int compression_file(FILE* file)
{
  if (!file) return -1;

  fseek(file, 0, SEEK_SET);
  int scan_list_length = 0;

  fread(&scan_list_length, sizeof(scan_list_length), 1, file);
  
  scan_info* scan_list = (scan_info*) malloc(sizeof(scan_info) * scan_list_length);

  int iterator_place = 0;

  for (int i = 0; i < scan_list_length; ++i)
  {
    fread(&scan_list[iterator_place], sizeof(scan_info), 1, file);

    if (strcmp(scan_list[iterator_place].name, ""))
    {
      ++iterator_place;
    }
  }

  scan_list_length = iterator_place;
  scan_list = (scan_info*) realloc(scan_list, sizeof(scan_info) * scan_list_length);
  
  fseek(file, 0, SEEK_SET);

  fwrite(&scan_list_length, sizeof(scan_list_length), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);

  free(scan_list);

  return 0;
}

int main()
{
  // Создаём файл
  char filename[] = "data.bin";
  FILE* file = fopen(filename, "w+b");
  if (!file) return -1;
  
  // scan_info_length - длина массива структур
  char buf[16];
  int scan_info_length = 0;

  // Получаем кол-во сткуртур
  printf("Number of records: ");
  scan_info_length = atoi(fgets(buf, 16, stdin));
  printf("\n");

  // Создаём массив
  scan_info* scan_list = (scan_info*) malloc(sizeof(scan_info) * scan_info_length);

  // Заполняем массив
  for (size_t i = 0; i < scan_info_length; ++i)
  {
  	read_scan_info(&scan_list[i]);
  }

  // Записываем массив в файл и выводим все из файла в консоль
  create_record(scan_list, scan_info_length, file);

  // Получаем из консоли новое место для записи и саму запись
  printf("Place to add the record: ");
  int new_place = atoi(fgets(buf, 16, stdin));
  printf("\n");
  
  scan_info new_scan;
  read_scan_info(&new_scan);

  printf("\n   BEFORE COMPRESSION\n");

  // Добавляем запись в файл и выводим всю информацию из файла в консоль
  add_record(&new_scan, new_place, file);
  print_records(file);

  printf("\n   COMPRESSION\n");

  // Сжимаем записи в файле и выводим всю информацию из файла в консоль
  compression_file(file);
  print_records(file);

  free(scan_list);
  fclose(file);

  return 0;
}
