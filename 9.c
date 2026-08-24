#include <stdio.h>

int main()
{
    int num, temp, digit, sum, count = 0;

    printf("Armstrong numbers: ");

    for(num = 1; num <= 1000; num++)
    {
        temp = num;
        sum = 0;

        while(temp > 0)
        {
            digit = temp % 10;
            sum += digit * digit * digit;
            temp /= 10;
        }

        if(sum == num)
        {
            printf("%d ", num);
            count++;
        }
    }

    printf("\nCount = %d", count);

    return 0;
}
