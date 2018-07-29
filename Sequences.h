#pragma once

#define END_SEQUENCE      -127
#define END_SEQUENCE_UINT 129

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Missions
 *  
 *  Format:  [Number of formations], 
 *           [Upper_Scenery] | [Lower_Scenery] | [Formation Index 1], 
 *           [Upper_Scenery] | [Lower_Scenery] | [Formation Index 2], .. 
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#ifdef HAS_SCENERY

#define SCENERY_MASK_NONE  0b00001111
#define SCENERY_MASK_LOWER 0b00110000
#define SCENERY_MASK_UPPER 0b11000000

#define SCENERY_LOWER_NONE 0b00000000
#define SCENERY_LOWER_INCR 0b00010000
#define SCENERY_LOWER_DECR 0b00100000
#define SCENERY_LOWER_RAND 0b00110000

#define SCENERY_UPPER_NONE 0b00000000
#define SCENERY_UPPER_INCR 0b01000000
#define SCENERY_UPPER_DECR 0b10000000
#define SCENERY_UPPER_RAND 0b11000000

#else

#define SCENERY_LOWER_NONE 0b00000000
#define SCENERY_LOWER_INCR 0b00000000
#define SCENERY_LOWER_DECR 0b00000000
#define SCENERY_LOWER_RAND 0b00000000

#define SCENERY_UPPER_NONE 0b00000000
#define SCENERY_UPPER_INCR 0b00000000
#define SCENERY_UPPER_DECR 0b00000000
#define SCENERY_UPPER_RAND 0b00000000

#endif

const uint8_t PROGMEM mission_00[] = { 
  14, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_DECR | 0, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_DECR | 1, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_RAND | 2, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_INCR | 3, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 4, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 5,    // Boat
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 6, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 7, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 8, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_NONE | 9, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_NONE | 10,   // Boat
  SCENERY_UPPER_RAND | SCENERY_LOWER_DECR | 11, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 12, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 13 };

const uint8_t PROGMEM mission_01[] = { 
  14,
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 14, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 7, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 8, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_DECR | 9, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_DECR | 13, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_DECR | 0, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 1, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 2, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 3, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 10,   // Boat 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 11, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 12, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 4, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 5,    // Boat
  SCENERY_UPPER_NONE | SCENERY_LOWER_DECR | 6 };

const uint8_t PROGMEM mission_02[] = { 
  14, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 0, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 2, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 4, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 6, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_NONE | 8, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_NONE | 10,   // Boat 
  SCENERY_UPPER_NONE | SCENERY_LOWER_DECR | 5,    // Boat
  SCENERY_UPPER_NONE | SCENERY_LOWER_DECR | 12, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 1, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 3, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 7, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 9, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 11, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 13 };

const uint8_t PROGMEM mission_03[] = { 
  14, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 2, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 1, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 7, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 10,   // Boat 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 4, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 12, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 5,    // Boat
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 0, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 8, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 9, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 13, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 3, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_DECR | 11, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_DECR | 6,};

const uint8_t PROGMEM mission_04[] = { 
  14, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 2, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 1, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 10, 
  SCENERY_UPPER_INCR | SCENERY_LOWER_NONE | 7, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 4, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 12, 
  SCENERY_UPPER_RAND | SCENERY_LOWER_NONE | 0, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_DECR | 8, 
  SCENERY_UPPER_DECR | SCENERY_LOWER_DECR | 9, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 13, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_RAND | 3, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 11, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_INCR | 5, 
  SCENERY_UPPER_NONE | SCENERY_LOWER_NONE | 6,};



/* -----------------------------------------------------------------------------------------------------------------------------
 *  Formations
 *
 *  Format:  [Enemy Type],             - 0 solid, 1 hollow, 2 bomber, 5 boat
 *           [Sequence Index],   
 *           [X Offset], [Y Offset],
 *           [Delay Start],           
 *           [Invert X Axis],  
 *           [Invert Y Axis],  
 *           [Number of Hits]  
 * -----------------------------------------------------------------------------------------------------------------------------
 */


const int8_t PROGMEM formation_00[] = {
  2, 
  (uint8_t)EnemyType::Zero, 0, 0, 5, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 0, 24, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -5, 80, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -10, 104, false, false, 1,
};

const int8_t PROGMEM formation_01[] = {  /* V Shape */
  5, 
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -25, 36, false, false, 2,
  (uint8_t)EnemyType::Zero, 0, 0, -16, 18, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -7, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 2, 18, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, 11, 36, false, false, 2,
};

const int8_t PROGMEM formation_02[] = { /* Leading two cross and disappear top and bottom, remaining three in straight line */
  5, 
  (uint8_t)EnemyType::Zero, 1, 0, 5, 0, false, true, 1,
  (uint8_t)EnemyType::Zero, 1, 0, -20, 20, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -23, 50, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 50, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, 9, 50, false, false, 2,
};

const int8_t PROGMEM formation_03[] = { /* Four straight ahead */
  4, 
  (uint8_t)EnemyType::Zero, 0, 0, -15, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 3, 0, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -15, 18, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, 3, 18, false, false, 2,
};

const int8_t PROGMEM formation_04[] = { /* three in row 1, followed by bomber */
  4, 
  (uint8_t)EnemyType::Zero, 0, 0, -25, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -7, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 11, 0, false, false, 1,
  (uint8_t)EnemyType::Bomber, 0, 0, -11, 22, false, false, 1,
};

const int8_t PROGMEM formation_05[] = { /* three in row 1, followed by boat */
  4, 
  (uint8_t)EnemyType::Zero, 0, 0, -16, 18, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -7, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 2, 18, false, false, 1,
  (uint8_t)EnemyType::Boat, 0, 0, 14, 48, false, false, 10,
};

const int8_t PROGMEM formation_06[] = { /* 2, 2, 1 */
  5, 
  (uint8_t)EnemyType::Zero, 1, 0, 5, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 1, 0, 5, 20, false, false, 1,
  (uint8_t)EnemyType::Zero, 1, 0, -20, 0, false, true, 1,
  (uint8_t)EnemyType::Zero, 1, 0, -20, 20, false, true, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -7, 40, false, true, 1,
};

const int8_t PROGMEM formation_07[] = {
  5, 
  (uint8_t)EnemyType::Zero, 2, -16, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 40, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 68, false, false, 2,
  (uint8_t)EnemyType::Zero, 2, -16, 32, 20, false, true, 1,
  (uint8_t)EnemyType::Zero, 2, -16, 32, 60, false, true, 1,
};

const int8_t PROGMEM formation_08[] = {
  6, 
  (uint8_t)EnemyType::Zero, 3, -42, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 3, -26, -32, 0, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -21, 20, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 36, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, 7, 52, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 64, false, false, 2,
};

const int8_t PROGMEM formation_09[] = {
  6, 
  (uint8_t)EnemyType::Zero, 0, 0, -16, 18, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, -7, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 0, 0, 2, 18, false, false, 1,
  (uint8_t)EnemyType::Bomber, 0, 0, -28, 64, false, false, 2,
  (uint8_t)EnemyType::Bomber, 0, 0, -11, 44, false, false, 2, 
  (uint8_t)EnemyType::Bomber, 0, 0, 6, 64, false, false, 2,
};

const int8_t PROGMEM formation_10[] = {
  5, 
  (uint8_t)EnemyType::Boat, 0, 0, 14, 4, false, false, 5,
  (uint8_t)EnemyType::Boat, 0, 0, -32, 62, false, false, 5,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 35, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 70, false, false, 1,
};

const int8_t PROGMEM formation_11[] = {
  5, 
  (uint8_t)EnemyType::Zero, 2, -16, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 40, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 68, false, false, 2,
  (uint8_t)EnemyType::Zero, 2, 0, 32, 0, true, true, 1,
  (uint8_t)EnemyType::Zero, 2, 0, 32, 40, true, true, 1,
};

const int8_t PROGMEM formation_12[] = {
  5, 
  (uint8_t)EnemyType::Zero, 2, -16, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 40, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -7, 68, false, false, 2,
  (uint8_t)EnemyType::Zero, 2, 0, 32, 0, true, true, 1,
  (uint8_t)EnemyType::Zero, 2, 0, 32, 40, true, true, 1,
};

const int8_t PROGMEM formation_13[] = {
  6, 
  (uint8_t)EnemyType::Zero, 2, -16, -48, 0, false, false, 1,
  (uint8_t)EnemyType::Zero, 2, -16, -48, 16, false, false, 1,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, -16, 36, false, false, 2,
  (uint8_t)EnemyType::ZeroHollow, 0, 0, 2, 36, false, false, 2,
  (uint8_t)EnemyType::Zero, 2, -16, 32, 74, false, true, 1,
  (uint8_t)EnemyType::Zero, 2, -16, 32, 90, false, true, 1,
};

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Sequences
 *
 *  Format:  [X Position],             - Single step
 *           [Y Position],   
 *           [Direction]        
 *
 *           [X Increment],            - Multiple steps           
 *           [Y Increment],  
 *           [Number of Repeats]       - Must be between 10 and 127
 *
 *  Note:    All sequences should be terminated by the following 0, 0, END_SEQUENCE.
 *           Arduboy screen dimensions 128 x 64.  
 *           Centre of screen is therefore 64 x 32 = position 0, 0.
 *           Top left of screen is therefore 64 x 32 = position 0, 0.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */

const int8_t PROGMEM seq_00[] = {          /* Straight line  */
  64, 0, (int8_t)Direction::West,
  -1, 0, 64,
  -1, 0, 40,
  -1, 0, 80,
  0, 0, END_SEQUENCE,
};

const int8_t PROGMEM seq_01[] = {          /* L from right, going down */ 
  64, 0, (int8_t)Direction::West,
  -1, 0, 37,
  26, 1, (int8_t)Direction::SouthWest,
  -1, 1, 24,
  3, 24, (int8_t)Direction::South,
  0, 1, 25,
  0, 0, END_SEQUENCE,
};

const int8_t PROGMEM seq_02[] = {         /* Diagonal */ 
  64, 0, (int8_t)Direction::SouthWest,
  -1, 1, 80,
  0, 0, END_SEQUENCE,
};

const int8_t PROGMEM seq_03[] = {         /* Straight Down */ 
  64, 0, (int8_t)Direction::South,
  0, 1, 80,
  0, 0, END_SEQUENCE,
};

