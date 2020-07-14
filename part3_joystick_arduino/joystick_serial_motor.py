import pygame
from pygame.locals import *
import time
import serial

#シリアル通信(PC⇔Arduino)
ser = serial.Serial()
ser.port = "COM4"     #デバイスマネージャでArduinoのポート確認
ser.baudrate = 9600 #Arduinoと合わせる
ser.setDTR(False)     #DTRを常にLOWにしReset阻止
ser.open()            #COMポートを開く

joy_list =  [[0]*3]*3

pygame.joystick.init()
try:
    j = pygame.joystick.Joystick(0) # create a joystick instance
    j.init() # init instance
    print ('Joystickの名称: ' + j.get_name())
    print ('ボタン数 : ' + str(j.get_numbuttons()))
#キーが押されたフレーム時間を記録するリストを生成
    key_list =[0]*j.get_numbuttons()
except pygame.error:
    print ('Joystickが見つかりませんでした。')

def main():
    pygame.init()
    frametime = 0
    x,y =0,0
    while 1:

        for e in pygame.event.get(): # イベントチェック

            if e.type == QUIT: # 終了が押された？
                return
            if (e.type == KEYDOWN and
                e.key  == K_ESCAPE): # ESCが押された？
                return
            # Joystick関連のイベントチェック

            #Joystick
            if e.type == pygame.locals.JOYAXISMOTION:

                joy_list[x][y]  =   pygame.time.get_ticks() - joy_list[x][y]
                print('joystick')

                L_stick_LR = '{:.1f}'.format(j.get_axis(0))
                L_stick_updown = '{:.1f}'.format(j.get_axis(1))
                #符号変換
                L_stick_updown = float(L_stick_updown) * -1
                L_stick_updown = str(L_stick_updown)

                R_stick_LR = '{:.1f}'.format(j.get_axis(4))
                R_stick_updown = '{:.1f}'.format(j.get_axis(3))

                back_button = '{:.1f}'.format(j.get_axis(2))
                print('左スティック:', L_stick_updown, L_stick_LR)
                print('右スティック:', R_stick_updown, R_stick_LR)
                print('バックスティック:', back_button)

                L_stick = L_stick_updown + ',' + L_stick_LR + 'z'
                
                print(L_stick.encode())
                ser.write(L_stick.encode())

            #十字キー
            elif e.type == pygame.locals.JOYHATMOTION:
                #print ('hat motion')
                x,y = j.get_hat(0)
                joy_list[x][y]  =  pygame.time.get_ticks()
                cross_key = str(float(y)) + ',' + str(float(x)) + 'z'

                print(cross_key.encode())
                ser.write(cross_key.encode())

            #各種ボタン押下
            elif e.type == pygame.locals.JOYBUTTONDOWN:
                print (str(e.button)+'番目のボタンを押した')
                key_list[e.button] = pygame.time.get_ticks()

            #各種ボタン離脱
            elif e.type == pygame.locals.JOYBUTTONUP:
                print('ボタン'+str(e.button)+'を離した')
                key_list[e.button] = pygame.time.get_ticks() - key_list[e.button]
                #print("frame : " + str(key_list[e.button]/1000*60 ))

if __name__ == '__main__':
    main()

    ser.close()           #COMポートを閉じる
