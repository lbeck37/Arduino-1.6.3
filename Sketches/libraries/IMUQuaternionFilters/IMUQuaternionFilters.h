// IMUQuaternionFilters.h, Beck 2/20/19a
#pragma once

extern float q[4];

void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);

//Last line.
