#include <stdio.h>
#include <math.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "GUI_Paint.h"
#include "LCD_1in28.h"

#include "hardware/clocks.h"
#include "hardware/rtc.h"
#include "hardware/adc.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "colors.h"

#define PI 3.14159265358979323846

#define UART_ID uart0

#define UART_TX 23
#define UART_RX 29

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define NINA_ESCAPE_CHERECTER = "+"

double toRad(int deg){
    deg-=3;
    return deg*(PI/6);
}

uint NINA_started = 0;
uint aquiredTimeDate = 0;

datetime_t empty = {
        .year  = 0,
        .month = 0,
        .day   = 0,
        .dotw  = 0, // 0 is Sunday, so 5 is Friday
        .hour  = 0,
        .min   = 0,
        .sec   = 0
};

datetime_t t1 = {
        .year  = 0,
        .month = 0,
        .day   = 0,
        .dotw  = 0, // 0 is Sunday, so 5 is Friday
        .hour  = 0,
        .min   = 0,
        .sec   = 0
};

int strStartWith(char* str, char* prefix){
    uint prefixLength = strlen(prefix);
    uint result = strncmp(str, prefix, prefixLength);

    if(0 == result){
        return 1;
    }else{
        return 0;
    }
}

int strContain(char* str, char* content){
    if (strstr(str, content) != NULL) {
        return 1;
    } else {
        return 0;
    }
}

void on_uart_rx(){
    uint length = 0;
    uint8_t *raw_str = NULL;
    while (uart_is_readable(UART_ID)){
        uint8_t ch = uart_getc(UART_ID);
        raw_str += ch;
        length++;
    }
    raw_str += '\0';

    if(strStartWith(raw_str, "+UBTLE:<role>")){
        if(NINA_started == 0){
            NINA_started = 1;
        }
    }
}

int datetime_equal(datetime_t *time1, datetime_t *time2) {
    if(
            time1->year == time2->year &&
            time1->month == time2->month &&
            time1->day == time2->day &&
            time1->hour == time2->hour &&
            time1->min == time2->min &&
            time1->sec == time2->sec
            ){
        return 1;
    } else{
        return 0;
    }
}

int main() {

    if (DEV_Module_Init() != 0)
    {
        return -1;
    }

    // Initialise Real Time Clock
    rtc_init();

    uart_init(uart0, 9600);

    gpio_set_function(UART_TX, GPIO_FUNC_UART);
    gpio_set_function(UART_RX, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_ID, false, false);

    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);

    uart_set_irq_enables(UART_ID, true, false);


    uart_puts(UART_ID, "+STARTUP");





    srand(get_absolute_time());

    datetime_t t = {
            .year  = 2023,
            .month = 07,
            .day   = 06,
            .dotw  = 4, // 0 is Sunday, so 5 is Friday
            .hour  = 8,
            .min   = 00,
            .sec   = 0
    };

    rtc_set_datetime(&t);

    printf("LCD_1in28_test Demo\r\n");
    adc_init();
    adc_gpio_init(29);
    adc_select_input(3);
    /* LCD Init */
    printf("1.28inch LCD demo...\r\n");
    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    DEV_SET_PWM(60);
    // LCD_SetBacklight(1023);
    UDOUBLE Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
    UWORD *BlackImage;
    if ((BlackImage = (UWORD *)malloc(Imagesize)) == NULL)
    {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
    Paint_SetScale(65);
    Paint_Clear(WHITE);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(BLACK);

    // /* GUI */
    printf("drawing...\r\n");
    // /*2.Drawing on the image*/
#if 1
    Paint_DrawString_EN(70, 110, "Welcome", &Font20, WHITE, BLACK);

    // /*3.Refresh the picture in RAM to LCD*/
    LCD_1IN28_Display(BlackImage);
    sleep_ms(1500);

#endif

#if 1

    while (true) {

        if(NINA_started){
            if(aquiredTimeDate == 0 && datetime_equal(&t1, &empty)){



                rtc_set_datetime(&t1);
            }
        }

        Paint_Clear(BLACK);

        // draw numbers on the edge of the screen

        Paint_DrawString_EN(105, 20, "12", &Font20, WHITE, BLACK);
        Paint_DrawString_EN(113, 224-20, "6", &Font20, WHITE, BLACK);

        Paint_DrawString_EN(17, 111, "9", &Font20, WHITE, BLACK);
        Paint_DrawString_EN(226-17, 110, "3", &Font20, WHITE, BLACK);
        


        //Paint_DrawString_EN(110+(cos(toRad(1))*110)+3, 120+(sin(toRad(1))*120), "1", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(2))*110)+6, 120+(sin(toRad(2))*120)-6, "2", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(4))*110)+4, 120+(sin(toRad(4))*120)-11, "4", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(5))*110)+1, 120+(sin(toRad(5))*120)-11, "5", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(7))*110)+3, 120+(sin(toRad(7))*120)-11, "7", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(8))*110)+2, 120+(sin(toRad(8))*120)-10, "8", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(10))*110), 120+(sin(toRad(10))*120), "10", &Font20, WHITE, BLACK);
        //Paint_DrawString_EN(110+(cos(toRad(11))*110), 120+(sin(toRad(11))*120), "11", &Font20, WHITE, BLACK);

        for(int i = 0; i < 61; i++){

            if(i % 5 == 0) {
                Paint_DrawLine(120 + (cos((i - 15) * (PI / 30)) * 110), 120 + (sin((i - 15) * (PI / 30)) * 105),
                               120 + (cos((i - 15) * (PI / 30)) * 120), 120 + (sin((i - 15) * (PI / 30)) * 120), WHITE,
                               3, LINE_STYLE_SOLID);
            }else{
                Paint_DrawLine(120 + (cos((i - 15) * (PI / 30)) * 110), 120 + (sin((i - 15) * (PI / 30)) * 110),
                               120 + (cos((i - 15) * (PI / 30)) * 120), 120 + (sin((i - 15) * (PI / 30)) * 120), WHITE,
                               1, LINE_STYLE_SOLID);
            }
        }

        // reads the time

        datetime_t datetime;
        if (rtc_get_datetime(&datetime)) {
            double sec = datetime.sec;
            double min = datetime.min + (sec / 60.0);
            double hour = datetime.hour + (min / 60);

            // draws the time and date as digital
            char time[10];
            sprintf(time, "%02i:%02i:%02i", datetime.hour, datetime.min, datetime.sec);

            char date[10];
            sprintf(date, "%i/%02i/%02i", datetime.year, datetime.month, datetime.day);
            //sprintf(date, "%llu", get_absolute_time().ms);

            Paint_DrawString_EN(70, 100, time, &Font16, WHITE, BLACK);
            Paint_DrawString_EN(60, 116, date, &Font16, WHITE, BLACK);

            // draws clock hand's

            // Hour hand
            Paint_DrawLine(120, 120, 120 + (cos((hour - 3) * (PI / 6)) * 80), 120 + (sin((hour - 3) * (PI / 6)) * 80), WHITE, 1, LINE_STYLE_SOLID);

            // Second hand
            Paint_DrawLine(120, 120,120+(cos((min - 15) * (PI / 30)) * 90), 120 + (sin((min - 15) * (PI / 30)) * 90), GREEN, 1, LINE_STYLE_SOLID);

            // Paint_DrawString_EN(104,132,"Writen in NvChad NeoVIM",&Font16,WHITE,BLACK);

        }else{
            Paint_DrawString_EN(30, 100, "rtc broke", &Font16, WHITE, BLACK);
        }
        
        
        // Draw guid lines
        if(false) {
            Paint_DrawLine(119, 0, 119, 240, GREEN, 2, LINE_STYLE_DOTTED);
            Paint_DrawLine(0, 119, 240, 119, GREEN, 2, LINE_STYLE_DOTTED);
        }

        LCD_1IN28_Display(BlackImage);
        DEV_Delay_ms(100);
    }


#endif

    Paint_DrawString_EN(5, 120, "test 3", &Font20, 0x0fcf, 0x000f);

    /* Module Exit */
    free(BlackImage);
    BlackImage = NULL;

    DEV_Module_Exit();
    return 0;
}
