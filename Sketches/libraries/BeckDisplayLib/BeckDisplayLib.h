// BeckDisplayLib.h 3/4/19a
#pragma once
#include <BeckBiotaLib.h>
#include <Stream.h>

void  SetupDisplay        ();
void  SetupDisplay        (ProjectType eDisplayProjectType);
void  ClearDisplay        ();
void  UpdateDisplay       ();
void  UpdateThermDisplay  ();
void  UpdateIMUDisplay    ();
void  Update4LineDisplay  (String szLine1, String szLine2, String szLine3, String szLine4);
//Last Line.
