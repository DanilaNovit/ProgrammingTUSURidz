//  Для хранения данных о планшетных сканерах описать структуру вида:
//  struct scan_info
//  {
//    char model[25]; // наименование модели
//    int price;  // цена double
//    x_size; // горизонтальный размер области сканирования
//    double y_size; // вертикальный размер области сканирования
//    int optr;  // оптическое разрешение
//    int grey; // число градаций серого
//   };

// Написать функцию, которая динамически выделяет память
// под массив структур (не меньше шести элементов),
// заполняет его данными в режиме диалога и за писывает массив в бинарный файл.

// Структура файла: в первых двух байтах размещается значение
// типа int, определяющее количество сделанных в файл записей;
// далее без пропусков размещаются записи о сканерах. 

// Написать функцию, которая запрашивает данные о сканере в режиме
// диалога и замещает записи в бинарном файле по заданному номеру.
// Обязательный параметр — номер замещаемой записи. Функция должна
// возвращать нулевое значение, если запись прошла успешно, и -1 в противном случае.

// Привести пример программы, создающей файл с данными о сканерах
// (данные вводятся с клавиатуры) из не менее восьми записей и осуществляющий
// вставку новых данных о сканере. Все необходимые данные для функций должны
// передаваться им в качестве параметров. Использование глобальных переменных
// в функциях не допускается. 

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
  if (!file)
  {
    return -1;
  }

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

// Меняет информацию о записи в файле, номер которой получает как record_number
int replace_record(int record_number, FILE* file)
{
  if (!file) return -1;

  fseek(file, 0, SEEK_SET);
  int scan_list_length = 0;

  fread(&scan_list_length, sizeof(scan_list_length), 1, file);

  if (record_number > scan_list_length) return -1;
  
  scan_info scan_list[scan_list_length];
  fread(&scan_list, sizeof(scan_info), scan_list_length, file);

  read_scan_info(&scan_list[record_number - 1]);

  fseek(file, 0, SEEK_SET);

  fwrite(&scan_list_length, sizeof(scan_list_length), 1, file);
  fwrite(scan_list, sizeof(scan_info), scan_list_length, file);

  fseek(file, 0, SEEK_SET);
  return 0;
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
  printf("+-------------------+\n");
  create_record(scan_list, scan_info_length, file);
  print_records(file);
  printf("+-------------------+\n");

  // Получаем номер записи, которую нужно обновить
  printf("Number of modified record: ");
  int number_of_modified_record = atoi(fgets(buf, 16, stdin));
  printf("\n");

  // Если обновим запись успешно - выводим данные из файла в консоль
  // В проьтвном случаи сообщаем, что что-то пошло не так
  if (replace_record(number_of_modified_record, file) == 0)
  {
    printf("+-------------------+\n");
    print_records(file);
  } else
  {
    fprintf(stderr, "This entry is missing");
  }

  fclose(file);

  return 0;
}
