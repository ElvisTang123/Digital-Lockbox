# Software Architecture
There are two while loops in the software architecture, I label them as while loop 1 and while loop 2. The purpose of both while loops is to continuously detect the status of the keypad(pressed or released) and repeatedly check each condition.<br /> 
The software architecture can mainly separate into two parts, one is the lock state, and another is the unlock state. The region within the red box shown in the picture below is the sector that shows how the pins are being entered while in the lock state. The region within the blue sector is the sector that compares the pins we key in and the default keys, if two sets of pins align with each other then the lockbox system will be unlocked.<br />
After unlocking the lockbox, the software architecture comes to the second while loop. The green sector within this while loop offers the function of resetting the default pins and relocking the system after resetting the pins.<br />
The details and further information can be shown in the following figures, which include the flowcharts of the lockbox system and the functions that have been utilized while designing the software architecture of this system.<br />
* main():
<p align="center">
  <img src="../Media/main.jpg" alt="" width="70%"/>
</p>

* CheckPress():
<p align="center">
  <img src="../Media/CheckPress.jpg" alt="" width="50%"/>
</p>

* Key_in():
<p align="center">
  <img src="../Media/Key_in.jpg" alt="" width="50%"/>
</p>

* CompareArrays():
<p align="center">
  <img src="../Media/CompareArray.jpg" alt="" width="50%"/>
</p>
