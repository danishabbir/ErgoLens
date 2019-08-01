import tinyik
import numpy as np

kptDict = {'Nose': '0', 'Neck': '1', 'RShoulder': '2', 'RElbow': '3', 'RWrist': '4', 'LShoulder': '5', 'LElbow': '6', 'LWrist': '7', 'MidHip': '8', 'RHip': '9', 'RKnee': '10', 'RAnkle': '11', 'LHip': '12', 'LKnee': '13', 'LAnkle': '14', 'REye': '15', 'LEye': '16', 'REar': '17', 'LEar': '18', 'LBigToe': '19', 'LSmallToe': '20', 'LHeel': '21', 'RBigToe': '22', 'RSmallToe': '23', 'RHeel': '24', 'Background': '25'}

file = {"version":1.3,"people":[{"person_id":[-1],"pose_keypoints_2d":[414.231,124.481,0.856358,414.236,142.819,0.871896,380.337,147.999,0.834929,369.875,202.756,0.828784,393.386,197.565,0.824643,452.073,141.437,0.854994,466.443,196.279,0.776076,471.672,252.382,0.799852,423.414,247.178,0.680792,402.467,248.476,0.662479,401.212,339.87,0.804193,390.734,432.515,0.764147,445.548,245.863,0.673436,441.603,339.85,0.759187,433.795,418.142,0.755417,408.985,117.962,0.831285,422.086,117.977,0.885769,398.543,107.546,0.733607,432.494,108.784,0.709382,452.069,442.899,0.597114,454.721,435.101,0.64399,424.67,424.664,0.564868,373.801,462.489,0.647599,369.845,454.741,0.642548,395.959,441.623,0.522221],"face_keypoints_2d":[],"hand_left_keypoints_2d":[],"hand_right_keypoints_2d":[],"pose_keypoints_3d":[-178.915,53.7308,330.908,1,-160.967,88.9684,390.643,1,-174.408,175.158,435.495,1,-86.3098,207.775,448.867,1,-75.9256,95.1962,338.61,1,-162.434,39.3906,418.054,1,-79.375,30.4995,446.678,1,7.49758,24.6742,452.86,1,1.32546,79.1229,397.665,1,2.95816,104.482,375.041,1,142.223,97.4919,359.509,1,301.621,154.585,426.433,1,-0.63993,48.7916,412.204,1,160.527,117.104,542.151,1,284.733,89.6934,435.817,1,-189.845,62.3279,328.549,1,-188.956,46.0819,338.853,1,-212.113,86.3144,339.969,1,0,0,0,0,305.957,44.9196,415.393,1,299.877,46.3187,426.114,1,0,0,0,0,312.596,104.225,292.015,1,310.008,120.232,300.642,1,308.248,128.956,388.714,1],"face_keypoints_3d":[],"hand_left_keypoints_3d":[],"hand_right_keypoints_3d":[]}],"part_candidates":[{"0":[414.231,124.481,0.856358],"1":[414.236,142.819,0.871896],"2":[380.337,147.999,0.834929],"3":[369.875,202.756,0.828784],"4":[393.386,197.565,0.824643],"5":[452.073,141.437,0.854994],"6":[466.443,196.279,0.776076],"7":[471.672,252.382,0.799852],"8":[423.414,247.178,0.680792],"9":[402.467,248.476,0.662479],"10":[401.212,339.87,0.804193],"11":[228.915,307.24,0.0576431,390.734,432.515,0.764147],"12":[445.548,245.863,0.673436],"13":[441.603,339.85,0.759187],"14":[231.515,308.494,0.0574504,433.795,418.142,0.755417],"15":[408.985,117.962,0.831285],"16":[422.086,117.977,0.885769],"17":[398.543,107.546,0.733607],"18":[432.494,108.784,0.709382],"19":[214.571,309.826,0.0729799,452.069,442.899,0.597114],"20":[214.559,311.126,0.0714996,454.721,435.101,0.64399],"21":[228.902,309.824,0.0594748,424.67,424.664,0.564868],"22":[213.241,311.117,0.0858684,373.801,462.489,0.647599],"23":[213.276,313.746,0.0956845,369.845,454.741,0.642548],"24":[223.705,311.116,0.0917275,395.959,441.623,0.522221]}]}
parts = file["part_candidates"][0]

class Actuator: 
    def __init__(self, parts, bodypart1, bodypart2, eepart):
        self.eepartcoord = parts[kptDict[eepart]]
        self.bodypart1coord = parts[kptDict[bodypart1]]
        self.bodypart2coord = parts[kptDict[bodypart2]]

    def coord2Angle(self):
        actuator = tinyik.Actuator(['z', self.bodypart1coord, 'z', self.bodypart2coord])
        actuator.ee = self.eepartcoord
        return (np.rad2deg(actuator.angles))



class RULA():
    def __init__(self, right_arm, left_arm):
        self.right_arm = right_arm
        self.left_arm = left_arm

        self.upper_arm_score = 0
        self.lower_arm_score = 0
        self.wrist_score = 1
        self.wrist_twist_score = 1

        self.trunk_score = 0
        self.neck_score = 0

    def upperArmPositions(self):
        upper_arm_angle, _ = 90 - self.right_arm.coord2Angle()
        if upper_arm_angle > 90:
            self.upper_arm_score += 4
        elif upper_arm_angle > 45:
            self.upper_arm_score += 3
        elif upper_arm_angle > 20:
            self.upper_arm_score += 2
        elif upper_arm_angle > -20 and upper_arm_angle < 20:
            self.upper_arm_score += 1

        return self.upper_arm_score

    def lowerArmPositions(self):
        _, lower_arm_angle = self.right_arm.coord2Angle()
        if lower_arm_angle > 100:
            self.lower_arm_score += 2
        if lower_arm_angle < 60 and lower_arm_angle > 0:
            self.lower_arm_score += 2
        if lower_arm_angle > -60 and lower_arm_angle < 100:
            self.lower_arm_score += 1

        # add across midline, is arm out to side of body
        return self.lower_arm_score
    
    def wristScore(self):
        if wrist_angle == 0:
            self.wrist_score += 1
        elif wrist_angle > 0 and wrist_angle < 15:
            self.wrist_score += 2
        elif wrist_angle > 15:
            self.wrist_score += 3

        #if wrist is bent from midline +1
        return self.wrist_score

    def wristTwist(self):
        if wrist_twist == 'mid_range':
            self.wrist_twist_score += 1
        elif wrist_twist == 'high':
            self.wrist_twist_score += 2        
        return 
        
    def trunkPosition(self):
        trunk_angle, _ = back.coord2Angle()

        #standing & seated difference 
        if trunk_angle > 0 and trunk_angle < 10:
            self.trunk_score += 1
        elif trunk_angle > 0 and trunk_angle < 20:
            self.trunk_score += 2
        elif trunk_angle > 20 and trunk_angle < 60:
            self.trunk_score += 3
        elif trunk_angle < 60:
            self.trunk_score += 4
        return self.trunk_score
    
    def neckPosition(self):
        _, neck_angle = back.coord2Angle()

        if neck_angle > 0 and neck_angle < 10:
            self.neck_score += 1
        elif neck_angle > 10 and neck_angle < 20:
            self.neck_score += 2
        elif neck_angle > 20: #signs/angle direction will be important
            self.neck_score += 3
        elif neck_angle > -40: 
            self.neck_score += 4


    def computeRULA(self):
        score = 0
        
        upperArmPositions(self)
        lowerArmPositions(self)
        #wristScore(self)
        #wristTwist(self)





#def computeREBA():

def main():
    right_arm = Actuator(parts, 'RShoulder', 'RElbow', 'RWrist')
    right_arm_angle = right_arm.coord2Angle()
    print("right_arm_angle", right_arm_angle)

    #right_shin = Actuator(parts, 'RKnee', 'RAnkle', 'RHeel')
    #right_shin_angle = right_shin.coord2Angle()

    #right_full_leg = Actuator(parts, 'RHip', 'RKnee', 'RAnkle')
    #right_ful_leg_angle = right_full_leg.coord2Angle()
    #print("right_full_leg_angle", right_ful_leg_angle)

    left_arm = Actuator(parts, 'LShoulder', 'LElbow', 'LWrist')
    left_arm_angle = left_arm.coord2Angle()
    print("left_arm_angle", left_arm_angle)

    rula = RULA(right_arm, left_arm)
    upper_arm_score = rula.upperArmPositions()
    print(upper_arm_score)

    lower_arm_score = rula.lowerArmPositions()
    print(lower_arm_score)

    #left_shin = Actuator(parts, 'LKnee', 'LAnkle', 'LHeel')
    #left_shin_angle = left_shin.coord2Angle()

    #left_full_leg = Actuator(parts, 'LHip', 'LKnee', 'LAnkle')
    #left_full_leg_angle = left_full_leg.coord2Angle()
    #print("left_full_leg_angle", left_full_leg_angle)

    Head = (parts[kptDict['REye']] + parts[kptDict['LEye']]) / 2
    #modify this to be better
    back = Actuator(parts, 'MidHip', 'Neck', 'Nose') #change to head
    #back_angle = back.coord2Angle()
    #print("back_angle", back_angle)

if __name__ == "__main__": 
    main()
