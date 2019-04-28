import sys
import matplotlib.pyplot as plt

def main():
    #sys.argv[1] - algo
    #sys.argv[2] - input
    #sys.argv[3] - output
    #sys.argv[4] - plot
    with open(sys.argv[3], "r") as f:
        log = f.read().splitlines()
    i = 0
    x = []; y = []
    while log[i] != "hist_end":
        cur = log[i].split(" ")
        x.append(int(cur[0]))
        y.append(float(cur[1]))
        i += 1
    plt.figure(figsize=(8, 8))
    plt.title("{0} {1}\n".format(sys.argv[1], str(y[-1])))
    plt.xlabel("Time, ns")
    plt.ylabel("Optimal path length")
    plt.plot(x, y)
    plt.savefig(sys.argv[4])

if __name__ == "__main__":
    main()
