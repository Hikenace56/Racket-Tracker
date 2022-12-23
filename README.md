# Racket-Tracker
System that tracks the position and speed of a racket and displays it in the form of graphs to help athletes analyze their stats.
It is comprised of two parts: 
1) The physical part that contains an arduino nano BLE 33, an MPU-6050 sensor and the power unit all in a 3D printed case to measure data and send it, and an arduino nano BLE 33 to receive it.
2) The software part: acquiring the data through the MS Excel Data streamer add-in, and analyzing it with a python program heavily based on this program: https://github.com/xioTechnologies/Gait-Tracking
