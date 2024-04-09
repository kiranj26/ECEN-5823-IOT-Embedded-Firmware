Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to StrongAlternateStrong?**
   Answer:The system draws 5.24 mA when a single LED is on with StrongAlternateStrong setting. It draws 4.72 mA when LED is off.


**2. How much current does the system draw (instantaneous measurement) when a single LED is on with the GPIO pin set to WeakAlternateWeak?**
   Answer:The system draws 5.18 mA when a single LED is on with WeakAlternateWeak setting. It draws 4.68 mA when LED is off.


**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer, referencing the main board schematic, WSTK-Main-BRD4001A-A01-schematic.pdf, and AEM Accuracy in the ug279-brd4104a-user-guide.pdf. Both of these PDF files are available in the ECEN 5823 Student Public Folder in Google drive at: https://drive.google.com/drive/folders/1ACI8sUKakgpOLzwsGZkns3CQtc7r35bB?usp=sharing . Extra credit is available for this question and depends on your answer.**
   Answer: There is not much meaningful difference between the strong and weak drive strength current consumption in the above two cases.
   
   First of all by referring to the definition of drive strength, we know that drive strength specifies how much current the I/O can drive and sink while maintaining the minimum and maximum output voltage levels. 
   
   Higher drive strength means it can supply the required current faster while switching, thus the voltage switches faster leading to fast slew rate or short fall/rise time. Thus in the first case of strong drive strength we can say that the required current is delivered faster.

   This doesn't affect the total current consumption, that is totally dependent on the LED and it's consumption. After referring to the schematic , we can see both LEDs have 3K resistor in series as current limiting resistor. Thus Maximum current through LEDs=(Vs-Vled)/3K=(3.3-2)/3K(assuming 2V Forward voltage )=0.43mA which is going to be almost same in both cases.
   
   So it doesn't matter in this case whether it is high or low drive strength, similar current is consumed. Also AEM-Advanced Energy Monitor which is used to measure current has accuracy in the range of 0.1uA to 95mA. As in this case, when current gets above 250uA, it's accurate only within 0.1mA. So even if some difference in the range of 0.01mA which won't be accurately captured by AEM.
   
   This is why there isn't much difference between these values.


**4. With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 1 LED with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**
   Answer:Average current for complete on-off cycle for 1 LED is 4.96mA with WeakAlternateWeak setting.


**5. With the WeakAlternateWeak drive strength setting, what is the average current for 1 complete on-off cycle for 2 LEDs (both on at the time same and both off at the same time) with an on-off duty cycle of 50% (approximately 1 sec on, 1 sec off)?**
   Answer:Average current for complete on-off cycle for 2 LEDs is 5.14 mA with WeakAlternateWeak setting.


