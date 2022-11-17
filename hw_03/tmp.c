/*
 * =====================================================================================
 *
 *       Filename:  tmp.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15.11.2022 11:26:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
int TTT = 1;
int main ()
{
	for(int i=0; i<10;i++)
	{
		printf("%d \n", 2<<i);	   
		TTT++;
	}
}
