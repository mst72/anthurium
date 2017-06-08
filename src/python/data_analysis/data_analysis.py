#!/usr/bin/python
# -*- coding: utf-8 -*-

__author__ = 'Master'
__version__ = '0.0.0.1'
import matplotlib.pyplot as plt
import datetime


def process_data(fname):
    ctrs = []
    dates = []
    hours = []
    temperature = []
    humm = []
    msens = []
    wmarks = []
    daytimes = []
    wsign = 0
    watering = [
                datetime.datetime.strptime("2017-05-25 17:00:00.000000", "%Y-%m-%d %H:%M:%S.%f"),
                datetime.datetime.strptime("2017-05-31 08:50:00.000000", "%Y-%m-%d %H:%M:%S.%f")
               ]
    with open(fname, mode="rt") as myfile:
        for ctr, line in enumerate(myfile.readlines()):
            items = line.split('\t')
            point = items[0].strip()
            dt = datetime.datetime.strptime(point, "%Y-%m-%d %H:%M:%S.%f")
            if len(watering):
                item = watering[0]
                if dt > item:
                    if wsign == 800:
                        wsign = 0
                    else:
                        watering.pop(0)
                        wsign = 800
            titem = items[1].split(':')[1].strip()
            hitem = items[2].split(':')[1].strip()
            mitem = items[3].split(':')[1].strip()
            if titem and hitem and mitem:
                pass
            else:
                continue
            ctrs.append(ctr)
            dates.append(dt)
            if 20 >= dt.hour >= 8:
                daytime = 20
            else:
                daytime = 8
            daytimes.append(daytime)
            hours.append(dt.hour)
            temperature.append(float(titem))
            humm.append(float(hitem))
            msens.append(float(mitem))
            wmarks.append(wsign)

    plt.figure(1)
    plt.subplot(511)
    plt.plot(dates, temperature)
    plt.title("Temperature")

    plt.subplot(512)
    plt.plot(dates, humm)
    plt.title("Hummidity")

    ax = plt.subplot(513)
    plt.plot(dates, msens, "b", dates, wmarks, "r")
    plt.title("M-sensor")
    ax.axhline(y=600, c="green", linewidth=0.5, zorder=0)
    ax.axhline(y=300, c="green", linewidth=0.5, zorder=0)
    ax.axhline(y=200, c="green", linewidth=0.5, zorder=0)

    ax = plt.subplot(514)
    plt.plot(dates, hours, "b", dates, daytimes, "r")
    plt.title("Hours")
    ax.axhline(y=20, c="green", linewidth=0.5, zorder=0)
    ax.axhline(y=8, c="green", linewidth=0.5, zorder=0)

    ax = plt.subplot(515)
    plt.plot(dates, ctrs)
    plt.title("Ctrs")

    plt.show()

if __name__ == "__main__":
    process_data("./data/data.txt")