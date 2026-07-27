// =============================================================
//  Baryon Sweeper — Arduino (Tested only on Arduino Leonardo) K-Line Bridge
//  =============================================================
//  Part of the PsP-Baryon-Sweeper-Arduino project
//
//  This sketch bridges pysweeper (PC) ↔ PSP Syscon (K-line)
//  using an Arduino (Tested only on Arduino Leonardo).
// 
//  🔑 THE KEY FIX: SERIAL_8E2
//  ============================
//  Serial1.begin() defaults to 8N1 (8 bits, No parity, 1 stop).
//  The PSP Syscon communicates in 8E2 (8 bits, Even parity, 2 stops).
//  Using the wrong framing causes signal contention that makes
//  the Syscon ignore all responses, looping forever.
//
//  With SERIAL_8E2, the Leonardo works perfectly at 5V — no
//  diode, no 3.3V board, no extra components needed.
//
//  🔌 WIRING
//  =========
//                 
//  TX (pin 1) ────[10kΩ]── RX (pin 0) ── K-line (center PSP)
//  GND ──────────────────────────────────── PSP negative (-)
//  3.3V ─────────────────────────────────── PSP positive (+)
//
//  🖥️ USE WITH pysweeper
//  ======================
//  1. Upload this sketch to Arduino (Tested only on Arduino Leonardo)
//  2. Close Arduino IDE (Serial Monitor locks the port)
//  3. Open pysweeper.py → Select COM port → Service Mode → Start
//  4. Connect wires to PSP with AC charger + Magic Memory Stick
//  5. The PSP powers on automatically and boots from the stick
//
//  =============================================================

void setup() {
  Serial.begin(19200);
  Serial1.begin(19200, SERIAL_8E2);
}

void loop() {
  if (Serial.available() > 0) {
    size_t len = Serial.available();
    uint8_t buf[64];
    Serial.readBytes(buf, min(len, (size_t)64));
    Serial1.write(buf, min(len, (size_t)64));
    Serial1.flush();
  }

  if (Serial1.available() > 0) {
    size_t len = Serial1.available();
    uint8_t buf[64];
    Serial1.readBytes(buf, min(len, (size_t)64));
    Serial.write(buf, min(len, (size_t)64));
  }
}
