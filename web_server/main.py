from flask import Flask, render_template
import os
import websocket
import threading
import json

# WS CLIENT
SOLOIST_PIPE = "/tmp/soloist"

def on_message(ws, message):
    data = json.loads(message)
    if data['type'] == 'playback_state':
        item = data["item"]
        decorations = item["decorations"]
        song = {
            "name": decorations["identity"]["name"],
            "album_name": decorations["parent"]["entity"]["decorations"]["identity"]["name"],
            "album_art": next(
                cover["url"]
                for cover in decorations["visual_identity"]["cover"]
                if cover["size"] == "xlarge"
            ),
            "playback_status": data["status"],
            "total_duration": decorations["playback"]["duration_ms"],
            "current_duration": data["position"]["position_ms"],
        }
        playback_state = (
            f"{song['name']}|"
            f"{song['album_name']}|"
            f"{song['album_art']}|"
            f"{song['playback_status']}|"
            f"{song['total_duration']}|"
            f"{song['current_duration']}\n"
        )
        print(json.dumps(song, indent=2))
        if song["name"]:
            fd = os.open(SOLOIST_PIPE, os.O_WRONLY)
            try:
                os.write(fd, f"{playback_state}\n".encode())
            finally:
                os.close(fd)
        return song



def on_error(ws, error):
    print("error:", error)

def on_close(ws, close_status_code, close_msg):
    print("closed:", close_status_code, close_msg)

def on_open(ws):
    print("connected")

def listen_soloist_ws():
    ws = websocket.WebSocketApp(
        "ws:/127.0.0.1:5010",
        on_open=on_open,
        on_message=on_message,
        on_error=on_error,
        on_close=on_close,
    )

    ws.run_forever()


# HTTP
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

@app.get("/sketch/<int:idx>")
def set_sketch(idx):
    send_command(f"SKETCH {idx}")
    return {"ok": True}

if __name__ == "__main__":
    threading.Thread(
        target=listen_soloist_ws,
        daemon=True,
    ).start()
    app.run(host="0.0.0.0", port=1618)
