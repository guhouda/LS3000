#ifndef __MOTORMOVE_H_
#define __MOTORMOVE_H_

extern U8 Tube_Group; 
extern U8 Tube_Num;
extern _Bool CR100_Scan_Flag;
extern _Bool Test_Finish_Flag;
extern _Bool Piece_Finish_Flag;
extern _Bool Continue_Test_Flag;
extern _Bool Aging_Flag;
extern _Bool Scan_OK;
extern _Bool ScanTemple_OK;
extern U8 Shaking_Flag;
U8 DetectInit_MM(void);
U8 MotorReset(void);
void StartAutoTest(U8 TestMode);
void MoveToNextPiecePos(U8 num);	
U8 MoveRackOut(void);
void Aging_Test(void);
void Tube_Shaking(U8 shakeflag);
#endif
