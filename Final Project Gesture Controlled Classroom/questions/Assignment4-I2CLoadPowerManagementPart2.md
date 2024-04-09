Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements with logging disabled to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See the file Instructions to add screenshots in assignment.docx in the ECEN 5823 Student Public Folder.* 

1. What is the average current per period?
   Answer:13.49uA
   <br>Screenshot:  
   ![Avg_current_per_period](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ayswariya088/blob/master/questions/screenshots/assignment4/avg_current_per_period.JPG)  

2. What is the average current when the Si7021 is Powered Off?
   Answer:2.12uA
   <br>Screenshot:  
   ![Avg_current_LPM_Off](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ayswariya088/blob/master/questions/screenshots/assignment4/avg_current_lpm_off.JPG)  

3. What is the average current when the Si7021 is Powered On?
   Answer:328.49uA
   <br>Screenshot:  
   ![Avg_current_LPM_On](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ayswariya088/blob/master/questions/screenshots/assignment4/avg_current_lpm_on.JPG)  

4. How long is the Si7021 Powered On for 1 temperature reading?
   Answer:104ms
   <br>Screenshot:  
   ![duration_lpm_on](https://github.com/CU-ECEN-5823/ecen5823-assignment4-ayswariya088/blob/master/questions/screenshots/assignment4/duration_lpm_on.JPG)  

5. Compute what the total operating time of your design for assignment 4 would be in hours, assuming a 1000mAh battery power supply?
   Answer (in hours):
   - Total average current per hour = 13.49e-6/3s *(3600)s= 16.188mA
   - Operating time of design= Battery Capacity/ Current per hour = 1000mAh/16.128mA= 61.77 hours

6. How has the power consumption performance of your design changed since the previous assignment?
   Answer:
   <br /> The power consumption has drastically reduced in this design since current reduced from 144.06uA per period to 13.49uA per period. Almost 1/10th of the initial consumption, this is because of the reduction in current consumed during on period between time wait and I2C transfer.
   It has become 328.49uA from 4.37mA , almost 1/13th from the initial value. Thus the overall power consumption is also reduced. Off time current consumption also reduced slightly from 2.93uA to 2.12uA.
   We have used interrupt based time wait function instead of polling based function and sleep to EM1 between I2C transfers that has contributed to this change.


