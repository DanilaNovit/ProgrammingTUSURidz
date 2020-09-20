// Создать массив структур, содержащих сведения о людях: {код; имя; пол; рост}.
// Написать и протестировать функцию AvgHt ( группа ),
// определяющую средний рост мужчин в группе.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Структура
typedef struct person_s
{
  int code;
  int height;
  char name[64];
  char gender;
} person;

// Функция принимает массив структур и его размер. 
// Складывает рост всех мужчик и делим на их колличество
double avgHt(person* group, int groupSize)
{
  double average_height = 0;
  int male_count = 0;

  for (int i = 0; i < groupSize; ++i)
  {
    if (group[i].gender == 'm')
    {
      average_height += group[i].height;
      ++male_count;
    }
  }

  average_height /= male_count;

  return average_height;
}

int main()
{
  char buf[8];

  // Просим ввести строку и присваиваем её в buf
  // fgets(куда записываем строку, её макс. размер, откуда читаем)
  // В данном случае читаем из потога ввода (stdin)
  printf("Размер группы = ");
  fgets(buf, 8, stdin);

  // С помощью atoi() преобразуем строку в число и присваиваем в groupSize
  int groupSize = atoi(buf);
  person* group = (person*)calloc(groupSize, sizeof(person)); // создаем массив сткуктур

  for (int i = 0; i < groupSize; ++i)
  {
    group[i].code = i + 1;  // Как я понял, код - это id. Поэтому присвоил i

    // Просим ввести имя через функцию fgets
    printf("Имя %d человека = ", group[i].code);
    fgets(group[i].name, 8, stdin);

    // Вводим рост
    printf("Рост %d человека = ", group[i].code);
    fgets(buf, 8, stdin);
    group[i].height = atoi(buf);

    // Вводим пол
    printf("Пол %d человека\n(m/w) = ", group[i].code);
    fgets(&group[i].gender, 8, stdin);

    printf("\n");
  }

  // Просто выводим то, что возвращает avgHt()
  double average_height = avgHt(group, groupSize);
  printf("%.3lf\n", average_height);

  return 0;
}
