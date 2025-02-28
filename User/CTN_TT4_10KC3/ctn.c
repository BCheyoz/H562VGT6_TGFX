/*
 * ctn.c
 *
 *  Created on: Feb 20, 2025
 *  Author: m.faget
 *
 *  Updated on: 27 Feb. 2025
 *  Updated by: b.chhay
 *
 *  Version : 1.1
 */

#ifndef CTN_TT4_10KC3_CTN_C_
#define CTN_TT4_10KC3_CTN_C_

#include "ctn.h"

#define SIZE_TAB_CTN        146
#define TEMPERATURE_MIN     -4000
#define TEMPERATURE_MAX     10500
#define ADC_IDX 0
#define TEMP_IDX 1

const int TableConversionsAdc12bCtn3977[2][SIZE_TAB_CTN] = {{3980,3972,3964,3955,3946,3936,3926,3915,3904,3892,3880,3866,3853,3838,3823,
        3807,3791,3773,3755,3736,3716,3696,3675,3652,3629,3605,3581,3555,3528,3501,3472,3443,3413,3381,3349,3316,3282,3248,3212,
        3176,3139,3101,3062,3022,2982,2941,2900,2858,2815,2772,2728,2684,2639,2595,2549,2504,2459,2413,2367,2321,2275,2229,
        2184,2138,2093,2048,2003,1958,1914,1870,1826,1783,1741,1699,1657,1616,1576,1536,1497,1459,1421,1384,1347,1312,1277,
        1242,1209,1176,1144,1112,1082,1052,1022,994,966,939,912,886,861,837,813,790,767,745,724,703,683,663,644,626,608,590,
        573,557,541,525,510,496,482,468,455,442,429,417,405,394,383,372,362,352,342,333,323,314,306,297,289,281,274,266,259,
        252,245,239,233,226},
        {-40,-39,-38,-37,-36,-35,-34, -33,-32, -31,-30, -29, -28, -27,-26,-25,-24,-23, -22, -21,-20,-19,-18, -17, -16,  -15,-14, -13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
        26, 27,28,29,30,31,32,33,34,35,36,37,38,39,40, 41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,
        94, 95,96,97,98,99,100, 101,102, 103,104,105}
           };


int16_t convertADC_to_CTN_10K(uint16_t Val_ADC)
{
    uint8_t i;
    float Ax, B;// 0 is convPoint  / 1 is TEMP_IDX

    if (Val_ADC >= (uint16_t)TableConversionsAdc12bCtn3977[ADC_IDX][0]) {
    	return (int16_t)TEMPERATURE_MIN;

    } else if (Val_ADC <= (uint16_t)TableConversionsAdc12bCtn3977[ADC_IDX][(SIZE_TAB_CTN - 1)]) {
    	return (int16_t)TEMPERATURE_MAX;
    }

    // On recherche ou l'on se trouve dans la table
    // recherche par dichotomie
    uint8_t border_a = 0;
    uint8_t border_b = SIZE_TAB_CTN-1;
    uint8_t middle = 0;
    while(border_b > border_a + 1)
    {
    	middle = (border_a + border_b)/2;
    	if(TableConversionsAdc12bCtn3977[ADC_IDX][middle] < Val_ADC)
		{
    		border_b = middle;
		}
		else
		{
			border_a = middle;
		}
    }
    i = border_a+1;

    //On calcule la pente (extrapolation lineaire)
    Ax = ((float) TableConversionsAdc12bCtn3977[TEMP_IDX][i]*100
            - (float) TableConversionsAdc12bCtn3977[TEMP_IDX][i - 1]*100)
            / ((float) TableConversionsAdc12bCtn3977[ADC_IDX][i]
                    - (float) TableConversionsAdc12bCtn3977[ADC_IDX][i - 1]);
    B = TableConversionsAdc12bCtn3977[TEMP_IDX][i]*100
            - Ax * TableConversionsAdc12bCtn3977[ADC_IDX][i];

    //On calcul la Textrapol
    float r = ((float) Val_ADC) * Ax + B;
    //gestion de l'arrondi  l'unit
    if(r < 0)
    {
    	return (int16_t) (r - 0.5);
    }
    else
    {
    	return (int16_t) (r + 0.5);
    }
}



#endif /* CTN_TT4_10KC3_CTN_C_ */
