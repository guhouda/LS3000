#ifndef __MOTORMOVE_H_
#define __MOTORMOVE_H_

extern _Bool Aging_Flag;
extern _Bool Turnplate_cardout;
U8 DetectInit_MM(void);
void PushCardToReversePos(U8 cardID);
void PushCardToRoller(void);
U8 MotorReset(void);
void Aging_Test(void);
void PushCardToReversePosAndScan(U8 cardID);
#endif
