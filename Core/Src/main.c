#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

int main(void) 
{ 
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x38;            //0x38 Включение тактирования портов GPIOD, GPIOE и GPIOF(RCC на мемори мап)
    //настройка GPIOD портов
    *(uint32_t*)(0x40020C00UL + 0x00UL) &= ~0xC0000000UL;   //Настройка работы 15-го пина GPIOD в режиме вывода сигнала (Output mode) 
    *(uint32_t*)(0x40020C00UL + 0x00UL) |= 0x40000000UL;
    *(uint32_t*)(0x40020C00UL + 0x04UL) &= ~0xC000UL;       //Настройка на Push Pull работу 15-го пина GPIOD (Output Push-Pull) 
    *(uint32_t*)(0x40020C00UL + 0x04UL) |= 0x4000UL;
    *(uint32_t*)(0x40020C00UL + 0x08UL) &= ~0xC0000000UL;   //Настройка скорости работы 15-го пина GPIOD на среднюю 
    *(uint32_t*)(0x40020C00UL + 0x08UL) |= 0x40000000UL;
    *(uint32_t*)(0x40020C00UL + 0x0CUL) &= ~0xC0000000UL;   //Отключение PU/PD резисторов для 15-го пина GPIOD 

    *(uint32_t*)(0x40021400UL + 0x00UL) &= ~0x0F000000UL; //Настройка работы 12,13-го пина GPIOF в режиме вывода сигнала (Output mode) 
    *(uint32_t*)(0x40021400UL + 0x00UL) |=  0x05000000UL; 
    *(uint32_t*)(0x40021400UL + 0x04UL) &= ~0x3000UL; //Настройка на Push Pull работу 12,13-го пина GPIOF (Output Push-Pull) 
    *(uint32_t*)(0x40021400UL + 0x08UL) &= ~0x0F000000UL; //Настройка скорости работы 12,13-го пина GPIOF на среднюю 
    *(uint32_t*)(0x40021400UL + 0x08UL) |=  0x05000000UL; 
    *(uint32_t*)(0x40021400UL + 0x0CUL) &= ~0x0F000000UL;  //Отключение PU/PD резисторов для 12,13-го пина GPIOF 

    uint8_t count = 1;
    bool flag = false;
    while(1)
    { 
        if((*(uint32_t*)(0x40021400UL + 0x10UL) & 0x4000UL) != 0)
        { 
           if(flag==false)
           {
                flag=true;
                for (uint32_t i = 0; i < 1000000UL; i++)
                {
                }
           }
           else
           {    
                flag=false;
                for (uint32_t i = 0; i < 1000000UL; i++)
                {
                }
           }
        }

        switch (count)
        {
            case 0:
            *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x80000000UL;
            *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x20000000UL;
            *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x10000000UL;
            if(flag == false)
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                     count++;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                        {
                        }
                    }
                }
            else
                {
                    flag=false;
                }
            break;
            case 1:
                *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x8000UL;
                *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x20000000UL;
                *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x10000000UL;
                if(flag == false)
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        count++;
                        for (uint32_t i = 0; i < 1000000UL; i++)
                            {
                            }
                    }
                }
                else
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        count--;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                    {
                    }
                    }
                }
                break;
            case 2:
                *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x8000UL;
                *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x20000000UL;
                *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x1000UL;
                if(flag == false)
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        count++;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                    {
                                    }
                    }
                }
                else
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        count--;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                    {
                    }
                    }
                }
                break;
            case 3:
            *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x8000UL;
            *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x2000UL;
            *(uint32_t*)(0x40021400UL + 0x18UL) |= 0x1000UL;
                if(flag == false)
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        flag=true;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                    {
                    }
                    }
                }
                else
                {
                    if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
                    {
                        count--;
                    for (uint32_t i = 0; i < 1000000UL; i++)
                    {
                    }
                    }
                }
                break;
        }
    }
}