Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

*Be sure to take measurements with logging disabled to ensure your logging logic is not impacting current/time measurements.*

*Please include screenshots of the profiler window detailing each current measurement captured.  See the file Instructions to add screenshots in assignment.docx in the ECEN 5823 Student Public Folder.*

1. Provide screen shot verifying the Advertising period matches the values required for the assignment.
   <br>Screenshot: 250.4ms , same as what we set for advertising period
   ![advertising_period](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/advertising_period.JPG)  

2. What is the average current between advertisements (Don't measure an interval with a LETIMER UF event)?
   Answer:2.88uA
   <br>Screenshot:  
   ![avg_current_between_advertisements](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/avg_current_between_advertisements.JPG)  

3. What is the peak current of an advertisement? 
   Answer:27.2mA
   <br>Screenshot:  
   ![peak_current_of_advertisement](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/peak_current_of_advertisement.JPG)  

4. Provide screen shot showing the connection interval settings. Do they match the values you set in your slave(server) code or the master's(client) values?.
   <br>Screenshot: 46.4ms , it is not same as what the slave sends for connection parameters i.e 75ms. It matches with what the master has set for the connection(as noted using logs)which is 39 *1.25 =48.75ms for connection interval
   ![connection_interval](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/connection_interval.JPG)  

5. What is the average current between connection intervals (Don't measure an interval with a LETIMER UF event)?
   Answer:2.55 uA
   <br>Screenshot:  
   ![avg_current_between_connection_intervals](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/avg_current_between_connection_intervals.JPG)  

6. If possible, provide screen shot verifying the slave latency matches what was reported when you logged the values from event = gecko_evt_le_connection_parameters_id. 
    <br>Ans : Slave latency set by master is 0 as per logged value. Thus slave has to send the indication in the next connection event. We can see a slight bump in the current due to indication getting queued to send. Indication gets send in the next connection event, thus proving that slave latency is 0.
    <br>Screenshot:  
   ![slave_latency](https://github.com/CU-ECEN-5823/ecen5823-assignment5-ayswariya088/blob/master/questions/screenshots/assignment5/slave_latency.JPG)  

