
import pandas as pd
import matplotlib.pyplot as plt
from numpy import trapz
import numpy as np
import seaborn as sns
import glob
import logging
import LatexifyMatplotlib

SPINE_COLOR = 'gray'
FORMAT = "pdf"
MARKER = "+"


    # Boot       Search        RA + Join     Network       Message       EDRX end
times = [

]

# RSSI: An integer indicating the received signal strength. These parameters are
#       available for GSM and LTE mode respectively.
# RSRP: An integer indicating the reference signal received power (RSRP). This
#       parameter is available for LTE mode.
# SINR: An integer indicating the signal to interference plus noise ratio (SINR).
#       Logarithmic value of SINR. Values are in 1/5th of a dB. The range is 0-250
#       which translates to -20dB - +30dB.
# RSRQ: An integer indicating the reference signal received quality (RSRQ) in dB.

#   RSSI, RSRP, SINR, RSRQ

# properties =
#     [0, -71, -74, 156, 0],
#     [0, -71, -75, 157, 0],
#     [0, -72, -75, 155, 0],
#     [0, -73, -78, 143, -5],
#     [0, -100, -104, 146, 0]
#     [0, -105, -107, 135, 0]
# ]

filenames = glob.glob("Metingen 7/*.csv")
#filenames.extend(glob.glob("Metingen 7/*.csv"))
dataCollection = []

print(filenames)
i = 0
for filename in filenames:
    tempDictionary = {}

    filename = filename.replace(".csv","")

    df = pd.read_csv(filename+".csv")

    inc = df["Increment"][df.index[0]]
    start = df["Start"][df.index[0]]
    df = df.drop(df.index[0])

    df["CH1"] = df["CH1"].astype(float)
    df["CH2"] = df["CH2"].astype(float)
    df["CH3"] = df["CH3"].astype(float)

    df["trigger"] = df["CH3"].diff()
    triggerDataFrame = df[abs(df["trigger"]) >= 2]

    df["time"] = df.index*inc
    df = df.drop(columns=["Start", "Increment"])
    df['current'] = (abs(df['CH1'] - df['CH2']))#*3.98
    df['power'] = df['current']*df['CH1']

    df["CH3"] = df["CH3"]*1000

    #plt.plot("time", "CH3", data=df)
    #plt.plot("time", "current", data=df)

    # with plt.style.context('seaborn-muted'): #dark_background
    #     plt.rcParams.update({'font.family': 'CMU Serif', 'font.size': 12, 'figure.figsize': '10,4'})
    #     ax = plt.gca()
    #     df.plot(x="time", y="current", ax=ax)
    #     ax.spines['right'].set_visible(False)
    #     ax.spines['top'].set_visible(False)
    #     #ax.get_legend().remove()
    #     plt.xlabel(r"Time (s)")
    #     plt.ylabel(r"Current (mA)")
    #     #plt.savefig(filename+".svg", bbox_inches='tight')

    #LatexifyMatplotlib.latexify()
    # plt.show()

    df.drop(columns=['CH1', 'CH2', 'Unnamed: 5', 'trigger'], inplace=True)
    try:
        tempDictionary["data"] = df
        tempDictionary["times"] = [[triggerDataFrame.index[0], triggerDataFrame.index[1]]]
    except:
        print("Not valid 1: ")
        print(filename)

    try:
        propertiesString = filename.replace("Metingen 4-6\\Payload ", "")
        propertiesString = propertiesString.replace("Metingen 7\\Payload ", "")
        tempDictionary["properties"] = np.array(propertiesString.split(" ")).astype(np.int16).tolist()
    except:
        print("Not valid 2: ")
        print(filename)

    try:
        dataCollection.append(tempDictionary)
    except:
        print("Not valid 3: ")
        print(filename)

    i = i+1

energyDataFrame = pd.DataFrame(columns = ['packet', 'payload', 'celevel'])

for dataDictionary in dataCollection:
    packetEnergy = trapz(dataDictionary["data"]["power"][dataDictionary["times"][0][0]:dataDictionary["times"][0][1]],
                       x=dataDictionary["data"]["time"][dataDictionary["times"][0][0]:dataDictionary["times"][0][1]])

    if packetEnergy < 0.1500 and dataDictionary["properties"][0] == 2:
        print(dataDictionary["properties"])
        fig, ax1 = plt.subplots()
        plt.plot("time", "power", data=dataDictionary["data"])

        ax2 = ax1.twinx()
        plt.plot("time", "CH3", data=dataDictionary["data"], color="tab:red")
        plt.show()
        #dataDictionary["data"].plot(x="time", y="power")

    if packetEnergy != 0:
        energyDataFrame = energyDataFrame.append({
            'packet': packetEnergy,
            'perbyte': packetEnergy / dataDictionary["properties"][1],
            'celevel': 'A' if dataDictionary["properties"][0] == 0 else 'B' if dataDictionary["properties"][0] == 1 else 'C',
            'payload': float(dataDictionary["properties"][1]),
        }, ignore_index=True)


energyDataFrame = energyDataFrame.sort_values(by=['payload'])

fig, ax = plt.subplots()

colors = {'A':'red', 'B':'blue', 'C':'green'}
grouped = energyDataFrame.groupby('celevel')

for key, group in grouped:
    plot = group.plot(ax=ax, kind='scatter', x='payload', y='packet', label=key, color=colors[key])

LatexifyMatplotlib.save("payload.tex", fig=plot.get_figure(), show=False)
plt.show()

fig, ax = plt.subplots()

for key, group in grouped:
    plot = group.plot(ax=ax, kind='line', x='payload', y='perbyte', label=key, color=colors[key])

LatexifyMatplotlib.save("perbyte.tex", fig=plot.get_figure(), show=False)
plt.show()


