# Amazon FreeRTOS Workshop for the Infineon XMC4800

### Lab 1: Setup the Environment

In this lab, you will download the tools needed for the subsequent sections. You can use the provided USB and let the download run in the background. You will then register your Infineon XMC4800 with AWS IoT Core.

Registering your device involves:

- Creating a Policy
- Creating a Thing
- Generating the X.509 certificate for your CC3220SF-LAUNCHXL to
  communicate with AWS IoT Core
- Attaching the Policy and Thing to the Certificate

[![Lab1](images/lab1.png)](./Lab1.md "Lab 1")

### Lab 2: Run the Amazon FreeRTOS Sample Code

In this lab, you will download the Amazon FreeRTOS sample code from AWS IoT, import the sample code into Infineon DAVE that you downloaded from section 1, configure the Amazon FreeRTOS sample with your WiFi settings and the certificates from Lab 1, and flash to the XMC4800 via the Debug flow.

[![Lab2](images/lab2.png)](./Lab2.md "Lab 2")

### Lab 3: Using AWS IoT Core

In this lab, you will program Amazon FreeRTOS to use the sensors on the XMC4800. First, you will be able to get the temperature sensor readings from the MCU and send to AWS IoT Core.

[![Lab3](images/lab3.png)](./Lab3.md "Lab 3")

### Lab 4: Using Device Shadows

In this lab, you will program Amazon FreeRTOS to change the on-board LED state based on user button manipulation. The standard way to perform this actuation through AWS IoT Core is by using Device Shadows. You will learn the details about Reported and Desired state.

[![Lab4](images/lab4.png)](./Lab4.md "Lab 4")


Copyright (C) 2018 Amazon.com, Inc. and Infineon Technologies AG.  All Rights Reserved.
