# Amazon FreeRTOS Workshop for the Infineon XMC4800 IoT Connectivity Kit
## Lab 4: Using Device Shadows

In this lab, you will program Amazon FreeRTOS to change the on-board LED state based on on-board buttons status. The standard way to perform this actuation through AWS IoT Core is by using Device Shadows. You will learn the details about Reported and Desired state and understand how to drive signals across Amazon FreeRTOS tasks using Queues.


### Premise

In this lab, you will discover the importance of the AWS IoT Device Shadow and how it enables command and control cabilities for your solution.

Command and control is the act of telling the physical entity to achieve a potential, and then the autonomous entity, through programmatic means, achieves that potential.

More specifically, you want a device to perform a specific act.  The communication must be explicit and prescriptive.  The act must be transcribed to a language the edge device understands.  The edge device must know how to receive the communication and transcribe it to instructions that eventually affect the environment.

Likewise, the human prescribing this act wants to know when, and sometimes to what degree, the act has been achieved. It is simpler if the channel under which the two entities communicate use the same language, but obviously use different contexts.
      
This is the crux of device shadow.  *Active* communication under different contexts as well as abrupt communication under specific contexts (such as initialization when context or current state must be understood).

This lab gets you started on Device Shadows.  Although we are not doing any actuation, we will update the device shadow, you will see changes in the Console, and perform some action in the AWS Cloud.

#### The Button Reader Task

We will start off by implementing the buttons status reader task.  We will first need to setup a declaration for it, and then go into the details of the code itself.

1. Start by adding the function stub.

   ```c
   static void prvButtonStatusReaderTask( void * pvParameters )
   {
   }
   ```
   The next steps will be in the context of this function.
2. Add the declarations to be used throughout the code.

   ```c
	    char cDataBuffer[ 128 ];
	    xMQTTTaskParameter *pxParameters;
	    pxParameters = ( xMQTTTaskParameter * ) pvParameters;
	
        uint32_t state;
        signed char b1 = 0;
        signed char b2 = 0;

        Buttons_Initialize();

   ```

The read-report process will run within an endless while loop.

1. Add the start of the while loop.  

   ```c
       while (1)
       {
        	state = Buttons_GetState();
        	b1 = state & 0x1;
        	b2 = state & 0x2;

   ```
3. Construct the JSON payload that will be sent to the AWS Cloud.

   ```c
		snprintf(cDataBuffer, sizeof( cDataBuffer),
		                      "{"
		                      "\"state\":{"
		                      "\"reported\":{"
		                      "\"b1\":%d,"
		                      "\"b2\":%d"
		                      "}},"
		                      "\"clientToken\": \"token-%d\"" \
		                      "}",
		                      b1, b2,
		                      ( int ) xTaskGetTickCount() );
   ```

	When it reaches the AWS Cloud, it will look similar to the following.  **DO NOT ADD THIS TO THE C SOURCE FILE**
	
	```json
	{
	  "state": {
	    "reported": {
	      "b1": 0,
	      "b2": 0
	    }
	  },
	  "clientToken": "token-2218795"
	}
	```
4. Add the code that sends the payload to the AWS Cloud. Pay special attention to the **topic** where the payload is being sent.  This is configured with the xCreateTask call we will discuss in the next session.  The publishing is to the special ```$aws``` topic specifically for the Client ID named with the MAC address.

   ```c
            configPRINTF(("##### b1: %d, b2: %d. #####\r\n", b1, b2));
            MQTTAgentPublishParams_t pxPublishParams;
            pxPublishParams.pucTopic = ( uint8_t * ) pxParameters->topic;
            pxPublishParams.usTopicLength = ( uint16_t )  strlen(( const char * )pxParameters->topic);
            pxPublishParams.pvData = cDataBuffer;
            pxPublishParams.ulDataLength = ( uint32_t )  strlen(( const char * )cDataBuffer);
            pxPublishParams.xQoS =  eMQTTQoS1;

            if ( MQTT_AGENT_Publish(xMQTTHandle, &( pxPublishParams ),
                democonfigMQTT_TIMEOUT) == eMQTTAgentSuccess )
            {
                configPRINTF(("Outbound sent successfully.\r\n"));
            }
            else
            {
                configPRINTF(("Outbound NOT sent successfully.\r\n"));
            }

            vTaskDelay( pdMS_TO_TICKS( TEMPERATURE_TASK_READ_DELAY_MS ) );
        }
   ```

#### Adding xCreateTask for the Button Reader Task

1. Pull up code from the Temperature task to the ```void vStartMQTTEchoDemo( void )``` function.  Cut and paste the following code.  Ensure this code is removed from the Temperature task. 

2. Next, add the declaration of the properties structure we will use to compose in the topic name for the buttons state.  You can put this directly under the declaration for the properties we did in the previous lab.

   ```c
       xMQTTTaskParameter taskParameter_buttons;
   ```
3. Finally, we add the call to xTaskCreate.  Directly after the similar block for the Temperature sensor, add this code. Pay special attention to the topic name.  This is the topic where shadow updates get published.

   ```c
    if ( xReturned == pdPASS )
    {
        memset( taskParameter_buttons.topic, 0x00, sizeof( taskParameter_buttons.topic ) );
        snprintf( taskParameter_buttons.topic, sizeof( taskParameter_buttons.topic ), "$aws/things/%s/shadow/update", thing_mac_address );

        xReturned = xTaskCreate( prvButtonStatusReaderTask,
                                 "Buttons",
                                 BUTTON_TASK_STACK_SIZE,
                                 ( void * ) &taskParameter_buttons,
                                 BUTTON_TASK_PRIORITY,
                                 NULL );
    }
   ```
   
   As we did in the last lab, we need to add the declares for ```BUTTON_TASK_STACK_SIZE``` and ```BUTTON_TASK_PRIORITY```.  You can place them directly beneath ```TEMP_TASK_STACK_SIZE``` and ```TEMP_TASK_PRIORITY```.  The result will look like the following.
   
   ```c	
   ...

   static MQTTAgentHandle_t xMQTTHandle = NULL;

   #define TEMP_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE * 7 )
   #define TEMP_TASK_PRIORITY            ( tskIDLE_PRIORITY )

   #define BUTTON_TASK_STACK_SIZE        ( configMINIMAL_STACK_SIZE * 7 )
   #define BUTTON_TASK_PRIORITY          ( tskIDLE_PRIORITY )

   ...
   ```

#### Running the code

Now, we will run the code from Dave4.  Do a rebuild, and then begin the debug process.

What you will see in the terminal output window will be interesting - you will note that messages saying "Outbound sent successfully" are virtually right next to each other.

```text
37 41022 [DTS] Celsius: 36.585365
38 41124 [Buttons] b1: 0, b2: 1
39 42464 [DTS] Outbound sent successfully.
40 42566 [Buttons] Outbound sent successfully.
```

As you can see, the two tasks seem to be running simultaneously.  Since this code is running on a single core, it's not multithreaded but instruction execution is being handled by the scheduler.  Since both the BMA222 and TMP006 tasks are running at the same priority (the default priority -- we didn't change it), the scheduler is handling the "balance".

### Experiencing the Cloud Integration

In this section, you will send an alert to your mobile phone when your board's reported state exceeds the desired state.

In this section, you will be receiving messages from the AWS IoT Core message broker by a rule in AWS IoT Core Rules Engine, and routing those messages to Amazon SNS to eventually send an alert.

#### Setting up mobile notifications

Amazon SNS enables a fast, easy, and cost-effective way of sending SNS messages to mobile devices.  In this section, you will configure SNS to send messages to your phone.

1. Login to the AWS Console.
2. Click ```Services``` > ```Simple Notification Service```. Use the filter or look under Application Integration to find it.

First, we need to create a Topic to use to proxy the message.  The SMS part of a service acts as an application subscriber.

1. On the left hand side, click **Topics**.
2. In the content panel, click **Create new topic**.
3. For the **Topic name**, enter **angleEmitter**.
4. For the **Display name**, enter **AngleAlert**
5. Click the **Create topic** button.
6. After creation, the Topic will be listed in the content panel.  Copy the ARN to the clipboard (Windows, CTRL-C; Mac, Command-C)
 
3. On the left hand side, click Subscriptions.
4. In the content panel, click the **Create subscription** button.
5. In Topic ARN, paste the ARN you copied in the previous subsection (Windows, CTRL-V; Mac, Command-V)
6. For **Protocol**, select SMS from the drop-down.
7. For **Endpoint**, enter your phone number.
8. Click **Create subscription**.

#### Setting up the rule

Now we will setup the rule where if the reported angle exceeds the desired angle, then the message will be sent to SNS, and then SNS will send you a message.

1. In the AWS Console, click Services > IoT Core.
2. On the left hand side, click **Act**.
3. On the right hand side, locate and click the **Create** button.
4. For the **Name** of **Create a rule**, enter **AngleEmitter**
5. Scroll down to **Rule query statement**.  Copy and paste the following statement.

   ```sql
   select current.state.reported as reported from "$aws/things/+/shadow/update/documents" 
   where abs(current.state.reported.x) > abs(current.state.desired.x) or
   abs(current.state.reported.y) > abs(current.state.desired.y)
   ```
6. Scroll down, and click **Add action**.
7. Scroll down, and select the radio button for **Send a message as an SNS push notification**.
8. Scroll down, and click **Configure action**.
9. In **Configure action**, for SNS Target, click **No topic selected**, and click the **Select** link on the same line as **angleEmitter**.
10. For **Message format**, select **RAW**.
11. For IAM Role name, click **Create a new role**.
12. Enter name aws-iot-sns-role.
13. Click the button **Create a new role**.
14. From the drop down, select the role name.
15. Click the **Add action** button.
16. **IMPORTANT** On the lower-right hand side, click **Create rule**.

### Testing the Rule

In order to test the functionality, you need to change the desired state within the Device Shadow. 

1. Login to the AWS Console.
2. Navigate to Services > IoT Core.
3. On the left hand side, click Manage, and then click Things.
4. On the right-hand side, click your Thing.
5. On the left-hand side, click Shadow.
6. For the **Shadow Document**, click **Edit**.
7. Edit the document to show the following:

   ```json
   {
     "desired": {
        "b1": 1,
        "b2": 0
     },
     "reported": {
      "b1": 0,
      "b2": 0
     }
   }
   ``` 
8. Click **Save**.

At this point, when you run **Debug** on your device, and then press BUTTON1, you will receive an alert on your phone.

### Outcomes

In this lab, you learned a lot about handling multiple tasks on a bus where only one communication can happen at a time - and send messages to Amazon Simple Notification Service based on Device Shadow state information.

