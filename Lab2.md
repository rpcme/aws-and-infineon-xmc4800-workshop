# Amazon FreeRTOS Workshop for the Infineon XMC4800 IoT Connectivity Kit

## Lab 2: Run the Amazon FreeRTOS Sample Code

In this lab, you will configure the Amazon FreeRTOS demo program with your Wi-Fi settings and AWS IoT Core credentials. Then, you will run the Hello World Demo on the Infineon XMC4800 IoT Connectivity Kit.

### Configuring the Hello World Demo

In DAVE4, open the file ```application_code``` > ```common_demos``` > ```include``` > ```aws_clientcredential.h```.

1. Edit the ```clientcredentialMQTT_BROKER_ENDPOINT``` variable on line 38.  To retrieve the value for this variable, issue the following command using the AWS CLI.

   ```bash
   aws iot describe-endpoint
   ```

   The output will be similar to the following.  **Use the output from the command on your workstation, not this value.  Endpoints can differ between AWS accounts.***

   ```text
   audqth7zumq6e.iot.us-east-1.amazonaws.com
   ```

   The result on line 38 will be similar to the following.

   ```c
   static const char clientcredentialMQTT_BROKER_ENDPOINT[] = "audqth7zumq6e.iot.us-east-1.amazonaws.com";
   ```

2. Edit the ```clientcredentialIOT_THING_NAME``` variable on line 43.  Although this value is used for the Device Shadow Client, we will edit it for completeness.  In the previous lab, you identified the MAC address for your device. For example, it may be similar to ```9884e3f60411```.  The value will then look similar to the following.

   ```c
   #define clientcredentialIOT_THING_NAME "9884e3f60411"
   ```
3. Edit the ```clientcredentialWIFI_SSID``` variable on line 58.  For example, if your SSID is ``MySuperWiFi", then the value will look similar to the following.

   ```c
   #define clientcredentialWIFI_SSID       "MySuperWiFi"
   ```
4. Edit the ```clientcredentialWIFI_PASSWORD``` variable on line 63.  For example, if your Wi-Fi password is ```p@s$woRd```, then the value will look similar to the following.

   ```c
   #define clientcredentialWIFI_PASSWORD   "p@s$woRd"
   ```

Next, we need to generate the header file that holds credentials for connecting to AWS IoT Core.  In DAVE4, open the file ```application_code``` > ```common_demos``` > ```include``` > ```aws_clientcredential_keys.h```.

On your workstation, open Explorer (Windows) or Finder (Mac OSX) and navigate to ```$LAB_REPOPATH``` > amazon-freertos > tools > certificate_configuration.  Double-click ```CertificateConfigurator.html```.

1. For **Certificate PEM file**, click **Browse...** and navigate to ```$LAB_REPOPATH``` > ```credentials```.  Select the certificate file, which will look similar to ```9884e3f60411_certificate.pem```.
2. For **Private Key PEM file**, click **Browse...** and navigate to ```$LAB_REPOPATH``` > ```credentials```.  Select the private key file, which will look similar to ```9884e3f60411_private_key.pem```.
3. Click **Generate and save ```aws_clientcredential_keys.h```**.  Save the file to the filesystem.  I most cases, it will be saved to your Downloads directory.

    <img src="images/Lab2/certificate_configuration_tool.png" alt="drawing" style="width:600px;"/>

4. Copy the ```aws_clientcredential_keys.h``` in your Downloads directory ***over*** the ```aws_clientcredential_keys.h``` in the directory ```$LAB_REPOPATH``` > amazon-freertos > demos > common > ```include```.
5. In DAVE4, the file should already be refreshed.  If not, double-click on aws_clientcredential_keys.h again.  It should look similar to the following (the content has been modified so this is for example purposes only).

   ```c
	/*
	 * PEM-encoded client certificate.
	 *
	 * Must include the PEM header and footer:
	 * "-----BEGIN CERTIFICATE-----"
	 * "...base64 data..."
	 * "-----END CERTIFICATE-----";
	 */
	static const char clientcredentialCLIENT_CERTIFICATE_PEM[] =
	"-----BEGIN CERTIFICATE-----\n"
	"MIIDWTCCAkGgAwIBAgIUBrxwa1bQrALUSLuVzFLgUNvLMuwwDQYJKoZIhvcNAQEL\n"
	"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
	"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE4MDkxOTE0NTIy\n"
	"v8oxIkX5xzR2iQ0RbM0IyroTp/WpTe5bLLwSOsWujeElC9PgLw+CB6WEZAlNxNrx\n"
	"fPSkKCqOF+WztJd2N8SFUlkr1uvzJrcy0L0V6HNdYFGDfUzal/IcV6cNadIgWu6m\n"
	"KSsrSqSGa+fxxyc/mCw83O5pm887dLVysnPB1EGTuGKLacP3QVJIrnJTasGb\n"
	"-----END CERTIFICATE-----";

	/*
	 * PEM-encoded client private key.
	 *
	 * Must include the PEM header and footer:
	 * "-----BEGIN RSA PRIVATE KEY-----"
	 * "...base64 data..."
	 * "-----END RSA PRIVATE KEY-----";
	 */
	static const char clientcredentialCLIENT_PRIVATE_KEY_PEM[] =
	"-----BEGIN RSA PRIVATE KEY-----\n"
	"MIIEowIBAAKCAQEAuRdLQNfQl+zKDu/FAQOGpHt8psX0b22QdS/6vQLFMhVniJhG\n"
	"7pzKC/IFAMYfW8r6kNrb5rWYZ73BCVW/V4Eu4nXtCoiZ6qiO7vtEg8uwvFOywBbV\n"
	"Ny6057Vf4dD7r5pVQkYat14fLkxm4lbB0Ot07gwisRNbTA69AxzkPk2YydZPM8fx\n"
	"icWLxbnkWwUM2qLMinjNZ+ifdmu9ZUCdHa4DeYs+yneK4HyhyA/y1iHE77mA6rv/\n"
	"EoCtyUVWXjoJKiR47TCApvuQPuN+MUnuclylg5Ipm0UPGhEh08iFlQL5wqJtLOKo\n"
	"jw8HTlzChZlxRR7sX8xscJpNOMMJMHn6w1b+SQIDAQABAoIBAGJlV8Qo9uNNJj26\n"
	"iWh4j2NxolqmccOq41jpIMrKZjqx1lBvY4YGJAMlrjMzIAiUZ5AKSUzkzRiZb72e\n"
	"sBG8w0fwmQs4g1qUMSgqu8m0x0fW1vf3K3IiilmDum05xBMeYKTSz/EbQEhEpY09\n"
	"Nyg7U2MvEsULcdz6wOLJ14OC5YCKo5LQvJ7W+7qXb7cAR0VuZbgnSj+iUQhunnsz\n"
	"GiLiLurrK+TI1bhZR1SGuQKBgDtkBzMGr8dnVSnVL5YkkcDUui5ZzUq8DDIXiRmp\n"
	"j0UPPmsuhCK8ZoxEcYt43BdwqaZbs1895rznLeks4evMnsxN6ZLYw8C4rXugZK5q\n"
	"RYBh/mimXorVp5cWTUaWgpEOSMTi7OVil9m5leDbouB8u6tSWuNiXt1MhVAU1fn+\n"
	"Iq1VAoGBAImYjzoWqsKowPF8xcqHg+C2ym2VeDEramB3VvXUDHf4jx+Ma/m/FMEM\n"
	"3qlix7Nks/N8+4XGXBod4fxRAhp9Eza/YjFlKhNqW1jzTi0p5g8iySbe6J7H1jPO\n"
	"th5obcRvBds7fPzbkiApuT7zhUpskXuQLTpsBpLfhHSMnXQzOtxT\n"
	"-----END RSA PRIVATE KEY-----";

	/*
	 * PEM-encoded Just-in-Time Registration (JITR) certificate (optional).
	 *
	 * If used, must include the PEM header and footer:
	 * "-----BEGIN CERTIFICATE-----"
	 * "...base64 data..."
	 * "-----END CERTIFICATE-----";
	 */
	static const char * clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;
   ```

### Testing the connection

1. Rebuild the project by clicking **Project > Build Active Project** or **Build Active Project** button in menu bar.

    <img src="images/Lab2/dave4_menubar_compile.png" alt="drawing" style="width:600px;"/>

2. To flash and start debugging the demo click the debug button in the menu bar.

    <img src="images/Lab2/dave4_menubar_debug.png" alt="drawing" style="width:600px;"/>

3. Before continuing, login to the AWS Console, and then navigate to the AWS IoT Core console.
4. On the left-hand side, click the **Test** menu item.
5. For the subscription topic, enter ```#```
6. Click the ```Subscribe to topic``` button.
7. Click the **Resume** button in the DAVE4 Debug perspective menu bar.

    <img src="images/Lab2/dave4_debug_resume.png" alt="drawing" style="width:600px;"/>

8. The demo should now be running.  When completed, the output in the console will be similar to the following.

    <img src="images/Lab2/hello_world_demo_run.png" alt="drawing" style="width:900px;"/>

In the following sections, we will modify the sample program to do something a bit more interesting with it.

### Modifying the Client ID

When connecting to AWS IoT Core, the MQTT Client ID that you are using for the MQTT sesson must be unique in your account.  If not unique, then the last connection made with the Client ID will cause a disconnect for the previous connection.

In these labs, we have established a unique ID - the MAC address.  If you look at the source file ```application_code``` > ```common_demos``` > ```source``` > ```aws_hello_world.c```, you will note that on line 85 the Client ID is hardcoded.  If we are going to use the same firmware image for hundreds, thousands, or even millions of devices, we cannot have this value hardcoded.

In the Wi-Fi abstraction layer in Amazon FreeRTOS, there is an API to retrieve the MAC Address.  We can use that to construct our unique identifier.

1. At the bottom of ```aws_hello_world.c```, paste the following code.  

   ```c
   static void prvMacForHumans(uint8_t * humanAddress)
	{
	    uint8_t buf[ wificonfigMAX_BSSID_LEN ];
	    char cbuf[2];
	    uint8_t idx = 0;

	    WIFI_GetMAC( ( uint8_t * )  buf);

	    // get the mac address in a string
	    for (idx = 0; idx < wificonfigMAX_BSSID_LEN; idx++)
	    {
	        sprintf(cbuf, "%x", buf[idx]);
	        if ( cbuf[1] == '\0')
	        {
	            humanAddress[( idx * 2 )] = '0';
	            humanAddress[( idx * 2 ) + 1] = cbuf[0];

	        }
	        else
	        {
	            humanAddress[( idx * 2 )] = cbuf[0];
	            humanAddress[( idx * 2 ) + 1] = cbuf[1];
	        }
	    }

	    humanAddress[ ( wificonfigMAX_BSSID_LEN * 2 ) ] = '\0';
	}
   ```
2. On line 72, add the following include.

   ```c
   #include "aws_wifi.h"
   ```
3. On line 182, add the following declarations.

   ```c
   static void prvMacForHumans(uint8_t * humanAddress);
   static uint8_t thing_mac_address[ ( wificonfigMAX_BSSID_LEN * 2 ) + 1 ];
   ```
4. Next, we need to initialize the value for ```thing_mac_address```.  We can do this in the entry function to this demo.  Scroll to line 512, and enter the line for assigning the value to ```thing_mac_address```.  Existing code has been added here for context.

   ```c
   void vStartMQTTEchoDemo( void )
   {
       configPRINTF( ( "Creating MQTT Echo Task...\r\n" ) );

       prvMacForHumans(thing_mac_address);
   ```

5. Finally, we need to edit the Client ID for the MQTT client.  Scroll to line 187, to this function:

   ```c
   static BaseType_t prvCreateClientAndConnectToBroker( void )
   ```
6. Four lines down, a structure is created to hold the MQTT connection parameters.  The structure should look like the following:

   ```c
        MQTTAgentConnectParams_t xConnectParameters =
	    {
	        clientcredentialMQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
	        democonfigMQTT_AGENT_CONNECT_FLAGS,   /* Connection flags. */
	        pdFALSE,                              /* Deprecated. */
	        clientcredentialMQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. Can be overridden by ALPN connection flag. */
	        echoCLIENT_ID,                        /* Client Identifier of the MQTT client. It should be unique per broker. */
	        0,                                    /* The length of the client Id, filled in later as not const. */
	        pdFALSE,                              /* Deprecated. */
	        NULL,                                 /* User data supplied to the callback. Can be NULL. */
	        NULL,                                 /* Callback used to report various events. Can be NULL. */
	        NULL,                                 /* Certificate used for secure connection. Can be NULL. */
	        0                                     /* Size of certificate used for secure connection. */
	    };
    ```
    To change the Client ID, we will change ```echoCLIENT_ID``` to ```thing_mac_address```.  It will look like the following:

    ```c
        MQTTAgentConnectParams_t xConnectParameters =
	    {
	        clientcredentialMQTT_BROKER_ENDPOINT, /* The URL of the MQTT broker to connect to. */
	        democonfigMQTT_AGENT_CONNECT_FLAGS,   /* Connection flags. */
	        pdFALSE,                              /* Deprecated. */
	        clientcredentialMQTT_BROKER_PORT,     /* Port number on which the MQTT broker is listening. Can be overridden by ALPN connection flag. */
	        thing_mac_address,                        /* Client Identifier of the MQTT client. It should be unique per broker. */
	        0,                                    /* The length of the client Id, filled in later as not const. */
	        pdFALSE,                              /* Deprecated. */
	        NULL,                                 /* User data supplied to the callback. Can be NULL. */
	        NULL,                                 /* Callback used to report various events. Can be NULL. */
	        NULL,                                 /* Certificate used for secure connection. Can be NULL. */
	        0                                     /* Size of certificate used for secure connection. */
	    };
   ```
7. Build and debug-run the application to ensure everything is working with your new unique Client ID.  How do we know that we are using the ID?  We can add logging instrumentation to help us know that.  Before the structure definition, add this line:

   ```c
      configPRINTF( ( "My Client ID is [%s]\r\n", thing_mac_address ) );
   ```

   In the Terminal logging window, you will see the following:

   ```c
	3 177 [Tmr Svc] Wi-Fi module initialized (MAC Address: 84:f3:eb:a3:1a:7a).
	4 177 [Tmr Svc] Connecting to AP...
	5 5134 [Tmr Svc] Wi-Fi Connected to AP. Creating tasks which use network...
	6 5135 [Tmr Svc] IP Address acquired 10.42.0.97
	7 5135 [Tmr Svc] Creating MQTT Echo Task...
	8 5136 [MQTTEcho] My Client ID is [84f3eba31a7a]
	9 5136 [MQTTEcho] MQTT echo attempting to connect toa3vp4g8bw5u3sd.iot.us-east-1.amazonaws.com.
   ```

   Next, we will learn how to integrate this demo with some of the on-board components.

### Making Local Actuation Happen

> If you are running out of time, you can directly use ```aws_hello_world_lab2.c```. This has the code in this lab already implemented.  
> 1. In DAVE, navigate to ```application_code``` > ```common_demos``` > ```source```.
> 2. Right-click aws_hello_world.c, and choose **Properties**.
> 3. Select **C/C++ Build**.
> 4. Check **Exclude resource from build**.
> 5. Click **OK**.
> 6. Right-click aws_hello_world_lab2.c, and choose **Properties**.
> 7. Uncheck **Exclude resource from build**.
> 8. Click **OK**.

The Infineon XMC4800 IoT Connectivity Kit is a feature-packed development board with a lot of options.  When you look at all the LEDs, buttons, and pins -- each of these are configurable in their own way through what is called Pin Multiplexing.

Out of the box, the LEDs are disabled because GPIOs connected to them act as inputs by default. In this lab, we will configure the GPIOs connected to the LEDs as outputs.

1. Add the inclusion of the LED library
   ```c
   #include "Board_LED.h"
   ```

2. Scroll to ```vStartMQTTEchoDemo()``` function. After the call to ```prvMacForHumans(thing_mac_address)``` add

   ```c
   LED_Initialize();
   ```

3. Next, we will turn on one of the LEDs in the publish function.  Find the function ```static void prvPublishNextMessage( BaseType_t xMessageNumber )```.  Add the following directly before the ```( void ) xReturned``` statement.

   ```c
    LED_On(0);
    LED_Off(1);
   ```
4. Next, we will add the opposite effect to the task performing the ACK publish.  The function name is ```static void prvMessageEchoingTask( void * pvParameters )```.  At the end of the block within the following ```if``` condition, place the alternating LED code.

   ```c
    if ( xBytesReceived <= ( sizeof( cDataBuffer ) - ( size_t ) echoACK_STRING_LENGTH ) )
    {
      ...
      LED_Off(0);
      LED_On(1);
    }
   ```
5. Run the DAVE project again.  You will notice the led alternating on what is marked D8 and D9 on the development board.

[Next Lab](./Lab3.md)
