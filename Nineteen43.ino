#include "Arduboy2Ext.h"
#include <ArduboyTones.h>
#include <EEPROM.h>

#include "Enemy.h"
#include "Player.h"
#include "Obstacle.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "Sequences.h"
#include "Sounds.h"
#include "Enums.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

#include "Images/Images_Enemy.h"
#include "Images/Images_Boat.h"
#include "Images/Images_Explosions.h"
#include "Images/Images_Obstacles.h"
#include "Images/Images_Player.h"
#include "Images/Images_Scoreboard.h"
#include "Images/Images_Splash.h"
#include "Images/Images_Scenery.h"
#include "Images/Images_Arrays.h"

#ifdef HAS_SCENERY
#include "Images/Images_Scenery.h"
#endif

#ifdef ORIENTATION_HORIZONTAL
  Arduboy2Ext arduboy;
#endif

#ifdef ORIENTATION_VERTICAL
  Arduboy2Ext arduboy;
#endif

ArduboyTones sound(arduboy.audio.enabled);

const uint8_t* const missions[] =        { mission_00, mission_01, mission_02, mission_03, mission_04 };
const int8_t* const formations[] =       { formation_00, formation_01, formation_02, formation_03, formation_04, formation_06, formation_05, 
                                           formation_06, formation_07, formation_08, formation_09, formation_10, formation_11, formation_12, 
                                           formation_13 };
const int8_t* const sequences[] =        { seq_00, seq_01, seq_02, seq_03 };

#ifdef ORIENTATION_HORIZONTAL
const uint8_t* const levels[] =          { level_00, level_01, level_02 };
#endif

const int8_t obstacleLaunchDelayInc[] =  { OBSTACLE_LAUNCH_DELAY_INC_L0, OBSTACLE_LAUNCH_DELAY_INC_L1, OBSTACLE_LAUNCH_DELAY_INC_L2 };
const int8_t frameRateInc[] =            { FRAME_RATE_INC_L0, FRAME_RATE_INC_L1, FRAME_RATE_INC_L2 };

const SQ7x8 obstacleBulletsValueDec[] =  { BULLETS_DECREMENT_L0, BULLETS_DECREMENT_L0, BULLETS_DECREMENT_L0 };
const SQ7x8 obstacleFuelValueDec[] =     { FUEL_DECREMENT_L0, FUEL_DECREMENT_L1, FUEL_DECREMENT_L2 };
const SQ7x8 obstacleHealthValueDec[] =   { HEALTH_DECREMENT_L0, HEALTH_DECREMENT_L1, HEALTH_DECREMENT_L2 };

Player player = { player_images };

Enemy enemies[NUMBER_OF_ENEMIES] = {
  { EnemyType::Zero, enemy_zero_images },
  { EnemyType::Zero, enemy_zero_images },
  { EnemyType::Zero, enemy_zero_images },
  { EnemyType::Zero, enemy_zero_images },
  { EnemyType::Zero, enemy_zero_images },
  { EnemyType::Zero, enemy_zero_images },
};

Obstacle obstacle = { -20, 24, fuel, fuel_mask, 0, 0 };

uint8_t playerBulletIdx = 0;
uint8_t enemyBulletIdx = 0;

Bullet playerBullets[PLAYER_BULLETS_MAX];
EnemyBullet enemyBullets[ENEMY_BULLETS_MAX];

const uint8_t scrollIncrement = 2;

uint16_t obstacleLaunchCountdown = OBSTACLE_LAUNCH_DELAY_MIN;
uint8_t enemyShotCountdown = 5;
uint8_t level = 0;
bool showLevel = false;

uint8_t mission = 0;                                        // Mission currently being played
uint8_t missionIdx = 0;                                     // Byte index within current mission
uint8_t mission_formations = 0;                             // Number of formations in the current mission.
uint8_t mission_formations_left = 0;                        // Number of formations left within current mission.
uint8_t formation = 0;
uint8_t gameState = STATE_INTRO_INIT;
int16_t intro;
uint16_t frameRate = INIT_FRAME_RATE;

uint16_t obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
uint16_t obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;

SQ7x8 obstacleBulletsValue = BULLETS_MAX;
SQ7x8 obstacleHealthValue = HEALTH_MAX;
SQ7x8 obstacleFuelValue = FUEL_MAX;

#ifdef HAS_SCENERY

  #define NUMBER_OF_SCENERY_TILES 5
  #define NUMBER_OF_SCENERY_ITEMS 4
  #define SCENERY_TILE_WIDTH 32

  uint8_t sceneryUpper = SCENERY_UPPER_NONE;                  // Defines the current 'trend' for the scenery - NONE, FULL, INCR, DECR
  uint8_t sceneryLower = SCENERY_LOWER_NONE;

  SceneryInfo upperSceneryInfo[NUMBER_OF_SCENERY_TILES];      // Scenery tiles are rendered across the screen with offset.
  SceneryInfo lowerSceneryInfo[NUMBER_OF_SCENERY_TILES];
  uint8_t sceneryOffset;

  SceneryItem sceneryItems[NUMBER_OF_SCENERY_ITEMS];

#endif

#ifdef HAS_SCENERY
void initSceneryItems() {
  sceneryItems[0] = { 128, 20, SceneryElement::Boat};
  sceneryItems[1] = { 176, 25, SceneryElement::Wave1};
  sceneryItems[2] = { 224, 30, SceneryElement::Wave2};
  sceneryItems[3] = { 272, 35, SceneryElement::Wave1};
}
#endif



/* -----------------------------------------------------------------------------------------------------------------------------
 *  Setup
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void setup() {

  initEEPROM(false);
  arduboy.boot();
  arduboy.flashlight(); 
  arduboy.audio.begin();
  
  obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
  obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
  obstacleBulletsValue = BULLETS_MAX;
  obstacleHealthValue = HEALTH_MAX;
  obstacleFuelValue = FUEL_MAX;

  frameRate = INIT_FRAME_RATE;
  level = EEPROM.read(EEPROM_LEVEL);
  
  arduboy.setFrameRate(frameRate);
  arduboy.initRandomSeed();

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Main loop
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.clear();
  arduboy.pollButtons();

  switch (gameState) {

    case STATE_INTRO_INIT:
      introInit();
      break;

    case STATE_INTRO_LOOP:
      introLoop();
      break;

    case STATE_GAME_INIT:
      gameInit();
      break;

    case STATE_GAME_LOOP:
      gameLoop();
      break;

    case STATE_GAME_END_OF_MISSION:
      #ifdef ORIENTATION_HORIZONTAL
        endOfMission();
      #else
        endOfSequence();
      #endif
      break;

    case STATE_GAME_END_OF_GAME:
      #ifdef ORIENTATION_HORIZONTAL
        endOfGame();
      #else
        endOfSequence();
      #endif
      break;

    #ifdef HAS_CREDITS
    case STATE_CREDITS_INIT:
      creditsInit();
      break;

    case STATE_CREDITS_LOOP:
      credits_loop();
      break;
    #endif

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Introduction loop initialisation ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void introInit() {

  mission = 0;                            
  missionIdx = 0;
  intro = -32;
  gameState = STATE_INTRO_LOOP;
  sound.tones(score);
  player.initGame();

  obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
  obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
  obstacleBulletsValue = BULLETS_MAX;
  obstacleHealthValue = HEALTH_MAX;
  obstacleFuelValue = FUEL_MAX;
  
  frameRate = INIT_FRAME_RATE;
  arduboy.setFrameRate(frameRate);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Credits loop initialisation ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifdef HAS_CREDITS
void creditsInit() {

  #ifdef ORIENTATION_HORIZONTAL
    intro = -32;
    arduboy.setTextBackground(BLACK);
    arduboy.setTextColor(WHITE);
  #endif
  
  gameState = STATE_CREDITS_LOOP;

}
#endif


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Credits loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifdef HAS_CREDITS
void credits_loop() {

  #ifdef ORIENTATION_HORIZONTAL

    for (int16_t i = -60; i < 129; i+=2) {
    
      arduboy.clear();
      
      arduboy.setCursor(44, 10);
      arduboy.print(F("Credits"));
      drawHorizontalDottedLine(20, 107, 7, WHITE);
      drawHorizontalDottedLine(20, 107, 19, WHITE);
      gameState = STATE_INTRO_INIT;
      
      arduboy.fillRect(i - 2, 5, 255, 20, BLACK);
      Sprites::drawOverwrite(i + 48, 0, zero_E, 0);
      Sprites::drawOverwrite(i + 30, 18, zero_E, 0);
      Sprites::drawOverwrite(i , 5, p38_0, 0);
      
      arduboy.display();
      delay(5);
      
    }
    
    Sprites::drawOverwrite(14, 28, filmote, 0);
    arduboy.setCursor(38, 30);
    arduboy.print(F("Filmote (Dev)"));
    Sprites::drawOverwrite(15, 41, pharap, 0);
    arduboy.setCursor(38, 44);
    arduboy.print(F("Pharap (Test)"));
    arduboy.display();
    
  #endif

  #ifdef ORIENTATION_VERTICAL

    for (int16_t i = -20; i < 100; i++) {

      while (!(arduboy.nextFrame())) {}

      Sprites::drawOverwrite(116, 12, credits_img, 0);
      arduboy.drawVerticalDottedLine(0, HEIGHT, 110, WHITE);
      arduboy.drawVerticalDottedLine(0, HEIGHT, 127, WHITE);

      arduboy.fillRect(110, i - 18, 127, 200, BLACK);
      Sprites::drawOverwrite(104, i - 18, zero_S, 0);
      Sprites::drawOverwrite(113, i, zero_S, 0);
    
      arduboy.display();
      gameState = STATE_GAME_INIT;

    }  

    Sprites::drawOverwrite(71, 3, filmote, 0);
    Sprites::drawOverwrite(53, 3, pharap, 0);
    Sprites::drawOverwrite(0, 3, aButton_continue, 0);
    
    arduboy.display(true);

  #endif

  while (!arduboy.pressed(A_BUTTON)) {
    delay(100);
  }
 
  gameState = STATE_INTRO_INIT;
  
}
#endif


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Introduction loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void introLoop() {

  // horizontal

  #ifdef ORIENTATION_HORIZONTAL

    Sprites::drawOverwrite(2, 0, title, 0);
    Sprites::drawOverwrite(5, 50, titleLower_Off, 0);
  
    if (arduboy.audio.enabled()) {
      Sprites::drawOverwrite(116, 50, titleLower_On, 0);
    }

    if (showLevel) {

      arduboy.fillRect(72, 50, 64, 14, BLACK);
      Sprites::drawOverwrite(72, 50, level_select, 0);
      Sprites::drawOverwrite(105, 50, levels[level], 0);
      
    }

    if (intro < 136) {

      arduboy.fillRect(intro - 2, 47, 200, 17, BLACK);
      Sprites::drawOverwrite(intro, 47, p38_0, 0);
      intro++;

    }

  #endif

  #ifdef ORIENTATION_VERTICAL

    Sprites::drawOverwrite(86, intro, title, 0);
    Sprites::drawOverwrite(14, 3, aButton, 0);
    
    Sprites::drawOverwrite(55, 70 - intro, zero_S, 0);
    Sprites::drawOverwrite(41, 52 - intro, zero_S, 0);
    Sprites::drawOverwrite(50, 30 - intro, zero_S, 0);

    if (!showLevel) {

      if (arduboy.audio.enabled()) {
        Sprites::drawOverwrite(0, 3, bButton_On, 0);
      }
      else {
        Sprites::drawOverwrite(0, 3, bButton_Off, 0);
      }

    }
    else {

      Sprites::drawOverwrite(0, 3, level_select, 0);
      Sprites::drawOverwrite(2, 37, numbers_vert, (level + 1));

    }

    intro--;
    if (intro < - 130) {
      intro = 80;
    }

  #endif

  arduboy.display(true);

  #ifdef ORIENTATION_HORIZONTAL

    if (arduboy.justPressed(UP_BUTTON)) {

      if (level < 2) level++;
      EEPROM.update(EEPROM_LEVEL, level);
      showLevel = true;

    }

    if (arduboy.justPressed(DOWN_BUTTON)) {

      if (level > 0) level--;
      EEPROM.update(EEPROM_LEVEL, level);
      showLevel = true;

    }

    #ifdef HAS_CREDITS
    if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON)) {

      gameState = STATE_CREDITS_INIT;   

    }
    #endif
    
  #endif

  #ifdef ORIENTATION_VERTICAL

    if (arduboy.justPressed(RIGHT_BUTTON)) {

      if (level < 2) level++;
      EEPROM.update(EEPROM_LEVEL, level);
      showLevel = true;

    }

    if (arduboy.justPressed(LEFT_BUTTON)) {

      if (level > 0) level--;
      EEPROM.update(EEPROM_LEVEL, level);
      showLevel = true;

    }

    #ifdef HAS_CREDITS
    if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {

      gameState = STATE_CREDITS_INIT;   

    }
    #endif

  #endif

  if (arduboy.justPressed(A_BUTTON)) { 
    
    gameState = STATE_GAME_INIT;   
    mission = 0; 
    
  }
  
  if (arduboy.justPressed(B_BUTTON)) {

    showLevel = false;
    if (arduboy.audio.enabled()) {
  
      arduboy.audio.off(); 
  
    }
    else {
  
      arduboy.audio.on(); 
  
    }

    arduboy.audio.saveOnOff();

  }
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Initialize a new game ready for play.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void gameInit() {

  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {
    enemies[i].setEnabled(false);
  }  
  for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {
    playerBullets[i].setEnabled(false);
  }
  for (uint8_t i = 0; i < ENEMY_BULLETS_MAX; ++i) {
    enemyBullets[i].setEnabled(false);
  }
  
  player.initMission();
  obstacle.setEnabled(false);

  sound.tones(mission_start);
  intro = 80;
  gameState = STATE_GAME_LOOP;

  obstacleLaunchDelayMin = OBSTACLE_LAUNCH_DELAY_MIN;
  obstacleLaunchDelayMax = OBSTACLE_LAUNCH_DELAY_MAX;
  obstacleBulletsValue = BULLETS_MAX;
  obstacleHealthValue = HEALTH_MAX;
  obstacleFuelValue = FUEL_MAX;
  
  frameRate = INIT_FRAME_RATE;
  arduboy.setFrameRate(frameRate);

}


#ifdef ORIENTATION_VERTICAL

int16_t groundLowerX = 0;
uint8_t groundLowerY1 = 48;
uint8_t groundLowerY2 = 48;

int16_t sailboatX = 47;
uint8_t sailboatY = 23;

#endif

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Let's play !
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void gameLoop() {

  //arduboy.clear();

  #ifdef ORIENTATION_HORIZONTAL
    uint8_t offsetX = (mission < 9 ? 2 : 0);
    uint8_t offsetL = (mission < 9 ? 93 : 99);
  #endif

  #ifdef ORIENTATION_VERTICAL
    uint8_t offsetY = (mission > 99 ? 0 : (mission > 9 ? 3 : 6));
    uint8_t offsetNumber = (mission > 99 ? 46 : (mission > 9 ? 44 : 41));
    #ifdef HAS_SCENERY
      renderScenery(arduboy.getFrameCount(2));
    #endif
  #endif
  
  switch (intro) {

    case 80: 
      initSceneryItems();

      for (uint8_t x = 0; x < NUMBER_OF_SCENERY_TILES; x++) {
        upperSceneryInfo[x].offset = 0;
        lowerSceneryInfo[x].offset = 0;
      }

    case 2 ... 79:

      #ifdef ORIENTATION_HORIZONTAL

        arduboy.setCursor(37 + offsetX, 29);
        arduboy.setTextBackground(BLACK);
        arduboy.setTextColor(WHITE);
        arduboy.print(F("Mission "));
        arduboy.print(mission + 1);
        drawHorizontalDottedLine(35 + offsetX, offsetL, 26, WHITE);
        drawHorizontalDottedLine(35 + offsetX, offsetL, 38, WHITE);

      #endif

      #ifdef ORIENTATION_VERTICAL

        Sprites::drawOverwrite(60, 2 + offsetY, mission_number, 0);
        if (mission >= 99) Sprites::drawOverwrite(60, offsetNumber, numbers_vert, (mission + 1) / 100);
        if (mission >= 9)  Sprites::drawOverwrite(60, offsetNumber + 6, numbers_vert, ((mission + 1) / 10) % 10);
        Sprites::drawOverwrite(60, offsetNumber + 12, numbers_vert, (mission + 1) % 10);
        arduboy.drawVerticalDottedLine(offsetY, HEIGHT - offsetY, 57, WHITE);
        arduboy.drawVerticalDottedLine(offsetY, HEIGHT - offsetY, 69, WHITE);

      #endif

      intro--;
      break;

    case 1:
      launchMission_FirstFormation(missions[mission % NUMBER_OF_MISSIONS]);
      intro--;
      break;

    case 0:
      break;
      
  }


  if (player.getFuel() > 0 && player.getHealth() > 0 && !player.inRoll()) {
    
    player.decFuel(FUEL_DECREMENT);

    
    // Handle player movement ..
  
    if (arduboy.pressed(UP_BUTTON) && player.getY() > PLAYER_MOVEMENT_INC_UP)                                     { player.setY(player.getY() - PLAYER_MOVEMENT_INC_UP); }
    if (arduboy.pressed(DOWN_BUTTON) && player.getY() < HEIGHT - PLAYER_HEIGHT)                                   { player.setY(player.getY() + PLAYER_MOVEMENT_INC_DOWN); }
    if (arduboy.pressed(LEFT_BUTTON) && player.getX() > PLAYER_MOVEMENT_INC_LEFT)                                 { player.setX(player.getX() - PLAYER_MOVEMENT_INC_LEFT); }
    if (arduboy.pressed(RIGHT_BUTTON) && player.getX() < WIDTH - PLAYER_WIDTH - SCOREBOARD_OUTER_RECT_WIDTH)      { //player.decFuel(FUEL_DECREMENT_BOOST);
                                                                                                                    player.setX(player.getX() + PLAYER_MOVEMENT_INC_RIGHT); }
  
    if (arduboy.justPressed(B_BUTTON))                                                                            { player.startRoll(); }
                                                                                                                    
    if (arduboy.justPressed(A_BUTTON)) {

      if (player.getBullets() > 0) {
        
        if (!sound.playing()) sound.tones(machine_gun_player);

        uint8_t playerX = player.getX().getInteger();
        uint8_t playerY = player.getY().getInteger();

        launchPlayerBullet(playerX, playerY, Direction::East);
        
        if (player.getPowerUp() && player.getBullets() > 1) {
        
          player.decPowerUp();
          launchPlayerBullet(playerX, playerY, Direction::NorthEast);
          launchPlayerBullet(playerX, playerY, Direction::SouthEast);
          
        }

      }
  
    }

    if (!intro) {

      checkForObstacleCollision();
      checkForEnemiesShot();
  
      if (!player.inRoll()) {
  
        checkForPlayerShot();
        checkForPlayerEnemyCollision();
    
      }
  
  
      // Enemy takes a shot?
  
      --enemyShotCountdown;
      if (enemyShotCountdown == 0) {
  
        checkCanEnemyShoot();
  
      }
  
  
      // Should we launch another obstacle?
  
      --obstacleLaunchCountdown;
      if (obstacleLaunchCountdown == 0 && !obstacle.getEnabled()) {

        launchObstacle();
  
      }

    }

  }


  // Render the screen ..

  moveAndRenderPlayerBullets();
  moveAndRenderEnemyBullets();
  moveAndRenderEnemies();
  moveAndRenderObstacle();
  player.renderImage();

  renderScoreboard();
  arduboy.display(true);


  // New wave ?

  if (intro == 0) {
      
    bool newFormation = true;
    for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

      if (enemies[i].getEnabled()) {
        newFormation = false;
        break;
      }

    }
  
    if (newFormation) {

      if (mission_formations_left > 0) {
  
        launchMission_NextFormation(missions[mission % NUMBER_OF_MISSIONS]);
  
      }
      else {

        player.setGrandScore(player.getGrandScore() + player.getScore());
        ++mission;
        intro = 40;
        sound.tones(mission_success);
        gameState = STATE_GAME_END_OF_MISSION;
        renderEndOfMission();
        
      }
  
    }

  }


  // End of life or mission?

  if ((player.getHealth() <= 0 || player.getFuel() <= 0) && !sound.playing()) {
    sound.tones(mission_failure);    
  }
  
  if (!player.getEnabled()) {
    player.setGrandScore(player.getGrandScore() + player.getScore());
    gameState = STATE_GAME_END_OF_GAME;
  }

}



/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch player bullet.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchPlayerBullet(uint8_t x, uint8_t y, Direction direction) {

  playerBullets[playerBulletIdx].setX(x + PLAYER_WIDTH);
  playerBullets[playerBulletIdx].setY(y + PLAYER_HEIGHT_HALF);
  playerBullets[playerBulletIdx].setDirection(direction);
  playerBullets[playerBulletIdx].setEnabled(true);
  player.decBullets();
  ++playerBulletIdx;
  if (playerBulletIdx == PLAYER_BULLETS_MAX) playerBulletIdx = 0;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new obstacle.
 *  
 *  If the player is running out of fuel or health, launch one of those first.
 *  
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchObstacle() {

  ObstacleType type = (ObstacleType)random((uint8_t)ObstacleType::First, (uint8_t)ObstacleType::Count);;
  SQ7x8 minValue = 0;
  SQ7x8 maxValue = 0;
  const uint8_t *bitmap = nullptr;
  const uint8_t *mask = nullptr;


  if (player.getFuel() <= 4) {
    type = ObstacleType::Fuel;
  }
  else if (player.getHealth() <= 4) {
    type = ObstacleType::Health;
  }
  
  switch (type) {

    case ObstacleType::Bullets:
      minValue = (obstacleBulletsValue / (SQ7x8)2);
      maxValue = obstacleBulletsValue;
      bitmap = bullets;
      mask = bullets_mask;
      break;

    case ObstacleType::Fuel:
      minValue = (obstacleFuelValue / (SQ7x8)2);
      maxValue = obstacleFuelValue;
      bitmap = fuel;
      mask = fuel_mask;
      break;

    case ObstacleType::Health:
      minValue = (obstacleHealthValue / (SQ7x8)2);
      maxValue = obstacleHealthValue;
      bitmap = health;
      mask = health_mask;
      break;

    case ObstacleType::PowerUp:
      minValue = (obstacleBulletsValue / (SQ7x8)2);
      maxValue = obstacleBulletsValue;
      bitmap = power_up;
      mask = power_up_mask;
      break;
      
    case ObstacleType::Count:
      break;
     
  }

  obstacle.setObstacleType(type);
  obstacle.setEnabled(true);
  obstacle.setX(WIDTH);
  obstacle.setY(random(0, 54));
  obstacle.setSpeed(randomSFixed<7,8>(1, 2));
  obstacle.setValue(randomSFixed<7,8>(minValue, maxValue));
  obstacle.setBitmap(bitmap);
  obstacle.setMask(mask);

  obstacleLaunchCountdown = random(obstacleLaunchDelayMin, obstacleLaunchDelayMax);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new mission.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchMission_FirstFormation(const uint8_t *mission) {

  missionIdx = 0;

  mission_formations_left = pgm_read_byte(&mission[missionIdx++]);
  mission_formations = mission_formations_left;

  #ifdef HAS_SCENERY
  {
    uint8_t formationPlusScenery = pgm_read_byte(&mission[missionIdx++]);
    formation = formationPlusScenery & SCENERY_MASK_NONE;
    sceneryUpper = formationPlusScenery & SCENERY_MASK_UPPER;
    sceneryLower = formationPlusScenery & SCENERY_MASK_LOWER;
  }
  #else
  formation = pgm_read_byte(&mission[missionIdx++]);
  #endif

  launchFormation(formations[formation]);
  --mission_formations_left;

  frameRate = frameRate + frameRateInc[level];
  obstacleLaunchDelayMin = obstacleLaunchDelayMin + obstacleLaunchDelayInc[level];
  obstacleLaunchDelayMax = obstacleLaunchDelayMax + obstacleLaunchDelayInc[level];

  if (obstacleBulletsValue > BULLETS_MIN)  obstacleBulletsValue = obstacleBulletsValue - obstacleBulletsValueDec[level];
  if (obstacleHealthValue > HEALTH_MIN)    obstacleHealthValue = obstacleHealthValue - obstacleHealthValueDec[level];
  if (obstacleFuelValue > FUEL_MIN)        obstacleFuelValue = obstacleFuelValue - obstacleFuelValueDec[level];

  arduboy.setFrameRate(frameRate);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch the next formation in a mission.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchMission_NextFormation(const uint8_t *mission) {
  
  #ifdef HAS_SCENERY
  {
    uint8_t formationPlusScenery = pgm_read_byte(&mission[missionIdx++]);
    formation = formationPlusScenery & SCENERY_MASK_NONE;
    sceneryUpper = formationPlusScenery & SCENERY_MASK_UPPER;
    sceneryLower = formationPlusScenery & SCENERY_MASK_LOWER;
  }
  #else
  formation = pgm_read_byte(&mission[missionIdx++]);
  #endif

  launchFormation(formations[formation]);
  --mission_formations_left;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Launch a new obstacle.
 *  
 *  If the player is running out of fuel or health, launch one of those first.
 *  
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void launchFormation(const int8_t *formation) {

  int16_t enemyIdx = 0;


  // Disable all enemies ..

  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {
    enemies[i].setEnabled(false);
  }


  // Retrieve number of enemies to launch ..

  uint8_t number_of_enemies = pgm_read_byte(&formation[enemyIdx]);
  ++enemyIdx;

  for (uint8_t i = 0; i < number_of_enemies; ++i) {

    switch ((EnemyType)pgm_read_byte(&formation[enemyIdx])) {

      case EnemyType::Zero:
        enemies[i] = { EnemyType::Zero, enemy_zero_images };
        break;

      case EnemyType::ZeroHollow:
        enemies[i] = { EnemyType::ZeroHollow, enemy_zero_hollow_images };
        break;

      case EnemyType::Bomber:
        enemies[i] = { EnemyType::Bomber, enemy_medium_images };
        break;

      case EnemyType::Boat:
        enemies[i] = { EnemyType::Boat, enemy_boat };
        enemies[i].setTurretDirection(Direction::SouthWest);
        break;

      case EnemyType::Count:
        break;
      
    }

    enemies[i].setStartingPos(sequences[pgm_read_byte(&formation[enemyIdx + 1])]);
    enemies[i].setOffsetX(pgm_read_byte(&formation[enemyIdx + 2]));
    enemies[i].setOffsetY(pgm_read_byte(&formation[enemyIdx + 3]));
    enemies[i].setDelayStart(pgm_read_byte(&formation[enemyIdx + 4]));
    enemies[i].setInvertX((pgm_read_byte(&formation[enemyIdx + 5]) == 1));
    enemies[i].setInvertY((pgm_read_byte(&formation[enemyIdx + 6]) == 1));
    enemies[i].setHealth(pgm_read_byte(&formation[enemyIdx + 7]));
    enemies[i].setEnabled(true);
    enemies[i].move();

    enemyIdx = enemyIdx + 8;

  }

};


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Is the nominated enemy facing the player?
 * -----------------------------------------------------------------------------------------------------------------------------
 */
bool isAimingAtPlayer(uint8_t enemyIdx) {
  
  int16_t playerX = player.getX().getInteger() + PLAYER_WIDTH_HALF;
  int16_t playerY = player.getY().getInteger() + PLAYER_HEIGHT_HALF;

  int16_t enemyX = enemies[enemyIdx].getX().getInteger() + (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_X : enemies[enemyIdx].getWidth() / 2);
  int16_t enemyY = enemies[enemyIdx].getY().getInteger() + (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_Y : enemies[enemyIdx].getHeight() / 2);

  int16_t deltaX = playerX - enemyX;
  int16_t deltaY = playerY - enemyY;

  SQ15x16 heading = AIM_Q2_MAX;
  if (deltaX != 0) heading = abs((SQ15x16)deltaY / (SQ15x16)deltaX);

  Direction direction = (enemies[enemyIdx].getEnemyType() == EnemyType::Boat ? enemies[enemyIdx].getTurretDirection() : enemies[enemyIdx].getDirection());

  if (deltaY <= 0) {
    
    if (deltaX > 0) {
  
            if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::East)             return true;
       else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::NorthEast)        return true;
  
    }
    
    if (deltaX < 0) {
  
           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::West)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::NorthWest)         return true;
  
    }
  
    if (heading >= AIM_Q2_MIN && heading <= AIM_Q2_MAX && direction == Direction::North)                    return true;

  }
  
  if (deltaY > 0) {
    
    if (deltaX > 0) {

           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::East)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::SouthEast)         return true;
    
    }
    
    if (deltaX < 0) {
  
           if (heading >= AIM_Q0_MIN && heading <= AIM_Q0_MAX && direction == Direction::West)              return true;
      else if (heading >= AIM_Q1_MIN && heading <= AIM_Q1_MAX && direction == Direction::SouthWest)         return true;
      
    }
    
    if (heading >= AIM_Q2_MIN && heading <= AIM_Q2_MAX && direction == Direction::South)                    return true;
  
  }
  
  return false;
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Rotate the boat's turret to aim at the player.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
Direction aimAtPlayer(uint8_t enemyIdx) {

  int16_t playerX = player.getX().getInteger() + PLAYER_WIDTH_HALF;
  int16_t playerY = player.getY().getInteger() + PLAYER_HEIGHT_HALF;

  int16_t enemyX = enemies[enemyIdx].getX().getInteger() + ENEMY_BOAT_TURRENT_X + 5;
  int16_t enemyY = enemies[enemyIdx].getY().getInteger() + (enemies[enemyIdx].getHeight() / 2);

  int16_t deltaX = playerX - enemyX;
  int16_t deltaY = playerY - enemyY;

  SQ15x16 heading = (SQ15x16)ROTATE_Q2_MAX;
  if (deltaX != 0) heading = abs((SQ15x16)deltaY / (SQ15x16)deltaX);

  Direction aimDirection = getAimDirection(deltaX, deltaY, heading);
  Direction curDirection = enemies[enemyIdx].getTurretDirection();

  if (aimDirection != Direction::None && aimDirection != curDirection) {
  
    if (aimDirection > curDirection) ++curDirection;
    if (aimDirection < curDirection) --curDirection;
  
  }
  
  return curDirection;

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Get the direction to turn turret towards.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
Direction getAimDirection(int8_t deltaX, int8_t deltaY, SQ15x16 heading) {

  if (deltaY <= 0) {

    if (deltaX > 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::East;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::NorthEast;

    }
      
    if (deltaX < 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::West;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::NorthWest;
  
    }

    if (heading > ROTATE_Q2_MIN && heading < ROTATE_Q2_MAX)        return Direction::North;
  
  }

  if (deltaY > 0) {
    
    if (deltaX > 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::East;      
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::SouthEast;
      
    }
  
    if (deltaX < 0) {
  
      if (heading > ROTATE_Q0_MIN && heading < ROTATE_Q0_MAX)      return Direction::West;
      if (heading > ROTATE_Q1_MIN && heading < ROTATE_Q1_MAX)      return Direction::SouthWest;
      
    }

    if (heading > ROTATE_Q2_MIN && heading <= ROTATE_Q2_MAX)       return Direction::South;

  }

  return Direction::None;
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has run over an obstacle.  If so, process the action ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForObstacleCollision(){

  if (obstacle.getEnabled()) {

    if (arduboy.collide(player.getRect(), obstacle.getRect())) {

      switch (obstacle.getObstacleType()) {

        case ObstacleType::Fuel:
          player.setFuel(obstacle.getValue());
          break;

        case ObstacleType::Bullets:
          player.addBullets(obstacle.getValue().getInteger());
          break;

        case ObstacleType::Health:
          player.addHealth(obstacle.getValue());
          break;

        case ObstacleType::PowerUp:
          player.setPowerUp(true);
          player.addBullets(obstacle.getValue().getInteger());
          break;

        case ObstacleType::Count:
          break;

      }

      obstacle.setEnabled(false);
      if (!sound.playing()) sound.tones(collect_obstacle);

    }

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has shot and enemy.  Reduce the enemy's health and if this kills the enemy, increase the score.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForEnemiesShot() {
  
  for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {

    if (playerBullets[i].getEnabled()) {
  
      Point bulletPoint = { playerBullets[i].getX(), playerBullets[i].getY() };
      
      for (uint8_t j = 0; j < NUMBER_OF_ENEMIES; ++j) {
  
        if (enemies[j].getEnabled() && enemies[j].getHealth() > 0) {
  
          if (arduboy.collide(bulletPoint, enemies[j].getRect())) {
  
            playerBullets[i].setEnabled(false);
            enemies[j].decHealth((SQ7x8)1.0);
  
            if (enemies[j].getHealth().getInteger() == 0) player.setScore(player.getScore() + 1);
            if (!sound.playing()) sound.tones(hit_by_bullets);

            break;
  
          }
  
        }
  
      }
  
    }
  
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player has been shot by an enemy bullet.  If so, reduce the player's health ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForPlayerShot() {
  
  Rect playerRect = player.getRect();
  
  for (uint8_t i = 0; i < ENEMY_BULLETS_MAX; ++i) {
  
    if (enemyBullets[i].getEnabled()) {
  
      Point bulletPoint = { enemyBullets[i].getX(), enemyBullets[i].getY() };
    
      if (arduboy.collide(bulletPoint, playerRect)) {
  
        /* TODO: Could add logic to make wing hit different to body hit */
  
        enemyBullets[i].setEnabled(false);
        player.decHealth(PLAYER_HIT_BULLET_DECREMENT);
        break;
  
      }
  
    }
  
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if the player and an enemy have collided.  If so, reduce the player's health ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkForPlayerEnemyCollision() {

  Rect playerRect = player.getRect(); 

  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

    if (enemies[i].getEnabled()) {

      if (enemies[i].getEnemyType() != EnemyType::Boat && arduboy.collide(playerRect, enemies[i].getRect())) {

        player.decHealth(PLAYER_HIT_PLANE_DECREMENT);
        if (!sound.playing()) sound.tones(hit_by_plane);
        
        break;

      }

    }

  }
      
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Check to see if an active enemy is point towards the player, if so fire a bullet ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void checkCanEnemyShoot() {
  
  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {

    Rect enemyRect = enemies[i].getRect();
    
    if (enemies[i].getEnabled() && enemies[i].getDelayStart() == 0 && enemies[i].getNumberOfBulletsFired() > 0
                                && enemyRect.x + enemyRect.width > 0 && enemyRect.x < WIDTH 
                                && enemyRect.y + enemyRect.height >= 0 && enemyRect.y < HEIGHT ) {
  
      if (isAimingAtPlayer(i)) {
  
        /* TODO: Bullets are shot from the center of the enemy .. probably should look to add code to move to the correct side */

        enemies[i].decNumberOfBulletsFired();
  
        enemyBullets[enemyBulletIdx].setX(enemies[i].getX().getInteger() + (enemies[i].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_X : (enemies[i].getWidth() / 2)));
        enemyBullets[enemyBulletIdx].setY(enemies[i].getY().getInteger() + (enemies[i].getEnemyType() == EnemyType::Boat ? ENEMY_BOAT_TURRENT_CENTER_Y : (enemies[i].getHeight() / 2)));
        enemyBullets[enemyBulletIdx].setDirection(enemies[i].getEnemyType() == EnemyType::Boat ? enemies[i].getTurretDirection() : enemies[i].getDirection());
        enemyBullets[enemyBulletIdx].setEnabled(true);
        
        ++enemyBulletIdx;
        if (enemyBulletIdx == ENEMY_BULLETS_MAX) enemyBulletIdx = 0;

        if (!sound.playing()) sound.tones(machine_gun_enemy);

        break;
  
      }
  
    }
  
  }


  // Reset the countdown to induce a delay before the next shot is fired ..
  
  enemyShotCountdown = random(10, 20);

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the player's active bullets ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderPlayerBullets() {
  
  for (uint8_t i = 0; i < PLAYER_BULLETS_MAX; ++i) {
   
    playerBullets[i].move();
    
    if (playerBullets[i].getEnabled()) {
      arduboy.drawPixel(playerBullets[i].getX(), playerBullets[i].getY(), WHITE);
    }
    
  }
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the enemies' active bullets ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderEnemyBullets() {
  
  for (uint8_t i = 0; i < ENEMY_BULLETS_MAX; ++i) {
    
    enemyBullets[i].move();
    
    if (enemyBullets[i].getEnabled()) {
      arduboy.drawPixel(enemyBullets[i].getX(), enemyBullets[i].getY(), WHITE);
    }
    
  }
  
}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the obstacle if it is active ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderObstacle() {
  
  if (obstacle.getEnabled()) {

    obstacle.move();
    obstacle.renderImage();
    
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move and render the active enemies ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void moveAndRenderEnemies() {
  
  for (uint8_t i = 0; i < NUMBER_OF_ENEMIES; ++i) {
    
    enemies[i].move();
    
    Rect enemyRect = enemies[i].getRect();
    
    if (enemies[i].getEnabled() && enemies[i].getDelayStart() == 0
                                && enemyRect.x + enemyRect.width > 0 && enemyRect.x < WIDTH 
                                && enemyRect.y + enemyRect.height >= 0 && enemyRect.y < HEIGHT ) {

      if (enemies[i].getEnemyType() == EnemyType::Boat) {

        enemies[i].setTurretDirection(aimAtPlayer(i));

      }

      enemies[i].renderImage();
      
    }
    
  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Move player off the screen ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void renderEndOfMission() {

  for (uint8_t i = 0; i < 128; ++i) {

//    arduboy.clear();

    #ifdef ORIENTATION_VERTICAL
    renderScenery((arduboy.getFrameCount(2) + i) % 2);
    #endif

#ifndef PLANES_HAVE_BORDERS
    Sprites::drawOverwrite(player.getX().getInteger() + i, player.getY().getInteger(), p38_0, 0);
#else
    Sprites::drawExternalMask(player.getX().getInteger() + i, player.getY().getInteger(), p38_0, p38_mask_0, 0, 0);
#endif
    renderScoreboard();
    arduboy.display(true);
    delay(20);

  }

}


/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render the score board.
 * -----------------------------------------------------------------------------------------------------------------------------
 */
uint16_t scoreFrameCnt;
uint16_t scoreFlash;

void renderScoreboard() {


  // Increment the frame count

  ++scoreFlash;        if (scoreFlash > SCOREBOARD_FLASH_MAX) { scoreFlash = 0; }
  ++scoreFrameCnt;     if (scoreFrameCnt > (SCOREBOARD_FRAME_COUNT_MAX * SCOREBOARD_NUMBER_OF_FRAMES)) { scoreFrameCnt = 0; }

  uint16_t player_score = player.getScore();


  // Clear the board space ..

  arduboy.fillRect(SCOREBOARD_OUTER_RECT_X, SCOREBOARD_OUTER_RECT_Y, SCOREBOARD_OUTER_RECT_WIDTH, SCOREBOARD_OUTER_RECT_HEIGHT, BLACK);
  arduboy.drawLine(SCOREBOARD_INNER_RECT_X, SCOREBOARD_INNER_RECT_Y, SCOREBOARD_INNER_RECT_X, HEIGHT, WHITE);

    
  // Render kills ..
  
  Sprites::drawOverwrite(SCOREBOARD_KILLS_X, SCOREBOARD_KILLS_ICON_Y, kills_gauge, 0);
  Sprites::drawOverwrite(SCOREBOARD_KILLS_X, SCOREBOARD_KILLS_DIGIT_0_Y, digits[player_score / 100], 0);
  player_score = player_score - (player_score / 100) * 100;
  Sprites::drawOverwrite(SCOREBOARD_KILLS_X, SCOREBOARD_KILLS_DIGIT_1_Y, digits[player_score / 10], 0);
  Sprites::drawOverwrite(SCOREBOARD_KILLS_X, SCOREBOARD_KILLS_DIGIT_2_Y, digits[player_score % 10], 0);

  switch (scoreFrameCnt / SCOREBOARD_FRAME_COUNT_MAX) {

    case 0:

      renderScoreboadGauge(SCOREBOARD_HEALTH_BAR_X, SCOREBOARD_HEALTH_BAR_Y, health_gauge, SCOREBOARD_HEALTH_BAR_TOP, (player.getHealth() < 0 ? 0 : player.getHealth().getInteger()));
      renderScoreboadGauge(SCOREBOARD_FUEL_BAR_X, SCOREBOARD_FUEL_BAR_Y, fuel_gauge, SCOREBOARD_FUEL_BAR_TOP, (player.getFuel() < 0 ? 0 : player.getFuel().getInteger()));
      break;

    case 1:

      if (player.getPowerUp() && scoreFlash >= (SCOREBOARD_FLASH_MAX / 2)) {
        Sprites::drawOverwrite(SCOREBOARD_BULLET_PU_X, SCOREBOARD_BULLET_PU_Y, power_up_gauge, 0);
      }

      renderScoreboadGauge(SCOREBOARD_BULLET_BAR_X, SCOREBOARD_BULLET_BAR_Y, bullets_gauge, SCOREBOARD_BULLET_BAR_TOP, (player.getBullets() / 3));
      
      break;

  }

}


/* ----------------------------------------------------------------------------
 *  Render score board gauge. 
 * ----------------------------------------------------------------------------
 */
void renderScoreboadGauge(const uint8_t imageX, const uint8_t imageY, const uint8_t *image, const uint8_t scoreboardY, const uint8_t value) {

  Sprites::drawOverwrite(imageX, imageY, image, 0);
  if ((value <= 4 && scoreFlash >= (SCOREBOARD_FLASH_MAX / 2)) || value > 4) {
    for (uint8_t i = 0; i < (value); i += 2) {
      arduboy.drawLine(imageX, scoreboardY + i, WIDTH, scoreboardY + i);
    }
  }

}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 *  
 *  So much nicer than a solid line!
 * ----------------------------------------------------------------------------
 */
void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour) {
/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised? 
 *   
 *   Looks for the characters '4' and '3' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 * ----------------------------------------------------------------------------
 */
void initEEPROM(bool forceOverwrite) {

  uint8_t c1 = EEPROM.read(EEPROM_START_C1);
  uint8_t c2 = EEPROM.read(EEPROM_START_C2);

  if (c1 != 52 || c2 != 51 || forceOverwrite) { 
  
    uint16_t score = 0;
    EEPROMWriteInt(EEPROM_START_C1, 52);
    EEPROMWriteInt(EEPROM_START_C2, 51);
    EEPROMWriteInt(EEPROM_SCORE, 0);
    EEPROMWriteInt(EEPROM_SCORE + 2, score);
    EEPROMWriteInt(EEPROM_SCORE + 4, score);
    EEPROMWriteInt(EEPROM_SCORE + 6, score);
    
  }

}



#ifdef HAS_SCENERY

#define SCENERY_TILE_DECR 1
#define SCENERY_TILE_FLAT_BEGIN 2
#define SCENERY_TILE_FLAT_END 6
#define SCENERY_TILE_INCR 7

#define SCENERY_LOWER_OFFSET_MIN -20    
#define SCENERY_LOWER_OFFSET_MIN_PLUS_INC SCENERY_LOWER_OFFSET_MIN + 4       
#define SCENERY_LOWER_OFFSET_MAX_MINUS_INC 20       
#define SCENERY_LOWER_OFFSET_MAX SCENERY_LOWER_OFFSET_MAX_MINUS_INC + 4

#define SCENERY_UPPER_OFFSET_MIN -24    
#define SCENERY_UPPER_OFFSET_MIN_PLUS_INC SCENERY_UPPEROFFSET_MIN + 4       
#define SCENERY_UPPER_OFFSET_MAX_MINUS_INC 20       
#define SCENERY_UPPER_OFFSET_MAX SCENERY_UPPER_OFFSET_MAX_MINUS_INC + 4

const int8_t lower_offsets[] = { -4, -4, 0,   0, 0, 4,    4, 0, 4 };
const int8_t upper_offsets[] = { -4, 0, 0,   -4, 0, 0,    4, 4, 4 };


void renderScenery(uint8_t frame) {


    // Draw scenery elements ..

    for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

      switch (sceneryItems[x].element) {

        case SceneryElement::Wave1:
          Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, wave_01, 0);
          break;
        case SceneryElement::Wave2:
          Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, wave_02, 0);
          break;

        case SceneryElement::Boat:
          Sprites::drawSelfMasked(sceneryItems[x].x, sceneryItems[x].y, sail_boat, 0);
          break;

      }

    }


    // Draw ground ..

    for (uint8_t x = 0; x < NUMBER_OF_SCENERY_TILES; x++) {

      if (lowerSceneryInfo[x].tile > 0) {

        Sprites::drawSelfMasked(-sceneryOffset + (SCENERY_TILE_WIDTH * x), upperSceneryInfo[x].offset - 28, pgm_read_word_near(&upper_scenery_images[upperSceneryInfo[x].tile]), 0);
        Sprites::drawSelfMasked(-sceneryOffset + (SCENERY_TILE_WIDTH * x), HEIGHT + lowerSceneryInfo[x].offset, pgm_read_word_near(&lower_scenery_images[lowerSceneryInfo[x].tile]), 0);

      }

    }
  
  

    if (frame == 0) {


      // Update scenery element positions ..

      for (uint8_t x = 0; x < NUMBER_OF_SCENERY_ITEMS; x++) {

        sceneryItems[x].x--;

        if (sceneryItems[x].x < -48 && gameState != STATE_GAME_END_OF_MISSION) {

          sceneryItems[x].x = 144;
          sceneryItems[x].y = random( 
                                      clamp(static_cast<int8_t>(4 + upperSceneryInfo[NUMBER_OF_SCENERY_ITEMS - 1].offset), static_cast<int8_t>(0), static_cast<int8_t>(32)), 
                                      clamp(static_cast<int8_t>(64 + lowerSceneryInfo[NUMBER_OF_SCENERY_ITEMS - 1].offset), static_cast<int8_t>(32), static_cast<int8_t>(48)) 
                                    );
          sceneryItems[x].element = static_cast<SceneryElement>(random(0, 3));

        }

      }


      // Update ground positions ..

      sceneryOffset++;

      if (sceneryOffset == SCENERY_TILE_WIDTH) {
        

        // Shuffle the scenery across ..
Serial.print(sceneryLower);
Serial.print(" [");
        sceneryOffset = 0;
        for (uint8_t x = 0; x < NUMBER_OF_SCENERY_TILES - 1; x++) {
          upperSceneryInfo[x] = upperSceneryInfo[x + 1];
          lowerSceneryInfo[x] = lowerSceneryInfo[x + 1];
Serial.print(lowerSceneryInfo[x].offset);
Serial.print(",");
Serial.print(lowerSceneryInfo[x].tile);
Serial.print(" ");
        }
  
Serial.print("] ");
        

        // Choose a new lower tile ..

        uint8_t updateTile = NUMBER_OF_SCENERY_TILES - 1;

        {
          uint8_t minimum = 0;
          uint8_t maximum = 0;
          int8_t offset = lowerSceneryInfo[updateTile].offset;
  
          switch (sceneryLower) {

            case SCENERY_LOWER_NONE:
            case SCENERY_LOWER_INCR:
              if (offset <= SCENERY_LOWER_OFFSET_MAX_MINUS_INC) {
                minimum = SCENERY_TILE_INCR;
                maximum = SCENERY_TILE_INCR + 1;
              }
              else {
                minimum = SCENERY_TILE_FLAT_BEGIN;
                maximum = SCENERY_TILE_FLAT_END + 1;
              }
              break;

            case SCENERY_LOWER_DECR:
              if (offset >= SCENERY_LOWER_OFFSET_MIN_PLUS_INC) {
                minimum = SCENERY_TILE_DECR;
                maximum = SCENERY_TILE_DECR + 1;
              }
              else {
                minimum = SCENERY_TILE_FLAT_BEGIN;
                maximum = SCENERY_TILE_FLAT_END + 1;
              }            
              break;

            case SCENERY_LOWER_RAND:
              minimum = (offset >= SCENERY_LOWER_OFFSET_MIN_PLUS_INC  ? SCENERY_TILE_DECR : SCENERY_TILE_FLAT_BEGIN);
              maximum = (offset <= SCENERY_LOWER_OFFSET_MAX_MINUS_INC ? SCENERY_TILE_INCR : SCENERY_TILE_FLAT_END) + 1;
              break;

          }

          uint8_t newTile = random(minimum, maximum);
          lowerSceneryInfo[updateTile].tile = newTile;
          lowerSceneryInfo[updateTile].offset = offset + lower_offsets[getOffsetsIndex(newTile, lowerSceneryInfo[updateTile - 1].tile)];

       

          switch (newTile) {

            case SCENERY_TILE_INCR:
              switch (lowerSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
                default:
  Serial.print(".c0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
              }
              break;

            case SCENERY_TILE_DECR:
              switch (upperSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset - 4;
                  break;
                default:
  Serial.print(".c1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset - 0;
                  break;
              }
              break;

            default:
              switch (upperSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a2.");
 //               upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b2.");
 //                 upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset -4;
                  break;
                default:
  Serial.print(".c2.");
 //                 upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
              }
              break;
          }



        }
        

        // Choose a new upper tile ..

        {
          uint8_t minimum = 0;
          uint8_t maximum = 0;
          int8_t offset = upperSceneryInfo[updateTile].offset;
  
          switch (sceneryUpper) {

            case SCENERY_UPPER_INCR:
              if (offset <= SCENERY_UPPER_OFFSET_MAX_MINUS_INC) {
                minimum = SCENERY_TILE_INCR;
                maximum = SCENERY_TILE_INCR + 1;
              }
              else {
                minimum = SCENERY_TILE_FLAT_BEGIN;
                maximum = SCENERY_TILE_FLAT_END + 1;
              }
              break;

            case SCENERY_UPPER_NONE:
            case SCENERY_UPPER_DECR:
              if (offset >= SCENERY_LOWER_OFFSET_MIN_PLUS_INC) {
                minimum = SCENERY_TILE_DECR;
                maximum = SCENERY_TILE_DECR + 1;
              }
              else {
                minimum = SCENERY_TILE_FLAT_BEGIN;
                maximum = SCENERY_TILE_FLAT_END + 1;
              }            
              break;

            case SCENERY_UPPER_RAND:
              minimum = (offset >= SCENERY_LOWER_OFFSET_MIN_PLUS_INC  ? SCENERY_TILE_DECR : SCENERY_TILE_FLAT_BEGIN);
              maximum = (offset <= SCENERY_LOWER_OFFSET_MAX_MINUS_INC ? SCENERY_TILE_INCR : SCENERY_TILE_FLAT_END) + 1;
              break;

          }
  // Serial.print(" min: ");
  // Serial.print(minimum);
  // Serial.print(",max: ");
  // Serial.print(maximum);
  // Serial.print(", rand ");
          uint8_t newTile = random(minimum, maximum);
  // Serial.print(newTile);
  // Serial.print(" ");



          upperSceneryInfo[updateTile].tile = newTile;
          upperSceneryInfo[updateTile].offset = offset + upper_offsets[getOffsetsIndex(newTile, upperSceneryInfo[updateTile - 1].tile)];
        
/*
          switch (newTile) {

            case SCENERY_TILE_INCR:
              switch (upperSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
                default:
  Serial.print(".c0.");
//                  upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 4;
                  break;
              }
              break;

            case SCENERY_TILE_DECR:
              switch (upperSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset - 4;
                  break;
                default:
  Serial.print(".c1.");
  //                upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset - 0;
                  break;
              }
              break;

            default:
              switch (upperSceneryInfo[updateTile - 1].tile) {
                case SCENERY_TILE_INCR:
  Serial.print(".a2.");
 //               upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
                case SCENERY_TILE_DECR:
  Serial.print(".b2.");
 //                 upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset -4;
                  break;
                default:
  Serial.print(".c2.");
 //                 upperSceneryInfo[updateTile].offset = upperSceneryInfo[updateTile].offset + 0;
                  break;
              }
              break;
          }
*/
        }


// Serial.println(upperSceneryInfo[updateTile].offset);

      }

    }

}


uint8_t getOffsetsIndex(uint8_t newTile, uint8_t oldTile) {

  uint8_t index = 0;

  switch (newTile) {

    case SCENERY_TILE_INCR:   
      index = 6;
      break;

    case SCENERY_TILE_DECR:   
      index = 0;
      break;

    default:                  
      index = 3;
      break;

  }

  switch (oldTile) {

    case SCENERY_TILE_INCR:   
      index = index + 2;
      break;

    case SCENERY_TILE_DECR:   
      break;

    default:                  
      index = index + 1;
      break;

  }

  return index;

}
#endif


/* ----------------------------------------------------------------------------
 * Write a 2 byte integer to the EEPROM at the specified address ..
 * ----------------------------------------------------------------------------
 */
void EEPROMWriteInt(int address, int value) {
  
  uint8_t lowByte = ((value >> 0) & 0xFF);
  uint8_t highByte = ((value >> 8) & 0xFF);
  
  EEPROM.write(address, lowByte);
  EEPROM.write(address + 1, highByte);

}


/* ----------------------------------------------------------------------------
 * Read a 2 byte integer from the EEPROM at the specified address ..
 * ----------------------------------------------------------------------------
 */
uint16_t EEPROMReadInt(int address) {
  
  uint8_t lowByte = EEPROM.read(address);
  uint8_t highByte = EEPROM.read(address + 1);
  
  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);

}