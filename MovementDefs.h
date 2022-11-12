#ifndef MovementDefs_h
#define MovementDefs_h

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

// Each axis have 2000 counts per revolution
// Resolution of X axis = 20 mm per revolution
// Resolution of Y axis = 20 mm per revolution
// Resolution of Z axis = 7.5 mm per revolution
// FACTOR_X = 20 /2000 = 0.01000 mm/counts
// FACTOR_Y = 20 /2000 = 0.01000 mm/counts
// FACTOR_Z = 7.5/2000 = 0.00375 mm/counts
#define FACTOR_X 0.01000
#define FACTOR_Y 0.01000
#define FACTOR_Z 0.00375

#endif /* MovementDefs_h */