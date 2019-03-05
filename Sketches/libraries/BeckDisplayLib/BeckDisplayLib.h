// BeckDisplayLib.h 3/5/19b
#pragma once
#include <BeckBiotaLib.h>
#include <BeckMPU9150Lib.h>
//#include <Stream.h>

const  int       wBuffChar             = 20;
extern char      aszAccGyroMagPRY  [eLastSensor][eLastAxis][wBuffChar];
extern char      szDegC            [wBuffChar];

void  SetupDisplay            (ProjectType eDisplayProjectType);
void  ClearDisplay            ();
void  UpdateDisplay           ();
//Last Line.
