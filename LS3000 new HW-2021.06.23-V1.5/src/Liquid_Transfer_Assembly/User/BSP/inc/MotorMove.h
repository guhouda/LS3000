#ifndef __MOTORMOVE_H_
#define __MOTORMOVE_H_

typedef enum
{
	EN_POS = 0x01,
	LOW_TUBE_POS,
	HIGH_TUBE_POS,
    DILUTION1_POS,
    DILUTION2_POS,
    DILUTION3_POS,
    DILUTION4_POS,
    DILUTION5_POS,
    DILUTION6_POS,
    BLEND_POS,
    HB_EN_POS,
    HB_LOW_TUBE_POS,
    HB_HIGH_TUBE_POS
}SUCTION_POS;

extern _Bool Aging_Flag;

U8 DetectInit_MM(void);
U8 Suction_Sample(U8 Suction_Pos,U32 sampleNum);
U8 Suction_Sample_Liq(U8 Suction_Pos,U32 sampleNum);
U8 Add_Sample_ER(void);
void CleaningNeedle_Out(U16 timeset);
void CleaningNeedle_In(U16 timeset);
void MoveCardOut(void);
U8 MotorReset(void);
void Aging_Test(void);
void Finger_Suction_sample(U8 loction,U32 sampleNum);
#endif
