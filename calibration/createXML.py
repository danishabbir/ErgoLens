# Authored by Sam Kwong on July 30, 2019

import os
import sys
import argparse
import numpy as np
import xml.etree.ElementTree as ET
from xml.dom.minidom import parse, parseString
import ast

def cleanFormats(rotMat, transMat):
    rotMatFormatted = np.array(rotMat).reshape(3,3)
    transMatFormatted = np.array(transMat).reshape(3,1)
    return (rotMatFormatted, transMatFormatted)

def createExtMat(rotMatFormatted, transMatFormatted):
    extMat = np.append(rotMatFormatted, transMatFormatted, axis=1).flatten()
    return extMat

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input_path', type=str, default='./ProjectKara/tools/KaraTools/builds/linux/calib.params', help='path to text file containing all cameras\' extrinsic params')
    parser.add_argument('--output_folder', type=str, default='../models/cameraParameters/flir', help='path to directory holding cameras\' XML files')
    opt = parser.parse_args()

    f_list = sorted([os.fsdecode(f) for f in os.listdir(os.fsencode(opt.output_folder)) if os.fsdecode(f).endswith('.xml')])
    extMatStrs = [0] * len(f_list)

    # ------------------------------- create an extrinsic parameters matrix for each camera ---------------------------------
    print('Retrieving camera extrinsic parameters... (' + opt.input_path + ')')
    info = open(opt.input_path, 'r')
    txt = info.read()
    rotMatStrs = txt.split('Calibration stack frame: ')[1].replace(';', ',').split('[')
    transMatStrs = txt.split('Translations: ')[1].replace(';', ',').split('[')

    for i in range(len(f_list)):
        rotMatStr, transMatStr = rotMatStrs[i+1], transMatStrs[i+1] # get what comes after '['
        # disregard ']'
        if i < len(f_list)-1:
            rotMatStr, transMatStr = rotMatStr.replace(']', ''), transMatStr.replace(']', '')
        else:
            rotMatStrEndIdx, transMatStrEndIdx = rotMatStr.find(']'), transMatStr.find(']')
            rotMatStr, transMatStr = rotMatStr[:rotMatStrEndIdx], transMatStr[:transMatStrEndIdx]
        
        rotMatList, transMatList = ast.literal_eval('[' + rotMatStr + ']'), ast.literal_eval('[' + transMatStr + ']')
        rotMatFormatted, transMatFormatted = cleanFormats(rotMatList, transMatList)
        extMat = createExtMat(rotMatFormatted, transMatFormatted)
        extMatStrs[i] = np.array2string(extMat).strip('[]').replace('  ', ' ')
    
    # ------------------------------- insert the appropriate extrinsic parameters matrix into each camera's XML file ----------------------------------
    for i,f in enumerate(f_list):
        tree = ET.parse(opt.output_folder + '/' + f)
        root = tree.getroot()
        for elem in root.findall('./CameraMatrix/data'):
            elem.text = extMatStrs[i]
        
        tree.write(opt.output_folder + '/' + f) # using xml_declaration=True will add in xml header with single quotes (codebase requires double quotes)

        # append header as a string at the beginning (in order to keep the double quotes instead of single quotes)
        xmlHeader = "<?xml version=\"1.0\"?>"
        xmlStr = xmlHeader + str(ET.tostring(root)).replace('b\'', '').replace('\'', '').replace('\\n', '').replace('  ', ' ')
        print(xmlStr)
        with open(opt.output_folder + '/' + f, 'w') as openF:
            openF.write(xmlStr)
        