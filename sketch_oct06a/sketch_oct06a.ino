//Pins on the 74HC595 shift reg chip - correspondence with Arduino digital pins
unsigned int ST_CP = D11;     // RCLK
unsigned int SH_CP = D12;     // SCLK
unsigned int DS    = D13;     // DIO
unsigned int ssr    = D10;     // SSR
unsigned int rst_btn = D9;     // RED BUTTON
unsigned int add_btn = D8; // green button


void setup() 
{
    Serial.begin(4800);
  // Set Arduino pins as outputs
  pinMode(ST_CP, OUTPUT);
  pinMode(SH_CP, OUTPUT);  
  pinMode(DS,    OUTPUT);
  pinMode(ssr, OUTPUT);
  pinMode(rst_btn, INPUT);
  pinMode(add_btn, INPUT);
}

// Table to convert a hex digit into the matching 7-seg display segments
//
//
int hexDigitValue[] = {
    0xFC,    /* Segments to light for 0  */
    0x60,    /* Segments to light for 1  */
    0xDA,    /* Segments to light for 2  */
    0xF2,    /* Segments to light for 3  */
    0x66,    /* Segments to light for 4  */
    0xB6,    /* Segments to light for 5  */
    0xBE,    /* Segments to light for 6  */
    0xE0,    /* Segments to light for 7  */
    0xFE,    /* Segments to light for 8  */
    0xF6     /* Segments to light for 9  */
};


/* Set display digit to a hexadecimal value '0'..'9','A'..'F'
 *   
 *    row = row number of digit to light up 0..3 where 3 is most significant (leftmost) display digit
 *    dgit = value 0 to 15 
 *    decimalPoint = true/false : whether to light thde decimal point or not
 *    thanks to :
 *    https://github.com/magiccow/_7seg-display-simple
 */
void setDigit(unsigned int row, unsigned int digit, boolean decimalPoint)
{
      unsigned int rowSelector;
      unsigned int data;

      rowSelector = bit(3-row)<<4;
      data =  ~  hexDigitValue[ digit & 0xF ] ;
      if(decimalPoint)
        data &= 0xFE;
        
      // First 8 data bits all the way into the second 74HC595 
      shiftOut(DS, SH_CP, LSBFIRST, data );

      // Now shift 4 row bits into the first 74HC595 and latch
      digitalWrite(ST_CP, LOW);
      shiftOut(DS, SH_CP, LSBFIRST, rowSelector );
      digitalWrite(ST_CP, HIGH);
      
}


/* Displays a number as a 4-digit decimal number on the display
 *   Note this is multiplexed, so you need to keep calling it
 *   or you'll end up with just one digit lit.
 */
void displayNumber(unsigned int number){
    for(unsigned int i=0; i<4; i++){
      setDigit(i, number % 10, false); // display righmost 4 bits (1 digit)
      number = number / 10;  // roll on to the next digit
      delay(1);
    } 
}

int add_btn_state ;
int rst_btn_state ;
int prev_rst = 0;
void loop() 
{
   digitalWrite(ssr, HIGH); // Turn the SSR on
   rst_btn_state = digitalRead(rst_btn);
   add_btn_state = digitalRead(add_btn);
   displayNumber( rst_btn_state  ); 
   Serial.println(rst_btn_state);
}
 
