#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    *(uint32_t*)(0x40023800UL + 0x30UL) |= 0x39;            //0x39 Включение тактирования портов GPIOA, GPIOD, GPIOE и GPIOF (RCC на мемори мап)

    //настройка GPIOA портов
    *(uint32_t*)(0x40020000UL + 0x00UL) &= ~0x0000FC00UL;   //Настройка работы 5, 6 и 7-го пинов GPIOA в режиме вывода сигнала (Output mode)
    *(uint32_t*)(0x40020000UL + 0x00UL) |=  0x00005400UL;
    *(uint32_t*)(0x40020000UL + 0x04UL) &= ~0x000000E0UL;   //Настройка на Push Pull работу 5, 6 и 7-го пинов GPIOA (Output Push-Pull)
    *(uint32_t*)(0x40020000UL + 0x04UL) |=  0x00000000UL;
    *(uint32_t*)(0x40020000UL + 0x08UL) &= ~0x0000FC00UL;   //Настройка скорости работы 5, 6 и 7-го пинов GPIOA на среднюю
    *(uint32_t*)(0x40020000UL + 0x08UL) |=  0x00005400UL;
    *(uint32_t*)(0x40020000UL + 0x0CUL) &= ~0x0000FC00UL;   //Отключение PU/PD резисторов для 5, 6 и 7-го пинов GPIOA

    //настройка GPIOD портов
    *(uint32_t*)(0x40020C00UL + 0x00UL) &= ~0xF0000000UL;   //Настройка работы 14 и 15-го пинов GPIOD в режиме вывода сигнала (Output mode)
    *(uint32_t*)(0x40020C00UL + 0x00UL) |=  0x50000000UL;
    *(uint32_t*)(0x40020C00UL + 0x04UL) &= ~0xC000UL;       //Настройка на Push Pull работу 14 и 15-го пинов GPIOD (Output Push-Pull)
    *(uint32_t*)(0x40020C00UL + 0x04UL) |=  0x0000UL;
    *(uint32_t*)(0x40020C00UL + 0x08UL) &= ~0xF0000000UL;   //Настройка скорости работы 14 и 15-го пинов GPIOD на среднюю
    *(uint32_t*)(0x40020C00UL + 0x08UL) |=  0x50000000UL;
    *(uint32_t*)(0x40020C00UL + 0x0CUL) &= ~0xF0000000UL;   //Отключение PU/PD резисторов для 14 и 15-го пинов GPIOD

    //настройка GPIOE портов
    *(uint32_t*)(0x40021000UL + 0x00UL) &= ~0x00C00000UL;   //Настройка работы 11-го пина GPIOE в режиме вывода сигнала (Output mode)
    *(uint32_t*)(0x40021000UL + 0x00UL) |=  0x00400000UL;
    *(uint32_t*)(0x40021000UL + 0x04UL) &= ~0x0800UL;       //Настройка на Push Pull работу 11-го пина GPIOE (Output Push-Pull)
    *(uint32_t*)(0x40021000UL + 0x04UL) |=  0x0000UL;
    *(uint32_t*)(0x40021000UL + 0x08UL) &= ~0x00C00000UL;   //Настройка скорости работы 11-го пина GPIOE на среднюю
    *(uint32_t*)(0x40021000UL + 0x08UL) |=  0x00400000UL;
    *(uint32_t*)(0x40021000UL + 0x0CUL) &= ~0x00C00000UL;   //Отключение PU/PD резисторов для 11-го пина GPIOE
    *(uint32_t*)(0x40021000UL + 0x00UL) &= ~0x0C000000UL;   //Настройка работы 13-го пина GPIOE в режиме входа (Input mode) для кнопки PE13
    *(uint32_t*)(0x40021000UL + 0x0CUL) &= ~0x0C000000UL;   //Отключение PU/PD резисторов для 13-го пина GPIOE

    //настройка GPIOF портов
    *(uint32_t*)(0x40021400UL + 0x00UL) &= ~0x30000000UL;   //Настройка работы 14-го пина GPIOF в режиме входа (Input mode) для кнопки PF14
    *(uint32_t*)(0x40021400UL + 0x0CUL) &= ~0x30000000UL;   //Отключение PU/PD резисторов для 14-го пина GPIOF


    uint8_t led_mask = 0x03U;               //Битовая маска состояния 6 светодиодов (биты 0..5)
    bool blink_enabled = true;             //Режим мерцания активных светодиодов
    bool blink_phase = true;                //Текущая полуволна мерцания (1 — вкл, 0 — выкл)
    uint32_t blink_ticks = 0U;              //Счётчик итераций для полупериода мерцания
    uint32_t blink_half_period = 50000U;

    const uint32_t blink_min_period = 50000U;   //Минимальный полупериод (максимальная заметная частота)
    const uint32_t blink_max_period = 200000U; //Максимальный полупериод (медленное мигание, ~2 с)
    const uint32_t blink_step_period = 50000U;  //Шаг изменения полупериода (~0.5 с полного периода / 0.25-0.5 с полупериода)

    const uint32_t long_press_threshold = 120000U;      //Порог длительного нажатия
    const uint32_t very_long_press_threshold = 350000U; //Порог очень длительного нажатия

    bool btn1_prev = false;
    uint32_t btn1_hold = 0U;
    bool btn1_very_long_done = false;

    bool btn2_prev = false;
    uint32_t btn2_hold = 0U;
    bool btn2_very_long_done = false;

    while (1)
    {
        bool btn1_now = (((*(uint32_t*)(0x40021000UL + 0x10UL)) & 0x2000UL) != 0U); //Опрос кнопки PE13 (13-й бит GPIOE)
        bool btn2_now = (((*(uint32_t*)(0x40021400UL + 0x10UL)) & 0x4000UL) != 0U); //Опрос кнопки PF14 (14-й бит GPIOF)

        // Обработка первой кнопки (PE13)
        if (btn1_now)
        {
            if (!btn1_prev)
            {
                btn1_hold = 0U;
                btn1_very_long_done = false;
            }
            else
            {
                if (btn1_hold < 0xFFFFFFFFU)
                {
                    btn1_hold++;
                }

                //Очень длительное нажатие: включает все светодиоды
                if (!btn1_very_long_done && (btn1_hold >= very_long_press_threshold))
                {
                    led_mask = 0x3FU;          //Включение всех 6 светодиодов (PE11, PD15, PD14, PA7, PA6, PA5)
                    blink_enabled = true;
                    blink_phase = true;
                    btn1_very_long_done = true;
                }
            }
        }
        else if (btn1_prev) //Кнопка PE13 отпущена
        {
           
            if (!btn1_very_long_done)
            {
                if (btn1_hold >= long_press_threshold)
                {
                    //Длительное нажатие: включает мерцание и увеличивает частоту
                    blink_enabled = true;
                    if (blink_half_period > (blink_min_period + blink_step_period))
                    {
                        blink_half_period -= blink_step_period;
                    }
                    else
                    {
                        blink_half_period = blink_min_period;
                    }
                    if (blink_ticks >= blink_half_period)
                    {
                        blink_ticks = 0U;
                    }
                }
                else
                {
                    //Короткое нажатие: поочерёдное включение в порядке PE11 -> PD15 -> PD14 -> PA7 -> PA6 -> PA5
                    for (uint8_t i = 0U; i < 6U; i++)
                    {
                        if ((led_mask & (uint8_t)(1U << i)) == 0U)
                        {
                            led_mask |= (uint8_t)(1U << i);
                            break;
                        }
                    }
                }
            }
            btn1_hold = 0U;
            btn1_very_long_done = false;
        }
        btn1_prev = btn1_now;

        /* Обработка второй кнопки (PF14) */
        if (btn2_now)
        {
            if (!btn2_prev)
            {
                btn2_hold = 0U;
                btn2_very_long_done = false;
            }
            else
            {
                if (btn2_hold < 0xFFFFFFFFU)
                {
                    btn2_hold++;
                }

                //Очень длительное нажатие: выключает все светодиоды
                if (!btn2_very_long_done && (btn2_hold >= very_long_press_threshold))
                {
                    led_mask = 0x00U;          //Выключение всех 6 светодиодов (PE11, PD15, PD14, PA7, PA6, PA5)
                    blink_enabled = true;
                    blink_phase = true;
                    btn2_very_long_done = true;
                }
            }
        }
        else if (btn2_prev)
        {
            //Кнопка PF14 отпущена
            if (!btn2_very_long_done)
            {
                if (btn2_hold >= long_press_threshold)
                {
                    //Длительное нажатие: включает мерцание и уменьшает частоту (увеличивает полупериод на небольшой шаг)
                    blink_enabled = true;
                    if ((blink_half_period + blink_step_period) < blink_max_period)
                    {
                        blink_half_period += blink_step_period;
                    }
                    else
                    {
                        blink_half_period = blink_max_period;
                    }
                }
                else
                {
                    //Короткое нажатие: поочерёдное выключение в обратном порядке PA5 -> PA6 -> PA7 -> PD14 -> PD15 -> PE11
                    for (int8_t i = 5; i >= 0; i--)
                    {
                        if ((led_mask & (uint8_t)(1U << (uint8_t)i)) != 0U)
                        {
                            led_mask &= (uint8_t)~(1U << (uint8_t)i);
                            break;
                        }
                    }
                }
            }
            btn2_hold = 0U;
            btn2_very_long_done = false;
        }
        btn2_prev = btn2_now;

        uint8_t active_mask = led_mask;
        if (blink_enabled)
        {
            blink_ticks++;
            if (blink_ticks >= blink_half_period)
            {
                blink_ticks = 0U;
                blink_phase = !blink_phase;
            }
            active_mask = blink_phase ? led_mask : 0x00U;
        }

        //PE11 (бит 0 маски -> бит 11 GPIOE)
        if ((active_mask & 0x01U) != 0U)
        {
            *(uint32_t*)(0x40021000UL + 0x18UL) = 0x00000800UL;
        }
        else
        {
            *(uint32_t*)(0x40021000UL + 0x18UL) = 0x08000000UL;
        }

        //PD15 (бит 1 маски -> бит 15 GPIOD) и PD14 (бит 2 маски -> бит 14 GPIOD)
        uint32_t gpiod_bsrr = 0U;
        gpiod_bsrr |= ((active_mask & 0x02U) != 0U) ? 0x00008000UL : 0x80000000UL;
        gpiod_bsrr |= ((active_mask & 0x04U) != 0U) ? 0x00004000UL : 0x40000000UL;
        *(uint32_t*)(0x40020C00UL + 0x18UL) = gpiod_bsrr;

        //PA7 (бит 3 маски -> бит 7 GPIOA), PA6 (бит 4 -> бит 6), PA5 (бит 5 -> бит 5)
        uint32_t gpioa_bsrr = 0U;
        gpioa_bsrr |= ((active_mask & 0x08U) != 0U) ? 0x00000080UL : 0x00800000UL;
        gpioa_bsrr |= ((active_mask & 0x10U) != 0U) ? 0x00000040UL : 0x00400000UL;
        gpioa_bsrr |= ((active_mask & 0x20U) != 0U) ? 0x00000020UL : 0x00200000UL;
        *(uint32_t*)(0x40020000UL + 0x18UL) = gpioa_bsrr;
    }
}