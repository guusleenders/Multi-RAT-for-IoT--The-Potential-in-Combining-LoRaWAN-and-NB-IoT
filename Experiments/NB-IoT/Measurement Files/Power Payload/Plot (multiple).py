
import pandas as pd
import matplotlib.pyplot as plt
import LatexifyMatplotlib

SPINE_COLOR = 'gray'
FORMAT = "pdf"
MARKER = "+"

filenames = [
    "Bureau - Met shielding - Celevel 0 - -63 -73 147 -10",
    "Bureau - Met shielding - Celevel 0 - -64 -75 162 -10",
    "Bureau - Met shielding - Celevel 0 - -65 -76 129 -11",
    "Bureau - Met shielding - Celevel 0 - -68 -72 144 0",
    "Bureau - Met shielding - Celevel 0 - -69 -74 143 -5",
    "Bureau - Met shielding - Celevel 0 - -70 -74 140 0",
    "Bureau - Met shielding - Celevel 0 - -81 -92 144 -11",
    "Bureau - Met shielding - Celevel 0 - -81 -93 146 -10"
    # "Bureau - Met shielding - Celevel 0 - -82 -87 152 -11",
    # "Bureau - Met shielding - Celevel 0 - -85 -94 129 -8",
    # "Bureau - Met shielding - Celevel 0 - -86 -97 176 -10",
    # "Bureau - Met shielding - Celevel 0 - -92 -103 132 -11",
    # "Bureau - Met shielding - Celevel 0 - -95 -99 150 0",
    # "Bureau - Met shielding - Celevel 0 - -98 -108 131 -4",
    # "Bureau - Met shielding - Celevel 0 - -99 -109 114 -15"
]

measurements = pd.DataFrame()
i = 0

for filename in filenames:
    df = pd.read_csv(filename+".csv")

    inc = df["Increment"][df.index[0]]
    start = df["Start"][df.index[0]]
    df = df.drop(df.index[0])

    df["CH1"] = df["CH1"].astype(float)
    df["CH2"] = df["CH2"].astype(float)

    df["time"] = df.index*inc
    df = df.drop(columns=["Start", "Increment"])
    df['sub'] = (abs(df['CH1'] - df['CH2']))*1000#*3.98
    df['avg'] = df["sub"].rolling(window=4).mean()

    measurements["time"] = df["time"]
    measurements[i] = df['avg']

    i = i+1

#measurements[0] = measurements[1].shift(-73+36)
#measurements[1] = measurements[1].shift(-103+73)
#measurements[2] = measurements[2].shift(-113+73)
#measurements[3] = measurements[3].shift(-99+73)
#measurements[4] = measurements[4].shift(-112+73)

with plt.style.context('seaborn-muted'): #dark_background
    plt.rcParams.update({'font.family': 'CMU Serif', 'font.size': 12, 'figure.figsize': '10,4'})
    ax = plt.gca()
    ax.set_ylim([-10, 400])
    for j in range(len(filenames)):
        if j == len(filenames)-1:
            alpha = 1
        else:
            alpha = 1/len(filenames)*(j+1)

        measurements.plot(x="time", y=j, ax=ax, alpha=alpha)

    ax.spines['right'].set_visible(False)
    ax.spines['top'].set_visible(False)
    ax.get_legend().remove()
    plt.xlabel(r"Time (s)")
    plt.ylabel(r"Current (mA)")
    plt.savefig("Multiple - "+ str(len(filenames)) +".svg", bbox_inches='tight')

LatexifyMatplotlib.latexify()


#plt.plot(df["time"], df["avg"])





LatexifyMatplotlib.save(filename+".tex", plt = plt, show = True)
