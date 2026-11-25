#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

int main(void) 
{ 
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x38;            //0x38 Включение тактирования портов GPIOD, GPIOE и GPIOF(RCC на мемори мап)
    *(uint32_t*)(0x40020C00UL + 0x00UL) &= ~0xC0000000UL;   //Настройка работы 15-го пина GPIOD в режиме вывода сигнала (Output mode) 
    *(uint32_t*)(0x40020C00UL + 0x00UL) |= 0x40000000UL;
    *(uint32_t*)(0x40020C00UL + 0x04UL) &= ~0xC000UL;       //Настройка на Push Pull работу 15-го пина GPIOD (Output Push-Pull) 
    *(uint32_t*)(0x40020C00UL + 0x04UL) |= 0x4000UL;
    *(uint32_t*)(0x40020C00UL + 0x08UL) &= ~0xC0000000UL;   //Настройка скорости работы 15-го пина GPIOD на среднюю 
    *(uint32_t*)(0x40020C00UL + 0x08UL) |= 0x40000000UL;
    *(uint32_t*)(0x40020C00UL + 0x0CUL) &= ~0xC0000000UL;   //Отключение PU/PD резисторов для 15-го пина GPIOD 
    uint8_t count = 1;
    bool flag = false;
    while (1)
    {
        if((*(uint32_t*)(0x40020C00UL + 0x10UL) & 0x4000UL) != 0)
        { 
            *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x8000UL; //Установка единицы в 15-ой бит регистра ODR 
        } 
        else
        { 
            *(uint32_t*)(0x40020C00UL + 0x18UL) |= 0x80000000UL; //Установка нуля в 15-ой бит регистра ODR 
        }
    }    
}