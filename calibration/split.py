# Authored by Sam Kwong on August 5, 2019

from PIL import Image
import os
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cam1_idx', type=str, help='index of camera (0, 2, 4 or 6) that is the identity camera')
    parser.add_argument('--cam2_idx', type=str, help='index of camera (0, 2, 4 or 6) that is actively being calibrated in relation to the identy camera')
    opt = parser.parse_args()

    camIdxToName = {0: 'earth', 2: 'air', 4: 'water', 6: 'fire'}

    cam1_name = camIdxToName[int(opt.cam1_idx)]
    cam2_name = camIdxToName[int(opt.cam2_idx)]
    inputPath = './images/combined/' + cam1_name + '-' + cam2_name
    savePath = './images/split/' + cam1_name + '-' + cam2_name
    if not os.path.exists(savePath):
        os.makedirs(savePath)
    images = [x for x in os.listdir(inputPath)]
    
    for i, img in enumerate(images):
        imgObj = Image.open(os.path.join(inputPath, img))
        width, height = imgObj.size
        cropped1 = imgObj.crop((0, 0, int(width/2), height))
        cropped2 = imgObj.crop((int(width/2), 0, width, height))

        name1 = str(i).zfill(12) + '_rendered'
        name2 = str(i).zfill(12) + '_rendered_1'
        
        cropped1.save(savePath + '/' + name1, 'PNG')
        cropped2.save(savePath + '/' + name2, 'PNG')