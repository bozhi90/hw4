hw4 readme

4-1 main.cpp中的main會一直接收XBee的rpc指令，去呼叫Go_Straight與Go_turn。

    Go_Straight利用encoder做距離的控制，當達到輸入(dist)的距離後就停止。

    Go_turn利用時間的延遲來做角度的控制，輸入(ang)單位為10度。

    在這裡我用mbed13的car_control.py發送rpc指令


    執行:板子燒main.cpp，openMV燒car_control.py，接線如同教授要求。

    結果:車子會根據rpc指令移動，但有誤差。


4-2 main.cpp中的main會一直接收uart的指令，根據指令決定要直走、偏右、偏左。

    其中car.go()是我額外寫的function，兩變數分別控制兩輪速度。


    執行:板子燒main.cpp，openMV燒car_control.py，接線如同教授要求。

    結果:勉強跟著線走，誤差很大，還需要優化。


4-3 控制與4-2一樣，接收uart的指令，根據指令決定要直走、偏右、偏左，多了ping的thread。

    ping的thread是mbed12中ping的code。


    執行:板子燒main.cpp，openMV燒car_control.py，接線如同教授要求。

    結果:很準確的朝AprilTag前進。
