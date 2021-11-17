#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=256,114
AudioEffectDelay         Delay1;         //xy=414,345
AudioMixer4              InputMixer;         //xy=418,126
AudioMixer4              Delay1Feedback;         //xy=445,222
AudioMixer4              OutputMixer;         //xy=592,146
AudioMixer4              Delay1Mixer;         //xy=616,317
AudioOutputI2S           i2s2;           //xy=754,152
AudioConnection          patchCord1(i2s1, 0, InputMixer, 0);
AudioConnection          patchCord2(i2s1, 1, InputMixer, 1);
AudioConnection          patchCord3(Delay1, 0, Delay1Mixer, 0);
AudioConnection          patchCord4(Delay1, 1, Delay1Mixer, 1);
AudioConnection          patchCord5(InputMixer, 0, OutputMixer, 0);
AudioConnection          patchCord6(InputMixer, 0, Delay1Feedback, 0);
AudioConnection          patchCord7(Delay1Feedback, Delay1);
AudioConnection          patchCord8(OutputMixer, 0, i2s2, 0);
AudioConnection          patchCord9(OutputMixer, 0, i2s2, 1);
AudioConnection          patchCord10(Delay1Mixer, 0, Delay1Feedback, 1);
AudioConnection          patchCord11(Delay1Mixer, 0, OutputMixer, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=249,73
// GUItool: end automatically generated code


Bounce button33 = Bounce(33, 100);

//int Delay1Time = 100;
//int Delay2Time = 100;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(33, INPUT_PULLUP);
  AudioMemory(1000); //Lot of memory
  sgtl5000_1.enable(); //Enable Code
  sgtl5000_1.muteHeadphone();
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); //Enable line input
  sgtl5000_1.lineInLevel(5);
  sgtl5000_1.lineOutLevel(29);
  sgtl5000_1.autoVolumeDisable();
  sgtl5000_1.surroundSoundDisable();
  sgtl5000_1.audioPostProcessorEnable(); //Enable effects on the output (Eq);
  sgtl5000_1.eqSelect(3); //Enable graphic Eq
  sgtl5000_1.eqBands(0, 0, 0, 0, 0); //Output 5 bands Eq

  InputMixer.gain(0, 1); //Input level Left
  InputMixer.gain(1, 1); //Input level Right
  InputMixer.gain(2, 0); //Not used
  InputMixer.gain(3, 0); //Not used

  Delay1Feedback.gain(0, 1); //Input level
  Delay1Feedback.gain(1, 0); //Feedback level Delay 1+2
  Delay1Feedback.gain(2, 0); //Not used
  Delay1Feedback.gain(3, 0); //Not used

  Delay1Mixer.gain(0, 0); //Delay 1 level
  Delay1Mixer.gain(1, 0); //Not used
  Delay1Mixer.gain(2, 0); //Not used
  Delay1Mixer.gain(3, 0); //Not used

  OutputMixer.gain(0, 1); //Dry level
  OutputMixer.gain(1, 1); //Delay 1+2 level
  OutputMixer.gain(2, 0); //Not used
  OutputMixer.gain(3, 0); //Not used

  Delay1.delay(0, 10);
  Delay1.delay(1, 10);
  Delay1.disable(2); //Not used
  Delay1.disable(3); //Not used
  Delay1.disable(4); //Not used
  Delay1.disable(5); //Not used
  Delay1.disable(6); //Not used
  Delay1.disable(7); //Not used

  delay(100);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Read ADC
  int knob1 = analogRead(A15);  // knob = 0 to 1023 Delay 1 level
  int knob2 = analogRead(A14);  // knob = 0 to 1023 Delay 2 level
  int knob3 = analogRead(A16);  // knob = 0 to 1023 Delay feedback level
  int knob4 = analogRead(A17);  // knob = 0 to 1023 Delay 1 time
  int knob5 = analogRead(A13);  // knob = 0 to 1023 Delay 2 time
  int knob6 = analogRead(A12); // knob = 0 to 1023 EQ Band 1
  int knob7 = analogRead(A11); // knob = 0 to 1023 EQ Band 2
  int knob8 = analogRead(A10); // knob = 0 to 1023 EQ Band 3
  int knob9 = analogRead(A0); // knob = 0 to 1023 EQ Band 4
  int knob10 = analogRead(A2); // knob = 0 to 1023 EQ Band 5
  //  Serial debug
  //  Serial.println("Start");
  //  Serial.println(knob1);
  //  Serial.println(knob2);
  //  Serial.println(knob3);
  //  Serial.println(knob4);
  //  Serial.println(knob5);
  //  Serial.println("Stop");


  //Set level
  float Delay1Level_Pot = (float)knob1 / 1023.0;
  float Delay2Level_Pot = (float)knob2 / 1023.0;
  float Delay1Feedback_Pot = (float)knob3 / 1023.0;
  Delay1Mixer.gain(0, Delay1Level_Pot);
  Delay1Mixer.gain(1, Delay2Level_Pot);
  Delay1Feedback.gain(1, Delay1Feedback_Pot);
  //  Serial.println("----");
  //  Serial.println(Delay1Level_Pot);
  //  Serial.println(Delay2Level_Pot);
  //  Serial.println(Delay1Feedback_Pot);
  //  Serial.println("----");
  //Set time for each delay
  float Delay1time  = map((float)knob4, 0, 1023, 50, 1000);
  Delay1time = round100(Delay1time);

  float Delay2time  = map((float)knob5, 0, 1023, 50, 500);
  Delay2time = round100(Delay2time);

  //Print results
  //  Serial.println(Delay1time);
  //  Serial.println(Delay2time);


  button33.update();
  if (button33.read() == LOW) {
    //Normal time
    Delay1.delay(0, Delay1time);
    Delay1.delay(1, Delay2time);
  }
  else {
    //Double time
    Delay1.delay(0, Delay1time * 2);
    Delay1.delay(1, Delay2time * 2);
  }


  //Setup levels of the 5 bands Eq
  //Range from 1.00 (+12dB) to -1.00 (-11.75dB)
  float BassLevel  = map((float)knob6, 0, 1023, -0.5, 0.5);
  float MidBassLevel  = map((float)knob7, 0, 1023, -0.5, 0.5);
  float MidLevel  = map((float)knob8, 0, 1023, -0.5, 0.5);
  float MidHighLevel  = map((float)knob9, 0, 1023, -0.5, 0.5);
  float HighLevel  = map((float)knob10, 0, 1023, -0.5, 0.5);
  sgtl5000_1.eqBands(BassLevel, MidBassLevel, MidLevel, MidHighLevel, HighLevel); //Output 5 bands Eq

  Serial.println("");
  Serial.println("MemoryUsage");
  Serial.println(AudioMemoryUsage());
  //  Serial.println("Main loop done");

}


//Round value from ADC
int round100 (int a)
{
  return (a / 100 * 100);
}
