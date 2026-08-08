from gpiozero import Button
import os

PIPE = "/tmp/pipe"

next_button = Button(17)
prev_button = Button(27)

def send_command(command):
    with open(PIPE, "w") as pipe:
        pipe.write(command + "\n")

next_button.when_pressed = lambda: send_command("NEXT")
prev_button.when_pressed = lambda: send_command("PREV")

pause()