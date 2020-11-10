#include <stdio.h>
#include <stdlib.h>
//Different state of ATM machine
typedef enum
{
    Idle_State,
    Card_Inserted_State,
    Pin_Eentered_State,
    Option_Selected_State,
    Amount_Entered_State,
} eSystemState;
//Different type events
typedef enum
{
    Card_Insert_Event,
    Pin_Enter_Event,
    Option_Selection_Event,
    Amount_Enter_Event,
    Amount_Dispatch_Event
} eSystemEvent;
//Prototype of eventhandlers
eSystemState AmountDispatchHandler(void)
{
    return Idle_State;
}
eSystemState EnterAmountHandler(void)
{
    return Amount_Entered_State;
}
eSystemState OptionSelectionHandler(void)
{
    return Option_Selected_State;
}
eSystemState EnterPinHandler(void)
{
    return Pin_Eentered_State;
}
eSystemState InsertCardHandler(void)
{
    return Card_Inserted_State;
}
int main(int argc, char *argv[])
{
    eSystemState eNextState = Idle_State;
    eSystemEvent eNewEvent;
    char input;
    while(1)
    {
        printf("curState: %d\n", eNextState);
        //Read system Events
        printf("please enter event\n0 = Card_Insert_Event\n1 = Pin_Enter_Event\n2 = Option_Selection_Event\n3 = Amount_Enter_Event\n4 = Amount_Dispatch_Event\n");
        input = getchar( );
        eSystemEvent eNewEvent = atoi(&input);
        switch(eNextState)
        {
        case Idle_State:
        {
            if(Card_Insert_Event == eNewEvent)
            {
                eNextState = InsertCardHandler();
            }
        }
        break;
        case Card_Inserted_State:
        {
            if(Pin_Enter_Event == eNewEvent)
            {
                eNextState = EnterPinHandler();
            }
        }
        break;
        case Pin_Eentered_State:
        {
            if(Option_Selection_Event == eNewEvent)
            {
                eNextState = OptionSelectionHandler();
            }
        }
        break;
        case Option_Selected_State:
        {
            if(Amount_Enter_Event == eNewEvent)
            {
                eNextState = EnterAmountHandler();
            }
        }
        break;
        case Amount_Entered_State:
        {
            if(Amount_Dispatch_Event == eNewEvent)
            {
                eNextState = AmountDispatchHandler();
            }
        }
        break;
        default:
            printf("invalid input\n");
            break;
        }
    }
    return 0;
}

