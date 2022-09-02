import csv
import json

from blist import sorteddict
import re

mainDictionary = dict()
columns = list()
json_file = open('query_result.json', 'w')
orderByReverse = False


# Checking the string values in terms of mathematical operations.
def checkTheConditions(elementValue, operator, comparedValue):
    if elementValue.isnumeric():
        elementValue = int(elementValue)
        comparedValue = int(comparedValue)
        if operator == ">":
            if elementValue > comparedValue:
                return True
        elif operator == "<":
            if elementValue < comparedValue:
                return True
        elif operator == "=":
            if elementValue == comparedValue:
                return True
        elif operator == "!=":
            if elementValue != comparedValue:
                return True
        elif operator == "<=":
            if elementValue <= comparedValue:
                return True

        elif operator == ">=":
            if elementValue >= comparedValue:
                return True

        elif operator == "!<":
            if not (elementValue < comparedValue):
                return True

        elif operator == "!>":
            if not (elementValue > comparedValue):
                return True
    else:
        elementValue = elementValue.upper()
        if operator == "=":
            if elementValue == comparedValue:
                return True
        elif operator == "!=":
            if elementValue != comparedValue:
                return True


# In order to obtain the key inputting a value from the mainDictionary and returning its matched key.
def getKey(val, dictionary):
    for key, value in dictionary.items():
        if val == value:
            return key


# Insert implementation method of the insert query keyword of the simplified SQL.
def insert(id, name, lastname, email, grade, dictionaryInsert):
    # Creating a new key-value paired mainDictionary here to store the values that are newly being stored in the main database.
    innerDictionary = {"NAME": name, "LASTNAME": lastname, "EMAIL": email, "GRADE": grade}
    dictionaryInsert[id] = innerDictionary
    # Below in the for loop, checking if both the keys are matching. And then inserting to the main database with related
    # keys to the related keys in the database.
    for key in innerDictionary.keys():
        if key == "NAME" or key == "LASTNAME":
            innerDictionary[key] = innerDictionary[key].lower().capitalize()
        elif key == "EMAIL":
            innerDictionary[key] = innerDictionary[key].lower()
        elif key == "GRADE":
            innerDictionary[key] = innerDictionary[key]
    print("Inserting process done successfully!")
    return dictionaryInsert



# Delete implementation of the simplified SQL language. It also takes and an or parameters to check the combined conditions.
def delete(operand1, firstOperator, operand2, dictionaryDelete, ANDorOR="not", operand3="not",
           secondOperator="not", operand4="not"):
    # Here if there is not an "and" or "or" keyword, this snippet of code will be processing.

    #Checking whether includes quotes or not, if exist clear
    if "'" in operand2 or "‘" in operand2:
        operand2 = operand2[1:-1]
    if "'" in operand4 or "‘" in operand4:
        operand4 = operand4[1:-1]

    if ANDorOR == "not":
        for key, eachRecord in list(dictionaryDelete.items()):
            if operand1 != "ID":
                if checkTheConditions(eachRecord[operand1], firstOperator, operand2):
                    dictionaryDelete.pop(key)
            else:
                if checkTheConditions(key, firstOperator, operand2):
                    dictionaryDelete.pop(key)
    # Here if there are and or or conditions. It checkes for them then process the same operations as it does above.
    elif ANDorOR == "AND":

        for key, eachRecord in list(dictionaryDelete.items()):
            if operand1 != "ID" and operand3 != "ID":
                if checkTheConditions(eachRecord[operand1], firstOperator, operand2) == True and checkTheConditions(
                        eachRecord[operand3], secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)
            elif operand1 == "ID" and operand3 != "ID":
                if checkTheConditions(key, firstOperator, operand2) == True and checkTheConditions(
                        eachRecord[operand3], secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)

            elif operand1 != "ID" and operand3 == "ID":
                if checkTheConditions(eachRecord[operand1], firstOperator, operand2) == True and checkTheConditions(
                        key, secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)

            elif operand1 == "ID" and operand3 == "ID":
                if checkTheConditions(key, firstOperator, operand2) == True and checkTheConditions(
                        key, secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)
    elif ANDorOR == "OR":
        for key, eachRecord in list(dictionaryDelete.items()):
            if operand1 != "ID" and operand3 != "ID":
                if checkTheConditions(eachRecord[operand1], firstOperator, operand2) == True or checkTheConditions(
                        eachRecord[operand3], secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)
            elif operand1 == "ID" and operand3 != "ID":
                if checkTheConditions(key, firstOperator,operand2) == True or checkTheConditions(eachRecord[operand3], secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)

            elif operand1 != "ID" and operand3 == "ID":
                if checkTheConditions(eachRecord[operand1], firstOperator, operand2) == True or checkTheConditions(
                        key, secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)

            elif operand1 == "ID" and operand3 == "ID":
                if checkTheConditions(key, firstOperator,
                                      operand2) == True or checkTheConditions(
                    key, secondOperator, operand4) == True:
                    dictionaryDelete.pop(key)

    return dictionaryDelete




# This method is the implementation of select keyword of the simplified SQL language.
# It checkes the and or not combined conditions as above functions do
def select(columnName, element1, condition1, element2, orderBy, dictionary, ANDOrOR="not", element3="not",
           condition2="not", element4="not"):

    #Checking whether includes quotes or not, if exist clear
    if "'" in element2 or "‘" in element2:
        element2 = element2[1:-1]

    if "'" in element4 or "‘" in element4:
        element4 = element4[1:-1]

    newSelectedDictionary = dict()
    newSelectedDictionary = sorteddict(newSelectedDictionary)
    global orderByReverse
    if orderBy == "DSC":
        orderByReverse = True
    for i in columnName.split(","):
        columns.append(i)
    if ANDOrOR == "not":
        for key, eachRecord in dictionary.items():
            if element1 != "ID":
                if checkTheConditions(eachRecord[element1], condition1, element2) == True:
                    newSelectedDictionary[key] = eachRecord

            else:
                if checkTheConditions(key, condition1, element2) == True:
                    newSelectedDictionary[key] = eachRecord
    elif ANDOrOR == "AND":
        for key, eachRecord in dictionary.items():
            if element1 != "ID" and element3 != "ID":

                if checkTheConditions(eachRecord[element1], condition1, element2) == True and checkTheConditions(
                        eachRecord[element3], condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

            elif element1 == "ID" and element3 != "ID":
                if checkTheConditions(key, condition1, element2) == True and checkTheConditions(
                        eachRecord[element3], condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord
            elif element1 != "ID" and element3 == "ID":
                if checkTheConditions(eachRecord[element1], condition1, element2) == True and checkTheConditions(
                        key, condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

            elif element1 == "ID" and element3 == "ID":
                if checkTheConditions(key, condition1, element2) == True and checkTheConditions(
                        key, condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

    elif ANDOrOR == "OR":
        for key, eachRecord in dictionary.items():
            if element1 != "ID" and element3 != "ID":
                if checkTheConditions(eachRecord[element1], condition1, element2) == True or checkTheConditions(
                        eachRecord[element3], condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

            elif element1 == "ID" and element3 != "ID":
                if checkTheConditions(key, condition1, element2) == True or checkTheConditions(
                        eachRecord[element3], condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord
            elif element1 != "ID" and element3 == "ID":
                if checkTheConditions(eachRecord[element1], condition1, element2) == True or checkTheConditions(
                        key, condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

            elif element1 == "ID" and element3 == "ID":
                if checkTheConditions(key, condition1, element2) == True or checkTheConditions(
                        key, condition2, element4) == True:
                    newSelectedDictionary[key] = eachRecord

    return newSelectedDictionary


def readCSV(dictionary):
    headerlineread = False
    header_list.append("ALL")
    with open('students.csv', 'r') as file:
        reader = csv.reader(file, delimiter=";")
        for row in reader:

            # This if block is used for extracting the headers of the line
            if headerlineread == False:
                header_list.extend(row)
                headerlineread = True
            else:
                innerDictionary = {"NAME": row[1], "LASTNAME": row[2], "EMAIL": row[3], "GRADE": row[4]}
                dictionary[row[0]] = innerDictionary


def make(query, dictionary):
    queryArray = query.split()
    attributeSplit = list()
    if (queryArray[0] == "SELECT"):
        if (len(queryArray[1].split(",")) > 1):
            attributeSplit = queryArray[1].split(",")
            for i in range(1, queryArray.index("FROM")):
                if queryArray[i][-1] == ",":
                    print("The format is wrong. Please do not include a space after the comma.")
                    break
        else:
            attributeSplit.append(queryArray[1])
        if (all([(attribute in header_list) for attribute in attributeSplit])):
            if (queryArray[queryArray.index("FROM") + 1] == "STUDENTS"):
                if "AND" in queryArray or "OR" in queryArray:
                    return select(queryArray[1], queryArray[5], queryArray[6], queryArray[7], queryArray[-1],
                                  dictionary,
                                  queryArray[8],
                                  queryArray[9], queryArray[10], queryArray[11])
                else:

                    return select(queryArray[1], queryArray[5], queryArray[6], queryArray[7], queryArray[-1],
                                  dictionary)
            else:
                print("Wrong table name.")
        else:
            print("INVALID ATTRIBUTE")
    elif (queryArray[0] == "INSERT"):
        if (queryArray[1] == "INTO"):
            if (queryArray[2] == "STUDENTS"):
                queryArray = query.split("(")
                if (queryArray[0].split(" ")[-1] == "VALUES"):
                    queryArray = queryArray[1].split(")")
                    queryArray = queryArray[0].split(",")
                    if (len(queryArray) == 5):
                        regex = '^[a-z0-9]+[\._]?[a-z0-9]+[@]\w+[.]\w{2,3}$'
                        if (queryArray[0].isnumeric()):
                            if (queryArray[1].isalpha()):
                                if (queryArray[2].isalpha()):
                                    if (re.search(regex, queryArray[3].lower())):
                                        if (queryArray[4].isnumeric()):
                                            return insert(queryArray[0], queryArray[1], queryArray[2], queryArray[3],
                                                          queryArray[4], dictionary)
                                        else:
                                            print("Grade must be integer!")
                                    else:
                                        print("Email must obey to email formation rules")
                                else:
                                    print("Last name must be comprised of letters!")
                            else:
                                print("Name must be comprised of letters!")

                        else:
                            print("ID must be comprised of digits!")
                    else:
                        print("Invalid input: incorrect number of attributes are entered!")
                else:
                    print("Wrong format of query. It should be 'VALUES'.")
            else:
                print("Wrong table name.")
        else:
            print("Wrong keyword. It has to be 'into' after 'insert.")

    elif (queryArray[0] == "DELETE"):
        if (queryArray[1] == "FROM"):
            if "AND" in queryArray or "OR" in queryArray:
                return delete(queryArray[4], queryArray[5], queryArray[6], dictionary, queryArray[7], queryArray[8],
                              queryArray[9], queryArray[10])
            else:
                return delete(queryArray[4], queryArray[5], queryArray[6], dictionary)
        else:
            print("Wrong format of query. It should be 'FROM' after 'DELETE'.")

    return dictionary



def writeJSON(dictionaryForJSON):
    id = True
    name = True
    lastname = True
    grade = True
    email = True

    if(columns[0] == "ALL"):
        pass
    if(len(columns) != 0):
        if columns.count("ID") == 0:
            id = False

        if columns.count("NAME") == 0:
            name = False

        if columns.count("LASTNAME") == 0:
            lastname = False

        if columns.count("EMAIL") == 0:
            email = False

        if columns.count("GRADE") == 0:
            grade = False




    outerDictionary = dict()

    for key, value in dictionaryForJSON.items():
        inDictionary = dict()
        if id == True:
            inDictionary["ID"] = int(key)

        if name == True:
            inDictionary["NAME"] = value["NAME"]

        if lastname == True:
            inDictionary["LASTNAME"] = value["LASTNAME"]

        if grade == True:
            inDictionary["GRADE"] = int(value["GRADE"])

        if email == True:
            inDictionary["EMAIL"] = value["EMAIL"]

        outerDictionary[int(key)] = inDictionary
    outerDictionary = sorteddict(outerDictionary)
    if orderByReverse == True:
        outerDictionary = dict(reversed(list(outerDictionary.items())))
    for eachValue in outerDictionary.values():
        json.dump(eachValue, json_file, indent=4, ensure_ascii=False)


# MAIN
header_list = list()
readCSV(mainDictionary)
mainDictionary = sorteddict(mainDictionary)
header_list = [word.upper() for word in header_list]

print("Note: There should be blank between operand and operator e.g: grade < 40, not grade<40")
print("Note: STUDENTS or students must be written in all queries, not STUDENT or student")
print("Sample query: SELECT name,grade,email FROM STUDENTS WHERE grade = 100 OR name = ‘Micheal’ ORDER BY ASC ")
print()

while (True):
    query = input("Please enter your query:")
    query = query.upper()
    if (query == "EXIT"):
        print("Program is being ended...")
        writeJSON(mainDictionary)
        break
    else:

        mainDictionary = make(query, mainDictionary)
