#include "Useful.h"

int stoi(char * src)
{
    char    index = 0;
    int     res = 0;

    for(;src[index] > 0;index++)
        res = (res * 10) + (src[index] - '0');

    return res;
}

void sort(unsigned *arr, unsigned size)
{
    char i, j;
    unsigned temp;
    for(i=1;i<size;i++)
    {
        temp = arr[i];                          // current value
        for(j=i;j>0;j--)
        {
            if(temp < arr[j-1])                 // less than prevoius, right shift value
                arr[j] = arr[j-1];
            else
                break;
        }
        arr[j] = temp;                          // put in last address
    }
}

