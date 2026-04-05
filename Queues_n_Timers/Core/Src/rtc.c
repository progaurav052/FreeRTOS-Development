/*
 * rtc.c
 *
 *  Created on: Apr 3, 2026
 *      Author: ggpai
 */




#include "main.h"

 void show_time_date_itm(void)
 {
	RTC_DateTypeDef rtc_date; // this structure is filled up with date info from rtc
	RTC_TimeTypeDef rtc_time; // this structure is filled up with time info from rtc

	memset(&rtc_date,0,sizeof(rtc_date));
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN); // api to fill the structure
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN); // api to fill the structre

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";


	printf("%02d:%02d:%02d [%s]",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	printf("\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);

 }

void show_time_date(void)
{
	static char showtime[40]; // buffer to store the time info after being filled from RTC
	static char showdate[40]; // buffer to store the date info after being filled from RTC

	RTC_DateTypeDef rtc_date; // this structure is filled up with date info from rtc
	RTC_TimeTypeDef rtc_time; // this structure is filled up with time info from rtc

	static char *time = showtime;
	static char *date = showdate;

	memset(&rtc_date,0,sizeof(rtc_date));
	memset(&rtc_time,0,sizeof(rtc_time));

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	/* Display time Format : hh:mm:ss [AM/PM] */
	sprintf((char*)showtime,"%s:\t%02d:%02d:%02d [%s]","\nCurrent Time&Date",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	xQueueSend(q_print_msg,&time,portMAX_DELAY);

	/* Display date Format : date-month-year */
	sprintf((char*)showdate,"\t%02d-%02d-%2d\n",rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);
	xQueueSend(q_print_msg,&date,portMAX_DELAY);
}


void rtc_configure_time(RTC_TimeTypeDef *time)
{
	// RTC <---> [<storage_structs --- [RTC_DateTypeDef ,RTC_TimeTypeDef ]---storage_structs >] <-----> User

	time->TimeFormat = RTC_HOURFORMAT12_AM;
	time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	time->StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&hrtc,time,RTC_FORMAT_BIN); // configures the RTC peripheral with info from storage structs
}


void rtc_configure_date(RTC_DateTypeDef *date)
{
	HAL_RTC_SetDate(&hrtc,date,RTC_FORMAT_BIN);
	// configure the RTC peripheral with info from storage structs

}


int validate_rtc_information(RTC_TimeTypeDef *time , RTC_DateTypeDef *date)
{
	if(time){
		if( (time->Hours > 12) || (time->Minutes > 59) || (time->Seconds > 59) )
			return 1;
	}

	if(date){
		if( (date->Date > 31) || (date->WeekDay > 7) || (date->Year > 99) || (date->Month > 12) )
			return 1;
	}

	return 0;
}
