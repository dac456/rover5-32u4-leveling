#! /usr/bin/env python

import sys

import serial
import matplotlib.pyplot as plt
import numpy as np

if __name__ == '__main__':
    if len(sys.argv) > 1:
        ser = serial.Serial("/dev/"+str(sys.argv[1]), 57600)
        ser.flush()

        pitch = []
        heading = []
        x = []
        y = []

        print "Press Ctrl+C to break and gerate plots."

        while True:

            try:
                line = ser.readline()
                print line
                if len(line) > 0:
                    if(line[0] == 'a'):
                        line = line[1:]
                        try:
                            v = float(line)
                            pitch.append(v)
                        except ValueError:
                            pass
                    if(line[0] == 'b'):
                        line = line[1:]
                        try:
                            v = float(line)
                            heading.append(v)
                        except ValueError:
                            pass
                    if(line[0] == 'x'):
                        line = line[1:]
                        try:
                            v = float(line)
                            x.append(v)
                        except ValueError:
                            pass
                    if(line[0] == 'y'):
                        line = line[1:]
                        try:
                            v = float(line)
                            y.append(v)
                        except ValueError:
                            pass
            except KeyboardInterrupt:
                break

        # Generate plot
        fig, ax = plt.subplots(nrows=2, ncols=2)

        ax[0,0].set_title("Pitch")
        #ax[0,0].set_xlabel("Timesteps (100ms)")
        ax[0,0].plot([i for i in range(len(pitch))], pitch)

        ax[0,1].set_title("Heading")
        #ax[0,1].set_xlabel("Timesteps (100ms)")
        ax[0,1].plot([i for i in range(len(heading))], heading)

        """ax[1,0].set_title("Position")
        ax[1,0].set_xlabel("Timesteps (100ms)")
        y2 = []
        for i in range(len(x)):
            y2.append(x[i])
        ax[1,0].scatter(y2,x)
        ax[1,0].plot()

        ax[1,1].set_title("Heading")
        ax[1,1].set_xlabel("Timesteps (100ms)")
        ax[1,1].plot([i for i in range(len(heading))], heading)"""

        plt.show()

        ser.flush()
        ser.close()

    else:
        print "Serial port must be specified."
