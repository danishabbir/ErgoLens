# Authored by Sam Kwong

import os
import sys
import argparse
import numpy as np
import xml.etree.ElementTree as ET
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

    # create an extrinsic parameters matrix for each camera
    print('Retrieving camera extrinsic parameters... (' + opt.input_path + ')')
    info = open(opt.input_path, 'r')
    txt = info.read()
    rotMatStrs = txt.split('Calibration stack frame: ')[1].replace(';', ',').split('[')
    transMatStrs = txt.split('Translations: ')[1].replace(';', ',').split('[')

    for i in range(len(f_list)):
        rotMatStr, transMatStr = rotMatStrs[i+1], transMatStrs[i+1] # get what comes after '['
        if i < len(f_list)-1:
            rotMatStr, transMatStr = rotMatStr.replace(']', ''), transMatStr.replace(']', '')
        else:
            rotMatStrEndIdx, transMatStrEndIdx = rotMatStr.find(']'), transMatStr.find(']')
            rotMatStr, transMatStr = rotMatStr[:rotMatStrEndIdx], transMatStr[:transMatStrEndIdx]
        print(rotMatStr, transMatStr)
        rotMatList, transMatList = ast.literal_eval('[' + rotMatStr + ']'), ast.literal_eval('[' + transMatStr + ']')

        rotMatFormatted, transMatFormatted = cleanFormats(rotMatList, transMatList)
        extMat = createExtMat(rotMatFormatted, transMatFormatted)
        extMatStrs[i] = np.array2string(extMat).strip('[]').replace('  ', ' ')
    
    # insert the appropriate extrinsic parameters matrix into each camera's XML file
    for i,f in enumerate(f_list): 
        tree = ET.parse(opt.output_folder + '/' + f)
        root = tree.getroot()
        for elem in root.findall('./CameraMatrix/data'):
            elem.text = extMatStrs[i]
        treeStr = ET.tostring(root, method='xml')
        
        #xml_str = "<?xml version=\"1.0\" encoding=\"%s\"?>" + sys.stdout.write(newline) + treeStr
        tree.write(opt.output_folder + '/' + f)
        # tree.write(opt.output_folder + '/' + f, encoding="UTF-8", xml_declaration=True)
        #with open(opt.output_folder + '/' + f, 'wb') as xml_file:
            #xml_file.write(xml_str)

    
