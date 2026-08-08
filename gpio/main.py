from gpiozero import Button
from signal import pause

PIPE = "/tmp/pipe"

next_button = Button(17, bounce_time=0.05)
prev_button = Button(27, bounce_time=0.05)


def send_command(command):
    print(command, flush=True)

    with open(PIPE, "w") as pipe:
        pipe.write(command + "\n")
        pipe.flush()


next_button.when_pressed = lambda: send_command("NEXT")
prev_button.when_pressed = lambda: send_command("PREV")

pause()