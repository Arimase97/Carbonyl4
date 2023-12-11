with open("/share/Criteo/20m") as f:
    data = f.readlines()
    for item in data:
        item = item.strip().split("\t")
        value = 0
        for i in range(14):
            value += float(item[i]) / 14
        print(int(item[14], 16), value)
