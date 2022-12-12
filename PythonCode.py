import re
import string

FILENAME = 'items.txt'

def readFile(FILENAME): # opens file and writes contents to a string then returns the string
       
    lines = None
    
    with open(FILENAME) as file:
        lines = file.readlines()

    return lines


def getInventory(): #calls readfile function, takes string and writes values to dictionary. Returns the dictionary.
     
    inventory = {}    
    items = readFile(FILENAME)
       
    for item in items:                
        item = item.strip()
        if item in inventory:
            inventory[item] += 1 
        else:
            inventory[item] = 1
            
    return inventory

    
def determineInventory(): # gets list of items and frequencies via returned dictionary value. Outputs the contents of inventory dictionary

    inventory = getInventory()
    
    print ("ITEMS      FREQUENCY")
    print ("")
    for item, value in inventory.items():
        print (f"{item:15}{value:5}") 



def determineFrequency(item): # gets list of items and frequencies via returned dictionary value. Takes string, validates presence in dict, returns 0 for null or returns frequency
    inventory = getInventory()
    
    if item not in inventory:
        return 0
    else:
        itemValue = inventory[item]
        return  itemValue
   
   
    
    

def output(): # gets list of items and frequencies via returned dictionary value.  Then ieterates through dictionary writing each key, pair to frequency.dat 
    inventory = getInventory()
    
    file = open("frequency.dat", "w")

    for item, value in inventory.items():
        line = (item, " ", str(value), "\n")
        file.writelines(line) 
    
    file.close()