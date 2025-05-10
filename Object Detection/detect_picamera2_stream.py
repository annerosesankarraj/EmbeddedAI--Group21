#!/usr/bin/env python3
"""
Continuous object‑classification stream using:
 • Picamera2  – grabs 640×480 RGB frames
 • ONNXRuntime – MobileNet‑V2 fp32 model
 • OpenCV     – draws bounding boxes & confidence
Press “q” in the preview window to quit.
"""

from pathlib import Path
import time, cv2, numpy as np, onnxruntime as ort
from picamera2 import Picamera2, Preview

# ── paths & model ────────────────────────────────────────────────────────
MODEL = "/home/barath/indoor_detector/mobilenet_v2_float.onnx"
LABEL = "/home/barath/indoor_detector/imagenet_classes.txt"
IMG   = 224
THR   = 0.35          # confidence threshold for demo (optional)

labels = Path(LABEL).read_text().splitlines()
sess   = ort.InferenceSession(MODEL, providers=["CPUExecutionProvider"])
inp    = sess.get_inputs()[0].name
out    = sess.get_outputs()[0].name

# ── helper functions ─────────────────────────────────────────────────────
def preprocess(rgb):
    """Resize → CHW → normalise exactly like training."""
    x = cv2.resize(rgb, (IMG, IMG), cv2.INTER_LANCZOS4).transpose(2,0,1).astype(np.float32)
    mean = np.array([0.079, 0.05, 0.000]) + 0.406
    std  = np.array([0.005, 0.000, 0.001]) + 0.224
    for c in range(3):
        x[c] = (x[c] / 255.0 - mean[c]) / std[c]
    return x[np.newaxis]                  # NCHW

def softmax(v):
    e = np.exp(v - v.max())
    return e / e.sum()

# ── initialise Picamera2 (RGB888 frames) ────────────────────────────────
cam = Picamera2()
cam.preview_configuration.main.size   = (640, 480)
cam.preview_configuration.main.format = "RGB888"
cam.configure("preview")
cam.start()
time.sleep(1)                           # AE/AWB settle
cam.start_preview(Preview.QTGL)         # live window via Qt GL

print("Press q to quit")

while True:
    frame = cam.capture_array("main")          # 640×480 RGB
    t0    = time.time()

    # ─ ONNX inference ─
    logits = sess.run([out], {inp: preprocess(frame)})[0][0]
    probs  = softmax(logits)
    top5   = probs.argsort()[-5:][::-1]

    # ─ draw top‑1 label on the frame ─
    best  = top5[0]
    label = f"{labels[best]} {probs[best]:.2f}"
    cv2.putText(frame, label, (8, 32),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,0), 2)

    fps = 1 / (time.time() - t0)
    cv2.putText(frame, f"{fps:.1f} FPS", (8, 64),
                cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0,255,255), 2)

    cv2.imshow("MobileNet‑V2 / Picamera2", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cam.stop()
cv2.destroyAllWindows()
