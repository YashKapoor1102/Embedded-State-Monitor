'''
Name: Yash Kapoor

Monitoring App using Pyserial

'''

# Must install Pyserial to import this module
import serial 

# Threading allows us to run functions/methods concurrently
from threading import Thread

# To gain access to the keyboard module, you have to 
# go to the command prompt and type "pip install keyboard"
import keyboard

# initializing the serial port 
serialcomm = serial.Serial('COM3', 115200);
serialcomm.timeout = 1;

'''
Returns the current state of the board 
(either 0, 1, 2, or 3)
'''
def get_state(): 
    
    # reading data sent from the microcontroller
    read_state = serialcomm.readline(1000).decode()
    
    return read_state

'''
Prints the current state of the board 
to the user every time a button is 
pressed on the microcontroller 
'''
def current_state():
    initial = get_state()
    
    while True:
        
        current = get_state()
        if (initial != current):
            ''' 
            every time the user presses either button 1 or button 2 
            on the microcontroller, the current state will be displayed
            to the user 
            '''
            
            if (current == '3'):
                print("\nCurrent State: 11 (Both LEDs ON)\n")
            elif (current == '2'):
                print("\nCurrent State: 10 (RED LED ON)\n")
            elif (current == '1'):
                print("\nCurrent State: 01 (RGB LED ON)\n")
            else:
                print("\nCurrent State: 00 (Both LEDs OFF)\n") 


''' 
Allows the user to enter any state of their choice
by pressing a button on their keyboard.

User can type "q" to exit the program at any given time. 
'''
def select_state():    
    
    print('''\nWelcome to this monitoring app!\n\nHit \"s\" on your keyboard to enter a state of your choice\nat any time.\n
Type \"q\" to exit this monitoring app whenever you want.\n\n
Initial State: 00 (Both LEDs OFF)\n''')
    
    while True:
    
        if keyboard.is_pressed("q"):
            # q is pressed on the keyboard
            
            print("Thank you for using this monitoring app!")
            
            exit()
            serialcomm.close()
            break
            
        if keyboard.is_pressed("s"):
            # s is pressed on the keyboard
            
            enter_state = input("\nEnter \"0\", \"1\", \"2\", or \"3\":\t").strip()

            # sending the input that the user puts in over the serial port
            # by encoding it into bytes
            serialcomm.write(enter_state.encode())
    
            if (enter_state == '3'):
                print("Current State: 11 (Both LEDs ON)\n")

            elif (enter_state == '2'):
                print("Current State: 10 (RED LED ON)\n")

            elif (enter_state == '1'):
                print("Current State: 01 (RGB LED ON)\n")

            elif (enter_state == '0'):
                print("Current State: 00 (Both LEDs OFF)\n")

            else:
                print("You must enter a number from 0-3 to choose the state.\nTry again by pressing \"s\" on your keyboard!\n")


# Main Method
if __name__ == "__main__":
    
    # starting the thread for the function called current_state
    Thread(target = current_state).start()
       
    select_state()
    
    
