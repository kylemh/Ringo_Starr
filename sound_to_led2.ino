class MusicListen{
public:
  MusicListen(int,int);
  int soundSensorPin;
  int LEDPin;
  float Threshold_Value;
  //int sensorValue;
  // Sample window width in mS (50 mS = 20Hz)
  unsigned int sample;
  int sampleWindow;
  
  public:
  void turnonLED()
  {
    digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);  
  }
  void turnoffLED(){
    digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW
    delay(2000);  
  }
  void performAction(){
  //wrapper for turning on led, place the code to send ir signal here
  turnonLED();
      turnoffLED();}
  public:
  double AmpVolts(){
    //find the amplitude of wave from souind sensor by sampling for 50ms and taking the range
  unsigned long startMillis= millis();  // 
     unsigned int peakToPeak = 0;   // 
     unsigned int signalMax = 0;
     unsigned int signalMin = 1024;
   
     // collect data for 50 mS
     while (millis() - startMillis < sampleWindow)
     {
        sample = analogRead(soundSensorPin);
        delay(2);//let the mic rest//test this
        if (sample < 1024)  // toss out spurious readings
        {
           if (sample > signalMax)
           {
              signalMax = sample;  // save just the max levels
           }
           else if (sample < signalMin)
           {
              signalMin = sample;  // save just the min levels
           }
        }
     }
     peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
     double volts = peakToPeak;  // convert to volts
     return volts;
  }
  
  double minMax(){
    float small=3;
    float large=0;
    int patience=0;
    float amplitude=0;
    float measure =0;
    while (patience<500){
      measure = AmpVolts();
      if (measure<small){
        small=measure;}
        else{large=measure;}
        patience+=5;
       amplitude=large-small;
    }
     return amplitude;
  }
  //for no one by the beetles
  public:
  double Ambient(){
    //takes 25 readings and averages them
    //return the ambient noise level in volts. collects 25 readings and averages them 
    double reading;
    double acc=0;
    int patience=0;
    float amb = 10;
    if (amb>1){
      while(patience<=500){
         
        reading = AmpVolts();
        patience+=20;
        acc+=reading;}
        amb=acc/25;//500/20=25 evenly this could be 25, so the number is slightly larger to include a treshold    
   }
    return amb;
  }
  public:
   int levels(float volts){
     //divides voltage as input into 3 parts, returns case number 
        float one=0.4;
        float two=0.67;
        float three=1.1;
        if(volts<one) 
          return 2;
        
        //for what ever reason this just wont output the right thing
        else if (volts>=one && volts<two)
        return 3;
        
        //this is  astrnage bug: wont return 3 when volts in range
        else if (volts>=two&&volts<three)
        return 4;
        
        if (volts>=three) 
        return 5;
  }  
};//end class

MusicListen::MusicListen(int output, int soundPin){
//Set the pin that is getting the analog signal from the sound sensor,
//output is a pin to send a digital frequency to
soundSensorPin = soundPin;
LEDPin = output;
sampleWindow = 50; //50miliseconds
Threshold_Value = 0;
}/*
class SetUpDriver{
  public:
  int output, input;
  SetUpDriver();
  
  void pins_init(){
    pinMode(output,OUTPUT);//in this case blink led
    pinMode(input , INPUT);//read in analog input
    }
  void initialize(int OUT,int IN){
  output=OUT;
  input=IN;}
};
 */
    //must make sure these match the pins when declaring class
//protected is derivedclasses
//3.3vare binned into 512 spots.1bin is 1v is 150 bins so110 is 0.3v  is 50bins

void driver(){
  //create an instance of the music listener
  
  MusicListen dis(2,A0);//PIN NUMBERS
  float thresh=dis.Ambient();
    double volts =dis.AmpVolts();
    double sec= dis.minMax();//this isthe range of values found
    volts+=sec/2;//taking the upper error bound. 
    int level =dis.levels(volts);
    
    Serial.print("ambient");
 Serial.println(thresh);
    Serial.print("volts");
    Serial.println(volts);
    Serial.println(level);
    Serial.print("second sample");
    Serial.println(sec);
    
    if (level==2){
      dis.turnoffLED();//this is the place to send the ir signal
    }
      else if (level==3){
        dis.performAction();
      }
      else if (level==4){
        dis.performAction();
        dis.performAction();
      }
      else if (level==5){
        dis.performAction();
        dis.performAction();
        dis.performAction();
      }
}
void setup() {
  pinMode(3,OUTPUT);//Digital output,in this case blink led
    pinMode(A0 , INPUT);//Analog inputread in analog input
  // put your setup code here, to run once:
Serial.begin(9600);//this is for print statements
}
void loop() {
  driver();
}
