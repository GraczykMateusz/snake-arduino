#include <FastLED.h>

//Leds number
#define NUM_LEDS 256

//Controller input
#define X_PIN      A5
#define Y_PIN      A6
#define SW_PIN     A7

//Data to WS2812b panel
#define DATA_PIN    3

#define LED_OFFSET 32

#define COLOR CRGB::Blue
#define FRUINT_COLOR CRGB::Red

//Snake's movement
bool left;
bool right;
bool up;
bool down;

//Fruit
int fruit_position_X;
bool is_spawn_fruit;

//The array of leds. One item for each led in strip.
CRGB leds[NUM_LEDS];

//Needed to start game correctly
bool is_start_game = false; 
bool is_only_once  = true;
bool is_start_display = false;

//Wrong SW_PIN input defender
bool is_button_pressed = false;

//Game 
unsigned int head_position_X;
unsigned int head_position_Y;

int position_X;
int old_position_X;

//Keep current leds to display
bool read_value[NUM_LEDS];

const bool start[NUM_LEDS] PROGMEM = {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool three[NUM_LEDS] PROGMEM = {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool two[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,
                                      0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,
                                      0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
                                      0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                                      0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool one[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool out[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,
                                      0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,
                                      0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                      0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,
                                      1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

void check_exit() {
  Serial.println("checking");
  if(is_button_pressed) {
    is_start_game = false;
    is_only_once = true;
    is_button_pressed = false;
    Serial.println("EXIT");
    
    for(int i = 0; i < NUM_LEDS; ++i) {
      read_value[i] = pgm_read_byte(&out[i]);
    }
    displayer(read_value);
    
    delay(3000);
  }
}

/*--------> INPUT CONTROLLER / TIMER <--------*/
void inputController() {
  if(analogRead(X_PIN) < 50) { //Left
    left  = true;
    right = false;
    up    = false;
    down  = false;
  }
  if(analogRead(X_PIN) > 972) { //Right
    left  = false;
    right = true;
    up    = false;
    down  = false;
  }
  if(analogRead(Y_PIN) < 50) { //Up
    left  = false;
    right = false;
    up    = true;
    down  = false;
  }
  if(analogRead(Y_PIN) > 972) { //Down
    left  = false;
    right = false; 
    up    = false;
    down  = true;
  } 
  if(analogRead(SW_PIN) <  30  &&  
     analogRead(X_PIN)  >  411 &&
     analogRead(X_PIN)  <  611 &&
     analogRead(Y_PIN)  >  411 &&
     analogRead(Y_PIN)  <  611) {
    is_button_pressed = true;
  } else {
    is_button_pressed = false;
  }
}

/*----------------> DISPLAY <----------------*/
void displayer(bool* turnOnOffLeds) {
  FastLED.clear();
  
  int row = -1;
  
  for(int i = 0; i < NUM_LEDS; ++i) { 
    if(i % 16 == 0) {
      ++row;
    }
    if(turnOnOffLeds[i] && row % 2 == 0) {
      if(row == 0) {
        leds[15-i] = COLOR;
      } else if(row == 2) {
        leds[47 - i + LED_OFFSET] = COLOR;
      } else if(row == 4) {
        leds[79 - i + 2 * LED_OFFSET] = COLOR;
      } else if(row == 6) {
        leds[111 - i + 3 * LED_OFFSET] = COLOR;
      } else if(row == 8) {
        leds[143 - i + 4 * LED_OFFSET] = COLOR;
      } else if(row == 10) {
        leds[175 - i + 5 * LED_OFFSET] = COLOR;
      } else if(row == 12) {
        leds[207 - i + 6 * LED_OFFSET] = COLOR;
      } else if(row == 14) {
        leds[239 - i + 7 * LED_OFFSET] = COLOR;
      }
    } else if(turnOnOffLeds[i]){
      leds[i] = COLOR;
    }
  }
  row = 0;

  FastLED.show();
}

void start_display() {
  is_start_display = true;
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&start[i]);
  }
  displayer(read_value);
}

void starting_timer_display() {
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&three[i]);
  }
  displayer(read_value);

  delay(1000);
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&two[i]);
  }
  displayer(read_value);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&one[i]);
  }
  displayer(read_value);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = 0;
  }
}

/*----------------> GAMEPLAY <---------------*/
void gameplay() {
  move_snake();
  read_value[old_position_X] = 0;
  
  position_X = convert_XY_to_X(head_position_X, head_position_Y);
  read_value[position_X] = 1;

  displayer(read_value);

  if(position_X == fruit_position_X) {
    generate_fruit_position();
  }
    
  delay(100);

  old_position_X = position_X;
}

void move_snake() {
  if(left) {
    if(head_position_X > 0)
      --head_position_X;
  }
  else if(right) {
    if(head_position_X < 15)
      ++head_position_X;
  }
  else if(up) {
    if(head_position_Y > 0)
      --head_position_Y;
  }
  else if(down) {
    if(head_position_Y < 15)
      ++head_position_Y;
  }
}

void generate_fruit_position() {
  long randPosition = random(NUM_LEDS);

  fruit_position_X = int(randPosition);
  read_value[fruit_position_X] = 1;
  is_spawn_fruit = true;
}

int convert_XY_to_X(int headPositionX, int headPositionY) {
  const int ROW_OFFSET = 16;
  const int ROW_NUM = 16;
  
  int position;

  for(int i = 0; i < ROW_NUM; ++i) {
    if(head_position_Y == i)
      position = head_position_X + i * ROW_OFFSET;
  }
  return position;
}

/*-------------> CONFIGURATION <-------------*/
void setup() {
  Serial.begin(9600);

  //Clear display
  FastLED.clear();
  FastLED.show();

  FastLED.setBrightness(1);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  randomSeed(analogRead(0));

  //Controller Input
  pinMode     (X_PIN,  INPUT);
  pinMode     (Y_PIN,  INPUT);
  pinMode     (SW_PIN, INPUT);

  //Data to WS2812b panel
  pinMode     (DATA_PIN, OUTPUT);
}

void config_game() {
  is_start_game = true;
  is_only_once = false;
  is_start_display = false;

  left  = true; //Snake starts left movement
  right = false;
  up    = false;
  down  = false;

  head_position_X = sqrt(NUM_LEDS) / 2;
  head_position_Y = sqrt(NUM_LEDS) / 2;
}

/*---------------> MAIN LOOP <---------------*/
void loop() {
  inputController();
  
  if(is_start_game || is_button_pressed) {
    //Set up game data and display the timer (only once after starting the game).
    if(is_only_once) {
      config_game();
      starting_timer_display();
      generate_fruit_position(); 
    }
    //Game//
    inputController();
    gameplay();
    
    //Hold the button to exit the game
    inputController();
    check_exit();
  } else if(!is_start_game && !is_start_display) {
    start_display();
  }
}
