#! /usr/bin/env python
# -*- coding: utf-8 -*-
from os import listdir,makedirs
import json
from os.path import join, basename, exists

RESULT = "./result/"
Wi = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7,9, 10, 5, 8, 4, 2]
IndexTable = {
    0 : '1',
    1 : '0',
    2 : 'X',
    3 : '9',
    4 : '8',
    5 : '7',
    6 : '6',
    7 : '5',
    8 : '4',
    9 : '3',
    10 : '2'
    }
def ECC(nums):
  if len(nums) != 18:
    return 0;
  tsum= 0;
  for i in range(len(nums) - 1):
    if(nums[i] == 'X'):
      return 0
    tsum = Wi[i]*int(nums[i])+ tsum
  tsum = tsum % 11;
  #print(IndexTable[tsum])
  #print(nums[-1])
  if(IndexTable[tsum] == nums[-1]):
    return 1
  else:
    return 0


def Eval():
  CORRECT = 0;
  filenames = listdir(RESULT)
 # ALL = len(filenames)
  ALL = 0
  for filename in filenames :
    if filename.endswith(".jpg"):
      continue
    ALL = ALL + 1
    with open(join(RESULT, filename)) as f:
      data = f.read();
      data = json.loads(data)
      if len(data) < 1:
        continue
      nums = data['texts'][0]['text']
      flag = ECC(nums)
      CORRECT = CORRECT + int(flag)
      print("nums:\t%s"%(nums))
      print("status:\t%d"%(flag))
  pre = 1.0 * CORRECT / ALL
  print("correct %d, total %d"%(CORRECT,ALL))
  print("precise: %f%%"%(pre*100))

Eval()

