### Company: Tesla
### Position: Firmware

You have 120 minutes to complete the test. There are 100 points total.

All solutions should compile in Coderpad.io without error or warnings

Penalties:
 -1 / minute over time
 -3 for 1 or more compilation errors
 -2 for 1 or more compilation warnings

Do not use outside aid or share the content of this test

A main() function is provided at the bottom for your use
 
    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <stdbool.h>
    

    /* TESLA MOTORS FIRMWARE TEST
    * You have 120 minutes to complete the test. There are 100 points total.
    *
    * All solutions should compile in Coderpad.io without error or warnings
    *
    * Penalties:
    * -1 / minute over time
    * -3 for 1 or more compilation errors
    * -2 for 1 or more compilation warnings
    *
    * Do not use outside aid or share the content of this test
    *
    * A main() function is provided at the bottom for your use
    */
    
    ////////////////////////////////////////////////////////////////////////////////
    // 1) Macro (10 points)
    //    Create a macro (named C_TO_F) to convert from degrees Celsius to Fahrenheit
    //    Macro should work for integer or floating point types
    //    Note: degF = degC * (9/5) + 32

    #define VAR1 (9.0f)
    #define VAR1 (5.0f)
    #define C_TO_F(degC) \
            (degC * (VAR1 / VAR2) +32)


    
    ////////////////////////////////////////////////////////////////////////////////
    // 2) Bit Manipulation (5 points)
    //    Write a function that inverts (0 -> 1 or 1 -> 0) the most significant and
    //    least significant bits of the data value pointed to by b.
    
    void flip_hi_lo(uint8_t* b)
    {
        uint8_t op = 0x81   //1000 0001
        *b ^= op;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // 3) Debugging (5 points)
    //    The function computeSquareADC() has not been producing correct
    //    output consistently. Please describe all issues with the function.
    // Answer: TODO

    //1. The return type is uint8_t, and the retval type is defined as uint16_t, the result will be not accurate.
    //2. volatile type makes ADC_RESULT may change value when doing multiplication.
    
    ////////////////////////////////////////////////////////////////////////////////
    // 4) Memory dump (10 points)
    //    The following memory dump was taken while debugging an issue.
    //
    // Memory Dump:
    // Address:  Byte:
    // 0x1000    0xA0
    // 0x1001    0x0A
    // 0x1002    0xBA
    // 0x1003    0x48
    // 0x1004    0x2C
    // 0x1005    0xB7
    // 0x1006    0x3B
    // 0x1007    0x82
    // 0x1008    0x9C
    // 0x1009    0xE5
    // 0x100A    0x17
    // 0x100B    0x40
    // 0x100C    0xEF
    // 0x100D    0x47
    // 0x100E    0x0F
    // 0x100F    0x98
    // 0x1010    0x6F
    // 0x1011    0xD5
    // 0x1012    0x70
    // 0x1013    0x9E
    // 0x1014    0x94
    // 0x1015    0x99
    // 0x1016    0x4A
    // 0x1017    0xBA
    // 0x1018    0xCA
    // 0x1019    0xB2
    // 0x101A    0x32
    // 0x101B    0xE6
    // 0x101C    0x8E
    // 0x101D    0xB9
    // 0x101E    0xC5
    // 0x101F    0x2E
    // 0x1020    0xC3
    //
    // System is 32-bit, little-endian.
    // A variable called myPacket is of type packet_S (typedef below).
    // (Default compiler options; unpacked, naturally aligned.) -->This means no padding(-Colin)
    // The address of myPacket is 0x1010.
    //
    typedef struct
    {
        uint8_t count;
        uint16_t data[2]; -->Should be no padding in between elements, thus 2bytes*2(-Colin)
        uint32_t timestamp;
    } packet_S;
    
    // a) What are the values of each member of myPacket?
    //Answer: 
    count = 0x6F
    data[0] = 0x9994
    data[1] = 0xBA4A 
    timestamp = 0xE632B2CA
    
    // b) If the system was big-endian, what would the values of each member of
    //    myPacket be?
    //Answer:
    count = 0x6F
    data[0] = 0x9499
    data[1] = 0x4ABA
    timestamp = 0xCAB232E6

    ////////////////////////////////////////////////////////////////////////////////
    // 5) State Machine (20 points)
    //
    //    Complete the function below to implement the state machine shown in the
    //    diagram below for an electronic gumball vending machine.
    //     * The initial state of the state machine should be IDLE
    //     * The function should output the current state of the state machine
    //     * Unexpected or invalid input should not cause a state transition
    //     * GENERIC_FAULT may be received in any state and should put the machine
    //       into the FAULT state
    
    typedef enum
    {
        IDLE,
        READY,
        VENDING,
        FAULT
    } state_E;
    
    typedef enum
    {
        COIN,
        COIN_RETURN,
        BUTTON,
        VEND_COMPLETE,
        GENERIC_FAULT
    } input_E;


    state_E currState = IDLE;//set default current state to be IDLE
    state_E stateMachine(input_E input)
    {
        state_E retVal = currState;
        switch(input)
        {	
            case GENERIC_FAULT:
                    retVal = FAULT;;
                    currState = retVal;
                    break;
            case COIN:
                if (currState = IDLE) {
                    retVal = READY;
                    currState = retVal;
                }
                break;
                case COIN_RETURN:
                if (currState = READY) {
                    retVal = IDLE;
                    currState = retVal;
                }
                case BUTTON:
                    if (currState = READY) {
                        retVal = VENDING;
                        currState = retVal;
                    }
            break;
                case VEND_COMPLETE:
                    if (currState == VENDING) {
                        retVal = IDLE;
                        currState = retVal;           
                    }
                    break;
        default: 
                break;
        }
        
        return retVal;

    }
    
    
    ////////////////////////////////////////////////////////////////////////////////
    // 6) Unit Testing (10 points)
    //    Write a unit test for validatePointerAndData that exercises all code paths
    //    and branch conditions
    
    // @param dataPtr - int32_t pointer to data to be used
    //
    // @return TRUE if pointer is non-NULL, data value is positive, non-zero and not
    //         equal to the sentinel value 0x7FFFFFFF, FALSE otherwise
    //
    bool validatePointerAndData(int32_t* dataPtr)
    {
        bool status = false;
        if ((dataPtr != NULL) &&
            (*dataPtr > 0)    &&
            (*dataPtr != 0x7FFFFFFF))
        {
            status = true;
        }
        return status;
    }
    
    //
    // @return TRUE if all tests pass, FALSE otherwise
    //
    bool test_validatePointerAndData(void)
    { 
        //Answer: TODO
        bool result = false;

        result  = (validatePointerAndData(NULL) == false);
        If (!result) return result;

        Int32_t test_val = -1;
        result  = (validatePointerAndData(&test_val ) == false);
        If (!result) return result;

        test_val = 1;
        result  = (validatePointerAndData(&test_val ) == true);
        If (!result) return result;

        test_val = 0x7FFFFFFF;
        result  = (validatePointerAndData(&test_val ) == false);
        If (!result) return result;
    return result;

    }
    
    
    ////////////////////////////////////////////////////////////////////////////////
    // 7) Low Pass Filter (10 points)
    //    Implement a function that will be called at 10hz (every 100 ms) and returns
    //    an exponentially weighted average. The latest sample is given 1/10 weighting
    //    and previous filtered value a weighting of 9/10. The function should
    //    initialize the filter to the first sample value received if it is the first
    //    time the function has run.
    
    float value;
    int inited = 0;
    float lowPassSamples_10hz(float sample)
    {
        // Answer: TODO
        Static Float lpf_val;
        lpf_val = sample/10.0 + ((lpf_val) - (lpf_val)/10.0);
        mdelay(100);    
        Return lpf_val;
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // 8a) Buffer (20 points: 8a + 8b)
    //     Create a function to push a char into a FIFO. The FIFO should be implemented
    //     as a circular buffer of length 20. The FIFO will be used to cache the most
    //     recent data from a data stream, therefore, drop the oldest value if the
    //     buffer is full.
    //Answer:

    #define MAX_CIRCULAR_BUFFER_SIZE 20
    uint8_t current_num_elements = 0;
    uint8_t readInd = 0;
    uint8_t writeInd = 0;
    uint8_t cirular_buffer[MAX_CIRCULAR_BUFFER_SIZE];
    void EnQueue(int input_data)
    {
        if (readInd == writeInd && 
            current_num_elements ==  MAX_CIRCULAR_BUFFER_SIZE) {
                printf("%reaching max elements, discard the oldest item\n");
                readInd++;
                readInd = readInd % MAX_CIRCULAR_BUFFER_SIZE;
        }

        cirular_buffer[writeInd] = input_data;

        writeInd++;

        if (current_num_elements != MAX_CIRCULAR_BUFFER_SIZE) {
            current_num_elements++;
        }

        if (writeInd == MAX_CIRCULAR_BUFFER_SIZE){
            writeInd = 0;
        }
}

    
    ////////////////////////////////////////////////////////////////////////////////
    // 8b) Create a function to print out and empty the data buffer.
    //     Data should be printed in order from oldest to newest, active elements only.


    // Answer:
    uint8_t current_num_elements = 0;
    void printAndEmptyBuffer(void)
    {
        if (!current_num_elements) {
            return;
        }
        while (current_num_elements) {
            if(readInd == MAX_CIRCULAR_BUFFER_SIZE) {
                readInd = 0;
            }
            printf("%d\n", circular_buffer[readInd]);
            readInd++;
            current_num_elements--;
        }
    }

    
    ////////////////////////////////////////////////////////////////////////////////
    // 8c) Interrupts (10 points)
    //     The function bufferPush_ISR() will be called from an interrupt service
    //     routine whenever new data is available to be buffered.
    //     The function printAndEmptyBuffer() will be called from a periodic task.
    //     The functions disableInterrupts() and enableInterrupts() are available
    //     for disabling and enabling interrupts, respectively.
    //
    //     In your implementations of bufferPush_ISR() and printAndEmptyBuffer(),
    //     determine whether or not it is necessary to disable/enable interrupts.
    //     If so, add comments where the calls are necessary. If not required,
    //     briefly comment why not.
    //
    // Answer: In the bufferPush_ISR() we should call enableInterrupts() while servicing and should call disableInterrupts() when we are exiting, by doing so we can prevent cases where the current bufferPush_ISR() has not finsihed yet and new interrupt is coming in. Also in the printAndEmptyBuffer() we should do the same since in bufferPush_ISR() we have mecahnism to override when overflow happens which means that readInd will be updated. As readInd will be accessed by both ISR and task, we need to ensure mutual exclusion so
    that circular buffer can function properly.
    =======================================================================
