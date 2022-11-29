## socketcan_wrapper

## build
~~~bash
make clean
make
~~~

## create can0, vcan0 device
Before running the application, the following items have to be done.  
 - create can0 device
~~~bash
ls -al /dev/ttyUSB*
sudo slcand -o -c -s6 /dev/ttyUSB0 can0
sudo ip link set up can0
~~~
  
 - s option list
~~~
+----------------+---------------+
| ASCII Command  |  CAN Bitrate  |
+----------------+---------------+
|  s0            |  10 Kbit/s    |
|  s1            |  20 Kbit/s    |
|  s2            |  50 Kbit/s    |
|  s3            |  100 Kbit/s   |
|  s4            |  125 Kbit/s   |
|  s5            |  250 Kbit/s   |
|  s6            |  500 Kbit/s   |
|  s7            |  800 Kbit/s   |
|  s8            |  1000 Kbit/s  |
+----------------+---------------+
~~~
  
 - create vcan0 device
~~~bash
sudo ip link add name vcan0 type vcan
sudo ip link set up vcan0
~~~
  
 - check vcan0 device  
~~~
ifconfig
ip -details -statistics link show vcan0
~~~
  
## run application
~~~bash
./main
~~~

## can-utils to verify app.
 - At first, app is excuted before doing below.  
 - To verify this application, we will use can-utils
   - if can-utils package is not installed, type below command.
        ~~~bash
        sudo apt-get install can-utils
        ~~~
 - verify reading message from bus.
    Open new terminal(shell) session. And type below command.  
    ~~~bash
    cansend vcan0 01a#11223344AABBCCDD
    ~~~
    If reading message in app is completed successfully, app log will be printed as below.  
    ~~~bash
    read CAN message(blocked io). msg id = 0x01a, dlc = 8, data = 11 22 33 44 AA BB CC DD
    ~~~
 - verify writing message to bus.
    Open new terminal(shell) session. And type below command.  
    ~~~bash
    pi@raspberrypi:~/$ candump vcan0
    ~~~
    If writing message in app is completed successfully, app log will be printed as below.  
    ~~~bash
    vcan0  123   [2]  11 22
    vcan0  123   [2]  11 22
    vcan0  123   [8]  11 22 33 44 55 66 77 88
    vcan0  123   [8]  11 22 33 44 55 66 77 88
    ~~~

## usb-can device
In my experience, I used the below cable.  
https://www.seeedstudio.com/USB-CAN-Analyzer-p-2888.html

## reference
https://docs.kernel.org/networking/can.html
  
https://elinux.org/Bringing_CAN_interface_up
  