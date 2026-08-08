from flask import Flask, render_template
import os

app = Flask(__name__)

PIPE = "/tmp/pipe"

@app.get("/")
def index():
    return render_template("index.html")

@app.get("/next")
def next_sketch():
    fd = os.open(PIPE, os.O_WRONLY)
    os.write(fd, b"NEXT\n")
    os.close(fd)

    return {"ok": True}

@app.get("/prev")
def previous_sketch():
    fd = os.open(PIPE, os.O_WRONLY)
    os.write(fd, b"PREV\n")
    os.close(fd)

    return {"ok": True}

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=1618)