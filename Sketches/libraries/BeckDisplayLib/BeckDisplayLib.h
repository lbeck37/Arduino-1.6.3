// BeckDisplayLib.h 2/18/19b
#pragma once
#include <Stream.h>

void  SetupDisplay        ();
void  ClearDisplay        ();
void  UpdateDisplay       ();
void  UpdateThermDisplay  ();
void  UpdateIMUDisplay    ();
void  Update4LineDisplay  (String szLine1, String szLine2, String szLine3, String szLine4);
//Last Line.
