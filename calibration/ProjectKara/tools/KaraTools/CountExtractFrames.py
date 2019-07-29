import cv2
import sys
import os

if __name__ == '__main__':
    if(len(sys.argv) != 2 and len(sys.argv) != 3):
        print('[ USAGE ]: ', sys.argv[0], '<VideoFile> [<ExtractOutputDirectory>]')
        quit()

    VideoFileName = sys.argv[1]
    isExtractFrames = False
    OutDir = None
    if(len(sys.argv) == 3):
        isExtractFrames = True
        OutDir = sys.argv[2]

    if isExtractFrames:
        if not os.path.exists(OutDir):
            os.makedirs(OutDir)
        VideoFNNoExt, _ = os.path.splitext(os.path.basename(VideoFileName))

    print('[ INFO ]: Opening file ', VideoFileName)
    VideoFile = cv2.VideoCapture(VideoFileName)

    Ctr = 0
    while (VideoFile.isOpened()):
        ret, Frame = VideoFile.read()

        if ret == True:
            Ctr = Ctr + 1

            if isExtractFrames:
                FrameName = OutDir + os.path.sep + VideoFNNoExt + '_' + str(Ctr).zfill(8) + '.png'
                print('[ INFO ]: Writing file ', FrameName)
                cv2.imwrite(FrameName, Frame)
        else:
            break

    print('Total frames: ', Ctr)
    VideoFile.release()
