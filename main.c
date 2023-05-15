#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)(ts.tv_sec * 1000000000L + ts.tv_nsec);
}

int *fileread(FILE *file, int coeff)
{
    int *list;
    char *str_num = (char *)calloc(0, sizeof(char) * 10);
    fgets(str_num, 10, file);
    int num = atoi(str_num);
    free(str_num);
    list = (int *)malloc(sizeof(int) * (num + 1));
    *list = num;
    for (int i = 1; i <= num; i++)
    {
        char *str_num = (char *)calloc(0, sizeof(char) * 10);
        fgets(str_num, 10, file);
        list[i] = atoi(str_num) % coeff;
        free(str_num);
    }
    return list;
}

void print_list(int *list)
{
    for (int i = 1; i <= *list; i++)
    {
        printf("%d ", list[i]);
    }
}

int min_all(int *mas_copy)
{
    int *mas = mas_copy + 1;
    int temp = 2147483647;
    for (int i = 0; i < *mas_copy; i++)
        if (mas[i] < temp)
            temp = mas[i];
    return temp;
}

int max_all(int *mas_copy)
{
    int *mas = mas_copy + 1;
    int temp = -2147483648;
    for (int i = 0; i < *mas_copy; i++)
        if (mas[i] > temp)
            temp = mas[i];
    return temp;
}

void countsort(int *mas_copy)
{
    int min = min_all(mas_copy);
    int max = max_all(mas_copy);
    int *mas = mas_copy + 1;
    int k = 0;
    // printf("max - min + 1: %d\n",max - min + 1);
    int ctr_mas[max - min + 1]; // встреча чисел от min до max

    for (int i = 0; i < max - min + 1; i++)
        ctr_mas[i] = 0; // массив подсчета встреч элементов

    for (int i = 0; i < *mas_copy; i++)
        ctr_mas[mas[i] - min]++; // заполнили

    /*for (int i = 0; i < max - min + 1; i++)
            printf("%d ", ctr_mas[i]);
        putchar('\n');*/

    for (int i = 0; i < max - min + 1; i++)
    {
        if (ctr_mas[i] != 0)
        {
            for (int j = 0; j < ctr_mas[i]; j++)
                mas[k++] = i + min;
        }
    }
}

void quicksort(int *mas, int a, int b)
{
    int left = a, right = b;
    int pivot = mas[(left + right) / 2];
    do
    {
        while (mas[left] < pivot)
        {
            left++;
        }
        while (mas[right] > pivot)
        {
            right--;
        }
        if (left <= right)
        {
            if (mas[left] > mas[right])
            {
                int tmp = mas[left];
                mas[left] = mas[right];
                mas[right] = tmp;
            }
            left++;
            if (right > 1)
            {
                right--;
            }
        }
    } while (left <= right);

    if (left < b)
    {
        quicksort(mas, left, b);
    }
    if (right > a)
    {
        quicksort(mas, a, right);
    }
}

char* get_half_command()
{
    char *command = (char *) malloc(14 * sizeof(char));
    for (int i = 0; i < 14; i++)
    {
        command[i] = '\0';
    }
    char buff[] = ".\\gen.py ";
    for (int i = 0; i < 9; i++)
    {
        command[i] = buff[i];
    }
    return command;
}

int main()
{
    int start, now;
    char* command = get_half_command();
    FILE* count_sort = fopen("count_sort.txt", "w");
    FILE* qsort = fopen("qsort.txt", "w");
    for (int coeff = 10; coeff <= 100000; coeff *= 10)
    {
        
        char *num = (char *)calloc(0, 5 * sizeof(char));
        char* command = get_half_command();
        fprintf(qsort, "%s: %d\n", "radius", coeff);
        fprintf(count_sort, "%s: %d\n", "radius", coeff);
        for (int len = 10; len <= 1000; len += 10)
        {
            long long mid_sum_c = 0, mid_sum_q = 0;
            itoa(len, num, 10);
            int j = 0;
            while (num[j])
            {
                command[9 + j] = num[j];
                j++;
                printf("%s\n%s\n", command, num);
            }
            {
                for (int i = 0; i < 30; i++)
                {
                    system(command);
                    FILE *file = fopen("list.txt", "r");
                    int *list = fileread(file, coeff);
                    // print_list(list);
                    start = get_nanos();
                    countsort(list);
                    now = get_nanos();
                    mid_sum_c += ((now - start) / 30);
                    //printf("%ld\n", now - start);
                    free(list);
                    list = NULL;
                    fclose(file);
                    file = fopen("list.txt", "r");
                    list = fileread(file, coeff);
                    // print_list(list);
                    start = get_nanos();
                    quicksort(list, 1, *list);
                    now = get_nanos();
                    mid_sum_q += ((now - start) / 30);
                    fclose(file);
                    //printf("%ld\n", now - start);
                    //print_list(list);
                }
                fprintf(qsort, "%.3lf\n", (double)mid_sum_q / 1000);
                fprintf(count_sort, "%.3lf\n", (double)mid_sum_c / 1000);
            }
        }
        free(command);
        free(num);
        command = NULL;
        num = NULL;
    }
}