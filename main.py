try:
    from tkinter import *
    from tkinter import ttk as tk
except ImportError:
    print("Please use Python 3.5 or newer")
    exit()
import time
import serial

arduino = serial.Serial('/dev/ttyACM0')
arduino.baudrate = 9600
arduino.timeout = None

if (not arduino.is_open):
    exit()

def calculate(*args):
    arduino.write(bytes("1", "ascii"))
    try:
        T = arduino.readline().split()
        if (T[0] != bytes("Temperature(C):", "ascii")):
            print(T[0])
            raise ValueError
        xPos = arduino.readline().split()
        yPos = arduino.readline().split()
        if (T):
            temperature.set(T[1])
            xShow.set(xPos[1]);
            yShow.set(yPos[1]);
            hits.set(hits.get()+1)
        else:
            misses.set(misses.get()+1)
    except ValueError:
        print("Temperature field incorrect. Resetting.")
    except:
        pass
    root.after(100, calculate)

def sendDataWrapper():
    sendData(None)

def sendData(event):
    arduino.write(bytes("1"+data.get(), "ascii"))

def endTK(event):
    root.destroy()
    arduino.close()
    quit = True

def key(event):
    if (str(event.keysym) == 'asdf'):
        endTK()

root = Tk()
root.title("[JUMP] JIAM Universal Measurement Probe")
root.configure(bg="white")

quit = False


# Variables

temperature = StringVar()
hits = IntVar()
misses = IntVar()
xShow = StringVar()
yShow = StringVar()
data = StringVar()


# Output

tk.Label(root, text="Temperature (C)").grid(column=0, row=0)
tk.Label(root, textvariable=temperature).grid(column=1, row=0)

tk.Label(root, text="xShow").grid(column=0, row=1)
tk.Label(root, textvariable=xShow).grid(column=1, row=1)

tk.Label(root, text="yShow").grid(column=0, row=2)
tk.Label(root, textvariable=yShow).grid(column=1, row=2)

tk.Label(root, text="Hits").grid(column=0, row=3)
tk.Label(root, textvariable=hits).grid(column=1, row=3)

tk.Label(root, text="Misses").grid(column=0, row=4)
tk.Label(root, textvariable=misses).grid(column=1, row=4)


# Input

ardData = tk.Entry(root, textvariable=data).grid(column=0, row=5)
sendButton = tk.Button(root, text="Send Data", command=sendDataWrapper).grid(column=1, row=5)

for child in root.winfo_children():
    child.grid_configure(padx=5, pady=5, sticky=W+E+N)
    try:
        child.configure(foreground="black", background="pink")
    except:
        pass

root.bind('<Key>', key)
root.bind('<Escape>', endTK)
root.bind('<Return>', sendData)
root.after(100, calculate)
root.mainloop()
