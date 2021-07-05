
import pandas as pd
import matplotlib.pyplot as plt
from numpy import trapz
from numpy import average
import seaborn as sns
import LatexifyMatplotlib as lm

SPINE_COLOR = 'gray'
FORMAT = "pdf"
MARKER = "+"

filename = "Bureau - Met shielding - Celevel 1 - -111 -120 127 -8"

    # Boot       Search        RA + Join     Network       Message       EDRX end
times = [
    [[0, 0],     [93, 171],    [203, 243],   [279, 302],   [360, 372],   [593]],
    [[55, 92],   [127, 206],   [303, 327],   [373, 400],   [454, 464],   [742]],
    [[109, 134], [196, 254],   [274, 307],   [348, 367],   [446, 458],   [778]],
    [[24, 53],   [94, 165],    [170, 203],   [249, 271],   [317, 324],   [646]],
    [[180, 212], [250, 323],   [360, 393],   [435, 482],   [501, 531],   [848]],
    [[0, 34],    [58, 105],    [147, 183],   [229, 244],   [297, 320],   [698]],
    [[142, 173], [236, 282],   [324, 357],   [396, 420],   [489, 494],   [835]],
    [[120, 154], [200, 278],   [338, 371],   [427, 448],   [500, 507],   [799]],
    [[78, 113],  [149, 224],   [324, 429],   [596, 628],   [756, 781],   [996]],
    [[198, 220], [270, 362],   [455, 536],   [553, 576],   [635, 654],   [870]],
    [[184, 215], [256, 333],   [396, 427],   [462, 493],   [548, 556],   [912]],
    [[29, 64],   [102, 174],   [185, 237],   [266, 291],   [334, 357],   [695]],
    [[74, 107],  [159, 219],   [290, 322],   [370, 414],   [461, 474],   [691]],
    [[474, 490], [540, 621],   [670, 728],   [772, 805],   [850, 866],   [1087]],
    [[26, 60],   [97, 172],    [223, 285],   [334, 381],   [413, 424],   [645]],
    [[156, 207], [227, 303],   [416, 476],   [523, 556],   [607, 617],   [835]],
    [[112, 159], [208, 255],   [286, 427],   [490, 535],   [567, 577],   [972]],
    [[8, 50],    [102, 148],   [170, 226],   [282, 296],   [349, 377],   [607]],
    [[98, 131],  [194, 245],   [318, 373],   [432, 447],   [494, 506],   [723]],
    [[283, 302], [356, 428],   [488, 541],   [600, 614],   [664, 676],   [891]],
    [[200, 234], [296, 346],   [415, 477],   [530, 570],   [604, 618],   [838]],
    [[218, 251], [303, 361],   [454, 527],   [579, 598],   [644, 655],   [1092]],
    [[142, 175], [214, 289],   [366, 437],   [490, 505],   [569, 580],   [829]],
    [[80, 114],  [167, 225],   [269, 343],   [396, 410],   [472, 484],   [703]],
    [[134, 165], [217, 280],   [389, 531],   [584, 638],   [668, 678],   [900]],
    [[238, 270], [334, 380],   [415, 457],   [517, 529],   [581, 591],   [807]],
    [[124, 143], [220, 270],   [316, 476],   [510, 555],   [586, 615],   [822]],
    [[86, 118],  [170, 231],   [260, 402],   [438, 487],   [525, 535],   [755]],
    [[199, 235], [292, 350],   [388, 481],   [495, 534],   [575, 615],   [777]],
    [[0, 30],    [156, 198],   [212, 315],   [355, 414],   [457, 487],   [902]],
    [[180, 212], [275, 321],   [458, 600],   [648, 709],   [751, 799],   [1026]],
    #[[68, 104],  [289, 405],   [412, 430],   [479, 533],   [566, 579],   [799]],
    [[82, 108],  [169, 217],   [247, 386],   [427, 483],   [517, 590],   [821]],
    [[128, 155], [212, 271],   [381, 516],   [597, 715],   [751, 809],   [1075]],
    [[156, 175], [251, 328],   [394, 514],   [553, 609],   [645, 677],   [1051]],
    [[194, 228], [290, 344],   [429, 535],   [596, 637],   [670, 705],   [940]]
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
properties = [
    ["A", -63, -73, 147, -10],
    ["A", -64, -75, 162, -10],
    ["A", -65, -76, 129, -11],
    ["A", -68, -72, 144, 0],
    ["A", -69, -74, 143, -5],
    ["A", -70, -74, 140, 0],
    ["A", -81, -92, 144, -11],
    ["A", -81, -93, 146, -10],
    ["A", -82, -87, 152, -11],
    ["A", -85, -94, 129, -83],
    ["A", -86, -97, 176, -10],
    ["A", -92, -103, 132, -11],
    ["A", -95, -99, 150, 0],
    ["A", -98, -108, 131, -4],
    ["A", -99, -109, 114, -15],
    ["B", -104, -117, 133, -13],
    ["B", -105, -121, 128, -11],
    ["B", -109, -119, 121, -9],
    ["B", -110, -117, 135, -7],
    ["B", -111, -117, 129, -12],
    ["B", -111, -118, 140, -7],
    ["B", -111, -119, 117, -8],
    ["B", -111, -120, 127, -8],
    ["B", -112, -118, 129, -6],
    ["B", -112, -119, 123, -7],
    ["B", -115, -119, 129, 0],
    ["C", -110, -121, 110, -10],
    ["C", -110, -121, 125, -10],
    ["C", -111, -126, 66, -20],
    ["C", -111, -127, 108, -10],
    ["C", -112, -119, 117, -6],
    #["C", -112, -120, 123, -8],
    ["C", -113, -125, 112, -11],
    ["C", -113, -131, 88, -18],
    ["C", -114, -127, 96, -12],
    ["C", -114, -130, 77, -16]
]

filenames = [                                                # Boot       Search     RA + Join  Network    Message    EDRX end
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -63 -73 147 -10",  #            93-171     203-243    279-302    360-372    593
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -64 -75 162 -10",  # 55-92      127-206    303-327    373-400    454-464    742
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -65 -76 129 -11",  # 109-134    196-254    274-307    348-367    446-458    778
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -68 -72 144 0",    # 24-53      94-165     170-203    249-271    317-324    646
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -69 -74 143 -5",   # 180-212    250-323    360-393    435-482    501-531    848
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -70 -74 140 0",    # 0-34       58-105     147-183    229-244    297-320    698
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -81 -92 144 -11",  # 142-173    236-282    324-357    396-420    489-494    835
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -81 -93 146 -10",  # 120-154    200-278    338-371    427-448    500-507    799
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -82 -87 152 -11",  # 78-113     149-224    324-429    596-628    756-781    996
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -85 -94 129 -8",   # 198-220    270-362    455-536    553-576    635-653    870
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -86 -97 176 -10",  # 184-215    256-333    396-427    462-493    548-556    912
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -92 -103 132 -11", # 29-64      102-174    185-237    266-291    334-357    695
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -95 -99 150 0",    # 74-107     159-219    290-322    370-414    461-474    691
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -98 -108 131 -4",  # 474-490    540-621    670-728    772-805    850-866    1087
    "../Metingen 2/Bureau - Met shielding - Celevel 0 - -99 -109 114 -15", # 26-60      97-172     223-285    334-381    413-424    645
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -104 -117 133 -13",# 156-207    227-303    416-476    523-556    607-617    835
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -105 -121 128 -11",# 112-159    208-255    286-427    490-535    567-577    972
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -109 -119 121 -9", # 8-50       102-148    170-226    282-296    349-377    607
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -110 -117 135 -7", # 98-131     194-245    318-373    432-447    494-506    723
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -111 -117 129 -12",# 283-302    356-428    488-541    600-614    664-676    891
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -111 -118 140 -7", # 200-234    296-346    415-477    530-570    604-618    838
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -111 -119 117 -8", # 218-251    303-361    454-527    579-598    644-655    1092
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -111 -120 127 -8", # 142-175    214-289    366-437    490-505    569-580    829   -
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -112 -118 129 -6", # 80-114     167-225    269-343    396-410    472-484    703
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -112 -119 123 -7", # 134-165    217-280    389-531    584-638    668-678    900
    "../Metingen 2/Bureau - Met shielding - Celevel 1 - -115 -119 129 0",  # 238-270    334-380    415-457    517-529    581-591    807
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -110 -121 110 -10",# 124-143    220-270    316-476    510-555    586-615    822
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -110 -121 125 -10",# 86-118     170-231    260-402    438-487    525-535    755
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -111 -126 66 -20", # 199-235    292-350    388-481    495-534    575-615    777
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -111 -127 108 -10",# 0-30       156-198    212-315    355-414    457-487    902
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -112 -119 117 -6", # 180-212    275-321    458-600    648-709    751-799    1026
    #../Metingen 2/"Bureau - Met shielding - Celevel 2 - -112 -120 123 -8", # 68-104     289-405    412-430    479-533    566-579    799
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -113 -125 112 -11",# 82-108     169-217    247-386    427-483    517-590    821  -
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -113 -131 88 -18", # 128-155    212-271    381-516    597-715    751-809    1075
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -114 -127 96 -12", # 156-175    251-328    394-514    553-609    645-677    1051
    "../Metingen 2/Bureau - Met shielding - Celevel 2 - -114 -130 77 -16"  # 194-228    290-344    429-535    596-637    670-705    940
]

dataCollection = []

for filename in filenames:
    df = pd.read_csv(filename+".csv")

    inc = df["Increment"][df.index[0]]
    start = df["Start"][df.index[0]]
    df = df.drop(df.index[0])

    df["CH1"] = df["CH1"].astype(float)
    df["CH2"] = df["CH2"].astype(float)

    df["time"] = df.index*inc
    df = df.drop(columns=["Start", "Increment"])
    df['current'] = (abs(df['CH1'] - df['CH2']))*1000#*3.98
    df['power'] = df['current']*df['CH1']
    df['average'] = df["current"].rolling(window=50).mean()


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
    #df.plot(x="time", y="power")
    #plt.show()

    df.drop(columns=['CH1', 'CH2', 'Unnamed: 4', 'average'], inplace=True)
    dataCollection.append(df)

energyDataFrame = pd.DataFrame(columns = ['search', 'join', 'network', 'searchjoinnetwork', 'packet', 'rsrp', 'rssi', 'sinr', 'rsrq', 'celevel'])

for i, time in enumerate(times):
    searchEnergy = trapz(dataCollection[i]["power"][time[1][0]:time[1][1]],
                         x=dataCollection[i]["time"][time[1][0]:time[1][1]])
    joinEnergy = trapz(dataCollection[i]["power"][time[2][0]:time[2][1]],
                       x=dataCollection[i]["time"][time[2][0]:time[2][1]])
    networkEnergy = trapz(dataCollection[i]["power"][time[3][0]:time[3][1]],
                          x=dataCollection[i]["time"][time[3][0]:time[3][1]])
    packetEnergy = trapz(dataCollection[i]["power"][time[4][0]:time[4][1]],
                         x=dataCollection[i]["time"][time[4][0]:time[4][1]])
    cdrxEnergy = trapz(dataCollection[i]["power"][time[4][1]:time[5][0]],
                         x=dataCollection[i]["time"][time[4][1]:time[5][0]])
    edrxEnergy = average(dataCollection[i]["power"][time[5][0]:])*10.24 # 10.24 seconds in drx (requested)
    searchJoinNetworkEnergy = searchEnergy + joinEnergy + networkEnergy;

    energyDataFrame = energyDataFrame.append({
        'search': searchEnergy,
        'join': joinEnergy,
        'network': networkEnergy,
        'searchjoinnetwork': searchJoinNetworkEnergy,
        'packet': packetEnergy,
        'cdrx' : cdrxEnergy,
        'edrx': edrxEnergy,
        'celevel': properties[i][0],
        'rssi': properties[i][1],
        'rsrp': properties[i][2],
        'sinr': properties[i][3],
        'rsrq': properties[i][4]
    }, ignore_index=True)


energyDataFrame = energyDataFrame.sort_values(by=['rsrp'])

sns.scatterplot(x="rsrp", y="search", hue="celevel", data=energyDataFrame)
plt.show()
sns.scatterplot(x="rsrp", y="join", hue="celevel", data=energyDataFrame)
plt.show()
sns.scatterplot(x="rsrp", y="network", hue="celevel", data=energyDataFrame)
plt.show()
sns.scatterplot(x="rsrp", y="searchjoinnetwork", hue="celevel", data=energyDataFrame)
plt.show()
sns.scatterplot(x="rsrp", y="packet", hue="celevel", data=energyDataFrame)
plt.show()

labels = 'Search', 'Join', 'Network', 'Packet', 'cDRX', 'eDRX'
energyDataFrameA = energyDataFrame.loc[energyDataFrame['celevel'] == "A"]
energySplitA = [energyDataFrameA["search"].mean(),
                energyDataFrameA["join"].mean(),
                energyDataFrameA["network"].mean(),
                energyDataFrameA["packet"].mean(),
                energyDataFrameA["cdrx"].mean(),
                energyDataFrameA["edrx"].mean()]
plt.pie(energySplitA, labels=labels, autopct='%1.1f%%', counterclock=False, startangle=90 )
lm.latexify(fig_width=4, fig_height=4)
lm.save("energySplitA.tex", plt=plt, show=True)

energyDataFrameB = energyDataFrame.loc[energyDataFrame['celevel'] == "B"]
energySplitB = [energyDataFrameB["search"].mean(),
                energyDataFrameB["join"].mean(),
                energyDataFrameB["network"].mean(),
                energyDataFrameB["packet"].mean(),
                energyDataFrameB["cdrx"].mean(),
                energyDataFrameB["edrx"].mean()]
plt.pie(energySplitB, labels=labels, autopct='%1.1f%%', counterclock=False, startangle=90 )
lm.latexify(fig_width=4, fig_height=4)
lm.save("energySplitB.tex", plt=plt, show=True)

energyDataFrameC = energyDataFrame.loc[energyDataFrame['celevel'] == "C"]
energySplitC = [energyDataFrameC["search"].mean(),
                energyDataFrameC["join"].mean(),
                energyDataFrameC["network"].mean(),
                energyDataFrameC["packet"].mean(),
                energyDataFrameC["cdrx"].mean(),
                energyDataFrameC["edrx"].mean()]
plt.pie(energySplitC, labels=labels, autopct='%1.1f%%', counterclock=False, startangle=90 )
lm.latexify(fig_width=4, fig_height=4)
lm.save("energySplitC.tex", plt=plt, show=True)



print(energyDataFrame)
#LatexifyMatplotlib.save(filename+".tex", plt = plt, show = True)
