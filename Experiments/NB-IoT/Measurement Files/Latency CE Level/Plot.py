
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

filenames = glob.glob("Metingen 1/*.csv")
#filenames.extend(glob.glob("Metingen 7/*.csv"))
dataCollection = []

latencyDataFrame = pd.DataFrame(columns = ['arrived', 'sent', 'latency', 'celevel'])
latencyDataFrameHorizontal = pd.DataFrame();

print(filenames)
i = 0
for filename in filenames:
    tempDictionary = {}
    filename = filename.replace(".csv", "")
    df = pd.read_csv(filename+".csv")

    propertiesString = filename.replace("Metingen 1\\Latency ", "")
    tempDictionary["properties"] = np.array(propertiesString.split(" ")).astype(np.int16).tolist()

    df["latency"] = df["arrived"]-df["sent"]
    celevel = 'A' if tempDictionary["properties"][0] == 0 else 'B' if tempDictionary["properties"][0] == 1 else 'C'
    df["celevel"] = celevel
    latencyDataFrame = latencyDataFrame.append(df)
    latencyDataFrameHorizontal[celevel] = df["latency"]/1000

fig, ax = plt.subplots()
_, bp = latencyDataFrameHorizontal.boxplot(column=['A', 'B', 'C'], whis=[5, 95], return_type='both')

outliers = [flier.get_ydata() for flier in bp["fliers"]]
boxes = [box.get_ydata() for box in bp["boxes"]]
medians = [median.get_ydata() for median in bp["medians"]]
whiskers = [whiskers.get_ydata() for whiskers in bp["whiskers"]]

#LatexifyMatplotlib.save("latency.tex", fig=plot.get_figure(), show=False)
plt.show()


