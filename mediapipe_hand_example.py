import cv2
from random import choice
import mediapipe as mp
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands



# *---*---*---*---*     Loading assets for display   *---*---*---*---*

#Loading left hand 
left_hand  =  cv2.imread("HandGCrop.png",-1)
left_hand_height, left_hand_width, _ = left_hand.shape
left_hand = cv2.resize(left_hand, (int(left_hand_width/4),int(left_hand_height/4) ))
left_hand_height, left_hand_width, _ = left_hand.shape

#Loading right hand 
right_hand  =  cv2.imread("HandRCrop.png",-1) 
right_hand_height, right_hand_width, _ = right_hand.shape
right_hand = cv2.resize(right_hand, (int(right_hand_width/4),int(right_hand_height/4) ))
right_hand_height, right_hand_width, _ = right_hand.shape

#Loading pink spot 
pink_spot  =  cv2.imread("PinkSpot.png",-1) 
pink_spot_height, pink_spot_width, _ = pink_spot.shape
pink_spot = cv2.resize(pink_spot, (int(pink_spot_width/4),int(pink_spot_height/4) ))
pink_spot_height, pink_spot_width, _ = right_hand.shape

#Loading blue spot 
blue_spot  =  cv2.imread("BlueSpot.png",-1) 
blue_spot_height, blue_spot_width, _ = blue_spot.shape
blue_spot = cv2.resize(blue_spot, (int(blue_spot_width/4),int(blue_spot_height/4) ))
blue_spot_height, blue_spot_width, _ = right_hand.shape

bg = cv2.imread("bg2.jpg")
bg_image_height, bg_image_width, _ = bg.shape

#            *---*---*---*---*       *---*---*---*---*
    


# Returns X and Y of the palm by calculating the mean between wrist and middle finger
# from the hand landmarks
# returns a tuple with X and Y
def getPalmPos(landmarks):
    wX = int(landmarks[mp_hands.HandLandmark.WRIST].x*image_width)  
    wY = int(landmarks[mp_hands.HandLandmark.WRIST].y*image_height)  
    fX = int(landmarks[mp_hands.HandLandmark.MIDDLE_FINGER_MCP].x*image_width) 
    fY = int(landmarks[mp_hands.HandLandmark.MIDDLE_FINGER_MCP].y*image_height)  
    pX = int((wX+fX)/2)
    pY = int((wY+fY)/2)
    return (pX,pY)  

def overlay(src, dst, x_offset, y_offset, center):
    if(center):
        # Recalculate offset to center the overlay
        x_offset =abs( x_offset - int(src.shape[1]/2))
        y_offset=  abs(y_offset - int(src.shape[0]/2))

    # Calculating start and end position on the bg 
    y1, y2 = y_offset, y_offset + src.shape[0]
    x1, x2 = x_offset, x_offset + src.shape[1]

    alpha_s = src[:, :, 3] / 255.0
    alpha_l = 1.0 - alpha_s

    # Check if starting and ending positon for overlay is within
        # the background image size (hard coded values for now)
    if((x1 <= 640 and x2 <= 640) and (y1 <= 425 and y2 <= 425)):
        for c in range(0, 3):
            dst[y1:y2, x1:x2, c] = (alpha_s * src[:, :, c] + alpha_l * dst[y1:y2, x1:x2, c])
    return dst

x1= 0
x2= 160
x3= 320
x4= 480
y1= 0
y2= 142
y3= 284

alphabet = ['A','B','C','D','E','F','G','H','I','J','K','L']

base_positions = {'A' : (x1,y1), 'B' : (x2,y1), 'C' : (x3,y1), 'D' : (x4,y1),
 'E' : (x1,y2), 'F' : (x2,y2), 'G' : (x3,y2), 'H' : (x4,y2),
  'I' : (x1,y3), 'J' : (x2,y3), 'K' : (x3,y3), 'L' :(x4,y3)}


def spotGeneration(spot,background, position):
    overlay(spot,background,base_positions[position][0],base_positions[position][1], False)

rand_pos = choice(alphabet)
touched  = False

def detectContact(palmA, palmB, spot_pos):
    is_touched = False
    spot_x , spot_y = base_positions[spot_pos][0],base_positions[spot_pos][1]

    # center_spot_pos = (base_positions[spot_pos][0]+int(blue_spot_width/2),base_positions[spot_pos][1]+int(blue_spot_height/2))
    # spot_x , spot_y = center_spot_pos

    paX, paY, pbX, pbY = palmA[0], palmA[1],palmB[0], palmB[1]   
    # print( f'Spot Center X : {spot_x} \n Spot Center Y : {spot_y} \n ')
    # print(paX, paY, pbX, pbY)
    paX_in = paX>= spot_x and paX <= spot_x +blue_spot_width 
    paY_in = paY>= spot_y and paY <= spot_y + blue_spot_height 
    pa_in = paX_in and paY_in

    pbX_in = pbX>= spot_x and pbX <= blue_spot_width     
    pbY_in = pbY>= spot_y and pbY <= blue_spot_height 
    pb_in = pbX_in and pbY_in
    # print( pb_in , pa_in)
    if( pa_in or pb_in):
        # print("CONTACTS\n\n\n")
        is_touched = True

    return is_touched


# For webcam input:
cap = cv2.VideoCapture(0)

with mp_hands.Hands(min_detection_confidence=0.5, min_tracking_confidence=0.3) as hands:

  while cap.isOpened():

    success, image = cap.read()
    image_height, image_width, _ = image.shape
    image = cv2.resize(image, (int(image_width/4),int(image_height/4) ))
    
    bg = cv2.imread("bg2.jpg")
    bg_image_height, bg_image_width, _ = bg.shape

    if not success:
      print("Ignoring empty camera frame.")
      # If loading a video, use 'break' instead of 'continue'.
      continue

    # Flip the image horizontally for a later selfie-view display, and convert
    # the BGR image to RGB.
    image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)

    
# *---*---*---*---*     Starting processing of video   *---*---*---*---*

    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    image.flags.writeable = False
    results = hands.process(image)

    # Draw the hand annotations on the image.

    palmA = (-100,-100)
    palmB = (-100,-100)

    if results.multi_hand_landmarks:
      for indx, hand_landmarks in enumerate(results.multi_hand_landmarks):
        if( indx<1):
            palmA = getPalmPos(hand_landmarks.landmark)
            if( palmA[0]< bg_image_width/2 ):                
                overlay(left_hand,bg,palmA[0],palmA[1],True)
            else:
                overlay(right_hand,bg,palmA[0],palmA[1],True)
        else:
            palmB = getPalmPos(hand_landmarks.landmark)       
            if( palmA[0]<palmB[0]): # If palmA is more to the left
                overlay(right_hand,bg,palmB[0],palmB[1],True)
                overlay(left_hand,bg,palmA[0],palmA[1],True)
            else:
                overlay(left_hand,bg,palmB[0],palmB[1],True)
                overlay(right_hand,bg,palmA[0],palmA[1],True)
        # cv2.circle(bg, palmA, 20,(145+50*indx+1,78+50*indx,64+50*indx+1),-1)
        # cv2.circle(bg, palmB, 20,(145+50*indx+1,78+50*indx,64+50*indx+1),-1)

    # print("PalmA : ", palmA, end="\n")
    # print("PalmB : ", palmB, end="\n")

    # For the full screen image 
    # cv2.namedWindow("Game Window", cv2.WND_PROP_FULLSCREEN)
    # cv2.setWindowProperty("Game Window",cv2.WND_PROP_FULLSCREEN,cv2.WINDOW_FULLSCREEN)
    # cv2.imshow("Game Window", bg)

    touched = detectContact(palmA, palmB, rand_pos)
   
    if(not touched):
        spotGeneration(blue_spot,bg,rand_pos)
    else:
        rand_pos = choice(alphabet)
    # spotGeneration(pink_spot,bg, 'G' )
    # spotGeneration(blue_spot,bg, 'L' )
    cv2.imshow('MediaPipe Hands', bg)
    # cv2.imshow("OG IMaGE", cv2.cvtColor(image,cv2.COLOR_RGB2BGR))
    if cv2.waitKey(5) & 0xFF == 27:
      break
cap.release()
