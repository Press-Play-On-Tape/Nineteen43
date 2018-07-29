

/* -----------------------------------------------------------------------------------------------------------------------------
 *  End of mission loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifdef ORIENTATION_HORIZONTAL
void endOfMission() {

  uint16_t missionScore = player.getScore();
  uint16_t grandScore = player.getGrandScore();
  uint16_t high = EEPROMReadInt(EEPROM_SCORE + (level * 2));
  
  if (grandScore > high) EEPROMWriteInt(EEPROM_SCORE + (level * 2), grandScore);

  
  for (int16_t i = -60; i < 129; i+=2) {

    arduboy.clear();
    
    arduboy.setCursor(11, 10);
    arduboy.print(F("Mission Successful"));
    arduboy.drawHorizontalDottedLine(9, 118, 7, WHITE);
    arduboy.drawHorizontalDottedLine(9, 118, 19, WHITE);
    gameState = STATE_GAME_INIT;

    arduboy.fillRect(i - 2, 5, 255, 20, BLACK);
    Sprites::drawOverwrite(i + 48, 0, zero_E, 0);
    Sprites::drawOverwrite(i + 30, 18, zero_E, 0);
    Sprites::drawOverwrite(i , 5, p38_0, 0);
  
    arduboy.display();
    delay(5);

  }


  arduboy.setCursor(14, 28);
  arduboy.print(F("Mission Score "));
  if (missionScore < 100) arduboy.print("0");
  if (missionScore < 10)  arduboy.print("0");
  arduboy.print(missionScore);

  arduboy.setCursor(18, 40);
  arduboy.print(F("Total Score "));
  if (grandScore < 100) arduboy.print("0");
  if (grandScore < 10)  arduboy.print("0");
  arduboy.print(grandScore);
  arduboy.display();

  for (uint8_t i = 0; i < 120; ++i) {
    delay(10);
  }
  arduboy.setCursor(34, 55);
  arduboy.print(F("A Continue"));

  
  arduboy.drawHorizontalDottedLine(34, 38, 53, WHITE);
  arduboy.drawHorizontalDottedLine(34, 38, 63, WHITE);

  arduboy.display();

  while (!arduboy.justPressed(A_BUTTON)) {
    arduboy.pollButtons();
    delay(100);
  }

  if (level == 0 && mission == 30) { gameState = STATE_GAME_END_OF_GAME; }
  if (level == 1 && mission == 60) { gameState = STATE_GAME_END_OF_GAME; }
  
}
#endif



/* -----------------------------------------------------------------------------------------------------------------------------
 *  End of mission loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifdef ORIENTATION_HORIZONTAL
void endOfGame() {

  uint16_t playerScore = player.getGrandScore();
  uint16_t high = EEPROMReadInt(EEPROM_SCORE + (level * 2));
  
  arduboy.fillRect(0, 0, WIDTH, HEIGHT, WHITE);  
  Sprites::drawOverwrite((playerScore > 999 ? -3 : 0), 0, p38_3d, 0);
  
  arduboy.setTextBackground(WHITE);
  arduboy.setTextColor(BLACK);
  
  arduboy.setCursor(74, 4);
  arduboy.print(F("Game"));
  arduboy.setCursor(101, 4);
  arduboy.print(F("Over"));
  arduboy.drawHorizontalDottedLine(72, 124, 2, BLACK);
  arduboy.drawHorizontalDottedLine(72, 124, 12, BLACK);
  
  if (playerScore > high) {
    EEPROMWriteInt(EEPROM_SCORE + (level * 2), playerScore);
    high = playerScore;
  }

  if (playerScore > 999 || high > 999) {
    arduboy.setCursor(74, 40);
    arduboy.print(F("Scor"));
    arduboy.setCursor(102, 40);
    if (playerScore < 1000) arduboy.print("0");
  }

  else {
    arduboy.setCursor(76, 40);
    arduboy.print(F("Score"));
    arduboy.setCursor(109, 40);
  }
      
  if (playerScore < 100) arduboy.print("0");
  if (playerScore < 10)  arduboy.print("0");
  arduboy.print(playerScore);
  
  arduboy.setCursor((playerScore > 999 || high > 999 ? 72 : 76), 52);
  arduboy.print(F("High"));
  arduboy.setCursor((playerScore > 999 || high > 999 ? 102 : 109), 52);
  if (high < 100) arduboy.print("0");
  if (high < 10)  arduboy.print("0");
  arduboy.print(high);

  arduboy.display();

  if (arduboy.justPressed(UP_BUTTON) && arduboy.justPressed(DOWN_BUTTON)) { initEEPROM(true); player.setGrandScore(0); }
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_INTRO_INIT;

}
#endif



/* -----------------------------------------------------------------------------------------------------------------------------
 *  End of mission / game loop ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
#ifdef ORIENTATION_VERTICAL
void endOfSequence() {

  uint16_t missionScore = player.getScore();
  uint16_t grandScore = player.getGrandScore();
  uint16_t high = EEPROMReadInt(EEPROM_SCORE + (level * 2));
  
  if (grandScore > high) EEPROMWriteInt(EEPROM_SCORE + (level * 2), grandScore);
  
  for (int16_t i = -20; i < 100; i++) {

    while (!(arduboy.nextFrame())) {}

    if (gameState == STATE_GAME_END_OF_MISSION) {
      Sprites::drawOverwrite(106, 2, mission_successful, 0);
    }
    else {
      Sprites::drawOverwrite(104, 8, game_over, 0);
    }

    arduboy.drawVerticalDottedLine(0, HEIGHT, 102, WHITE);
    arduboy.drawVerticalDottedLine(0, HEIGHT, 127, WHITE);

    arduboy.fillRect(102, i - 18, 127, 200, BLACK);
    Sprites::drawOverwrite(102, i - 18, zero_S, 0);
    Sprites::drawOverwrite(111, i, zero_S, 0);
  
    arduboy.display();

  }

  while (true) {
    
    while (!(arduboy.nextFrame())) {}
    arduboy.pollButtons();

    if (gameState == STATE_GAME_END_OF_MISSION) {
      Sprites::drawOverwrite(106, 2, mission_successful, 0);
    }
    else {
      Sprites::drawOverwrite(104, 8, game_over, 0);
    }

    arduboy.drawVerticalDottedLine(0, HEIGHT, 102, WHITE);
    arduboy.drawVerticalDottedLine(0, HEIGHT, 127, WHITE);


    // Score ..
    {
      Sprites::drawOverwrite(61, 4, score_img, 0);
      uint8_t digits[4] = {};
      extractDigits(digits, missionScore);
      
      for (uint8_t i = 0, y = 56; i < 4; ++i, y -= 6) {
        Sprites::drawSelfMasked(61, y, numbers_vert, digits[i]);
      }
      
    }

    // Total ..
    {
      Sprites::drawOverwrite(48, 4, total_img, 0);
      uint8_t digits[4] = {};
      extractDigits(digits, grandScore);
      
      for (uint8_t i = 0, y = 56; i < 4; ++i, y -= 6) {
        Sprites::drawSelfMasked(48, y, numbers_vert, digits[i]);
      }
      
    }

    Sprites::drawOverwrite(2, 3, aButton_continue, 0);

    arduboy.display(true);


    if (gameState == STATE_GAME_END_OF_MISSION) {
      if (arduboy.justPressed(A_BUTTON)) { gameState = STATE_GAME_INIT; break; }
    }
    else {
      if (arduboy.justPressed(UP_BUTTON) && arduboy.justPressed(DOWN_BUTTON)) { initEEPROM(true); player.setGrandScore(0); }
      if (arduboy.justPressed(A_BUTTON)) { gameState = STATE_INTRO_INIT; break; }

    }

  }

  if (level == 0 && mission == 30) { gameState = STATE_GAME_END_OF_GAME; }
  if (level == 1 && mission == 60) { gameState = STATE_GAME_END_OF_GAME; }

}
#endif



