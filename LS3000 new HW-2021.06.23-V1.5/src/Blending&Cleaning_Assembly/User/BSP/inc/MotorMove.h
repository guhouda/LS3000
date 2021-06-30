#ifndef __MOTORMOVE_H_
#define __MOTORMOVE_H_

extern _Bool Aging_Flag;

void Aging_Test(void);
U8 DetectInit_MM(void);
void MoveCardOut(void);
U8 MotorReset(void);
void Clean_ReactingCup(U8 CupNum,U8 BlendNum,U16 timeset);
void Blending_samples(U8 CupNum,U16 BlendNum);
void Suction_samples(U8 CupFromNum,U8 CupToNum,U32 SuckNum);
void CleaningTankWall(U16 timeset);
void CleaningNeedle_In(U16 timeset);
void CleaningNeedle_Out(U16 timeset);
void Wiping_ReactingCup(U8 CupNum,U16 timeset);
#endif
