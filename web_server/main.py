from flask import Flask, render_template
import os

app = Flask(__name__)

PIPE = "/tmp/pipe"

def send_command(command):
    fd = os.open(PIPE, os.O_WRONLY)
    try:
        os.write(fd, f"{command}\n".encode())
    finally:
        os.close(fd)

@app.get("/")
def index():
    return render_template("index.html")

@app.get("/next")
def next_sketch():
    send_command("NEXT")
    return {"ok": True}

@app.get("/prev")
def previous_sketch():
    send_command("PREV")
    return {"ok": True}

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=1618)