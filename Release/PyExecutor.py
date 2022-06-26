import re
import os


def FormatInputStr(inp):
  # Run all input data through this function so that comparison
  # is always done with lowercase letters, no special characters, and no spaces
  # Remove special characters
  formattedInp = re.sub("\W", "", inp.strip().lower())
  # Replace spaces and underscores with hyphens
  formattedInp = re.sub("\s|_", "-", formattedInp)
  return formattedInp


def FormatOutputStr(out):
  # Always capitalize before outputting
  formattedOut = out.capitalize()
  return formattedOut


def ValidateInputStr(inp):
  # Check if input is a valid string
  match = False
  # Load regex of letters, spaces, and hyphens
  reg = re.compile("^[a-zA-Z -]*$")
  check = reg.match(inp)  # Compare input to the loaded regex
  if check is not None:
    match = True
  return match


def ReadFile():
  # Constant variable for the name of the input file
  INVENTORY_FILE = "DailyInventoryPurchases.txt"
  tempList = []
  purchaseList = []
  inputPassed = True
  badInput = []

  try:
    # Report error if input file isn't present
    if not os.path.isfile(INVENTORY_FILE):
      raise Exception("File input '%s' not found." % INVENTORY_FILE)

    with open(INVENTORY_FILE, "r", newline="\r\n", encoding="utf-8") as inFile:
      # Read all data from file
      data = inFile.read()
      # Create a list of the lines by splitting at the line breaks
      tempList = data.split("\r\n")

      # Loop though the list and validate + format input data
      for item in tempList:
        item = FormatInputStr(item)
        # If input is bad even after formatting, add it to a list to be printed later
        if not ValidateInputStr(item):
          inputPassed = False
          badInput.append(item)

        # Ignore list items that are empty
        if len(item) > 0:
          purchaseList.append(item)
    # If bad input data was found, report bad values
    if not inputPassed:
      errorMsg = str("%s invalid input: %s! Item names may only contain uppercase letters, "
                     "lowercase letters, spaces, and hyphens." % (INVENTORY_FILE, str(badInput)))
      raise Exception(errorMsg)

  except Exception as err:
    purchaseList = []  # Reset to an empty list so no info will be reported
    print(err)

  return purchaseList


def GetItemSoldFrequency(p_itemName):
  item = FormatInputStr(p_itemName)
  # Return -1 to CPP function if input is bad
  if not ValidateInputStr(item) or len(item) == 0:
    return -1

  purchaseList = ReadFile()  # Get the list of items from the daily sales file
  itemCount = purchaseList.count(item)  # Count how many times the item appears in the list
  return itemCount


def GetItemizedReport():
  LINE_LEN = 29
  purchaseList = ReadFile()  # Get the list of items from the daily sales file
  # Create a copy of the list that removes duplicates and is sorted alphabetically
  purchasedNoDups = sorted(list(set(purchaseList)))
  # Only ouput data if there is at least one item in the list
  if len(purchaseList) > 0:
    # Output title
    print("%25s" % "ITEMIZED SALES REPORT")
    print(str("-" * LINE_LEN))
    # Loop though the shortened list
    for item in purchasedNoDups:
      # Using the name of the item from the shortened list, count how many times it appears in the full list
      itemCount = purchaseList.count(item)
      item = FormatOutputStr(item)  # Make the string pretty to print
      print(" %15s: %d" % (item, itemCount))

    print(str("-" * LINE_LEN))


def WriteItemizedData():
   # Constant variable for the name of the output file
  OUTPUT_FILE = "frequency.dat"
  purchaseList = ReadFile()  # Get the list of items from the daily sales file
  # Create a copy of the list that removes duplicates and is sorted alphabetically
  purchasedNoDups = sorted(list(set(purchaseList)))

  if os.path.isfile(OUTPUT_FILE):
    os.remove(OUTPUT_FILE)  # Delete the file if already present

  try:
    # Report error if the list of items is empty
    if len(purchaseList) < 1:
      raise Exception("No items were found in the daily sales report. '%s' was not created." % OUTPUT_FILE)

    with open(OUTPUT_FILE, "w", encoding="utf-8") as outFile:
      # Iterate though the shortened list
      for index, item in enumerate(purchasedNoDups):
        # Using the name of the item from the shortened list, count how many times it appears in the full list
        itemCount = purchaseList.count(item)
        item = item = FormatOutputStr(item)  # Make the string pretty to print
        # Write the item name and frequency to the data file
        outFile.write("%s %d" % (item, itemCount))

        if index < (len(purchasedNoDups) - 1):
          outFile.write("\n")  # Only include a new line character if this is not the last item in the list

  except Exception as err:
    print(err)
