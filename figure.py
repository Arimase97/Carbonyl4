import matplotlib.pyplot as plt

file = open("figure/raw_data.txt", "r")


def CompressFigure(exper, xlabel, xlim, xticks, ylabel, ylim, metric, legend):
    plt.figure(figsize=(4, 3))
    plt.yscale('linear')
    plt.tick_params(labelsize=12)
    plt.xlabel(xlabel, fontweight='bold', fontsize=12)
    plt.ylabel(ylabel.replace("_", " "), fontweight='bold', fontsize=12)
    plt.plot(param, metric[0], label=name[0], linestyle='-', marker='o',
             markersize=7, alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    plt.plot(param, metric[1], label=name[1], linestyle='-', marker='v',
             markersize=7, alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    ax = plt.gca()
    plt.xlim(xlim)
    ax.set_xticks(xticks)
    plt.ylim(ylim)
    ax.yaxis.get_offset_text().set_fontsize(12)
    # ax.set_yscale('log', base=10)
    ax.set_yscale('linear')
    plt.ticklabel_format(style='scientific', scilimits=(0, 0), axis='y')
    plt.legend(loc=legend, ncol=2, handlelength=1, fontsize=10)
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    plt.setp(ltext, fontweight='bold', fontsize=10)
    plt.grid(True, linestyle='--', axis='y')
    plt.grid(True, linestyle='--', axis='x')
    plt.tight_layout()
    plt.savefig("./figure/" + exper + "_" + ylabel + ".pdf")


def ParameterFigure(exper, xlabel, xlim, xticks, ylabel, ylim, metric, legend):
    plt.figure(figsize=(4, 3))
    plt.yscale('linear')
    plt.tick_params(labelsize=12)
    plt.xlabel(xlabel, fontweight='bold', fontsize=12)
    plt.ylabel(ylabel.replace("_", " "), fontweight='bold', fontsize=12)
    plt.plot(param, metric, label="Carbonyl4 (Ours)", linestyle='-', marker='o',
             markersize=7, alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    ax = plt.gca()
    plt.xlim(xlim)
    ax.set_xticks(xticks)
    plt.ylim(ylim)
    ax.yaxis.get_offset_text().set_fontsize(12)
    # ax.set_yscale('log', base=10)
    ax.set_yscale('linear')
    plt.ticklabel_format(style='scientific', scilimits=(0, 0), axis='y')
    plt.legend(loc=legend, ncol=2, handlelength=1, fontsize=10)
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    plt.setp(ltext, fontweight='bold', fontsize=10)
    plt.grid(True, linestyle='--', axis='y')
    plt.grid(True, linestyle='--', axis='x')
    plt.tight_layout()
    plt.savefig("./figure/" + exper + "_" + ylabel + ".pdf")


def ComparativeFigure(exper, xlabel, xlim, xticks, ylabel, ylim, metric, legend):
    plt.figure(figsize=(4, 3))
    plt.yscale('linear')
    plt.tick_params(labelsize=12)
    plt.xlabel(xlabel, fontweight='bold', fontsize=12)
    plt.ylabel(ylabel.replace("_", " "), fontweight='bold', fontsize=12)
    plt.plot(param, metric[3], label=name[3], linestyle='-', marker='v', markersize=7,
             alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    plt.plot(param, metric[2], label=name[2], linestyle='-', marker='s', markersize=7,
             alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    plt.plot(param, metric[1], label=name[1], linestyle='-', marker='D', markersize=7,
             alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    plt.plot(param, metric[0], label=name[0], linestyle='-', marker='o', markersize=7,
             alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    plt.plot(param, metric[4], label=name[4], linestyle='-', marker='p', markersize=7,
             alpha=1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
    ax = plt.gca()
    plt.xlim(xlim)
    ax.set_xticks(xticks)
    plt.ylim(ylim)
    ax.yaxis.get_offset_text().set_fontsize(12)
    # ax.set_yscale('log', base=10)
    ax.set_yscale('linear')
    plt.ticklabel_format(style='scientific', scilimits=(0, 0), axis='y')
    plt.legend(loc=legend, ncol=2, handlelength=1, fontsize=10)
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    plt.setp(ltext, fontweight='bold', fontsize=10)
    plt.grid(True, linestyle='--', axis='y')
    plt.grid(True, linestyle='--', axis='x')
    plt.tight_layout()
    plt.savefig("./figure/" + exper + "_" + ylabel + ".pdf")

############################## Zipfian Error ##############################


exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 4e0), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 7e1), MSE, "upper left")

############################## Zipfian TopK ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
REC = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))
        REC[j].append(float(data[4]))

ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "ARE", (0, 0.6e0), ARE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "AAE", (0, 4.0e1), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "MSE", (0, 2.0e3), MSE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "Recall", (0.5e0, 1.05e0), REC, "lower right")

############################## Zipfian Subset ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 3e1), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 1e3), MSE, "upper left")

############################ CAIDA Error ##############################


exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 1e2), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 1e4), MSE, "upper left")

############################## CAIDA TopK ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
REC = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))
        REC[j].append(float(data[4]))

ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "ARE", (0, 0.002e0), ARE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "AAE", (0, 2e2), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "MSE", (0, 2e5), MSE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "Recall", (0.95e0, 1.005e0), REC, "lower right")

############################## CAIDA Subset ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 1e3), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 2e5), MSE, "upper left")

############################# Webpage Error ##############################


exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 0.5e0), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 0.5e0), MSE, "upper left")

############################## Webpage TopK ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
REC = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))
        REC[j].append(float(data[4]))

ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "ARE", (0, 0.6e0), ARE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "AAE", (0, 5.0e0), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "MSE", (0, 2.0e1), MSE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "Recall", (0.5e0, 1.05e0), REC, "lower right")

############################## Webpage Subset ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 3e0), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 2e1), MSE, "upper left")

############################## Criteo Error ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 4e2), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 2e5), MSE, "upper left")

############################## Criteo TopK ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
REC = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))
        REC[j].append(float(data[4]))

ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "ARE", (0, 0.05e0), ARE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "AAE", (0, 4.0e3), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "MSE", (0, 1.0e8), MSE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.8, 5.2), [
    1, 2, 3, 4, 5], "Recall", (0.95e0, 1.005e0), REC, "lower right")

############################## Criteo Subset ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "AAE", (0.0, 4e3), AAE, "upper left")
ComparativeFigure(exper, "Load Factor", (0.875, 1.225), [
    0.9, 1.0, 1.1, 1.2], "MSE", (0.0, 2e6), MSE, "upper left")

############################## Ratio Error ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Reset Ratio", (-0.05, 1.05), [
    0, 0.2, 0.4, 0.6, 0.8, 1.0], "AAE", (0.0, 4e0), AAE, "upper left")
ComparativeFigure(exper, "Reset Ratio", (-0.05, 1.05), [
    0, 0.2, 0.4, 0.6, 0.8, 1.0], "MSE", (0.0, 7e1), MSE, "upper left")

############################## Skewness Error ##############################

exper = ""
param = []
ARE = [[], [], [], [], []]
AAE = [[], [], [], [], []]
MSE = [[], [], [], [], []]
name = ["Ours", "Cuckoo", "Coco*", "USS*", "Elastic"]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]) / 10)
    for j in range(5):
        data = file.readline().strip().split()

        ARE[j].append(float(data[1]))
        AAE[j].append(float(data[2]))
        MSE[j].append(float(data[3]))

ComparativeFigure(exper, "Skewness", (-0.05, 1.05), [
    0, 0.2, 0.4, 0.6, 0.8, 1.0], "AAE", (0.0, 4e0), AAE, "upper left")
ComparativeFigure(exper, "Skewness", (-0.05, 1.05), [
    0, 0.2, 0.4, 0.6, 0.8, 1.0], "MSE", (0.0, 7e1), MSE, "upper left")

############################## Parameter D ##############################

exper = ""
param = []
AAE = [[], [], []]
COM = [[], [], []]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(3):
        data = file.readline().strip().split()

        AAE[j].append(float(data[2]))
        COM[j].append(float(data[-1]))

name = "Carbonyl4 (Ours)"
ParameterFigure(exper, "parameter: d", (0.5, 11.5), [
    1, 4, 7, 11], "AAE", (0.0, 4e0), AAE[0], "upper left")
ParameterFigure(exper, "parameter: d", (0.5, 11.5), [
    1, 4, 7, 11], "Avg_Step", (0.0, 4e0), COM[0], "upper left")
ParameterFigure(exper, "parameter: d", (0.5, 11.5), [
    1, 4, 7, 11], "TopK_AAE", (0.0, 1e1), AAE[1], "upper left")
ParameterFigure(exper, "parameter: d", (0.5, 11.5), [
    1, 4, 7, 11], "TopK_Avg_Step", (0.0, 2e1), COM[1], "upper left")

############################ Parameter M ##############################


exper = ""
param = []
AAE = [[], [], []]
COM = [[], [], []]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(3):
        data = file.readline().strip().split()

        AAE[j].append(float(data[2]))
        COM[j].append(float(data[-1]))

name = "Carbonyl4 (Ours)"
ParameterFigure(exper, "parameter: M", (-2, 22), [
    0, 5, 10, 15, 20], "AAE", (0.0, 4e0), AAE[0], "upper left")
ParameterFigure(exper, "parameter: M", (-2, 22), [
    0, 5, 10, 15, 20], "Avg_Step", (0.0, 1e0), COM[0], "upper left")
ParameterFigure(exper, "parameter: M", (-2, 22), [
    0, 5, 10, 15, 20], "TopK_AAE", (0.0, 1e1), AAE[1], "upper left")
ParameterFigure(exper, "parameter: M", (-2, 22), [
    0, 5, 10, 15, 20], "TopK_Avg_Step", (0.0, 1.5e1), COM[1], "upper left")

############################## Parameter E ##############################

exper = ""
param = []
AAE = [[], [], []]
COM = [[], [], []]
for i in range(11):
    data = file.readline().strip().split()
    exper = data[0]
    param.append(float(data[1]))
    for j in range(3):
        data = file.readline().strip().split()

        AAE[j].append(float(data[2]))
        COM[j].append(float(data[-1]))

name = "Carbonyl4 (Ours)"
ParameterFigure(exper, "parameter: p", (-0.05, 0.55), [
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5], "AAE", (0.0, 2e0), AAE[0], "upper left")
ParameterFigure(exper, "parameter: p", (-0.05, 0.55), [
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5], "Avg_Step", (0.0, 5e0), COM[0], "upper left")
ParameterFigure(exper, "parameter: p", (-0.05, 0.55), [
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5], "TopK_AAE", (0.0, 3e0), AAE[1], "upper left")
ParameterFigure(exper, "parameter: p", (-0.05, 0.55), [
    0.0, 0.1, 0.2, 0.3, 0.4, 0.5], "TopK_Avg_Step", (0.0, 3e1), COM[1], "upper left")

############################## Compression Error ##############################

# exper = ""
# param = []
# MSE = [[], []]
# SUP = [[], []]
# name = ["Heuristic", "Resampling"]
# for i in range(11):
#     data = file.readline().strip().split()
#     exper = data[0]
#     param.append(float(data[1]))
#
#     data = file.readline().strip().split()
#     baseline = float(data[6])
#     data = file.readline().strip().split()
#     MSE[0].append(float(data[6]) - baseline)
#     SUP[0].append(float(data[-1]))
#     data = file.readline().strip().split()
#     data = file.readline().strip().split()
#     MSE[1].append(float(data[6]) - baseline)
#     SUP[1].append(float(data[-1]))
#
# CompressFigure(exper, "Load Factor", (0.95, 2.05), [
#     1.0, 1.2, 1.4, 1.6, 1.8, 2.0], "Delta_MSE", (-2e1, 3e1), MSE, "upper left")
# CompressFigure(exper, "Load Factor", (0.95, 2.05), [
#     1.0, 1.2, 1.4, 1.6, 1.8, 2.0], "Speedup", (20, 35), SUP, "upper left")
#
# ############################## Compression TopK ##############################
#
# exper = ""
# param = []
# REC = [[], []]
# SUP = [[], []]
# name = ["Heuristic", "Resampling"]
# for i in range(11):
#     data = file.readline().strip().split()
#     exper = data[0]
#     param.append(float(data[1]))
#
#     data = file.readline().strip().split()
#     Baseline = float(data[8])
#     data = file.readline().strip().split()
#     REC[0].append(float(data[8]) - Baseline)
#     SUP[0].append(float(data[-1]))
#     data = file.readline().strip().split()
#     data = file.readline().strip().split()
#     REC[1].append(float(data[8]) - Baseline)
#     SUP[1].append(float(data[-1]))
#
# CompressFigure(exper, "Load Factor", (1.9, 4.1), [
#     2.0, 2.5, 3.0, 3.5, 4.0], "Delta_Recall", (-0.305, 0.105), REC, "upper left")
# CompressFigure(exper, "Load Factor", (1.9, 4.1), [
#     2.0, 2.5, 3.0, 3.5, 4.0], "Speedup", (20, 30), SUP, "upper left")
