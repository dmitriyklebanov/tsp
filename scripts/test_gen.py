import sys
import re
import os
from subprocess import call
import random

test_sets_folder = os.path.join("..", "test_sets")
inputs_folder_name = "inputs"
test_set_log_file = "test_set.log"

def readInt(var_name, check_func):
    while True:
        try:
            ans = int(input("Enter a {}: ".format(var_name)))
            if check_func(ans):
                raise ValueError
        except ValueError:
            print("Invalid {}.".format(var_name))
        else:
            break
    return ans;

def readBoundaries(var_name, check_func = lambda x: False):
    while True:
        left = readInt("minimal {}".format(var_name), check_func)
        right = readInt("maximal {}".format(var_name), check_func)
        if left > right:
            print("Invalid boundaries of {}.".format(var_name))
        else:
            break
    return (left, right)

def generateTests(folder_name, number_of_tests, number_of_points, x, y):

    filenameid = 1
    for i in range(number_of_tests):
        while checkPath(folder_name, str(filenameid)):
            filenameid += 1

        with open(getPath(folder_name, str(filenameid)), 'w') as f:
            cur_number_of_points = random.randint(*number_of_points)
            f.write("{}\n".format(cur_number_of_points))
            for p in range(cur_number_of_points):
                p = [random.uniform(*x), random.uniform(*y)]
                f.write("{0} {1}\n".format(*p))

def restoreTestSet(folder_name):

    res = call("bash sys/clear_test_set.sh {}".format(folder_name), shell = True)
    if res:
        print("Can't clear test set.")
        return

    with open(getPath(folder_name, test_set_log_file), "r") as f:
        log = f.read().splitlines()
    folder_name = os.path.join(folder_name, inputs_folder_name)

    i = 0;
    while i < len(log):
        if log[i] == "del_started":
            i += 1
            while log[i] != "del_finished":
                os.remove(getPath(folder_name, log[i])); i += 1
            i += 1
            continue
        seed = int(log[i]); i += 1
        random.seed(seed)

        number_of_tests = int(log[i]); i += 1
        number_of_points = [int(t) for t in log[i].split(" ")]; i += 1
        x = [int(t) for t in log[i].split(" ")]; i += 1
        y = [int(t) for t in log[i].split(" ")]; i += 1

        generateTests(folder_name, number_of_tests, number_of_points, x, y)

    print("Test set has been restored successfully.")


def deleteTestSet(folder_name):
    print("\nDeleting test set...\n")
    res = call("bash sys/delete_test_set.sh {}".format(folder_name), shell = True)
    print("Test set has{} been deleted successfully.".format(" not" if res else ""))

def deleteTests(folder_name):
    print("\nDeleting some tests...\n")

    with open(getPath(folder_name, test_set_log_file), "a") as f:
        folder_name = os.path.join(folder_name, inputs_folder_name)
        f.write("del_started\n")
        while True:
            print("0 - finish")
            test_number = readInt("test number", lambda x: x < 0)
            if test_number:
                try:
                    os.remove(getPath(folder_name, str(test_number)))
                    f.write("{}\n".format(test_number))
                except OSError:
                    print("Can't delete file.")
            else:
                break
        f.write("del_finished")

    print("Some tests have been deleted successfully.");

def addTests(folder_name):
    print("\nAdding some tests....\n")

    seed = int.from_bytes(os.urandom(8), byteorder="big")
    random.seed(seed)

    number_of_tests = readInt("number of tests", lambda x: x <= 0)
    number_of_points = readBoundaries("number of points", lambda x: x <= 0)
    x = readBoundaries("x-coordinate")
    y = readBoundaries("y-coordinate")

    with open(getPath(folder_name, test_set_log_file), "a") as f:
        f.write("{}\n".format(seed))
        f.write("{}\n".format(number_of_tests))
        f.write("{0} {1}\n".format(*number_of_points))
        f.write("{0} {1}\n".format(*x))
        f.write("{0} {1}\n".format(*y))

    folder_name = os.path.join(folder_name, inputs_folder_name)
    generateTests(folder_name, number_of_tests, number_of_points, x, y)
    print("Some tests have been created successfully.")

def createTestSet(folder_name):
    print("\nCreating the new test set....\n")
    res = call("bash sys/create_test_set.sh {}".format(folder_name), shell = True)
    print("Test set has{} been created successfully.".format(" not" if res else ""))
    if res: return

    while True:
        answer = input("Empty test set has been created. Would you like to add some test? y/n ").lower()
        if answer == "y":
            addTests(folder_name)
            break;
        if answer == "n": break

def getPath(folder_name, file_name = ""):
    return os.path.join(test_sets_folder, folder_name, file_name)

def checkPath(folder_name, file_name = ""):
    return os.path.exists(getPath(folder_name, file_name))

def getFolderName(check_func = lambda x: True):
    while True:
        folder_name = input("Enter a folder name which consists from letters and digits: ")
        if (re.search("[^A-Za-z0-9]|^$", folder_name) is None) and (check_func(folder_name)):
            break
        else:
            print("Invalid name.")

    return folder_name

def main():
    while True:
        print(  "\n"
                "This script provides the ability to work with test sets\n"
                "You can enter:\n"
                "0 - exit from the script\n"
                "1 - create a test set\n"
                "2 - add some tests to an existing test set\n"
                "3 - delete some tests from an existing test set\n"
                "4 - delete test set\n"
                "5 - restore a test set\n"
                "Choose what you want: ",
            end = '')

        action = 0;
        try:
            action = int(input())
            if action < 0 or action > 5:
                raise ValueError
        except ValueError:
            print("Invalid action.")
        else:
            try:
                if action == 0: break;
                if action == 1: createTestSet(getFolderName(lambda x: not checkPath(x)))
                if action == 2: addTests(getFolderName(checkPath))
                if action == 3: deleteTests(getFolderName(checkPath))
                if action == 4: deleteTestSet(getFolderName(checkPath))
                if action == 5: restoreTestSet(getFolderName(checkPath))
            except IOError:
                print("Can't create or open file.")
                break;
    print("Finished.")

if __name__ == "__main__":
    main()
